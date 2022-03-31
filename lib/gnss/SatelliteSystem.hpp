
/// @file SatelliteSystem.hpp
///
/// shoujian zhang, 2019/12/01

#ifndef SatelliteSystem_HPP
#define SatelliteSystem_HPP

#include <iostream>
#include <string>

#include "Exception.hpp"

using namespace utilSpace;

namespace gnssSpace
{
   /// This class encapsulates satellite systems, including std::string I/O. This is an
   /// example of a 'smart enum' class.
   class SatelliteSystem
   {
   public:

      /// list of time systems supported by this class
      enum Systems
      {
         // add new systems BEFORE count, then
         // *** add to sytemStrings[] in SatelliteSystem.cpp and make parallel to this enum. ***

         // Unknown MUST BE FIRST, and must = 0
         Unknown = 0, ///< unknown sat system ; for legacy code compatibility
         GPS,         ///< GPS system time
         GLONASS,         ///< GLONASS system time
         Galileo,         ///< Galileo system time
         BDS,         ///< BDS system time
         BD2,
         BD3,
         SBAS,          ///< SBAS
         QZSS,         ///< QZSS system time
         IRNSS,         ///< IRNSS system time
         Mixed, 
         // count MUST BE LAST
         count        ///< the number of systems - not a system
      };

      /// Constructor, including empty constructor
      SatelliteSystem(const Systems sys = Unknown)
      {
         if(sys < 0 || sys >= count)
            system = Unknown;
         else
            system = sys;
      }

      /// constructor from int
      SatelliteSystem(int i)
      {
         if(i < 0 || i >= count)
            system = Unknown;
         else
            system = static_cast<Systems>(i);
      }

      /// constructor from int
      SatelliteSystem(char& sysChar)
      {
          fromChar(sysChar);
      }

      // (copy constructor and operator= are defined by compiler)

      /// set the time system
      void setSatelliteSystem(const Systems& sys);

      /// get the time system
      Systems getSatelliteSystem() const
      { return system; }

      /// define system based on input string
      /// @param str input string, expected to match output string for given system
      void fromString(const std::string& str);

      /// Return a std::string for each system (these strings are const and static).
      /// @return the std::string
      std::string toString() const
      { return sytemStrings[system]; }
      /// Return a std::string for each system (these strings are const and static).

      std::string systemString() const
      { return sytemStrings[system]; }

      std::string systemString3() const
      { return sytemString3[system]; }

      /// define system based on input char 
      /// @param ch input char, expected to match output char for given system
      void fromChar(const char& ch);

      /// @return the std::string
      char toChar() const
      { return systemChars[system]; }

      /// @return the std::string
      char systemChar() const
      { return systemChars[system]; }

      /// boolean operator==
      bool operator==(const SatelliteSystem& right) const
      { return system == right.system; }

      /// boolean operator< (used by STL to sort)
      bool operator<(const SatelliteSystem& right) const
      { return system < right.system; }

      // the rest follow from Boolean algebra...
      /// boolean operator!=
      bool operator!=(const SatelliteSystem& right) const
      { return !operator==(right); }

      /// boolean operator>=
      bool operator>=(const SatelliteSystem& right) const
      { return !operator<(right); }

      /// boolean operator<=
      bool operator<=(const SatelliteSystem& right) const
      { return (operator<(right) || operator==(right)); }

      /// boolean operator>
      bool operator>(const SatelliteSystem& right) const
      { return (!operator<(right) && !operator==(right)); }

   public:

      /// time system (= element of Systems enum) for this object
      Systems system;

      /// set of string labels for elements of Systems
      static const std::string sytemStrings[];
      static const std::string sytemString3[];

      /// set of char labels for elements of Systems
      static const char systemChars[];

   };   // end class SatelliteSystem

   /// Write name (asString()) of a SatelliteSystem to an output stream.
   /// @param os The output stream
   /// @param ts The SatelliteSystem to be written
   /// @return reference to the output stream
   inline std::ostream& operator<<(std::ostream& os, const SatelliteSystem& ts)
   {
      return os << ts.toString();
   }

}   // end namespace

#endif // SatelliteSystem_HPP
