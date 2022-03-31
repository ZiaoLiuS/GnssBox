/// @file JulianDate.cpp

#include <cmath>
#include "JulianDate.hpp"
#include "TimeConstants.hpp"

using namespace utilSpace;
namespace timeSpace
{
   JulianDate& JulianDate::operator=( const JulianDate& right )
   {
      jd = right.jd;
      timeSystem = right.timeSystem;
      return *this;
   }

   CommonTime JulianDate::convertToCommonTime() const
   {
      try
      {
         long double temp_jd( jd + 0.5 );
         long jday( static_cast<long>( temp_jd ) );
         long double sod =
            ( temp_jd - static_cast<long double>( jday ) ) * SEC_PER_DAY;

         CommonTime ct;
         return ct.set( jday,
                        static_cast<long>( sod ),
                        static_cast<double>( sod - static_cast<long>( sod ) ),
                        timeSystem );
      }
      catch (InvalidParameter& ip)
      {
         InvalidRequest ir(ip);
         THROW(ir);
      }
   }

   void JulianDate::convertFromCommonTime( const CommonTime& ct )
   {
      long jday, sod;
      double fsod;
      ct.get( jday, sod, fsod, timeSystem );

      jd =   static_cast<long double>( jday ) +
           (   static_cast<long double>( sod )
             + static_cast<long double>( fsod ) ) * DAY_PER_SEC
           - 0.5;
   }


   bool JulianDate::isValid() const
   {
      JulianDate temp;
      temp.convertFromCommonTime( convertToCommonTime() );
      if( *this == temp )
      {
         return true;
      }
      return false;
   }

   void JulianDate::reset()
   {
      jd = 0.0;
      timeSystem = TimeSystem::Unknown;
   }

   bool JulianDate::operator==( const JulianDate& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
         return false;

      if( fabs(jd - right.jd) < CommonTime::eps )
      {
         return true;
      }
      return false;
   }

   bool JulianDate::operator!=( const JulianDate& right ) const
   {
      return ( !operator==( right ) );
   }

   bool JulianDate::operator<( const JulianDate& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
      {
         InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
         THROW(ir);
      }

      if( jd < right.jd )
      {
         return true;
      }
      return false;
   }

   bool JulianDate::operator>( const JulianDate& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool JulianDate::operator<=( const JulianDate& right ) const
   {
      return ( operator<( right ) ||
               operator==( right ) );
   }

   bool JulianDate::operator>=( const JulianDate& right ) const
   {
      return ( !operator<( right ) );
   }

} // namespace
