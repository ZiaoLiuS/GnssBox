#pragma ident "$Id$"


#include "ReferenceFrame.hpp"

using namespace std;

namespace coordSpace
{

   // Static initialization of const std::strings for asString().
   // Must parallel enum Frames in ReferenceFrames.hpp.
   // NB: DO NOT use std::map here; on some systems initialization fails.
   const string ReferenceFrame::Strings[count] =
     {
       string("Unknown"),
       string("WGS84"),          // WGS84, assumed to be the latest version
       string("WGS84(G730)"),    // WGS84, GPS week 730 version
       string("WGS84(G873)"),    // WGS84, GPS week 873 version
       string("WGS84(G1150)"),   // WGS84, GPS week 1150 version
       string("PZ90"),           // PZ90 (GLONASS), assumed to be the latest version
       string("PZ90KGS"),        // PZ90 (KGS) the "original"
       string("GTRF"),           // GTRF (Galileo), assumed to be the latest version
       string("CGCS2000"),       // CGCS2000 (BDS), assumed to be the latest version
       string("ITRF"),           // ITRF, assumed to be the latest version
     };

   ReferenceFrame::ReferenceFrame(const string str) throw()
   {
      frame = Unknown;
      for(int i=0; i<count; i++) {
         if(Strings[i] == str) {
            frame = static_cast<Frames>(i);
            return;
         }
      }
   }

   void ReferenceFrame::setReferenceFrame(const Frames& frm)
      throw()
   {
      if(frm < 0 || frm >= count)
         frame = Unknown;
      else
         frame = frm;
   }

   ostream& operator<<(ostream os, const ReferenceFrame& rf)
   {
      return os << rf.asString();
   }
}
