//
// Created by zero on 3/2/22.
//

#include "LsqRTK.hpp"
#include "ARLambda.hpp"

#define debug 0

namespace gnssSpace{

    std::string LsqRTK::getClassName() const
    {
        return "LsqRTK";
    }


    double LsqRTK::getSolution( const TypeID& type,
                                const VectorXd& stateVec ) const
    noexcept(false)
    {
        // Declare an varIterator for 'stateMap' and go to the first element
        auto varIt = currentUnkSet.begin();
        while( (*varIt).getType() != type )
        {
            // If the same type is not found, throw an exception
            if( varIt == currentUnkSet.end() )
            {
                InvalidRequest e(getClassName() + "Type not found in solution vector.");
                THROW(e);
            }

            varIt++;
        }

        // Else, return the corresponding value
        return stateVec( (*varIt).getNowIndex() );

    }  // End of method 'SolverGeneral::getSolution()'

    void LsqRTK::printComment() const
    noexcept(false)
    {
        if(pOutStream!=NULL)
        {
            // output the header of solution
            (*pOutStream)
                    << "# "
                    << leftJustify("solution order",20)
                    << ": "
                    << "year doy sod Y Y Z"
                    << endl;;
        }
        else
        {
            cerr << getClassName()
                 << "pOutStream is NULL" << endl;
        }
    }

    void LsqRTK::printSolution(CommonTime& time, Triple& rcvPos) const
    noexcept(false)
    {
        (*pOutStream)
                << ios::fixed
                << setprecision(3)
                << YDSTime(time).year << " "
                << YDSTime(time).doy << " "
                << rightJustify(asString(YDSTime(time).sod, 3), 14) << " "
                << rcvPos[0] << " "
                << rcvPos[1] << " "
                << rcvPos[2] << " "
                << endl;
    }


    void LsqRTK::defineEquations() {

        // warning:
        // must clear equation system for each epoch
        equSys.clearEquations();


        //////////////////////////////////////////////////
        // for spp, only dx/dy/dz/dt/iono are estimated,
        // tropopsheric delay will
        // be ignored.
        //////////////////////////////////////////////////

        // source-indexed variable
        Variable dx( TypeID::dX );
        Variable dy( TypeID::dY );
        Variable dz( TypeID::dZ );

        VariableSet commonVarSet;
        commonVarSet.insert(dx);
        commonVarSet.insert(dy);
        commonVarSet.insert(dz);

        Variable cdtGPS( TypeID::dcdtGPS );
        Variable cdtBDS( TypeID::dcdtBDS );

        Variable N1(TypeID::N1,true,true, false);
        Variable N2(TypeID::N2,true,true, false);


        // GPS Equation
        Equation equC1GPS( TypeID::prefitC1GDiff, SatelliteSystem::GPS );
        Equation equC2GPS( TypeID::prefitC2GDiff, SatelliteSystem::GPS );
        Equation equL1GPS( TypeID::prefitL1GDiff, SatelliteSystem::GPS );
        Equation equL2GPS( TypeID::prefitL2GDiff, SatelliteSystem::GPS );

        // BDS
        Equation equC2BDS( TypeID::prefitC2CDiff, SatelliteSystem::BDS );
        Equation equC6BDS( TypeID::prefitC6CDiff, SatelliteSystem::BDS );
        Equation equL2BDS( TypeID::prefitL2CDiff, SatelliteSystem::BDS );
        Equation equL6BDS( TypeID::prefitL6CDiff, SatelliteSystem::BDS );


        // cdt for GPS/GAL/BDS
        equC2GPS.addVarSet(commonVarSet);
        equC1GPS.addVarSet(commonVarSet);
        equL2GPS.addVarSet(commonVarSet);
        equL1GPS.addVarSet(commonVarSet);

        equC6BDS.addVarSet(commonVarSet);
        equC2BDS.addVarSet(commonVarSet);
        equL6BDS.addVarSet(commonVarSet);
        equL2BDS.addVarSet(commonVarSet);

        equC1GPS.setWeight(1/(0.3*0.3));
        equC2GPS.setWeight(1/(0.3*0.3));
        equC2BDS.setWeight(1/(0.3*0.3));
        equC6BDS.setWeight(1/(0.3*0.3));

        equL1GPS.setWeight(1/(0.003*0.003));
        equL2GPS.setWeight(1/(0.003*0.003));
        equL2BDS.setWeight(1/(0.003*0.003));
        equL6BDS.setWeight(1/(0.003*0.003));


        // add clock-bias into equations
        equC6BDS.addVariable(cdtBDS, 1.0);
        equC2BDS.addVariable(cdtBDS, 1.0);
        equL6BDS.addVariable(cdtBDS, 1.0);
        equL2BDS.addVariable(cdtBDS, 1.0);

        equC2GPS.addVariable(cdtGPS, 1.0);
        equC1GPS.addVariable(cdtGPS, 1.0);
        equL2GPS.addVariable(cdtGPS, 1.0);
        equL1GPS.addVariable(cdtGPS, 1.0);

        //

        equL6BDS.addVariable(N2, L6_WAVELENGTH_BDS);
        equL2BDS.addVariable(N1, L2_WAVELENGTH_BDS);

        equL2GPS.addVariable(N2, L2_WAVELENGTH_GPS);
        equL1GPS.addVariable(N1, L1_WAVELENGTH_GPS);


        //> GPS ionospheric delays
        //
        // insert all equations into equation system
        //
        equSys.addEquation(equC1GPS);
        equSys.addEquation(equC2GPS);
        equSys.addEquation(equL1GPS);
        equSys.addEquation(equL2GPS);

        equSys.addEquation(equC2BDS);
        equSys.addEquation(equC6BDS);
        equSys.addEquation(equL2BDS);
        equSys.addEquation(equL6BDS);

    }

