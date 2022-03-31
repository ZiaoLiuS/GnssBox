#pragma ident "$Id$"

/**
 * @file EquSysForPoint.hpp
 * Class to define and handle complex equation systems for solvers.
 */

#ifndef EquSysForPoint_HPP
#define EquSysForPoint_HPP

//============================================================================
//
//  2015/07/01  
//  design a new 'equationSystem' for the generation of the equations
//  for 'SolverGeneral'
//
//  2020/09/03
//  add dump for descriptionEquation output 
//
//============================================================================


#include <algorithm>

#include "DataStructures.hpp"
#include "StochasticModel.hpp"
#include "Equation.hpp"
#include "Rx3ObsData.hpp"
#include <Eigen/Eigen>

using namespace Eigen;

using namespace utilSpace;
using namespace timeSpace;
using namespace mathSpace;
using namespace gnssSpace;

namespace gnssSpace
{


      /** This class defines and handles complex equation systems for solvers,
       *  generating the Vectors and Matrices they will need, including:
       *
       *    \li Measurements Vector (prefit residuals).
       *    \li Geometry matrix.
       *    \li Equation weights matrix.
       *    \li State Transition Matrix (PhiMatrix).
       *    \li Process noise covariance matrix (QMatrix).
       *
       * In addition, this class will provide important/interesting information
       * such as number of variables, satellites and data sources being
       * processed.
       *
       * In order to achieve this, the 'EquSysForPoint' class will start from a
       * set of 'Equation descriptions' and the currently available GNSS data.
       * From there, it will deduce the unknowns corresponding to each data
       * source as well as the specific equations.
       *
       * A typical way to use this class follows, showing how to set up an
       * object to perform "Precise Point Positioning" (PPP):
       *
       * @code
       *
       *      // DEFINE VARIABLES
       *
       *      // Declare stochastic models to be used
       *   StochasticModel coordinatesModel;
       *   TropoRandomWalkModel tropoModel;
       *   PhaseAmbiguityModel ambiModel;
       *
       *      // These variables are, by default, SourceID-indexed
       *   Variable dx( TypeID::dx, &coordinatesModel, true, false, 100.0 );
       *
       *   Variable dy( TypeID::dy, &coordinatesModel );
       *   dy.setInitialVariance( 100.0 );     // Equivalent to 'dx' setup
       *
       *   Variable dz( TypeID::dz, &coordinatesModel );
       *   dz.setInitialVariance( 100.0 );
       *
       *   Variable cdt( TypeID::cdt );
       *   cdt.setDefaultForced(true);   // Force default coefficient (1.0)
       *
       *   Variable tropo( TypeID::wetMap, &tropoModel );
       *   tropo.setInitialVariance( 25.0 );
       *
       *
       *      // The following variable is, SourceID and SatID-indexed
       *   Variable ambi( TypeID::BLC, &ambiModel, true, true );
       *   ambi.setDefaultForced(true);   // Force default coefficient (1.0)
       *
       *      // This will be the independent term for code equation
       *   Variable prefitC( TypeID::prefitC );
       *
       *      // This will be the independent term for phase equation
       *   Variable prefitL( TypeID::prefitL );
       *
       *
       *      // DESCRIBE EQUATIONS
       *
       *      // Define Equation object for code equations, and add variables
       *   Equation equPC( prefitC );
       *
       *   equPC.addVariable(dx);
       *   equPC.addVariable(dy);
       *   equPC.addVariable(dz);
       *   equPC.addVariable(cdt);
       *   equPC.addVariable(tropo);
       *
       *      // Define Equation object for phase equations, and add variables
       *   Equation equLC( prefitL );
       *
       *   equLC.addVariable(dx);
       *   equLC.addVariable(dy);
       *   equLC.addVariable(dz);
       *   equLC.addVariable(cdt);
       *   equLC.addVariable(tropo);
       *   equLC.addVariable(ambi);       // This variable is for phase only
       *
       *      // Phase equations should have higher relative weight
       *   equLC.setWeight(10000.0);     // 100.0 * 100.0
       *
       *
       *      // SETUP EQUATION SYSTEM
       *
       *      // Create 'EquSysForPoint' object
       *   EquSysForPoint eqSystem;
       *
       *      // Add equation descriptions
       *   eqSystem.addEquation(equPC);
       *   eqSystem.addEquation(equLC);
       *
       * @endcode
       *
       * In this way, rather complex processing strategies may be set up in a
       * handy and flexible way.
       *
       * \warning Please be aware that this class requires a significant amount
       * of overhead. Therefore, if your priority is execution speed you should
       * either use the already provided 'purpose-specific' solvers (like
       * 'SolverPPP' or hand-code your own class. For new processing strategies
       * you should balance the importance of machine time (extra overhead)
       * versus researcher time (writing a new solver).
       *
       * @sa Variable.hpp, Equation.hpp.
       *
       */
   class EquSysForPoint
   {
   public:

