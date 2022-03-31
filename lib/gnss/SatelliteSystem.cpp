
//*****************************************
// 
// Class to manage satellite system 
//
// Modification
// 2020/01/24 
// created by shjzhang
//
// copywright
// shjzhang
//
//*****************************************

#include <cmath>
#include "SatelliteSystem.hpp"
#include "Exception.hpp"

using namespace std;

using namespace utilSpace;

namespace gnssSpace
{
   // Static initialization of const std::strings for toString().
   // Must parallel enum Systems in SatelliteSystem.hpp.
   // NB: DO NOT use std::map here; on some systems initialization fails.
   const string SatelliteSystem::sytemStrings[count] =
     { "UNK", 
       "GPS", 
       "GLONASS", 
       "Galileo", 
       "BDS", 
       "BD2",
       "BD3",
       "SBAS", 
       "QZSS", 
       "IRNSS",
       "MIXED"
     };

   const string SatelliteSystem::sytemString3[count] =
     { "UNK", 
       "GPS", 
       "GLO", 
       "GAL", 
       "BDS", 
       "BD2",
       "BD3",
       "SBA", 
       "QZS", 
       "IRN",
       "MIX"
     };

   const char SatelliteSystem::systemChars[count] =
     { '?', 'G', 'R', 'E', 'C','C','C', 'S', 'J', 'I', 'M' };

   void SatelliteSystem::setSatelliteSystem(const Systems& sys)
   {
      if(sys < 0 || sys >= count)
         system = Unknown;
      else
         system = sys;
   }

   void SatelliteSystem::fromString(const string& str)
   {
      system = Unknown;
      for(int i=0; i<count; i++) {
         if(sytemStrings[i] == str) {
            system = static_cast<Systems>(i);
            break;
         }
      }
   }

   void SatelliteSystem::fromChar(const char& ch)
   {
      system = Unknown;
      switch(ch)
      {
         case ' ': case 'G': case 'g':
            system = GPS;
            break;
         case 'R': case 'r':
            system = GLONASS;
            break;
         case 'E': case 'e':
            system = Galileo;
            break;
         case 'C': case 'c':
            system = BDS;
            break;
         case 'S': case 's':
            system = SBAS;
            break;
         case 'J': case 'j':
            system = QZSS;
            break;
         case 'I': case 'i':
            system = IRNSS;
            break;
         case 'M': case 'm':
            system = Mixed;
            break;
         default:                   // non-SP3 system character
            Exception e(std::string("Invalid system character \"")
                        + ch + std::string("\""));
            THROW(e);
      }
   }

}   // end namespace
