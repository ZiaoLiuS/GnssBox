
#include <cmath>
#include "TimeSystem.hpp"
#include "Exception.hpp"

using namespace std;
using namespace utilSpace;

namespace timeSpace
{
   // Static initialization of const std::strings for asString().
   // Must parallel enum Systems in TimeSystem.hpp.
   // NB: DO NOT use std::map here; on some systems initialization fails.
   const string TimeSystem::Strings[count] =
     {
       string("UNK"),
       string("Any"),
       string("GPS"),
       string("GLO"),
       string("GAL"),
       string("QZS"),
       string("BDT"),
       string("IRN"),
       string("UTC"),
       string("UT1"),
       string("TAI"),
       string("TT"),
       string("TDB"),
     };

   void TimeSystem::setTimeSystem(const Systems& sys)
   {
      if(sys < 0 || sys >= count)
         system = Unknown;
      else
         system = sys;
   }

   void TimeSystem::fromString(const string& str)
   {
      system = Unknown;
      for(int i=0; i<count; i++) {
         if(Strings[i] == str) {
            system = static_cast<Systems>(i);
            break;
         }
      }
   }

   ostream& operator<<(ostream& os, const TimeSystem& ts)
   {
      return os << ts.asString();
   }


}   // end namespace
