#pragma ident "$Id$"

//============================================================================
//
// 
//=============================================================================

/**
 * @file GALEllipsoid.hpp
 * GAL model of the Ellipsoid
 */
#pragma once

#include "EllipsoidModel.hpp"

namespace coordSpace
{
    /** @addtogroup geodeticgroup */
    //@{

      /// This class represents the ellipsoid model defined in NIMA
      /// TR8350.2, "Department of Defense World Geodetic System 1984".
   class GALEllipsoid : public EllipsoidModel
   {
   public:

         /// Defined in TR8350.2, Appendix A.1
         /// @return semi-major axis of Earth in meters.
      virtual double a() const throw()
      { return 6378137.0; }

         /// Derived from TR8350.2, Appendix A.1
         /// @return semi-major axis of Earth in km.
      virtual double a_km() const throw()
      { return a() / 1000.0; }

         /**
          * Derived from TR8350.2, Appendix A.1
          * @return flattening (ellipsoid parameter).
          */
      virtual double flattening() const throw()
      { return 0.335281066475e-2; }

         /// Defined in TR8350.2, Table 3.3
         /// @return eccentricity (ellipsoid parameter).
      virtual double eccentricity() const throw()
      { return 8.1819190842622e-2; }

         /// Defined in TR8350.2, Table 3.3
         /// @return eccentricity squared (ellipsoid parameter).
      virtual double eccSquared() const throw()
      { return 6.69437999014e-3; }

         /// Defined in TR8350.2, 3.2.4 line 3-6, or Table 3.1
         /// @return angular velocity of Earth in radians/sec.
      virtual double angVelocity() const throw()
      { return 7.2921151467e-5; }

         /// Defined in TR8350.2, Table 3.1
         /// @return geocentric gravitational constant in m**3 / s**2
      virtual double gm() const throw()
      { return 3986004.418e8; }

         /// Derived from TR8350.2, Table 3.1
         /// @return geocentric gravitational constant in km**3 / s**2
      virtual double gm_km() const throw()
      { return 398600.4418; }

         /// Defined in TR8350.2, 3.3.2 line 3-11
         /// @return Speed of light in m/s.
      virtual double c() const throw()
      { return 299792458; }

         /// Derived from TR8350.2, 3.3.2 line 3-11
         /// @return Speed of light in km/s
      virtual double c_km() const throw()
      { return c()/1000.0; }

      /// Destructor.
      virtual ~GALEllipsoid() throw() {};

   }; // class GALEllipsoid

   //@}

} // namespace

