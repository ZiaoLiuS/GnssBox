#pragma ident "$Id$"


#include "SystemTime.hpp"
//#include <sys/time.h> // #include <ctime>      // for gettimeofday()

using namespace utilSpace;
namespace timeSpace
{
   SystemTime& SystemTime::update()
   {
      gettimeofday( &tv, NULL );
      return *this;
   }

} // namespace
