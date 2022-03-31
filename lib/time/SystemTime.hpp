#pragma ident "$Id$"



#ifndef SYSTEMTIME_HPP
#define SYSTEMTIME_HPP

#include "UnixTime.hpp"
#include <ctime>

using namespace utilSpace;
namespace timeSpace
{
      /**
       * This class reads the current system time and stores it in
       * a UnixTime.
       */
   class SystemTime
      : public UnixTime
   {
   public:
         /**
          * @defgroup stbo SystemTime Basic Operations
          */
         //@{

         /**
          * Default Constructor.
          * Reads the system clock for the current system time.
          */
      SystemTime()
      {
         timeSystem = TimeSystem::UTC;
         update();
      }

         /// Virtual Destructor.
      virtual ~SystemTime()
      {}

         /**
          * Copy Constructor.
          * @param right a reference to the SystemTime object to copy.
          */
      SystemTime( const SystemTime& right )
            : UnixTime( right )
      {}

         //@}

         /**
          * Update this object to the current system time.
          * @return a reference to this SystemTime
          */
      SystemTime& update();
   };

}

#endif // SYSTEMTIME_HPP
