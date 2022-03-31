/// @file CivilTime.cpp

#include <cmath>
#include "CivilTime.hpp"
#include "ConvertCalendar.hpp"
#include "Exception.hpp"
#include "StringUtils.hpp"

#define debug 0

using namespace std;
using namespace utilSpace;

namespace timeSpace
{
      /// Long month names for converstion from numbers to strings
   const char * CivilTime::MonthNames[] =
   {
      "Error",
      "January","February", "March", "April",
      "May", "June","July", "August",
      "September", "October", "November", "December"
   };

      /// Short month names for converstion from numbers to strings
   const char * CivilTime::MonthAbbrevNames[] =
   {
      "err", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
   };

   CivilTime& CivilTime::operator=( const CivilTime& right )
   {
      year       = right.year;
      month      = right.month;
      day        = right.day;
      hour       = right.hour;
      minute     = right.minute;
      second     = right.second;
      timeSystem = right.timeSystem;
      return *this;
   }

   CommonTime CivilTime::convertToCommonTime() const
   {
      try
      {
            // get the julian day
         long jday = convertCalendarToJD( year, month, day );
            // get the second of day
         double sod = convertTimeToSOD( hour, minute, second );

         if(debug)
         {
             cout << sod << endl;
         }

            // make a CommonTime with jd, whole sod, and
            // fractional second of day
         CommonTime ct;
         return ct.set(  jday, static_cast<long>(sod) ,
                         (sod - static_cast<long>(sod)),
                         timeSystem );
      }
      catch (InvalidParameter& ip)
      {
         InvalidRequest ir(ip);
         THROW(ir);
      }
   }

   void CivilTime::convertFromCommonTime( const CommonTime& ct )
   {
      long jday, sod;
      double fsod;
         // get the julian day, second of day, and fractional second of day
      ct.get( jday, sod, fsod, timeSystem );
         // convert the julian day to calendar "year/month/day of month"
      convertJDtoCalendar( jday, year, month, day );
         // convert the (whole) second of day to "hour/minute/second"
      convertSODtoTime( static_cast<double>( sod ), hour, minute, second );
         // add the fractional second of day to "second"
      second += fsod;
   }


   bool CivilTime::isValid() const
   {
      CivilTime temp;
      temp.convertFromCommonTime( convertToCommonTime() );
      if( *this == temp )
      {
         return true;
      }
      return false;
   }

   void CivilTime::reset()
   {
      year = 0;
      month = day = 1;
      hour = minute = 0;
      second = 0.0;
      timeSystem = TimeSystem::Unknown;
   }

   bool CivilTime::operator==( const CivilTime& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
         return false;

      if( year   == right.year    &&
          month  == right.month   &&
          day    == right.day     &&
          hour   == right.hour    &&
          minute == right.minute  &&
          fabs(second - right.second) < CommonTime::eps )
      {
         return true;
      }
      return false;
   }

   bool CivilTime::operator!=( const CivilTime& right ) const
   {
      return ( !operator==( right ) );
   }

   bool CivilTime::operator<( const CivilTime& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
      {
         InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
         THROW(ir);
      }

      if( year < right.year )
      {
         return true;
      }
      if( year > right.year )
      {
         return false;
      }
      if( month < right.month )
      {
         return true;
      }
      if( month > right.month )
      {
         return false;
      }
      if( day < right.day )
      {
         return true;
      }
      if( day > right.day )
      {
         return false;
      }
      if( hour < right.hour )
      {
         return true;
      }
      if( hour > right.hour )
      {
         return false;
      }
      if( minute < right.minute )
      {
         return true;
      }
      if( minute > right.minute )
      {
         return false;
      }
      if( second < right.second )
      {
         return true;
      }

      return false;
   }

   bool CivilTime::operator>( const CivilTime& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool CivilTime::operator<=( const CivilTime& right ) const
   {
      return ( operator<( right ) || operator==( right ) );
   }

   bool CivilTime::operator>=( const CivilTime& right ) const
   {
      return ( !operator<( right ) );
   }

   std::string CivilTime::asString() const
   {
      ostringstream oss;

      oss << setfill('0')
          << setw(7) << year << " "
          << setw(8) << month << " "
          << setw(8) << day << " "
          << setw(8) << hour<< " "
          << setw(8) << minute << " "
          << fixed << setprecision(15) << setw(17) << second 
          << " " << timeSystem.asString() ;

      return oss.str();
   }

      // ----------- CivilTime operator<< --------------
      //
      // Stream output for CivilTime objects.  Typically used for debugging.
      // @param s stream to append formatted CivilTime to.
      // @param cit CivilTime to append to stream \c s.
      // @return reference to \c s.

   std::ostream& operator<<( std::ostream& s,
                             const CivilTime& cit )
   {
      s << cit.year << " "
        << cit.month << " "
        << cit.hour << " " 
        << cit.minute << " "
        << cit.second << " "
        << cit.timeSystem << " ";

      return s;
   }


} // namespace
