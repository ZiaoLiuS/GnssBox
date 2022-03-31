/// @file WeekSecond.cpp  Implement full week, mod week and seconds-of-week time
/// representation.

#include "WeekSecond.hpp"
#include "TimeConstants.hpp"
#include "MJD.hpp"

using namespace utilSpace;

namespace timeSpace
{
   WeekSecond& WeekSecond::operator=( const WeekSecond& right )
   {
      Week::operator=(right);
      sow = right.sow;
      return *this;
   }

   CommonTime WeekSecond::convertToCommonTime() const
   {
      try
      {
	      //int dow = static_cast<int>( sow * DAY_PER_SEC );
         // Appears to have rounding issues on 32-bit platforms

         int dow = static_cast<int>( sow / SEC_PER_DAY );
         // NB this assumes MJDEpoch is an integer - what if epoch H:M:S != 0:0:0 ?
         long jday = MJD_JDAY + MJDEpoch() + (7 * week) + dow;
         double sod(sow - SEC_PER_DAY * dow);
         CommonTime ct;
         return ct.set( jday,
                        static_cast<long>(sod),
                        sod - static_cast<long>(sod),
                        timeSystem );
      }
      catch (InvalidParameter& ip)
      {
         RETHROW(ip);
      }
   }

   void WeekSecond::convertFromCommonTime( const CommonTime& ct )
   {
      if(static_cast<MJD>(ct).mjd < MJDEpoch())
      {
         InvalidRequest ir("Unable to convert to Week/Second - before Epoch.");
         THROW(ir);
      }

      long jday, sod;
      double fsod;
      ct.get( jday, sod, fsod, timeSystem );
         // find the number of days since the beginning of the Epoch
      jday -= MJD_JDAY + MJDEpoch();
         // find out how many weeks that is
      week = static_cast<int>( jday / 7 );
         // find out what the day of week is
      jday %= 7;

      sow = static_cast<double>( jday * SEC_PER_DAY + sod ) + fsod;
   }

   bool WeekSecond::isValid() const
   {
      return ( Week::isValid() &&
               sow < FULLWEEK );
   }

   void WeekSecond::reset()
   {
      Week::reset();
      sow = 0.0;
   }

   bool WeekSecond::operator==( const WeekSecond& right ) const
   {
      return ( Week::operator==(right) &&
               sow == right.sow );
   }

   bool WeekSecond::operator!=( const WeekSecond& right ) const
   {
      return ( !operator==( right ) );
   }

   bool WeekSecond::operator<( const WeekSecond& right ) const
   {
      if( Week::operator<(right) )
      {
         return true;
      }
      if( Week::operator>(right) )
      {
         return false;
      }
      if( sow < right.sow )
      {
         return true;
      }
      return false;
   }

   bool WeekSecond::operator>( const WeekSecond& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool WeekSecond::operator<=( const WeekSecond& right ) const
   {
      return ( operator<( right ) || operator==( right ) );
   }

   bool WeekSecond::operator>=( const WeekSecond& right ) const
   {
      return ( !operator<( right ) );
   }

} // namespace
