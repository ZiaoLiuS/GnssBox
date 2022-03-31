#pragma ident "$Id$"

#ifndef REFERENCE_FRAME_HPP
#define REFERENCE_FRAME_HPP


#include <iostream>
#include <string>

namespace coordSpace
{

   /// This class encapsulates ECEF reference systems or frames, with std::string I/O.
   class ReferenceFrame
   {
   public:

      /// list of reference frames supported by this class
      enum Frames
      {
         // add new frames BEFORE count, then add to Strings[] in ReferenceFrame.cpp
         // and make parallel to this enum.

         // Unknown MUST BE FIRST, and MUST = 0
         Unknown=0,  ///< unknown frame
         WGS84,      ///< WGS84, assumed to be the latest version
         WGS84G730,  ///< WGS84, GPS week 730 version
         WGS84G873,  ///< WGS84, GPS week 873 version
         WGS84G1150, ///< WGS84, GPS week 1150 version
         PZ90,       ///< PZ90 (GLONASS), assumed to be the latest version
         PZ90KGS,    ///< PZ90 the "original"
         GTRF,       ///< GTRF (Galileo), assumed to be the latest version
         CGCS2000,   ///< CGCS2000 (BDS), assumed to be the latest version
         ITRF,       ///< ITRF, assumed to be the latest version
         // count MUST BE LAST
         count        ///< the number of frames - not a frame
      };

      /// Constructor, including empty constructor
      ReferenceFrame(Frames f=Unknown) throw()
      {
         if(f < 0 || f >= count)
            frame = Unknown;
         else
            frame = f;
      }

      /// Constructor from string
      ReferenceFrame(const std::string str) throw();

      // TD is this required?
      ///// Constructor from int
      //ReferenceFrame(int i) throw()
      //{
      //   if(i < 0 || i >= count)
      //      frame = Unknown;
      //   else
      //      frame = static_cast<Frames>(i);
      //}

      // copy constructor and operator= defined by the compiler

      /// Define using input value of Frames enum.
      void setReferenceFrame(const Frames& f) throw();

      /// Return the value of Frames enum for this object.
      Frames getReferenceFrame() const throw()
      { return frame; }

      /// Return std::string for each system (these strings are const and static).
      /// @return std::string description of the frame.
      std::string asString() const throw()
      { return Strings[frame]; }

      /// define system based on input string
      /// @param str input std::string, expected to match output string for a given
      /// frame.
      void fromString(const std::string str) throw()
      { frame = ReferenceFrame(str).frame; }

      /// boolean operator==
      bool operator==(const ReferenceFrame& right) const throw()
      { return frame == right.frame; }

      /// boolean operator< (used by STL for sorting)
      bool operator<(const ReferenceFrame& right) const throw()
      { return frame < right.frame; }

      // the rest follow from Boolean algebra...
      /// boolean operator!=
      bool operator!=(const ReferenceFrame& right) const throw()
      { return !operator==(right); }

      /// boolean operator>=
      bool operator>=(const ReferenceFrame& right) const throw()
      { return !operator<(right); }

      /// boolean operator<=
      bool operator<=(const ReferenceFrame& right) const throw()
      { return (operator<(right) || operator==(right)); }

      /// boolean operator>
      bool operator>(const ReferenceFrame& right) const throw()
      { return (!operator<(right) && !operator==(right)); }

   private:

      /// this reference frame == element of Frames enum
      Frames frame;

      /// set of string labels for Frames; MUST be parallel to enum Frames
      static const std::string Strings[];

   }; // end ReferenceFrame class

   /// Write name (asString()) of a ReferenceFrame to an output stream.
   /// @param os the output stream
   /// @param ts the ReferenceFrame to be written
   /// @return The reference to the ostream passed to this operator.
   std::ostream& operator<<(std::ostream& os, const ReferenceFrame& f);

}

#endif // REFERENCE_FRAME_HPP
