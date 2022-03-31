#pragma ident "$Id$"

/**
 * @file Xvt.hpp
 * Position and velocity as Triples, clock bias and drift as doubles.
 */

#ifndef XVT_INCLUDE
#define XVT_INCLUDE

#include <iomanip>
#include <iostream>
#include "Triple.hpp"
#include "EllipsoidModel.hpp"
#include "ReferenceFrame.hpp"

using namespace std;
using namespace mathSpace;
using namespace coordSpace;

namespace coordSpace
{
   /** @addtogroup geodeticgroup */
   //@{

   /// Earth-Centered, Earth-Fixed Cartesian position, velocity, clock bias and drift
   class Xvt
   {
   public:

      /// Default constructor
      Xvt() : x(0.,0.,0.), v(0.,0.,0.),
              clkbias(0.), clkdrift(0.),
              relcorr(0.), frame(ReferenceFrame::Unknown)
         {};

      /// Destructor.
      virtual ~Xvt()
         {}

      /// access the position, ECEF Cartesian in meters
      Triple getPos() throw()
         { return x; }

      /// access the velocity in m/s
      Triple getVel() throw()
         { return v; }

      /// access the clock bias, in second
      double getClockBias() throw()
         { return clkbias; }

      /// access the clock drift, in second/second
      double getClockDrift() throw()
         { return clkdrift; }

      /// access the relativity correction, in seconds
      double getRelativityCorr() throw()
         { return relcorr; }

      /// Compute and return the relativity correction (-2R dot V/c^2) in seconds
      /// NB -2*dot(R,V)/(c*c) = -4.4428e-10(s/sqrt(m)) * ecc * sqrt(A(m)) * sinE
      double computeRelativityCorrection(void);

      /// Given the position of a ground location, compute the range
      /// to the spacecraft position.
      /// @param rxPos ground position at broadcast time in ECEF.
      /// @param ellipsoid geodetic parameters.
      /// @param correction offset in meters (include any factors other
      /// than the SV clock correction and the relativity correction).
      /// @return Range in meters
      double preciseRho(const Triple& rxPos,
                        const EllipsoidModel& ellipsoid,
                        double correction = 0) const throw();

      // member data

      Triple x;        ///< Sat position ECEF Cartesian (X,Y,Z) meters
      Triple v;        ///< satellite velocity in ECEF Cartesian, meters/second
      double clkbias;  ///< Sat clock correction in seconds
      double clkdrift; ///< satellite clock drift in seconds/second
      double relcorr;  ///< relativity correction (standard 2R.V/c^2 term), seconds
      ReferenceFrame frame;   ///< reference frame of this data

   }; // end class Xvt

   //@}


// /**
//  * Output operator for Xvt
//  * @param os output stream to which \c xvt is sent
//  * @param xvt Xvt that is sent to \c os
//  */
// std::ostream& operator<<(std::ostream& os, const coordSpace::Xvt& xvt) throw();

   // Output operator for Xvt
   // @param os output stream to which \c xvt is sent
   // @param xvt Xvt that is sent to \c os
   inline std::ostream& operator<<(std::ostream& os, const coordSpace::Xvt& xvt)
      throw()
   {
      os 
        << setprecision(10)
        << "x:" << xvt.x
        << ", v:" << xvt.v
        << ", clk bias:" 
        << setprecision(10)
        << xvt.clkbias
        << ", clk drift:" 
        << setprecision(10)
        << xvt.clkdrift
        << ", relcorr:" 
        << setprecision(10)
        << xvt.relcorr;
      return os;
   }

}  // end namespace coordSpace

#endif // XVT_INCLUDE
