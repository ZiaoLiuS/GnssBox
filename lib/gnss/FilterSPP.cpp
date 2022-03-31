/**
 * @file FilterSPP.cpp
 * General Solver.
 */

//============================================================================
//
//  Revision
//
//  2019/08/07
//  create a general solver for multi-gpss ppp solution
//  shjzhang
//
//  2020/01/16
//  remove stateMap/covMap, which makes the program complicated
//  shjzhang.
//
//============================================================================


#include "FilterSPP.hpp"
#include "SystemTime.hpp"
#include "NEUUtil.hpp"
#include "CProbability.hpp"

#define debug 0
#define debugBadObs 0

using namespace utilSpace;
using namespace gnssSpace;
//using namespace Eigen;

namespace gnssSpace
{

      // Index initially assigned to this class
   int FilterSPP::classIndex = 9600000;

      // Returns an index identifying this object.
   int FilterSPP::getIndex() const
   { return index; }

      // Returns a string identifying this object.
   std::string FilterSPP::getClassName() const
   { return "FilterSPP"; }

   void FilterSPP::Init(void)
   {
   }

   void FilterSPP::setUpEquations(void)
   {
/*
       warning:
       本程序是为了实现单站迭代求解接收机初始位置，为了保证在动态定位时，综合利用多系统
       卫星进行单点定位，只估计gps接收机钟差，bds/gal估计系统偏差。
       需要注意：
       在估计接收机系统间钟差偏差和电离层延迟时，为了实现快速得到接收机位置，系统偏差的过程噪声
       要给的小一些，电离层延迟过程噪声给定一个很大的数值，否则整体的位置计算迭代收敛速度很慢,
       因为当前历元的位置将受到上一个历元估值的影响。
*/

       ///////////////////////////////////////////////
       // Now, define variables need to be estimated 
       ///////////////////////////////////////////////
       
       // These variables are, by default, SourceID-indexed
       Variable dx( TypeID::dX );
       Variable dy( TypeID::dY );
       Variable dz( TypeID::dZ );
       dx.setInitialVariance(100*100);
       dy.setInitialVariance(100*100);
       dz.setInitialVariance(100*100);

       whiteNoiseDXModel.setSigma(100);
       whiteNoiseDYModel.setSigma(100);
       whiteNoiseDZModel.setSigma(100);

       dx.setModel(&whiteNoiseDXModel);
       dy.setModel(&whiteNoiseDYModel);
       dz.setModel(&whiteNoiseDZModel);
       
       VariableSet xyzVarSet;

       xyzVarSet.insert(dx);
       xyzVarSet.insert(dy);
       xyzVarSet.insert(dz);

       // 必须估计isb
       Variable dcdtGPS( TypeID::dcdtGPS, &dcdtGPSModel );
       Variable dcdtGAL( TypeID::dcdtGAL, &dcdtGALModel );
       Variable dcdtBDS( TypeID::dcdtBDS, &dcdtBDSModel );

       // 设置一个很小的过程噪声
       isbGALModel.setQprime(0.0001);
       isbBDSModel.setQprime(0.0001);

       // isbBDS/isbGAL的绝对数值一般是多少?　
       Variable isbGAL( TypeID::isbGAL, &isbGALModel );
       Variable isbBDS( TypeID::isbBDS, &isbBDSModel );

       isbGAL.setInitialVariance(100.0*100.0);
       isbBDS.setInitialVariance(100.0*100.0);

       // about 0.03m per 30s 1.0e-2*30=0.01*30=0.3, sqrt(0.3)=0.5
       // 设置一个较大的过程噪声
       ionoC1GStoModel.setQprime(100.0);
       ionoC1EStoModel.setQprime(100.0);
       ionoC2CStoModel.setQprime(100.0);

       Variable ionoC1GVar(TypeID::ionoC1G, &ionoC1GStoModel, true, true, false);
       Variable ionoC1EVar(TypeID::ionoC1E, &ionoC1EStoModel, true, true, false);
       Variable ionoC2CVar(TypeID::ionoC2C, &ionoC2CStoModel, true, true, false);

       //
       // Define Equation object for code/phase observations, and add variables 
       //

       // GPS PC
       // code
       Equation equC1G( TypeID::prefitC1G, SatelliteSystem::GPS);
       Equation equC2G( TypeID::prefitC2G, SatelliteSystem::GPS);

       // GAL code
       Equation equC1E( TypeID::prefitC1E, SatelliteSystem::Galileo );
       Equation equC5E( TypeID::prefitC5E, SatelliteSystem::Galileo );

       // BDS code
       Equation equC2C( TypeID::prefitC2C, SatelliteSystem::BDS );
       Equation equC6C( TypeID::prefitC6C, SatelliteSystem::BDS );

       equC1G.addVarSet(xyzVarSet);
       equC2G.addVarSet(xyzVarSet);

       equC1E.addVarSet(xyzVarSet); 
       equC5E.addVarSet(xyzVarSet); 

       equC2C.addVarSet(xyzVarSet);
       equC6C.addVarSet(xyzVarSet);

       //> clock
       equC1G.addVariable(dcdtGPS, 1.0);
       equC2G.addVariable(dcdtGPS, 1.0);

       // GAL
//     equC5E.addVariable(dcdtGAL, 1.0); 
//     equC1E.addVariable(dcdtGAL, 1.0); 

       // only the code has isb
       equC1E.addVariable(dcdtGPS, 1.0); 
       equC5E.addVariable(dcdtGPS, 1.0); 
       equC1E.addVariable(isbGAL, 1.0); 
       equC5E.addVariable(isbGAL, 1.0); 

       // only the code has isb
       equC2C.addVariable(dcdtGPS, 1.0);
       equC6C.addVariable(dcdtGPS, 1.0);
       equC2C.addVariable(isbBDS, 1.0);
       equC6C.addVariable(isbBDS, 1.0);

       //> GPS ionospheric delays
       equC1G.addVariable(ionoC1GVar,  1.0);
       equC2G.addVariable(ionoC1GVar,  GAMMA_GPS_L1L2);

       // GAL
       equC1E.addVariable(ionoC1EVar,  1.0); 
       equC5E.addVariable(ionoC1EVar,  GAMMA_GAL_L1L5); 

       //> BDS
       equC2C.addVariable(ionoC2CVar,  1.0);
       equC6C.addVariable(ionoC2CVar,  GAMMA_BDS_L2L6);


       // define the weight for all combinations relative PC21GPS

       // Phase equations should 
       double varCodeGPS  = 1.0 ;
       double varCodeGAL  = 1.0 ;
       double varCodeBDS  = 1.0 ;

       equC2G.setWeight(1/varCodeGPS);
       equC1G.setWeight(1/varCodeGPS);

       equC5E.setWeight(1/varCodeGAL); 
       equC1E.setWeight(1/varCodeGAL); 

       equC6C.setWeight(1/varCodeBDS);
       equC2C.setWeight(1/varCodeBDS);

       ///////////
       // SETTING THE RULES: SETUP EQUATION SYSTEM
       ///////////

       std::set<Equation> equSet;
       equSet.insert(equC1G);
       equSet.insert(equC2G);

       equSet.insert(equC1E);
       equSet.insert(equC5E);

       equSet.insert(equC2C);
       equSet.insert(equC6C);

       // YOU MUST clear the equation system firstly!!!!!
       // only current equations is feed to equSystem
       equSystem.clearEquations();

       for(auto equ:equSet)
       {
          equSystem.addEquation(equ);
       }
       
   } // end of setUpEquations