    Rx3ObsData &LsqRTK::Process(Rx3ObsData &rxDataRover) noexcept(false) {
        defineEquations();
        equSys.Prepare(rxDataRover.currEpoch, source, rxDataRover.stvData);

        currentUnkSet = equSys.getVarUnknowns();

        if(currentUnkSet.size() == 0)
        {
            cerr << "currentUnkSet size is zero!"<< endl;
            exit(-1);
        }

        if(debug)
        {
            cout << "Unknowns:" << endl;
            for(auto it=currentUnkSet.begin(); it!= currentUnkSet.end(); it++)
            {
                cout << (*it) << endl;
            }
        }

        std::set<Equation> desSet = equSys.getDescripEqus();
        if(debug)
        {
            cout << "desSet" << endl;
            for(auto it = desSet.begin(); it!= desSet.end(); it++)
            {
                cout << (*it) << endl;
            }
        }

        std::set<Equation> equSet = equSys.getCurrentEquationsSet();
        if(debug)
        {
            cout << "equSet" << endl;
            for(auto it = equSet.begin(); it!= equSet.end(); it++)
            {
                cout << (*it) << endl;
            }
        }

        VectorXd prefit = equSys.getPrefitsVector();
        MatrixXd hMatrix = equSys.getGeometryMatrix();
        MatrixXd hT = hMatrix.transpose();

//        if(debug)
//        {
//            cout << "prefit??????:" << endl;
//            cout << prefit << endl;
//
//            cout << "hMatrix?????:" << endl;
//            cout << hMatrix<< endl;
//
//        }

        try
        {
            covMatrix = hT * equSys.getWeightsMatrix() * hMatrix;
            covMatrix = covMatrix.inverse();
        }
        catch(...)
        {
            InvalidSolver e("Unable to invert matrix covMatrix");
            THROW(e);
        }

        solution = covMatrix * hT * equSys.getWeightsMatrix() * prefit;

//        cout<<"solution:"<<endl;
//        cout<<solution<<endl;

        double dx = getSolution(TypeID::dX, solution);
        double dy = getSolution(TypeID::dY, solution);
        double dz = getSolution(TypeID::dZ, solution);

        delta[0] = dx;
        delta[1] = dy;
        delta[2] = dz;

        // 计算固定解

        solution = solution - fixAmbiguity(rxDataRover,SatelliteSystem::GPS);
        solution = solution - fixAmbiguity(rxDataRover,SatelliteSystem::BDS);

        double dxFixed = getSolution(TypeID::dX, solution);
        double dyFixed = getSolution(TypeID::dY, solution);
        double dzFixed = getSolution(TypeID::dZ, solution);

        deltaFixed[0] = dxFixed;
        deltaFixed[1] = dyFixed;
        deltaFixed[2] = dzFixed;






//        auto varIt = currentUnkSet.begin();
//        while( varIt != currentUnkSet.end() ){
//            if(varIt->getType() == TypeID::N1&&varIt->getSatSys() == SatelliteSystem::GPS){
//                rxDataRover.stvData.find(varIt->getSatellite())->second[TypeID::IntL1G] = solution(varIt->getNowIndex());
//                cout<<rxDataRover.stvData.find(varIt->getSatellite())->second[TypeID::elevation]<<endl;
//            }
//            if(varIt->getType() == TypeID::N2&&varIt->getSatSys() == SatelliteSystem::GPS){
//                rxDataRover.stvData.find(varIt->getSatellite())->second[TypeID::IntL2G] = solution(varIt->getNowIndex());
//            }
//            if(varIt->getType() == TypeID::N1&&varIt->getSatSys() == SatelliteSystem::GPS){
//                rxDataRover.stvData.find(varIt->getSatellite())->second[TypeID::IntL2C] = solution(varIt->getNowIndex());
//            }
//            if(varIt->getType() == TypeID::N2&&varIt->getSatSys() == SatelliteSystem::GPS){
//                rxDataRover.stvData.find(varIt->getSatellite())->second[TypeID::IntL6C] = solution(varIt->getNowIndex());
//            }
//
//            varIt++;
//
//        }


        return rxDataRover;
    }

