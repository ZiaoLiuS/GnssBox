/// @file YDSTime.cpp

#include <cmath>
#include <fstream>
#include <iostream>

#include "YDSTime.hpp"
#include "ConvertCalendar.hpp"
#include "StringUtils.hpp"

using namespace utilSpace;

using namespace std;

namespace timeSpace
{
// YDSTime constant corresponding to CommonTime::BEGINNING_OF_TIME


   const YDSTime YDSTime::BEGIN_TIME(CommonTime::BEGINNING_OF_TIME);

   YDSTime& YDSTime::operator=( const YDSTime& right )
   {
      year = right.year;
      doy  = right.doy;
      sod  = right.sod;
      timeSystem = right.timeSystem;
      return *this;
   }

   CommonTime YDSTime::convertToCommonTime() const
   {
      try
      {
         long jday = convertCalendarToJD( year, 1, 1 ) + doy - 1;
         CommonTime ct;
         return ct.set( jday, sod, timeSystem );
      }
      catch ( InvalidParameter& ip )
      {
         InvalidRequest ir(ip);
         THROW(ir);
      }
   }

   void YDSTime::convertFromCommonTime( const CommonTime& ct )
   {
      long jday, secDay;
      double fsecDay;
      ct.get( jday, secDay, fsecDay, timeSystem );
      sod = static_cast<double>( secDay ) + fsecDay;

      int month, day;
      convertJDtoCalendar( jday, year, month, day );
      doy = jday - convertCalendarToJD( year, 1, 1 ) + 1;
   }

   bool YDSTime::isValid() const
   {
      YDSTime temp;
      temp.convertFromCommonTime( convertToCommonTime() );
      if( *this == temp )
      {
         return true;
      }
      return false;
   }

   void YDSTime::reset()
   {
      year = doy = 0;
      sod = 0.0;
      timeSystem = TimeSystem::Unknown;
   }

   bool YDSTime::operator==( const YDSTime& right ) const
   {
     /// Any (wildcard) type exception allowed, otherwise must be same time systems
      if ((timeSystem != TimeSystem::Any &&
           right.timeSystem != TimeSystem::Any) &&
          timeSystem != right.timeSystem)
         return false;

      if( year == right.year &&
          doy  == right.doy  &&
          fabs(sod - right.sod) < CommonTime::eps )
      {
         return true;
      }
      return false;
   }

   bool YDSTime::operator!=( const YDSTime& right ) const
   {
      return ( !operator==( right ) );
   }

   bool YDSTime::operator<( const YDSTime& right ) const
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
      if( doy < right.doy )
      {
         return true;
      }
      if( doy > right.doy )
      {
         return false;
      }
      if( sod < right.sod )
      {
         return true;
      }
      return false;
   }

   bool YDSTime::operator>( const YDSTime& right ) const
   {
      return ( !operator<=( right ) );
   }

   bool YDSTime::operator<=( const YDSTime& right ) const
   {
      return ( operator<( right ) || operator==( right ) );
   }

   bool YDSTime::operator>=( const YDSTime& right ) const
   {
      return ( !operator<( right ) );
   }

      // ----------- YDSTime operator<< --------------
      //
      // Stream output for YDSTime objects.  Typically used for debugging.
      // @param s stream to append formatted YDSTime to.
      // @param t YDSTime to append to stream \c s.
      // @return reference to \c s.

   std::ostream& operator<<( std::ostream& s,
                             const YDSTime& yt )
   {
      s << yt.year << " "
        << setw(3) <<  yt.doy << " "
        << fixed <<  setprecision(6) << setw(14) << yt.sod << " "
        << yt.timeSystem.asString();

      return s;
   }


} // namespace
