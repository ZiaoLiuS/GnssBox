#pragma ident "$Id$"

#ifndef GPSWEEK_HPP
#define GPSWEEK_HPP

#include "TimeSystem.hpp"
#include "Exception.hpp"
#include "CommonTime.hpp"

using namespace utilSpace;

namespace timeSpace
{
      /** 
       * This class is an abstract base class inherited by other
       * GPS-time-related classes. It is abstract. 
       * It handles the week portion of the GPS TimeTag classes.
       *
       * The GPSWeek class inherits from TimeTag and handles the epoch and
       * 10-bit week special cases:
       *  getEpoch() returns week / 1024,
       *  getWeek10() returns week % 1024,
       *  getEpochWeek10(int& e, int& w) e = getEpoch(); w = getWeek10();
       *  setEpoch(int e) sets week = (week & 0x3FF) | (e << 10);
       *  setWeek10(int w) sets week = (week & ~0x3FF) | (w & 0x3FF);
       *  setEpochWeek10(int e, int w) calls setEpoch(e); setWeek(w);
       *
       * GPSWeekSecond inherits from GPSWeek. 
       *
       */
   class GPSWeek 
   {
   public:
         /// This is a 10-bit mask used in the several special Epoch and
         /// 10-bit Week methods.
      static const int bits10 = 0x3FF;
         /// This is the greatest week value for which a conversion to or
         /// from CommonTime would work.
      static const int MAX_WEEK;

         /// Constructor.
      GPSWeek( int w = 0,
               TimeSystem ts = TimeSystem::Unknown )
            : week(w)
      { timeSystem = ts; }

         /// Virtual Destructor.
      virtual ~GPSWeek()
      {}

         /// Assignment Operator.
      GPSWeek& operator=(const GPSWeek& right);

         /// @name Comparison Operators.
         //@{
      inline bool operator==(const GPSWeek& right) const
      {
        /// Any (wildcard) type exception allowed, otherwise must be same time systems
         if ((timeSystem != TimeSystem::Any &&
              right.timeSystem != TimeSystem::Any) &&
             timeSystem != right.timeSystem)
            return false;

         return week == right.week;
      }

      inline bool operator!=(const GPSWeek& right) const
      {
        /// Any (wildcard) type exception allowed, otherwise must be same time systems
         if ((timeSystem != TimeSystem::Any &&
              right.timeSystem != TimeSystem::Any) &&
             timeSystem != right.timeSystem)
         {
            InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
            THROW(ir);
         }

         return week != right.week;
      }

      inline bool operator<(const GPSWeek& right) const
      {
        /// Any (wildcard) type exception allowed, otherwise must be same time systems
         if ((timeSystem != TimeSystem::Any &&
              right.timeSystem != TimeSystem::Any) &&
             timeSystem != right.timeSystem)
         {
            InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
            THROW(ir);
         }

         return week < right.week;
      }

      inline bool operator<=(const GPSWeek& right) const
      {
        /// Any (wildcard) type exception allowed, otherwise must be same time systems
         if ((timeSystem != TimeSystem::Any &&
              right.timeSystem != TimeSystem::Any) &&
             timeSystem != right.timeSystem)
        {
            InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
            THROW(ir);
         }

         return week <= right.week;
      }

      inline bool operator>(const GPSWeek& right) const
      {
         if ((timeSystem != TimeSystem::Any &&
              right.timeSystem != TimeSystem::Any) &&
             timeSystem != right.timeSystem)
         {
            InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
            THROW(ir);
         }

         return week > right.week;
      }

      inline bool operator>=(const GPSWeek& right) const
      {

        if ((timeSystem != TimeSystem::Any &&
             right.timeSystem != TimeSystem::Any) &&
             timeSystem != right.timeSystem)
        {
            InvalidRequest ir("CommonTime objects not in same time system, cannot be compared");
            THROW(ir);
         }

         return week >= right.week;
      }
         //@}

         /// @name Special Epoch and 10-bit Week Methods.
         /// @todo Should the "set" methods return a reference?
         //@{
      inline virtual unsigned int getEpoch() const
      {
         return week >> 10;
      }

      inline virtual unsigned int getWeek10() const
      {
         return week & bits10;
      }

      inline virtual void getEpochWeek10(unsigned int& e,
                                         unsigned int& w) const
      {
         e = getEpoch();
         w = getWeek10();
      }

      inline virtual void setEpoch(unsigned int e)
      {
         week &= bits10;
         week |= e << 10;
      }

      inline virtual void setWeek10(unsigned int w)
      {
         week &= ~bits10;
         week |= w & bits10;
      }

      inline virtual void setEpochWeek10(unsigned int e,
                                         unsigned int w)
      {
         setEpoch(e);
         setWeek10(w);
      }

      virtual bool isValid() const
      {
         return (week >= 0 && week <= MAX_WEEK);
      }

      inline virtual void reset()
      {
         week = 0;
      }

         /// Force this interface on this classes descendants.
      virtual unsigned int getDayOfWeek() const = 0;

      int week;
      TimeSystem timeSystem;
   };

} // namespace

#endif // GPSTIME_HPP