    VectorXd LsqRTK::fixAmbiguity(Rx3ObsData &rxDataRover, SatelliteSystem sys) noexcept(false) {
        /// MainSat
        SatID mainSat;
        double MaxElev = -1;
        int MaxIndexN1 = -1;
        int MaxIndexN2 = -1;

        for (satTypeValueMap::iterator it = rxDataRover.stvData.begin();
             it != rxDataRover.stvData.end();
             ++it){
            if(it->first.system == sys.system ){
                if(it->second[TypeID::elevation]>MaxElev){
                    mainSat = it->first;
                    MaxElev = it->second[TypeID::elevation];
                }
            }
        }

        /// 构成H矩阵
        MatrixXd h = MatrixXd::Zero(  2*rxDataRover.stvData.numSats(sys.system)-2,currentUnkSet.size());
        VectorXd floatAmb = VectorXd::Zero( 2*rxDataRover.stvData.numSats(sys.system)-2);

        MatrixXd h_other = MatrixXd::Zero(  currentUnkSet.size()-2*rxDataRover.stvData.numSats(sys.system),currentUnkSet.size());
        MatrixXd h_now = MatrixXd::Zero(  2*rxDataRover.stvData.numSats(sys.system),currentUnkSet.size());

        MatrixXd t_all = MatrixXd::Zero(  currentUnkSet.size(),currentUnkSet.size()-2*rxDataRover.stvData.numSats(sys.system));
        MatrixXd t_diff = MatrixXd::Zero(  2*rxDataRover.stvData.numSats(sys.system),2*rxDataRover.stvData.numSats(sys.system)-2);

        int iter = 0;
        int iter2 = 0;
        int iter3 = 0;
        auto varIt = currentUnkSet.begin();
        while( varIt != currentUnkSet.end() )
        {
            if(varIt->getSatSys() == sys.system){

                if(varIt->getSatellite() == mainSat&&varIt->getType() == TypeID::N1){
                   MaxIndexN1 = varIt->getNowIndex();

                }

                if(varIt->getSatellite() == mainSat&&varIt->getType() == TypeID::N2){
                    MaxIndexN2 = varIt->getNowIndex();
                }



            }
            varIt++;

        }
//        rxDataRover.stvData.find(mainSat)->second[type] = solution(MaxIndex);

        varIt = currentUnkSet.begin();
        while( varIt != currentUnkSet.end() )
        {
            if(varIt->getSatSys() == sys.system){


                if(varIt->getSatellite() != mainSat&&varIt->getType() == TypeID::N1){
                    h(iter,varIt->getNowIndex()) = -1.0;
                    h(iter,MaxIndexN1) = 1.0;
                    floatAmb(iter) = solution(MaxIndexN1)-solution(varIt->getNowIndex());
                    t_diff(iter3,iter) = 1.0;
                    iter++;
                }
                if(varIt->getSatellite() != mainSat&&varIt->getType() == TypeID::N2){
                    h(iter,varIt->getNowIndex()) = -1.0;
                    h(iter,MaxIndexN2) = 1.0;
                    floatAmb(iter) = solution(MaxIndexN2)-solution(varIt->getNowIndex());
                    t_diff(iter3,iter) = 1.0;
                    iter++;
                }

                h_now(iter3,varIt->getNowIndex()) = 1.0;
                iter3++;


            }else{
                t_all(varIt->getNowIndex(),iter2) = 1.0;
                h_other(iter2,varIt->getNowIndex()) = 1.0;
                iter2++;
            }

            varIt++;
        }


        MatrixXd floatAmbCov = h * covMatrix * h.transpose();
//        cout<<h<<endl;
//        cout<<floatAmbCov<<endl;
//        cout<<floatAmb<<endl;

        ARLambda AR;
        VectorXd intAmb(floatAmb.size()); intAmb.setZero();
        intAmb = AR.resolve(floatAmb, floatAmbCov);
        isFixed = AR.isFixed();

        MatrixXd dxFloatAmbCov = covMatrix * h.transpose();

//        VectorXd ds= t_all *(h_other*covMatrix * h_now.transpose())*(h_now*covMatrix*h_now.transpose()).inverse()* t_diff * (floatAmb - intAmb);
        VectorXd ds =t_all*h_other * covMatrix * h.transpose() * floatAmbCov.inverse() *(floatAmb - intAmb);


        return ds;





//        iter = 0;
//        varIt = currentUnkSet.begin();
//        while( varIt != currentUnkSet.end() )
//        {
//            if(varIt->getType() == id.type&&varIt->getSatSys() == sys.system){
//                if(varIt->getSatellite() != mainSat){
//                    rxDataRover.stvData.find(mainSat)->second[type] = solution(MaxIndex)-intAmb(iter);
//                    iter++;
//                }
//            }
//            varIt++;
//        }

    }
}