#pragma ident "$Id$"



/**
 * @file EllipsoidModel.hpp
 * Abstract base class modeling a Ellipsoid
 */

#ifndef ELLIPSOIDMODEL_HPP
#define ELLIPSOIDMODEL_HPP

namespace coordSpace
{

      /**
       * This abstract class encapsulates ellipsoid models (e.g. WGS84,
       * GPS, etc).
       */
   class EllipsoidModel
   {
   public:
         /// @return semi-major axis of Earth in meters.
      virtual double a() const throw() = 0;

         /// @return semi-major axis of Earth in km.
      virtual double a_km() const throw() = 0;

         /// @return flattening (ellipsoid parameter).
      virtual double flattening() const throw() = 0;

         /// @return eccentricity (ellipsoid parameter).
      virtual double eccentricity() const throw() = 0;

         /// @return eccentricity squared (ellipsoid parameter).
      virtual double eccSquared() const throw()
      { return eccentricity() * eccentricity(); }

         /// @return angular velocity of Earth in radians/sec.
      virtual double angVelocity() const throw() = 0;

         /// @return geocentric gravitational constant in m**3 / s**2
      virtual double gm() const throw() = 0;

         /// @return geocentric gravitational constant in m**3 / s**2
      virtual double gm_km() const throw() = 0;

         /// @return Speed of light in m/s.
      virtual double c() const throw() = 0;

         /// @return Speed of light in km/s
      virtual double c_km() const throw() = 0;

      /// Destructor.
      virtual ~EllipsoidModel() throw() {};

   }; // class EllipsoidModel

}
#endif
