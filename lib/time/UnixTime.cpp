/// @file UnixTime.cpp


#include "Exception.hpp"
#include "UnixTime.hpp"
#include "TimeConstants.hpp"
#include "CommonTime.hpp"
#include "Exception.hpp"

using namespace utilSpace;

namespace timeSpace
{
   UnixTime& UnixTime::operator=( const UnixTime& right )
   {
      tv.tv_sec  = right.tv.tv_sec ;
      tv.tv_usec = right.tv.tv_usec;
      timeSystem = right.timeSystem;
      return *this;
   }

   CommonTime UnixTime::convertToCommonTime() const
   {
      try
      {
         CommonTime ct;
         return ct.set( ( MJD_JDAY + UNIX_MJD + tv.tv_sec / SEC_PER_DAY ),
                        ( tv.tv_sec % SEC_PER_DAY ),
                        ( static_cast<double>( tv.tv_usec ) * 1e-6 ),
                        timeSystem );
      }
      catch (InvalidParameter& ip)
      {
         InvalidRequest ir(ip);
         THROW(ip);
      }
   }

   void UnixTime::convertFromCommonTime( const CommonTime& ct )
   {
         /// This is the earliest CommonTime for which UnixTimes are valid.
      static const CommonTime MIN_CT = UnixTime(0, 0, TimeSystem::Any).convertToCommonTime();
         /// This is the latest CommonTime for which UnixTimes are valid.
         /// (2^31 - 1) s and 999999 us
      static const CommonTime MAX_CT = UnixTime(2147483647, 999999, TimeSystem::Any).convertToCommonTime();

      if ( ct < MIN_CT || ct > MAX_CT )
      {
         InvalidRequest ir("Unable to convert given CommonTime to UnixTime.");
         THROW(ir);
      }

      long jday, sod;
      double fsod;
      ct.get( jday, sod, fsod, timeSystem );

      tv.tv_sec = (jday - MJD_JDAY - UNIX_MJD) * SEC_PER_DAY + sod;

         // round to the nearest microsecond
      tv.tv_usec = static_cast<time_t>( fsod * 1e6 + 0.5 ) ;

      if (tv.tv_usec >= 1000000)
      {
         tv.tv_usec -= 1000000;
         ++tv.tv_sec;
      }
   }

   bool UnixTime::isValid() const
   {
      UnixTime temp;
      temp.convertFromCommonTime( convertToCommonTime() );
      if( *this == temp )
      {
         return true;
      }
      return false;
   }

   void UnixTime::reset()
   {
      tv.tv_sec = tv.tv_usec = 0;
      timeSystem = TimeSystem::Unknown;
   }

   bool UnixTime::operator==( const UnixTime& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
         return false;

      if( tv.tv_sec == right.tv.tv_sec  &&
          abs(tv.tv_usec - right.tv.tv_usec) < CommonTime::eps )
      {
         return true;
      }
      return false;
   }

   bool UnixTime::operator!=( const UnixTime& right ) const
   {
      return ( !operator==( right ) );
   }

   bool UnixTime::operator<( const UnixTime& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
      {
         InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
         THROW(ir)
      }

      if( tv.tv_sec  <  right.tv.tv_sec )
      {
         return true;
      }
      if( tv.tv_sec  == right.tv.tv_sec  &&
          tv.tv_usec <  right.tv.tv_usec   )
      {
         return true;
      }
      return false;
   }

   bool UnixTime::operator>( const UnixTime& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool UnixTime::operator<=( const UnixTime& right ) const
   {
      return ( operator<( right ) ||
               operator==( right ) );
   }

   bool UnixTime::operator>=( const UnixTime& right ) const
   {
      return ( !operator<( right ) );
   }

} // namespace
