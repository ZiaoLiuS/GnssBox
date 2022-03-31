#pragma ident "$Id$"

/**
 * @file DumpRinex.hpp
 * This class dumps the values inside a GNSS Data Structure.
 */

//============================================================================
//
//  2009-2011
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2009, 2011
//
//  2019/10/29
//  shjzhang, only print typeid that exist!
//
//============================================================================


#include "StringUtils.hpp"
#include "DumpRinex.hpp"

using namespace utilSpace;

namespace gnssSpace
{

      // Return a string identifying this object.
   std::string DumpRinex::getClassName() const
   { return "DumpRinex"; }


      /* Dumps data from a gnnsRinex object.
       *
       * @param gData    Data object holding the data.
       */
   satTypeValueMap& DumpRinex::Process( CommonTime& epoch, 
                                        satTypeValueMap& gData )
      noexcept(false)
   {

      try
      {

            // Iterate through all items in the GNSS Data Structure
         for( satTypeValueMap::const_iterator it = gData.begin();
              it!= gData.end();
              it++ )
         {

             // system
             SatelliteSystem sys = (*it).first.system;

             // get typeSet for current sys
             TypeIDSet typeSet = sysTypes[sys];

             // Declare a 'YDSTime' object to ease printing
             YDSTime time( epoch );

             (*outStr) << fixed
                       << setprecision(6) 
                       << time.year << " "
                       << time.doy << " "
                       << rightJustify(utilSpace::asString(time.sod,6), 12) << " ";

             (*outStr) << markerName << " ";

             // Then, print satellite (system and PRN)
             (*outStr) << (*it).first << " ";

                 // Iterate through all 'tvMap'
             for( typeValueMap::const_iterator itObs = (*it).second.begin();
                  itObs != (*it).second.end();
                  itObs++ )
             {
                 if( typeSet.find((*itObs).first) != typeSet.end() )
                 {

                     (*outStr) << (*itObs).first << " ";

                     (*outStr) << fixed
                               << setprecision(3)
                               << (*itObs).second << " ";

                 }

             }  // End of 'for( typeValueMap::const_iterator itObs = ... )'

             // Print end of line
             (*outStr) << std::endl;

         }  // End of 'for( satTypeValueMap::const_iterator it = ...'

      }
      catch(Exception& u)
      {
            // Throw an exception if something unexpected happens
         ProcessingException e( getClassName() + ":"
                                + u.what() );

         THROW(e);

      }

   }  // End of method 'DumpRinex::Process()'

}  // End of namespace gnssSpace
