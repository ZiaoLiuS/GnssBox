/// @file QZSWeekSecond.hpp  Define QZS week and seconds-of-week; inherits WeekSecond

#ifndef QZSWEEKSECOND_HPP
#define QZSWEEKSECOND_HPP

#include "Exception.hpp"
#include "WeekSecond.hpp"

using namespace utilSpace;

namespace timeSpace
{
   /// This class handles the week and seconds-of-week of the QZS TimeTag classes.
   /// The QZS week is specified by (GPS without the rollover)
   /// 16-bit ModWeek, rollover at 65535, bitmask 0xFFFF and epoch QZS_EPOCH_MJD
   class QZSWeekSecond : public WeekSecond
   {
   public:

      /// Constructor.
      QZSWeekSecond(unsigned int w = 0,
                       double s = 0.,
                       TimeSystem ts = TimeSystem::QZS)
         : WeekSecond(w,s)
      { timeSystem = ts; }

      /// Constructor from CommonTime
      QZSWeekSecond( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

      /// Destructor.
      ~QZSWeekSecond() {}

      
      /// Return the number of bits in the bitmask used to get the ModWeek from the
      /// full week.
      int Nbits(void) const
      {
         static const int n=16;
         return n;
      }

      /// Return the bitmask used to get the ModWeek from the full week.
      int bitmask(void) const
      {
         static const int bm=0xFFFF;
         return bm;
      }

      /// Return the Modified Julian Date (MJD) of epoch for this system.
      long MJDEpoch(void) const
      {
         static const long e=QZS_EPOCH_MJD;
         return e;
      }

   }; // end class QZSWeekSecond

} // namespace

#endif // QZSWEEKSECOND_HPP
