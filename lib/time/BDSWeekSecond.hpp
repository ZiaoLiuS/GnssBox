/// @file BDSWeekSecond.hpp  Define BDS week and seconds-of-week; inherits WeekSecond

#ifndef BDSWEEKSECOND_HPP
#define BDSWEEKSECOND_HPP


#include "Exception.hpp"
#include "WeekSecond.hpp"

using namespace utilSpace;

namespace timeSpace
{
   /// This class handles the week and seconds-of-week of the BDS TimeTag classes.
   /// The BDS week is specified by
   /// 13-bit ModWeek, rollover at 8192, bitmask 0x1FFF and epoch BDS_EPOCH_MJD
   class BDSWeekSecond : public WeekSecond
   {
   public:

      /// Constructor.
      BDSWeekSecond(unsigned int w = 0,
                       double s = 0.,
                       TimeSystem ts = TimeSystem::BDT)
         : WeekSecond(w,s)
      { timeSystem = ts; }

      /// Constructor from CommonTime
      BDSWeekSecond( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

      /// Destructor.
      ~BDSWeekSecond() throw() {}

      // the rest define the week rollover and starting time

      /// Return the number of bits in the bitmask used to get the ModWeek from the
      /// full week.
      int Nbits(void) const
      {
         static const int n=13;
         return n;
      }

      /// Return the bitmask used to get the ModWeek from the full week.
      int bitmask(void) const
      {
         static const int bm=0x1FFF;
         return bm;
      }

      /// Return the Modified Julian Date (MJD) of epoch for this system.
      long MJDEpoch(void) const
      {
         static const long e=BDS_EPOCH_MJD;
         return e;
      }

   }; // end class BDSWeekSecond

} // namespace

#endif // BDSWEEKSECOND_HPP
