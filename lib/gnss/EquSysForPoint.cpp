#pragma ident "$Id: EquSysForPoint.cpp 2939 2012-04-06 19:55:11Z shjzhang $"

/**
 * @file EquSysForPoint.cpp
 * Class to define and handle complex equation systems for solvers.
 */

//============================================================================
//
//  History 
//
//  2012/06/19
//  Modify of the Method "preparePhiQ()". The gnssRinex is needed to 'Prepare' 
//  the stochastic model. When the variable is source-indexed, the gnssRinex 
//  can be given by finding the source in satData, but when the variable is 
//  only satellite-indexed, the gnssRinex is the first gnssRinex which "SEE"
//  this satellite. (2012.06.19)
//
//  2012/09/01
//  Change 
//  "currentUnkSet.insert( currentUnknowns.begin(), currentUnknowns.end() );"
//  to 
//  "currentUnkSet = currentUnknowns;"
//
//  2015/07/05 
//  Re-design the whole equation system for SolverGeneral/SolverGeneral2
//
//  2019/08/18
//  modify prepareEquations(); shjzhang
//
//  2022/01/15
//  make the code simple for point-positioning  
//
//  2022/01/20
//  replace all Matrix<double>/Vector<double> with Eigen:MatrixXd and Eigen::VectorXd
//  shoujian zhang 
//
//============================================================================


#include "CommonTime.hpp"
#include "EquSysForPoint.hpp"
#include <iterator>

#define debug 0
#define debugArc 0

using namespace utilSpace;
using namespace timeSpace;
using namespace mathSpace;

namespace gnssSpace
{



      // General white noise stochastic model
   WhiteNoiseModel EquSysForPoint::whiteNoiseModel;


      /* Add a new equation to be managed.
       *
       * @param equation   Equation object to be added.
       *
       */
   EquSysForPoint& EquSysForPoint::addEquation( const Equation& equation )
   {

         // Add "equation" to "equDescripSet"
      equDescripSet.insert(equation);

         // We must "Prepare()" this EquSysForPoint
      isPrepared = false;

      return (*this);

   }  // End of method 'EquSysForPoint::addEquation()'


      // Remove all Equation objects from this EquSysForPoint.
   EquSysForPoint& EquSysForPoint::clearEquations()
   {
         // First, clear the "equDescripSet"
      equDescripSet.clear();

      isPrepared = false;

      return (*this);

   }  // End of method 'EquSysForPoint::clearEquations()'


      /* Prepare this object to carry out its work.
       *
       * @param satData     Map of GNSS data structures (GDS), indexed
       *                   by SourceID.
       *
       */
   EquSysForPoint& EquSysForPoint::Prepare( CommonTime& epoch,
                                            SourceID& source, 
                                            satTypeValueMap& satData )
   {

      // Prepare set of current unknowns and set of current equations
      currentUnkSet = prepareEquations(source, satData);

      // Compute phiMatrix and qMatrix
      preparePhiQ(epoch);

      // Build prefit residuals vector
      preparePrefitGeometryWeights();

      // Setup indexs for vars
      setUpIndex();

      // Let's start storing 'current' unknowns set from 'previous' epoch
      oldUnkSet = currentUnkSet;

      // Set this object as "prepared"
      isPrepared = true;

      return (*this);

   }  // End of method 'EquSysForPoint::Prepare()'