         /// Default constructor
      EquSysForPoint()
         : isPrepared(false)
      {};


         /** Add a new equation to be managed.
          *
          * @param equation   Equation object to be added.
          */
      virtual EquSysForPoint& addEquation( const Equation& equation );


         /// Remove all Equation objects from this EquSysForPoint.
      virtual EquSysForPoint& clearEquations();


         /** Prepare this object to carry out its work.
          *
          * @param gData   Map of GNSS data structures (GDS), indexed
          *                by SourceID.
          */
      virtual EquSysForPoint& Prepare( CommonTime& epoch, 
                                       SourceID& source, 
                                       satTypeValueMap& satData);


      // setUp index for current variables
      virtual void setUpIndex();


         /** Return the TOTAL number of variables being processed.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual int getTotalNumVariables() const
         noexcept(false);


         /** Return the set containing all variables being processed.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual VariableSet getVarUnknowns() 
         noexcept(false);


         /** Get prefit residuals GPSTk Vector, given the current equation
          *  system definition and the GDS' involved.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual VectorXd getPrefitsVector() const
         noexcept(false);


         /** Get geometry matrix, given the current equation system definition
          *  and the GDS' involved.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual MatrixXd getGeometryMatrix() const
         noexcept(false);


         /** Get weights matrix, given the current equation system definition
          *  and the GDS' involved.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual MatrixXd getWeightsMatrix() const
         noexcept(false);


         /** Get the State Transition Matrix (PhiMatrix), given the current
          *  equation system definition and the GDS' involved.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual MatrixXd getPhiMatrix() const
         noexcept(false);


         /** Get the Process Noise Covariance Matrix (QMatrix), given the
          *  current equation system definition and the GDS' involved.
          *
          * \warning You must call method Prepare() first, otherwise this
          * method will throw an InvalidEquSysForPoint exception.
          */
      virtual MatrixXd getQMatrix() const
         noexcept(false);


         /// Get the number of equation descriptions being currently processed.
      virtual int getEquationDefinitionNumber() const
      { return equDescripSet.size(); };

         /// Get the set of current equations.
      virtual int getEquNumber() const
      { return currentEquSet.size(); };

         /// Get the set of current equations.
      virtual std::set<Equation> getDescripEqus() const
      { return equDescripSet; };

         /// Get the set of current equations.
      virtual std::set<Equation> getCurrentEquationsSet() const
      { return currentEquSet; };

      void dumpDescripEquations(std::ostream& os) 
      {
          for(auto it=equDescripSet.begin(); it!=equDescripSet.end(); it++)
          {
              cout << (*it) << endl;
          }
      }

      void dumpEquations(std::ostream& os) 
      {
          for(auto it=currentEquSet.begin(); it!=currentEquSet.end(); it++)
          {
              cout << (*it) << endl;
          }
      }

         /// Prepare set of current unknowns and set of current equations
      VariableSet prepareEquations( SourceID& source, satTypeValueMap& satData );

         /// Compute phiMatrix and qMatrix
      void preparePhiQ(CommonTime& epoch);

         /// Compute prefit residuals vector
      void preparePrefitGeometryWeights();



         /// Destructor
      virtual ~EquSysForPoint() {};


   private:

      SourceID source;

      /// Set containing the DESCRIPTIONS of Equation objects.
      std::set<Equation> equDescripSet;

      /// set of current equations
      std::set<Equation> currentEquSet;

      // Current set of unknowns
      VariableSet currentUnkSet;

      /// Old set of unknowns
      VariableSet oldUnkSet;

      /// Whether or not this EquSysForPoint is ready to be used
      bool isPrepared;

         /// State Transition Matrix (PhiMatrix)
      MatrixXd phiMatrix;

         /// Process noise covariance matrix (QMatrix)
      MatrixXd qMatrix;

         /// Geometry matrix
      MatrixXd hMatrix;

         /// Weights matrix
      MatrixXd rMatrix;

         /// Measurements vector (Prefit-residuals)
      VectorXd measVector;

         /// General white noise stochastic model
      static WhiteNoiseModel whiteNoiseModel;


   }; // End of class 'EquSysForPoint'

}  // End of namespace gnssSpace

#endif   // equSys_HPP
