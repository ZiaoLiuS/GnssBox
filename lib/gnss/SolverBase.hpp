#pragma ident "$Id$"

/**
 * @file SolverBase.hpp
 * Abstract base class for solver algorithms.
 */

#ifndef SOLVERBASE_HPP
#define SOLVERBASE_HPP

//============================================================================
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2006, 2007, 2008
//============================================================================



#include "Exception.hpp"
#include "CommonTime.hpp"
#include "Position.hpp"
#include <map>
#include <set>
#include <Eigen/Eigen>

using namespace Eigen;

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;

namespace gnssSpace
{


      // type definition for usage in other program
   typedef std::map<CommonTime, VectorXd> epochVectorMap;
   typedef std::map<CommonTime, MatrixXd> epochMatrixMap;
   typedef std::map<CommonTime, Position> epochPosMap;

      /**
       * Abstract base class for solver algorithms.
       */
   class SolverBase
   {
   public:


         /// Implicit constructor
      SolverBase() : valid(false) {};

         /// Return validity of results
      bool isValid(void)
      { return valid; }

         /// Solution
      VectorXd solution;

         /// Postfit-residuals.
      VectorXd postfitResiduals;

         /// Covariance matrix
      MatrixXd covMatrix;

         /// Destructor
      virtual ~SolverBase() {};


   protected:


        bool valid;         // true only if results are valid


   }; // End of class 'SolverBase'

}  // End of namespace gnssSpace
#endif   // SOLVERBASE_HPP