      /* Code to be executed before 'Compute()' method.
       *
       * @param gData    Data object holding the data.
       */
   void FilterSPP::Process(Rx3ObsData& rxData)
       noexcept(false)
   {
      try
      {
         if(debug)
         {
             cout << "epoch:" << YDSTime(rxData.currEpoch) << endl;

             std::set<Equation> descripEquSet;
             descripEquSet = equSystem.getDescripEqus();
             cout << "size:" << descripEquSet.size() << endl;
             for(std::set<Equation>::iterator it=descripEquSet.begin();
                     it!=descripEquSet.end();
                     ++it)
             {
                 (*it).dump(cout);
             }
         }

         // reset filter
         CommonTime epoch( rxData.currEpoch );

         if(debug)
             cout << "before equSystem" << endl;

         // Prepare the equation system with current data
         equSystem.Prepare(epoch, source, rxData.stvData);

         if(debug)
             cout << "after equSystem" << endl;

         // Get the number of unknowns being processed
         int numUnknowns( equSystem.getTotalNumVariables() );

         // Get the set with unknowns being processed
         currentUnkSet =  equSystem.getVarUnknowns() ;

         std::set<Equation> currentEquSet;
         currentEquSet = equSystem.getCurrentEquationsSet();

         if(debug)
         {
             std::set<Equation> currentEquSet;
             currentEquSet = equSystem.getCurrentEquationsSet();
             for(std::set<Equation>::iterator it=currentEquSet.begin();
                     it!=currentEquSet.end();
                     ++it)
             {
                 (*it).dump(cout);
             }

             cout << "currentUnkSet are as follows:" << endl;
             for(VariableSet::iterator vIt=currentUnkSet.begin();
                     vIt!=currentUnkSet.end();
                     vIt++)
             {
                 cout << (*vIt) << endl;
             }
         }


         // Get matrices and vectors out of equation system
         // Measurements vector (Prefit-residuals)
         measVector = equSystem.getPrefitsVector();
         hMatrix = equSystem.getGeometryMatrix();
         rMatrix = equSystem.getWeightsMatrix();

         int numMeas = measVector.size();

         // Feed the filter with the correct state and covariance matrix
         if( firstTime )
         {

            VectorXd initialState = Eigen::VectorXd::Zero(numUnknowns);
            MatrixXd initialCov = MatrixXd::Zero( numUnknowns, numUnknowns);

               // Fill the initialCov matrix
            for( auto v: currentUnkSet )
            {
                int index = v.getNowIndex();
                initialCov(index,index) = v.getInitialVariance();
            }

               // Reset Kalman filter state and covariance matrix
            xhat = initialState; 
            P    = initialCov;

               // No longer first time
            firstTime = false;
         }
         else
         {

               // Adapt the size to the current number of unknowns
            VectorXd currentState = VectorXd::Zero(numUnknowns);
            MatrixXd currentCov   = MatrixXd::Zero(numUnknowns, numUnknowns);

               // Fill the state vector
            for( auto v: currentUnkSet )
            {
                int now_index = v.getNowIndex();
                int old_index = v.getPreIndex();
                if( old_index != -1)
                {
                    currentState(now_index) = solution( old_index );
                }
            }

//////////////////////////////////////////////////////
//
//          注意的是，此时的dx应该设置为零.

            // Fill the covariance matrix, 2019/08/01, shjzhang
            // modify the code, 2020/01/16, shjzhang
            Variable dxVar = getVariable(TypeID::dX, currentUnkSet);
            Variable dyVar = getVariable(TypeID::dY, currentUnkSet);
            Variable dzVar = getVariable(TypeID::dZ, currentUnkSet);

            currentState(dxVar.getNowIndex()) = 0.0;
            currentState(dyVar.getNowIndex()) = 0.0;
            currentState(dzVar.getNowIndex()) = 0.0;
//
//////////////////////////////////////////////////////

            // We need a copy of 'currentUnkSet'
            VariableSet tempSet(currentUnkSet);
            for( auto v: currentUnkSet ) 
            {
                int nowIndex = v.getNowIndex();
                int oldIndex = v.getPreIndex();

                // old Variable
                if( oldIndex != -1 )
                {
                    for( auto v2: tempSet )
                    {
                        int nowIndex2 = v2.getNowIndex();
                        int oldIndex2 = v2.getPreIndex();

                        // old Variable
                        if( oldIndex2 != -1 )
                        {
                           currentCov(nowIndex, nowIndex2) 
                               = currentCov(nowIndex2, nowIndex) 
                               = covMatrix(oldIndex, oldIndex2);
                        }
                        else // new variable
                        {
                           currentCov(nowIndex, nowIndex2) 
                               = currentCov(nowIndex2, nowIndex) 
                               = 0.0;
                        }
                    }
                }
                else // new variable
                {
                    // only diagnol element is assigned to the new variable
                    currentCov(nowIndex, nowIndex) = v.getInitialVariance();
                }

                // erase var
                tempSet.erase(v);

            }  // End of for( VariableSet::const_iterator itVar1 = currentUnkSet...'

               // Reset Kalman filter to current state and covariance matrix
            xhat = currentState;
            P    = currentCov;

         }  // End of 'if(firstTime)'


         // State Transition Matrix (PhiMatrix)
         phiMatrix = equSystem.getPhiMatrix();
         qMatrix = equSystem.getQMatrix();

         if(debug)
         {
            cout << "measVector" << endl;
            cout <<  measVector<< endl;

            cout << "hMatrix" << endl;
            cout <<  hMatrix<< endl;

            cout << "rMatrix" << endl;
            cout <<  rMatrix  << endl;

            cout << "phiMatrix" << endl;
            cout <<  phiMatrix  << endl;

            cout << getClassName() << endl;
            cout << "qMatrix" << endl;
            cout <<  qMatrix  << endl;
         }

         /////////////////////////////
         // time-update
         /////////////////////////////

         // Compute the a priori state vector
         xhatminus = phiMatrix*xhat;

         MatrixXd phiT(phiMatrix.transpose());

         // Compute the a priori estimate error covariance matrix
         Pminus = phiMatrix*P*phiT + qMatrix;

         /////////////////////////////
         // meas-update
         /////////////////////////////

         // After checking sizes, let's do the real correction work
         MatrixXd invPMinus;

         try
         {
            invPMinus = Pminus.inverse();
         }
         catch(...)
         {
            InvalidSolver e("MeasUpdate(): Unable to compute invPMinus matrix.");
            cerr << e << endl;
            exit(-1);
         }

         MatrixXd hMatrixT( hMatrix.transpose() );

         // detect outliers 
         while(true)
         {
             // 卫星数不能少于10颗, 卫星数太少，假设检验是不可靠的, 退出异常检测
             if(rxData.stvData.numSats() <=10)
             {
                 break;
             }

             // Compute the a posteriori error covariance matrix
             try
             {
                MatrixXd invTemp( hMatrixT * rMatrix * hMatrix + invPMinus );

                P = invTemp.inverse() ;
                xhat = P * ( (hMatrixT * rMatrix * measVector) + (invPMinus * xhatminus) );
             }
             catch(Exception e)
             {
                InvalidSolver eis("MeasUpdate(): Unable to compute xhat.");
                cerr << eis << endl;
                exit(-1);
             }

             solution = xhat;
             covMatrix = P;

                // Compute the postfit residuals Vector
             VectorXd postfitVec = measVector - (hMatrix * solution);

             VectorXd VXVec( xhat - xhatminus );

             //cout << endl; cout << VXVec << endl;

             double totalVTPV, VTPV(0.0),VxPVx(0.0);
             for( int i = 0; i < numMeas; ++i)
             {
                 VTPV += postfitVec(i)*postfitVec(i)*rMatrix(i,i);
             }

             for( int i = 0; i<xhat.size(); ++i)
             {
                 double VxP(0.0);

                 for( int j = 0; j<xhat.size(); ++j)
                 {
                     VxP += VXVec(j)*invPMinus(j,i);
                 }

                 VxPVx += VxP*VXVec(i);
             }

             totalVTPV = VTPV + VxPVx;
             
             // Different from LS
             double sigma = std::sqrt( totalVTPV/numMeas);

             if(debug)
             {
                 cout << "sigma:" << sigma << endl;
             }

             double freedom = numMeas;

             //////////////////
             // 假设检验剔除粗差
             //////////////////
             CProbability x2Test;

             // alpha = 0.01
             double alpha = 0.95;
             double thresP2 = x2Test.re_chi2F(freedom, (1+alpha)/2.0);
             double thresP1 = x2Test.re_chi2F(freedom, (1-alpha)/2.0);

             if(debug)
             {
                 cout << "totalVTPV" << endl;
                 cout <<  totalVTPV << endl;
                 cout << "thresP2" << endl;
                 cout <<  thresP2  << endl;
                 cout << "thresP1" << endl;
                 cout <<  thresP1  << endl;
             }

             // Check VPV
             if( totalVTPV < thresP2 )
             {
                 break;
             }

             MatrixXd RMatrix = rMatrix.inverse();

             // else , local hypothesis test
             MatrixXd QVV = RMatrix - hMatrix*covMatrix* hMatrix.transpose();

             // Suppose sigma0 is known
             std::vector<double> standardResidual(numMeas,0.0);
             for(int i = 0; i < numMeas; ++i)
             {
                 standardResidual[i] = std::abs( postfitVec(i) ) /std::sqrt( QVV(i,i) );
             }

             if(debug)
             {
                 cout << "standardResidual" << endl;
                 for(int i=0; i<numMeas; i++)
                 {
                     cout << standardResidual[i] << " ";
                 }
                 cout << endl;
             }

             // Find the maximum standard residual
             std::vector<double>::iterator maxResidual =
                 std::max_element(std::begin(standardResidual),
                 std::end(standardResidual));

             double standMax = (*maxResidual);

             if(debug)
             {
                 cout << "standMax" << endl;
                 cout <<  standMax  << endl;
             }

             // Find the satellite with maximum residual
             int indexMax = std::distance(standardResidual.begin(), maxResidual);

             if(debug)
             {
                 cout << "indexMax" << endl;
                 cout << indexMax << endl;
             }

             CProbability x2TestLocal;

             // confidence level = 1%, much larger
             double thres2 = x2TestLocal.re_normF(0.99); // if sigma0 known

             if(debug)
             {
                 cout << "thres2" << endl;
                 cout <<  thres2  << endl;
             }

             // higher because of different code precision
             if( standMax > thres2 )
             {
                 // a lower weight 
                 rMatrix(indexMax, indexMax) = rMatrix(indexMax, indexMax)/100.0;

                 // 删除卫星 
                 Equation equ = findEquation(currentEquSet, indexMax);

                 SatID sat = equ.getEquationSat();
                 TypeID prefitType = equ.getIndepType();

                 // prefitType = prefitC1GDiff;
                 TypeID obsType = TypeID(prefitType.asString().substr(6,3));

                 // 卫星数过少，退出循环
                 if(rxData.stvData.numSats()<=10)
                 {
                     break;
                 }

                 rxData.stvData.removeSatID(sat);

                 if(debugBadObs)
                 {
                     cout << "bad obs:" << endl;
                     cout << YDSTime(epoch) << "sat" << sat << endl;
                     cout << "prefitType" << prefitType << endl;
                     cout << "obsType" << obsType << endl;
                 }
             }
             else
             {
                 break; // Must

             } // end of if(standMax > thres2)

         } // end of while(true)


      }
      catch(Exception& u)
      {
            // Throw an exception if something unexpected happens
         ProcessingException e( getClassName() + ":"
                                + asString( getIndex() ) + ":"
                                + u.what() );

         THROW(e);

      }

      return ;

   }  // End of method 'FilterSPP::preCompute()'

