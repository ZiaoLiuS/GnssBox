#pragma ident "$Id$"

/**
 * @file PrintSols.hpp
 * This class dumps the values inside a GNSS Data Structure (GDS).
 */

#ifndef PrintSols_HPP
#define PrintSols_HPP

//============================================================================
//
//  2022-01-22
//  create this program for printing sols
//  shoujian zhang
//
//============================================================================


#include <ostream>
#include "Exception.hpp"
#include "CommonTime.hpp"
#include "Rx3ObsData.hpp"

using namespace utilSpace;
using namespace timeSpace;
using namespace gnssSpace;

namespace gnssSpace
{

      /** This class print solutions into files.
       *
       * A typical way to use this class follows:
       */
   class PrintSols                         
   {
   public:

      /// Default constructor
      PrintSols()
      {};


      /** Common constructor
       *
       * @param out           Stream object used for output.
       * @param printtype     Flag to print TypeID's.
       * @param printtime     Flag to print CommonTime's.
       * @param printstation  Flag to print SourceID's.
       *
       */
      PrintSols( std::ofstream& out)
         : outStr(&out)
      { };


      /** Sets stream object used for output.
       *
       * @param out           Stream object used for output.
       */
      PrintSols& setOutputStream( std::ofstream& out )
      { outStr = &out; return (*this); };


      // print header of solutions
      inline void printHeader()
      {
          if(outStr!=NULL)
          {
              (*outStr) 
                 << "# year doy sod numOfSat lat lon height x y z" << endl;
              (*outStr) 
                 << "# end_of_header" << endl;
          }

      };

      // print record of solutions
      inline void printRecord( const CommonTime& epoch, 
                               const int& numSats,
                               const Position& pos )
      {
          if(outStr!=NULL)
          {
              double lat = pos.getGeodeticLatitude();
              double lon = pos.getLongitude();
              double height = pos.getAltitude();

              (*outStr) 
                 << fixed
                 << setprecision(6)
                 << YDSTime(epoch).year << " "
                 << YDSTime(epoch).doy  << " "
                 << rightJustify( asString(YDSTime(epoch).sod,6), 13  ) << " "
                 << numSats<< " "
                 << setprecision(10)
                 << lat << " "
                 << lon << " "
                 << setprecision(3)
                 << height  << " "
                 << pos[0] << " "
                 << pos[1] << " "
                 << pos[2] << " "
                 << endl;
          }
      };

      // print record of solutions
      inline void printRecord( const CommonTime& epoch, 
                               const int& numSats,
                               const Position& pos,
                               double isbGAL,
                               double isbBDS)
      {
          if(outStr!=NULL)
          {
              double lat = pos.getGeodeticLatitude();
              double lon = pos.getLongitude();
              double height = pos.getAltitude();

              (*outStr) 
                 << fixed
                 << setprecision(6)
                 << YDSTime(epoch).year << " "
                 << YDSTime(epoch).doy  << " "
                 << rightJustify( asString(YDSTime(epoch).sod,6), 13  ) << " "
                 << numSats<< " "
                 << setprecision(10)
                 << lat << " "
                 << lon << " "
                 << setprecision(3)
                 << height  << " "
                 << pos[0] << " "
                 << pos[1] << " "
                 << pos[2] << " "
                 << isbGAL << " "
                 << isbBDS << " "
                 << endl;
          }
      };

      void printPrefitDiff(satTypeValueMap &gmap,CommonTime curr){
          if(outStr!=NULL)
          {

              for (satTypeValueMap::iterator it = gmap.begin();
                   it != gmap.end();
                   ++it){
                  SatID sat = (*it).first;

                  if(sat.system == SatelliteSystem::GPS){
                      (*outStr)<<
                        sat<<" "<<
                        curr<<" "<<
                        (*it).second[TypeID::prefitC1GDiff]<<" "<<
                        (*it).second[TypeID::prefitC2GDiff]<<" "<<
                        (*it).second[TypeID::prefitL1GDiff]<<" "<<
                        (*it).second[TypeID::prefitL2GDiff]<<" "<<endl;



                  } else if(sat.system == SatelliteSystem::BDS){
                      (*outStr)<<
                        sat<<" "<<
                        curr<<" "<<
                        (*it).second[TypeID::prefitC2CDiff]<<" "<<
                        (*it).second[TypeID::prefitC6CDiff]<<" "<<
                        (*it).second[TypeID::prefitL2CDiff]<<" "<<
                        (*it).second[TypeID::prefitL6CDiff]<<" "<<endl;
                  }

              }



          }
      }




      /// Return a string identifying this object.
      inline std::string getClassName(void) const
      {
          return "PrintSols";
      }


      /// Destructor
      virtual ~PrintSols() {};


   private:

      string markerName;

      /// Stream object used for output.
      std::ofstream* outStr;

      std::map<SatelliteSystem, TypeIDSet> sysTypes;


   }; // End of class 'PrintSols'

}  // End of namespace gnssSpace

#endif  // PrintSols_HPP
