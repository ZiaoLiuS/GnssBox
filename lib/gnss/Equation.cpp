
/**
 * @file Equation.cpp
 * GNSS Data Structure to define and handle 'descriptions' of GNSS equations.
 */

#include "Equation.hpp"

using namespace utilSpace;

namespace gnssSpace
{


      // Assignment operator for equationHeader
   equationHeader& equationHeader::operator=(const equationHeader& right)
   {

      if ( this == &right )
      {
         return (*this);
      }

      equationSource = right.equationSource;
      equationSat = right.equationSat;
      indTerm = right.indTerm;
      satSys = right.satSys;
      orderIndex = right.orderIndex;
      constWeight = right.constWeight;

      return (*this);

   }  // End of 'equationHeader::operator='



      // Default constructor.
   Equation::Equation()
   {

      header.equationSource = SourceID("unknown");
      header.equationSat = SatID("00");
      header.constWeight = 1.0;

   }  // End of 'Equation::Equation()'



      /* Common constructor. It defines an Equation from its independent
       * term. You must later use other methods to input the variables.
       *
       * @param indep     Variable object describing the independent term.
       */
   Equation::Equation( const Variable& indep , SatelliteSystem::Systems sys)
   {

      header.equationSource = SourceID("unknown");
      header.equationSat = SatID("00");
      header.indTerm = indep;
      header.satSys = sys;
      header.constWeight = 1.0;

   }  // End of 'Equation::Equation()'



      /* Common constructor. It defines an Equation from its independent
       * term. You must later use other methods to input the variables.
       *
       * @param var     TypeID object describing the independent term.
       */
   Equation::Equation( const TypeID& type, 
                       SatelliteSystem::Systems sys,
                       const int& index)
   {

      header.equationSource = SourceID("unknown");
      header.equationSat = SatID("00");
      header.indTerm = Variable(type);
      header.satSys = sys;
      header.constWeight = 1.0;
      header.orderIndex = index;

   }  // End of 'Equation::Equation()'

}  // End of namespace gnssSpace