      // Prepare set of current unknowns and set of current equations
   VariableSet EquSysForPoint::prepareEquations(SourceID& source, satTypeValueMap& satData )
   {

      // Let's clear the current equations set
      currentEquSet.clear();

      // Let's create 'currentUnks' set
      VariableSet currentUnks;

      SourceID currentSource = source;

      // Iterate the satellite and create the equations
      for( auto sd: satData )
      {
          SatID currentSat( (sd.first));
          typeValueMap tvData (sd.second) ;

            // Visit each "Equation" in "equDescripSet"
          for( auto equ: equDescripSet )
          {
              // We need a copy of current Equation object description
              Equation tempEquation( equ );

              // 判断卫星系统
              if( tempEquation.getSystem() == currentSat.system )
              {
                  tempEquation.header.equationSource = currentSource;
                  tempEquation.header.equationSat = currentSat;
                  tempEquation.header.typeValueData = tvData;

                  Variable varIndep = tempEquation.getIndependentTerm();
                  TypeID obsType = varIndep.getType();

                  if(tvData.find(obsType)!=tvData.end())
                  {
                        // Remove all the variables from this equation
                     tempEquation.clear();
                     bool found(true);
                     for( auto vc : equ.body )
                     {
                           // We will work with a copy of current Variable
                        Variable var( vc.first );
                        TypeID varType(var.getType());

                        Coefficient coef( vc.second);
                        if( !coef.isConst )
                        {
                            if(tvData.find(coef.coeffType) == tvData.end())
                            {
                                found = false;
                            }
                        }

                        // Check what type of variable we are working on
                        if( var.getSourceIndexed() )
                        {
                            var.setSource( currentSource );
                        }

                        if( var.getSatIndexed() )
                        {
                            var.setSatellite( currentSat );
                        }

                        //////////////
                        //
                        // 随机模型分为两类:
                        // (1) 需要时间的
                        // (2) 需要状态的, 模糊度
                        //
                        // this is:
                        // we can divide the var to arc-indexed and un-arc-indexed.
                        //
                        /////////////////////
                        if( var.getArcIndexed() )
                        {
                            // 模糊度参数, BL1G
                            int length = varType.asString().length();
                            TypeID satArcType = TypeID( "satArc" + varType.asString().substr(1,length) );
                            if(debugArc)
                            {
                                cout << "EquSysForPoint:" << endl;
                                cout << "satArcType" << endl;
                                cout <<  satArcType  << endl;
                            }

                            double arcNum;
                            try
                            {
                                arcNum = tvData(satArcType);
                                var.setArc(arcNum);
                            }
                            catch(Exception& e)
                            {
                                cerr << e << endl;
                                exit(-1);
                            }

/*
                            typeValueMap tempTypeValueData;
                            tempTypeValueData[satArcType] = arcNum;

                            // only store satArc data into typeValueData for current variable
                            var.setTypeValueData(tempTypeValueData);
*/

                        }

                        // 将tvData存入到未知参数中，以方便后续形成随机模型的phi/q
                        var.setTypeValueData(tvData);

                        // 将构建随机模型需要的数据存储到未知参数中存储起来
  
                        currentUnks.insert(var);
                        tempEquation.addVariable(var, coef);

                     }  // End of 'for( VarCoeffMap::const_iterator varIt = ...'

                        // New equation is complete: Add it to 'currentEquSet'
                     if(found)
                     {
                        currentEquSet.insert( tempEquation );
                     }

                  }
              }

          }  // End of 'for( satTypeValueMap::const_iterator ...'

      }  // End of 'for(std::set<Equation>::const_iterator)'

      if(currentEquSet.size() ==0)
      {
          InvalidEquSysForPoint e("equation number is zero!");
          THROW(e);
      }

         // Return set of current unknowns
      return currentUnks;

   }  // End of method 'EquSysForPoint::prepareEquations()'



      // Compute PhiMatrix
   void EquSysForPoint::preparePhiQ(CommonTime& epoch)
   {
    
      const int numVar( currentUnkSet.size() );

      // Resize phiMatrix and qMatrix
      phiMatrix=MatrixXd::Zero( numVar, numVar);
      qMatrix = MatrixXd::Zero(numVar, numVar);

      int i(0);
      for( auto var: currentUnkSet)
      {
          // We will work with a copy of current Variable
          typeValueMap tData = var.getTypeValueData();

          SourceID varSource = var.getSource();
          SatID    varSat = var.getSatellite();

          // Prepare variable's stochastic model
          var.getModel()->Prepare(epoch,
                                  varSource, 
                                  varSat, 
                                  tData);

          // This variable is 'old'; compute its phi and q values
          phiMatrix(i,i) = var.getModel()->getPhi();
          qMatrix(i,i)   = var.getModel()->getQ();

          i++;
      }

      return;

   }  // End of method 'EquSysForPoint::preparePhiQ()'


