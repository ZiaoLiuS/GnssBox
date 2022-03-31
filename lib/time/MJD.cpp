/// @file MJD.hpp


#include <cmath>
#include "MJD.hpp"
#include "TimeConstants.hpp"

using namespace utilSpace;

namespace timeSpace
{
   MJD& MJD::operator=( const MJD& right )
   {
      mjd = right.mjd;
      timeSystem = right.timeSystem;
      return *this;
   }

   CommonTime MJD::convertToCommonTime() const
   {
      try
      {
            // convert to Julian Day
         long double tmp( mjd + MJD_JDAY );
            // get the whole number of days
         long jday( static_cast<long>( tmp ) );
            // tmp now holds the partial days
         tmp -= static_cast<long>( tmp );
            // convert tmp to seconds of day
         tmp *= SEC_PER_DAY;
            // Lose excess precision in 'tmp' because it may cause rounding
            // problems in the conversion to CommonTime.
         double dTmp( static_cast<double>( tmp ) );

         CommonTime ct;
         return ct.set( jday,
                        static_cast<long>( dTmp ),
                        dTmp - static_cast<long>( dTmp ),
                        timeSystem );
      }
      catch (InvalidParameter& ip)
      {
         InvalidRequest ir(ip);
         THROW(ip);
      }

   }

   void MJD::convertFromCommonTime( const CommonTime& ct )
   {
      long jday, sod;
      double fsod;
      ct.get( jday, sod, fsod, timeSystem );

      mjd =  static_cast<long double>( jday - MJD_JDAY ) +
           (  static_cast<long double>( sod )
            + static_cast<long double>( fsod ) ) * DAY_PER_SEC;
   }

   bool MJD::isValid() const
   {
      MJD temp;
      temp.convertFromCommonTime( convertToCommonTime() );
      if( *this == temp )
      {
         return true;
      }
      return false;
   }

   void MJD::reset()
   {
      mjd = 0.0;
      timeSystem = TimeSystem::Unknown;
   }

   bool MJD::operator==( const MJD& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
         return false;

      if( fabs(mjd - right.mjd) < CommonTime::eps )
      {
         return true;
      }
      return false;
   }

   bool MJD::operator!=( const MJD& right ) const
   {
      return ( !operator==( right ) );
   }

   bool MJD::operator<( const MJD& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
      {
         InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
         THROW(ir);
      }

      if( mjd < right.mjd )
      {
         return true;
      }
      return false;
   }

   bool MJD::operator>( const MJD& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool MJD::operator<=( const MJD& right ) const
   {
      return ( operator<(  right ) ||
               operator==( right )   );
   }

   bool MJD::operator>=( const MJD& right ) const
   {
      return ( !operator<( right ) );
   }

} // namespace
