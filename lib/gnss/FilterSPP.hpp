
/**
 * @file FilterSPP.hpp
 * General Solver.
 */

#ifndef FilterSPP_HPP
#define FilterSPP_HPP

//============================================================================
//
//  Revision
//
//  2019/08/13  
//  make a generic PPP solver for multiple-gnss data.  
//
//  2020/01/16 
//  modify the variable-class to replace the old ambiguity class  
//
//  shjzhang    
//
//============================================================================


#include "SolverBase.hpp"
#include "TypeID.hpp"
#include "EquSysForPoint.hpp"
#include "StochasticModel.hpp"
#include "Rx3ObsData.hpp"

#include <Eigen/Eigen>

using namespace Eigen;
using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;

namespace gnssSpace
{

   ///////////////////////////////////////////////////////////////////
   // warning:
   // some solution realted data structures are defined in SolverBase
   ///////////////////////////////////////////////////////////////////

   class FilterSPP : public SolverBase                        
   {
   public:

      FilterSPP()
          : firstTime(true)
      {
          Init();
          setUpEquations();
      }

      void setSource(string& marker)
      {
          source = marker;
      };

      virtual void Process(Rx3ObsData& rxData);

         /// Returns an index identifying this object.
      virtual void Init(void) ;

         /// Returns an index identifying this object.
      virtual void setUpEquations(void) ;

      virtual Variable getVariable( const TypeID& type, 
                                    const VariableSet& varSet) const
         noexcept(false);

         /** Returns the solution associated to a given TypeID.
          *
          * @param type    TypeID of the solution we are looking for.
          *
          * \warning In the case the solution contains more than one variable
          * of this type, only the first one will be returned.
          */

      virtual Triple getDx() 
         noexcept(false);


      virtual double getSolution( const Variable& var,
                                  const VectorXd& stateVec ) const
         noexcept(false);

      virtual double getSolution( const TypeID& type,
                                  const VectorXd& stateVec ) const
         noexcept(false);

      Equation findEquation( std::set<Equation>& equSet,
                             int index);

      /// Returns an index identifying this object.
      virtual int getIndex(void) const;

      /// Returns a string identifying this object.
      virtual std::string getClassName(void) const;

      /// Destructor.
      virtual ~FilterSPP() {};

   private:

      SourceID source;

      /// Boolean indicating if this filter was run at least once
      bool firstTime;

      VariableSet currentUnkSet;
      VariableSet oldUnkSet;

      /// Boolean indicating if this filter was run at least once
      CommonTime firstEpoch;

         /// State Transition Matrix (PhiMatrix)
      MatrixXd phiMatrix;

         /// Noise covariance matrix (QMatrix)
      MatrixXd qMatrix;

         /// Geometry matrix
      MatrixXd hMatrix;

         /// Weights matrix
      MatrixXd rMatrix;

         /// Measurements vector (Prefit-residuals)
      VectorXd measVector;

         /// Map holding state information
      VariableDataMap stateMap;

         /// Map holding covariance information
      CovarianceDataMap covMap;

         // Predicted state
      VectorXd xhatminus;

         // Predicted covariance.
      MatrixXd Pminus;

         // A bwderiori state
      VectorXd xhat;

         // A bwderiori covariance.
      MatrixXd P;

      // default model for variable define
      WhiteNoiseModel defaultModel;

      // Declare coordinate model

      // kinematic
      // xyz
      WhiteNoiseModel whiteNoiseDXModel;
      WhiteNoiseModel whiteNoiseDYModel;
      WhiteNoiseModel whiteNoiseDZModel;

      // neu
      WhiteNoiseModel whiteNoiseDNModel;
      WhiteNoiseModel whiteNoiseDEModel;
      WhiteNoiseModel whiteNoiseDUModel;

      // Declare de/dn/du model
      // static positioning
      // xyz
      StochasticModel constantDXModel;
      StochasticModel constantDYModel;
      StochasticModel constantDZModel;
      // neu
      StochasticModel constantDNModel;
      StochasticModel constantDEModel;
      StochasticModel constantDUModel;

      // trop
      TropoRandomWalkModel tropoModel;
      IonoRandomWalkModel ionoC1GStoModel;
      IonoRandomWalkModel ionoC1EStoModel;
      IonoRandomWalkModel ionoC2CStoModel;

      // cdt
      WhiteNoiseModel dcdtGPSModel;
      WhiteNoiseModel dcdtGALModel;
      WhiteNoiseModel dcdtBDSModel;

      // Stochastic model  for ISB biases
      ISBRandomWalkModel   isbGPSModel;
      ISBRandomWalkModel   isbGALModel;
      ISBRandomWalkModel   isbBDSModel;

      std::map<CommonTime, Triple> postDxData;

      // Equation system
      EquSysForPoint equSystem;

         /// Initial index assigned to this class.
      static int classIndex;

         /// Index belonging to this object.
      int index;

         /// Sets the index and increment classIndex.
      void setIndex(void)
      { index = classIndex++; };


   }; // End of class 'FilterSPP'

}  // End of namespace gnssSpace

#endif   // FilterSPP_HPP