   Variable FilterSPP::getVariable( const TypeID& type, 
                                    const VariableSet& varSet) const
      noexcept(false)
   {

         // Declare an iterator for 'stateMap' and go to the first element
      VariableSet::const_iterator it = varSet.begin();

         // Look for a variable with the same type
      while( (*it).getType() != type &&
             it != varSet.end() )
      {
         ++it;

         // If the same type is not found, throw an exception
         if( it == varSet.end() )
         {
             InvalidRequest e("Type not found in varSet.");
             THROW(e);
         }
      }

         // Else, return the corresponding value
      return (*it);

   }  // End of method 'SolverGeneral::getSolution()'

      /* Returns the solution associated to a given TypeID.
       *
       * @param type    TypeID of the solution we are looking for.
       *
       * \warning In the case the solution contains more than one variable
       * of this type, only the first one will be returned.
       */
   double FilterSPP::getSolution( const TypeID& type, 
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
             InvalidRequest e(type.asString() + "not found in solution vector.");
             THROW(e);
         }

         varIt++;
      }

         // Else, return the corresponding value
      return stateVec( (*varIt).getNowIndex() );

   }  // End of method 'SolverGeneral::getSolution()'

      /* Returns the solution associated to a given TypeID.
       *
       * @param type    TypeID of the solution we are looking for.
       *
       * \warning In the case the solution contains more than one variable
       * of this type, only the first one will be returned.
       */
   double FilterSPP::getSolution( const Variable& var, 
                                    const VectorXd& stateVec ) const
      noexcept(false)
   {
         // Else, return the corresponding value
      return stateVec( (var).getNowIndex() );

   }  // End of method 'SolverGeneral::getSolution()'


   Triple FilterSPP::getDx()
   {
       double dx = getSolution(TypeID::dX, solution);
       double dy = getSolution(TypeID::dY, solution);
       double dz = getSolution(TypeID::dZ, solution);

       Triple delta;
       delta[0] = dx;
       delta[1] = dy;
       delta[2] = dz;

       return delta;
   }

   Equation FilterSPP::findEquation( std::set<Equation>& equSet,
                                        int index)
   {
       Equation tempEqu;

       int i(0);
       for(auto equ: equSet)
       {
           if(i==index)
           {
               tempEqu = equ;
               break;
           }

           i++;
       }

       return tempEqu;
   }


}  // End of namespace gnssSpace