   // Compute prefit residuals vector
   void EquSysForPoint::preparePrefitGeometryWeights()
   {

         // Declare temporal storage for values
      std::vector<double> tempPrefit;

         // Total number of the current equations
      int numEqu( currentEquSet.size() );
      int numVar( currentUnkSet.size() );

      if(numVar==0)
      {
         THROW(InvalidEquSysForPoint("currentUnkSet is empty, you must set it first!"));
      }

      // Resize hMatrix and rMatrix
      // MUST INITIALIZE WITH ZERO METHOD !!!!
      hMatrix = MatrixXd::Zero( numEqu, numVar);
      rMatrix = MatrixXd::Zero(numEqu, numEqu);

      // We need an equation index
      int row(0);

      // Visit each Equation in "currentEquSet"
      for( auto equ:currentEquSet )
      {
         // Get the type value data from the header of the equation
         typeValueMap tData( equ.header.typeValueData );

         // Get the independent type of this equation
         TypeID indepType( equ.header.indTerm.getType() );

         // Temp measurement
         double tempMeas(tData(indepType));

         // insert current 'measurment vector' into 'tempPrefit'
         tempPrefit.push_back(tempMeas);

         // First, fill weights matrix
         // Check if current 'tData' has weight info. If you don't want those
         // weights to get into equations, please don't put them in GDS
         if( tData.find(TypeID::weight) != tData.end() )
         {
            // Weights matrix = Equation weight * observation weight
            rMatrix(row,row) = equ.header.constWeight * tData(TypeID::weight);
         }
         else
         {
            // Weights matrix = Equation weight
            rMatrix(row,row) = equ.header.constWeight;
         }

         // Second, fill geometry matrix: Look for equation coefficients
         
         // Now, let's visit all Variables and the corresponding 
         // coefficient in this equation description
         for( auto vc: equ.body )
         {
               // We will work with a copy of current Variable
            Variable var( vc.first );
            Coefficient coef( vc.second );

               // Coefficient values
            double tempCoef(0.0);
            if( coef.isConst )
            {
               // Use default coefficient
               tempCoef = coef.constCoeff;
            }
            else
            {
               // Look for the coefficient in 'tdata'
               TypeID type = coef.coeffType;

               // Check if this type has an entry in current GDS type set
               if( tData.find(type) != tData.end() )
               {
                  // If type was found, insert value into hMatrix
                  tempCoef = tData(type);
               }
               else
               {
                  cerr << "unknown coefficient for current vars" << endl;
                  InvalidEquSysForPoint e("can't find coefficient for current var");
                  exit(-1);
               }

            }  // End of 'if( (*itCol).isDefaultForced() ) ...'

               // Now, Let's get the position of this variable in 
               // 'currentUnkSet'
            int col(0);
            VariableSet::const_iterator itVar2=currentUnkSet.begin();
            while( (*itVar2) != var )
            {
               col++;
               itVar2++;
            }

               // Set the geometry matrix
            hMatrix(row, col) = tempCoef;
              
         }  // End of 'for( VarCoeffMap::const_iterator varIt = ...'

            // Increment row number
         row++;

      }  // End of 'for( std::set<Equation>::const_iterator itEq = ...'

         // Then, finally get prefit residuals into appropriate gnssSpace::Vector
      measVector 
          = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(tempPrefit.data(), tempPrefit.size()); ;

      return;

   }  // End of method 'EquSysForPoint::preparePrefitGeometryWeights()'


      /* Return the TOTAL number of variables being processed.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   int EquSysForPoint::getTotalNumVariables() const
      noexcept(false)
   {

         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }

      return currentUnkSet.size();

   }  // End of method 'EquSysForPoint::getTotalNumVariables()'



      /* Return the set containing all variables being processed.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   VariableSet EquSysForPoint::getVarUnknowns() 
      noexcept(false)
   {
         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }

      return currentUnkSet;

   }  // End of method 'EquSysForPoint::getVarUnknowns()'



      /* Get prefit residuals GPSTk Vector, given the current equation
       *  system definition and the GDS' involved.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   VectorXd EquSysForPoint::getPrefitsVector() const
      noexcept(false)
   {
         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }
      return measVector;

   }  // End of method 'EquSysForPoint::getPrefitsVector()'



      /* Get geometry matrix, given the current equation system definition
       *  and the GDS' involved.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   MatrixXd EquSysForPoint::getGeometryMatrix() const
      noexcept(false)
   {
         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }
      return hMatrix;
   }  // End of method 'EquSysForPoint::getGeometryMatrix()'



      /* Get weights matrix, given the current equation system definition
       *  and the GDS' involved.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   MatrixXd EquSysForPoint::getWeightsMatrix() const
      noexcept(false)
   {
         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }
      return rMatrix;

   }  // End of method 'EquSysForPoint::getWeightsMatrix()'


      /* Get the State Transition Matrix (PhiMatrix), given the current
       * equation system definition and the GDS' involved.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   MatrixXd EquSysForPoint::getPhiMatrix() const
      noexcept(false)
   {
         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }
      return phiMatrix;
   }  // End of method 'EquSysForPoint::getPhiMatrix()'



      /* Get the Process Noise Covariance Matrix (QMatrix), given the
       * current equation system definition and the GDS' involved.
       *
       * \warning You must call method Prepare() first, otherwise this
       * method will throw an InvalidEquSysForPoint exception.
       */
   MatrixXd EquSysForPoint::getQMatrix() const
      noexcept(false)
   {
         // If the object as not ready, throw an exception
      if (!isPrepared)
      {
         THROW(InvalidEquSysForPoint("EquSysForPoint is not prepared"));
      }
      return qMatrix;

   }  // End of method 'EquSysForPoint::getQMatrix()'


   void EquSysForPoint::setUpIndex()
   {

       // Then setup index for this epoch
       int nowIndex  = 0;
       VariableSet varSet;
       for( auto &var: currentUnkSet )
       {
           // set old index
           Variable nowVar = var; 
           auto preIter = oldUnkSet.find(nowVar);
           if( preIter != oldUnkSet.end() )
           {
               nowVar.setPreIndex( (*preIter).getNowIndex() );
           }


           /// set current index
           nowVar.setNowIndex( nowIndex );

           /// store the set
           varSet.insert(nowVar);

           /// index increment
           nowIndex++;
       }

       // copy 'varSet' to 'currentUnkSet'
       currentUnkSet = varSet;

   }  // End of method 'SolverPPPGNSS::SetupIndex()'


}  // End of namespace gnssSpace
