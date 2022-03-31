/** @file GPSWeekSecond.hpp Define GPS week and seconds-of-week;
 * inherits WeekSecond */

#ifndef GPSWEEKSECOND_HPP
#define GPSWEEKSECOND_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include "WeekSecond.hpp"
#include "StringUtils.hpp"

using namespace utilSpace;
using namespace std;
namespace timeSpace
{
      /// This class handles GPS Week and Seconds-of-week. It inherits
      /// WeekSecond
      /// The GPS week is specified by 10-bit ModWeek, rollover at
      /// 1024, bitmask 0x3FF and epoch GPS_EPOCH_MJD
   class GPSWeekSecond : public WeekSecond
   {
   public:

         /// Constructor.
      GPSWeekSecond(unsigned int w = 0,
                    double s = 0.,
                    TimeSystem ts = TimeSystem::GPS)
            : WeekSecond(w,s)
      { timeSystem = ts; }

         /// Constructor from CommonTime
      GPSWeekSecond( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /// Destructor.
      ~GPSWeekSecond() {}

         /// Return the number of bits in the bitmask used to get the
         /// ModWeek from the full week.
      int Nbits(void) const
      {
         static const int n=10;
         return n;
      }

         /// Return the bitmask used to get the ModWeek from the full week.
      int bitmask(void) const
      {
         static const int bm=0x3FF;
         return bm;
      }

         /// Return the Modified Julian Date (MJD) of epoch for this system.
      long MJDEpoch(void) const
      {
         static const long e=GPS_EPOCH_MJD;
         return e;
      }

         /** @name WeekSecond Comparison Operators
          * All comparison operators have a parameter "right" which
          * corresponds to the GPSWeekSecond object to the right of
          * the symbol.  All comparison operators are const and return
          * true on success and false on failure. */
         //@{
      inline bool operator==( const GPSWeekSecond& right ) const
      {
         return WeekSecond::operator==( right );
      }
      inline bool operator!=( const GPSWeekSecond& right ) const
      {
         return WeekSecond::operator!=( right );
      }
      inline bool operator<( const GPSWeekSecond& right ) const
      {
         return WeekSecond::operator<( right );
      }
      inline bool operator>( const GPSWeekSecond& right ) const
      {
         return WeekSecond::operator>( right );
      }
      inline bool operator<=( const GPSWeekSecond& right ) const
      {
         return WeekSecond::operator<=( right );
      }
      inline bool operator>=( const GPSWeekSecond& right ) const
      {
         return WeekSecond::operator>=( right );
      }

      inline std::string asString() const
      {
         ostringstream oss;

         oss << setfill('0')
             << setw(7) << week << " "
             << setw(8) << sow << " "
             << " " << timeSystem.asString() ;

         return oss.str();
      }

         //@}


   }; // end class GPSWeekSecond

      //@}

} // namespace

#endif // GPSWEEKSECOND_HPP
