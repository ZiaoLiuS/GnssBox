/**
 * solver for spp using multi-gnss observation data
 *
 * shoujian zhang
 *
 * created by shoujianzhang, 2020.08.07
 * modify the input data from gnssRinex to Rx3ObsData, 2022.01.15
 *
 **/

#include <fstream>
#include "LsqSPP.hpp"
#define debug 0

namespace gnssSpace
{

    std::string LsqSPP::getClassName() const
    {
        return "LsqSPP"; 
    }

    void LsqSPP::defineEquations()
    {

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

        // vertical ionospheric delays 
        Variable iono( TypeID::iono, &ionoStoModel, true, true, false);

        VariableSet commonVarSet;
        commonVarSet.insert(dx);
        commonVarSet.insert(dy);
        commonVarSet.insert(dz);

        Variable cdtGPS( TypeID::dcdtGPS );
        Variable cdtGAL( TypeID::dcdtGAL );
        Variable cdtBDS( TypeID::dcdtBDS );
        Variable cdtGLO( TypeID::dcdtGLO );
        Variable cdtQZS( TypeID::dcdtQZS );

        // about 0.03 per 30s 
        ionoC1GStoModel.setQprime(1.0e-3);
        ionoC1EStoModel.setQprime(1.0e-3);
        ionoC2CStoModel.setQprime(1.0e-3);

        Variable ionoC1GVar(TypeID::ionoC1G, &ionoC1GStoModel, true, true, false);
        Variable ionoC1EVar(TypeID::ionoC1E, &ionoC1EStoModel, true, true, false);
        Variable ionoC2CVar(TypeID::ionoC2C, &ionoC2CStoModel, true, true, false);

        // GPS Equation
        Equation equC1GPS( TypeID::prefitC1G, SatelliteSystem::GPS   );
        Equation equC2GPS( TypeID::prefitC2G, SatelliteSystem::GPS   );

        // GAL
        Equation equC1GAL( TypeID::prefitC1E, SatelliteSystem::Galileo );
        Equation equC5GAL( TypeID::prefitC5E, SatelliteSystem::Galileo );

        // BDS
        Equation equC2BDS( TypeID::prefitC2C, SatelliteSystem::BDS );
        Equation equC6BDS( TypeID::prefitC6C, SatelliteSystem::BDS );

        // GLO
        Equation equC1GLO( TypeID::prefitC1R, SatelliteSystem::GLONASS );
        Equation equC2GLO( TypeID::prefitC2R, SatelliteSystem::GLONASS );
        
        //
        // add variables into equation

        // cdt for GPS/GAL/BDS
        equC2GPS.addVarSet(commonVarSet);
        equC1GPS.addVarSet(commonVarSet);

        equC5GAL.addVarSet(commonVarSet);
        equC1GAL.addVarSet(commonVarSet);

        equC6BDS.addVarSet(commonVarSet);
        equC2BDS.addVarSet(commonVarSet);

        equC2GLO.addVarSet(commonVarSet);
        equC1GLO.addVarSet(commonVarSet);

        // add clock-bias into equations
        equC6BDS.addVariable(cdtBDS, 1.0);
        equC2BDS.addVariable(cdtBDS, 1.0);
        equC2GLO.addVariable(cdtGLO, 1.0);
        equC1GLO.addVariable(cdtGLO, 1.0);
        equC5GAL.addVariable(cdtGAL, 1.0);
        equC1GAL.addVariable(cdtGAL, 1.0);
        equC2GPS.addVariable(cdtGPS, 1.0);
        equC1GPS.addVariable(cdtGPS, 1.0);

        //> GPS ionospheric delays
        equC1GPS.addVariable(ionoC1GVar,  1.0);
        equC2GPS.addVariable(ionoC1GVar,  GAMMA_GPS_L1L2);

        // GAL
        equC1GAL.addVariable(ionoC1EVar,  1.0); 
        equC5GAL.addVariable(ionoC1EVar,  GAMMA_GAL_L1L5); 

        //> BDS
        equC2BDS.addVariable(ionoC2CVar,  1.0);
        equC6BDS.addVariable(ionoC2CVar,  GAMMA_BDS_L2L6);

        //
        // insert all equations into equation system 
        //
        equSys.addEquation(equC1GPS);
        equSys.addEquation(equC2GPS);

        equSys.addEquation(equC1GAL);
        equSys.addEquation(equC5GAL);

        equSys.addEquation(equC2BDS);
        equSys.addEquation(equC6BDS);

        // glonass需要再测试
//      equSys.addEquation(equC2GLO);
//      equSys.addEquation(equC1GLO);

    } // end of 'defineEquations()'

    
    Rx3ObsData& LsqSPP::Process(Rx3ObsData& rxData)
    {

        equSys.Prepare(rxData.currEpoch, source, rxData.stvData);
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

        if(debug)
        {
            cout << "prefit??????:" << endl;
            cout << prefit << endl;

            cout << "hMatrix?????:" << endl;
            cout << hMatrix<< endl;

        }

        try
        {
            covMatrix = hT * hMatrix;
            covMatrix = covMatrix.inverse(); 
        }
        catch(...)
        {
            InvalidSolver e("Unable to invert matrix covMatrix");
            THROW(e);
        }

        solution = covMatrix * hT * prefit;

        VectorXd postfit;
        postfit = prefit - hMatrix* solution;
//        double W=0;
//        int k=-1;
//        for(int i=0;i<solution.size();i++){
//            if(solution(i)>W){
//                W=solution(i);
//                k=i;
//            }
//        }
//        auto iter = currentUnkSet.begin();
//        for(int i=0;i<k;i++){
//            iter++;
//        }
//        SatID s=iter->getSatellite();
//        rxData.stvData.removeSatID(iter->getSatellite());


        double dx = getSolution(TypeID::dX, solution);
        double dy = getSolution(TypeID::dY, solution);
        double dz = getSolution(TypeID::dZ, solution);

        delta[0] = dx;
        delta[1] = dy;
        delta[2] = dz;

        return rxData;
    }

    double LsqSPP::getSolution( const TypeID& type,
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

    void LsqSPP::printComment() const
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

    void LsqSPP::printSolution(CommonTime& time, Triple& rcvPos) const
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

} // end of namespace gnssSpace
