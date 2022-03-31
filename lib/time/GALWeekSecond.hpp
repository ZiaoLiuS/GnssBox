/// @file GALWeekSecond.hpp  Define GAL week and seconds-of-week; inherits WeekSecond

#ifndef GALWEEKSECOND_HPP
#define GALWEEKSECOND_HPP

#include "WeekSecond.hpp"

using namespace utilSpace;
namespace timeSpace
{
   /// This class handles the week and seconds-of-week of the GAL TimeTag classes.
   /// The GAL week is specified by
   /// 12-bit ModWeek, rollover at 4096, bitmask 0xFFF and epoch GAL_EPOCH_MJD
   class GALWeekSecond : public WeekSecond
   {
   public:

      /// Constructor.
      GALWeekSecond(unsigned int w = 0,
                       double s = 0.,
                       TimeSystem ts = TimeSystem::GAL) throw()
         : WeekSecond(w,s)
      { timeSystem = ts; }

      /// Constructor from CommonTime
      GALWeekSecond( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

      /// Destructor.
      ~GALWeekSecond() throw() {}

      /// Return the number of bits in the bitmask used to get the ModWeek from the
      /// full week.
      int Nbits(void) const
      {
         static const int n=12;
         return n;
      }

      /// Return the bitmask used to get the ModWeek from the full week.
      int bitmask(void) const
      {
         static const int bm=0xFFF;
         return bm;
      }

      /// Return the Modified Julian Date (MJD) of epoch for this system.
      long MJDEpoch(void) const
      {
         static const long e=GAL_EPOCH_MJD;
         return e;
      }

   }; // end class GALWeekSecond

} // namespace

#endif // GALWEEKSECOND_HPP
