
#ifndef TIMESYSTEM_HPP
#define TIMESYSTEM_HPP

#include <iostream>
#include <string>

namespace timeSpace
{
   /// This class encapsulates time systems, including std::string I/O. This is an
   /// example of a 'smart enum' class.
   class TimeSystem
   {
   public:

      /// list of time systems supported by this class
      enum Systems
      {
         // add new systems BEFORE count, then
         // *** add to Strings[] in TimeSystem.cpp and make parallel to this enum. ***

         // Unknown MUST BE FIRST, and must = 0
         Unknown = 0, ///< unknown time frame; for legacy code compatibility
         Any,         ///< wildcard; allows comparison with any other type
         GPS,         ///< GPS system time
         GLO,         ///< GLONASS system time
         GAL,         ///< Galileo system time
         QZS,         ///< QZSS system time
         BDT,         ///< BDS system time
         IRN,         ///< IRNSS system time
         UTC,         ///< Coordinated Universal Time (e.g., from NTP)
         UT1,         ///< UT1
         TAI,         ///< International Atomic Time
         TT,          ///< Terrestrial time (used in IERS conventions)
         TDB,         ///< Barycentric dynamical time (JPL ephemeris); very near TT
         // count MUST BE LAST
         count        ///< the number of systems - not a system
      };

      /// Constructor, including empty constructor
      TimeSystem(const Systems sys = Unknown)
      {
         if(sys < 0 || sys >= count)
            system = Unknown;
         else
            system = sys;
      }

      /// constructor from int
      TimeSystem(int i)
      {
         if(i < 0 || i >= count)
            system = Unknown;
         else
            system = static_cast<Systems>(i);
      }

      // (copy constructor and operator= are defined by compiler)

      /// set the time system
      void setTimeSystem(const Systems& sys);

      /// get the time system
      Systems getTimeSystem() const
      { return system; }

      /// Return a std::string for each system (these strings are const and static).
      /// @return the std::string
      std::string asString() const
      { return Strings[system]; }

      /// define system based on input string
      /// @param str input string, expected to match output string for given system
      void fromString(const std::string& str);

      /// boolean operator==
      bool operator==(const TimeSystem& right) const
      { return system == right.system; }

      /// boolean operator< (used by STL to sort)
      bool operator<(const TimeSystem& right) const
      { return system < right.system; }

      // the rest follow from Boolean algebra...
      /// boolean operator!=
      bool operator!=(const TimeSystem& right) const
      { return !operator==(right); }

      /// boolean operator>=
      bool operator>=(const TimeSystem& right) const
      { return !operator<(right); }

      /// boolean operator<=
      bool operator<=(const TimeSystem& right) const
      { return (operator<(right) || operator==(right)); }

      /// boolean operator>
      bool operator>(const TimeSystem& right) const
      { return (!operator<(right) && !operator==(right)); }

   private:

      /// time system (= element of Systems enum) for this object
      Systems system;

      /// set of string labels for elements of Systems
      static const std::string Strings[];

      /// UT1 - UTC in seconds, |UT1mUTC| < 0.9s
//      double UT1mUTC;

   };   // end class TimeSystem

   /// Write name (asString()) of a TimeSystem to an output stream.
   /// @param os The output stream
   /// @param ts The TimeSystem to be written
   /// @return reference to the output stream
   std::ostream& operator<<(std::ostream& os, const TimeSystem& ts);

}   // end namespace

#endif // TIMESYSTEM_HPP
