#pragma ident "$Id$"

#ifndef TROPOSPHERIC_MODELS_GPSTK
#define TROPOSPHERIC_MODELS_GPSTK

/**
 * @file TropModel.hpp
 * Base class for tropospheric models, plus implementations
 * of several published models
 */

#include "Exception.hpp"
#include "WxObsMap.hpp"
#include "Position.hpp"
#include "constants.hpp"

#include <Eigen/Eigen>


// Model of the troposphere, used to compute non-dispersive delay of
// satellite signal as function of satellite elevation as seen at the
// receiver. Both wet and dry components are computed.
//
// The default model (implemented here) is a simple Black model.
//
// In this model (and many others), the wet and dry components are
// independent, the zenith delays depend only on the weather
// (temperature, pressure and humidity), and the mapping functions
// depend only on the elevation of the satellite as seen at the
// receiver. In general, this is not true; other models depend on,
// for example, latitude or day of year.
//
// Other models may be implemented by inheriting this class and
// redefining the virtual functions, and (perhaps) adding other
// 'set...()' routines as needed.

using namespace utilSpace;
using namespace coordSpace;
using namespace Eigen;

namespace gnssSpace
{
      /** Abstract base class for tropospheric models.
       * The wet and dry components of the tropospheric delay are each the
       * product of a zenith delay and a mapping function. Usually the zenith
       * delay depends only on the weather (temperature, pressure and humidity),
       * while the mapping function depends only on the satellite elevation, i.e.
       * the geometry of satellite and receiver. This may not be true in complex
       * models.
       * The full tropospheric delay is the sum of the wet and dry components.
       * A TropModel is valid only when all the necessary information
       * (weather + whatever else the model requires) is specified;
       * An InvalidTropModel exception will be thrown when any correction()
       * or zenith_delay() or mapping_function() routine is called for
       * an invalid TropModel.
       */
   class TropModel
   {
   public:

         /// Destructor
      virtual ~TropModel() {}

         /// Return validity of model
      bool isValid(void)
         { return valid; }

         /// Compute and return the full tropospheric delay
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double correction(double elevation) const
         noexcept(false);

         /**
          * Compute and return the full tropospheric delay, given the positions of
          * receiver and satellite and the time tag. This version is most useful
          * within positioning algorithms, where the receiver position and timetag
          * may vary; it computes the elevation (and other receiver location
          * information) and passes them to appropriate set...() routines and the
          * correction(elevation) routine.
          * @param RX  Receiver position
          * @param SV  Satellite position
          * @param tt  Time tag of the signal
          */
      virtual double correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt)
         noexcept(false);

         /// Compute and return the zenith delay for dry component of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false) = 0;

         /// Compute and return the zenith delay for wet component of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false) = 0;

         /// Compute and return the mapping function for dry component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation)
         const noexcept(false) = 0;

         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation)
         const noexcept(false) = 0;

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H)
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param wx the weather to use for this correction
      virtual void setWeather(const WxObservation& wx)
         noexcept(false);

         /// Define the receiver height; this required by some models before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param ht Height of the receiver in meters.
      virtual void setReceiverHeight(const double& ht) {};

         /// Define the latitude of the receiver; this is required by some models
         /// before calling correction() or any of the zenith_delay or
         /// mapping_function routines.
         /// @param lat Latitude of the receiver in degrees.
      virtual void setReceiverLatitude(const double& lat) {};

         /// Define the day of year; this is required by some models before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param d Day of year.
      virtual void setDayOfYear(const int& d) {};


      virtual void setAllParameters(const CommonTime& time, const Position& rxPos) {};


         /// get weather data by a standard atmosphere model
         /// reference to white paper of Bernese 5.0, P243
         /// @param ht     height of the receiver in meters.
         /// @param T      temperature in degrees Celsius
         /// @param P      atmospheric pressure in millibars
         /// @param H      relative humidity in percent
      static void weatherByStandardAtmosphereModel(const double& ht, double& T, double& P, double& H);

   protected:
      bool valid;                 // true only if current model parameters are valid
      double temp;                // latest value of temperature (kelvin or celsius)
      double press;               // latest value of pressure (millibars)
      double humid;               // latest value of relative humidity (percent)

   }; // end class TropModel


   //---------------------------------------------------------------------------------
   /// The 'zero' trop model, meaning it always returns zero.
   class ZeroTropModel : public TropModel
   {
   public:
         /// Compute and return the full tropospheric delay
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double correction(double elevation) const
         noexcept(false)
         { return 0.0; }

         /**
          * Compute and return the full tropospheric delay, given the positions of
          * receiver and satellite and the time tag. This version is most useful
          * within positioning algorithms, where the receiver position and timetag
          * may vary; it computes the elevation (and other receiver location
          * information) and passes them to appropriate set...() routines and the
          * correction(elevation) routine.
          * @param RX  Receiver position
          * @param SV  Satellite position
          * @param tt  Time tag of the signal
          */
      virtual double correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt)
         noexcept(false)
         { return 0.0; }

         /// Compute and return the zenith delay for dry component of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false)
         { return 0.0; }

         /// Compute and return the zenith delay for wet component of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false)
         { return 0.0; }

         /// Compute and return the mapping function for dry component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation)
         const noexcept(false)
         { return 0.0; }

         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation)
         const noexcept(false)
         { return 0.0; }

   }; // end class ZeroTropModel


   //---------------------------------------------------------------------------------
   /// A simple Black model of the troposphere. temp is in Kelvin.
   class SimpleTropModel : public TropModel
   {
   public:
         /// Empty constructor
      SimpleTropModel(void);

         /// Creates a trop model, with weather observation input
         /// @param wx the weather to use for this correction.
      SimpleTropModel(const WxObservation& wx)
         noexcept(false);

         /// Create a tropospheric model from explicit weather data
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      SimpleTropModel(const double& T,
                      const double& P,
                      const double& H)
         noexcept(false);

         /// Compute and return the zenith delay for dry component of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the zenith delay for wet component of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the mapping function for dry component
         /// of the troposphere
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);

         /// Compute and return the mapping function for wet component
         /// of the troposphere
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H)
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param wx the weather to use for this correction
      virtual void setWeather(const WxObservation& wx)
         noexcept(false);

   private:
      double Cdrydelay;
      double Cwetdelay;
      double Cdrymap;
      double Cwetmap;

   };    // end class SimpleTropModel

   //---------------------------------------------------------------------------------
   /** Tropospheric model based on Goad and Goodman(1974),
    *  "A Modified Hopfield Tropospheric Refraction Correction Model," Paper
    * presented at the Fall Annual Meeting of the American Geophysical Union,
    * San Francisco, December 1974, as presented in Leick, "GPS Satellite Surveying,"
    * Wiley, NY, 1990, Chapter 9 (note particularly Table 9.1).
    */
   class GGTropModel : public TropModel
   {
   public:
         /// Empty constructor
      GGTropModel(void);

         /// Creates a trop model, with weather observation input
         /// @param wx the weather to use for this correction.
      GGTropModel(const WxObservation& wx)
         noexcept(false);

         /// Create a tropospheric model from explicit weather data
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      GGTropModel(const double& T,
                  const double& P,
                  const double& H)
         noexcept(false);

         /// Compute and return the zenith delay for dry component
         /// of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the zenith delay for wet component
         /// of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the mapping function for dry component
         /// of the troposphere
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);

         /// Compute and return the mapping function for wet component
         /// of the troposphere
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called initially, and whenever the weather changes.
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H)
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param wx the weather to use for this correction
      virtual void setWeather(const WxObservation& wx)
         noexcept(false);

   private:
      double Cdrydelay;
      double Cwetdelay;
      double Cdrymap;
      double Cwetmap;

   };    // end class GGTropModel

   //---------------------------------------------------------------------------------
   /** Tropospheric model with heights based on Goad and Goodman(1974),
    *  "A Modified Hopfield Tropospheric Refraction Correction Model," Paper
    *  presented at the Fall Annual Meeting of the American Geophysical Union,
    *  San Francisco, December 1974.
    *
    *  (Not the same as GGTropModel because this has height dependence, and the
    *  computation of this model does not break cleanly into wet and dry components.)
    *
    *  NB this model requires heights, both of the weather parameters,
    *    and of the receiver.
    *  Thus, usually, caller will set heights at the same time the weather is set:
    *
    * @code
    *    GGHeightTropModel ggh;
    *    ggh.setWeather(T,P,H);
    *    ggh.setHeights(hT,hP,hH);
    * @endcode
    *
    *  and when the correction (and/or delay and map) is computed,
    *  receiver height is set before the call to correction(elevation):
    *
    * @code
    *    ggh.setReceiverHeight(height);
    *    trop = ggh.correction(elevation);
    * @endcode
    *
    *  NB setReceiverHeight(ht) sets the 'weather heights' as well, if they are not
    *    already defined.
    */
   class GGHeightTropModel : public TropModel
   {
   public:
         /// Empty constructor
      GGHeightTropModel(void);

         /// Creates a trop model, with weather observation input
         /// @param wx the weather to use for this correction.
      GGHeightTropModel(const WxObservation& wx)
         noexcept(false);

         /// Create a tropospheric model from explicit weather data
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      GGHeightTropModel(const double& T,
                        const double& P,
                        const double& H)
         noexcept(false);

         /// Create a valid model from explicit input.
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
         /// @param hT height at which temperature applies in meters.
         /// @param hP height at which atmospheric pressure applies in meters.
         /// @param hH height at which relative humidity applies in meters.
      GGHeightTropModel(const double& T,
                        const double& P,
                        const double& H,
                        const double hT,
                        const double hP,
                        const double hH)
         noexcept(false);

         /// Compute and return the full tropospheric delay
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double correction(double elevation) const
         noexcept(false);

         /**
          * Compute and return the full tropospheric delay, given the positions of
          * receiver and satellite and the time tag. This version is most useful
          * within positioning algorithms, where the receiver position and timetag
          * may vary; it computes the elevation (and other receiver location
          * information) and passes them to appropriate set...() routines and the
          * correction(elevation) routine.
          * @param RX  Receiver position
          * @param SV  Satellite position
          * @param tt  Time tag of the signal
          */
      virtual double correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt)
         noexcept(false);

         /// Compute and return the zenith delay for dry component
         /// of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the zenith delay for wet component of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the mapping function for dry component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);

         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);

         /// Re-define the weather data.
         /// Typically called initially, and whenever the weather changes.
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H)
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param wx the weather to use for this correction
      virtual void setWeather(const WxObservation& wx)
         noexcept(false);

         /// Re-define the heights at which the weather parameters apply.
         /// Typically called whenever setWeather is called.
         /// @param hT height at which temperature applies in meters.
         /// @param hP height at which atmospheric pressure applies in meters.
         /// @param hH height at which relative humidity applies in meters.
      void setHeights(const double& hT,
                      const double& hP,
                      const double& hH);

         /// Define the receiver height; this required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
      void setReceiverHeight(const double& ht);

   private:
      double height;                // height (m) of the receiver
      double htemp;                 // height (m) at which temp applies
      double hpress;                // height (m) at which press applies
      double hhumid;                // height (m) at which humid applies
      bool validWeather;
      bool validHeights;
      bool validRxHeight;

   };    // end class GGHeightTropModel


   //---------------------------------------------------------------------------------
   /** Tropospheric model developed by University of New Brunswick and described in
    * "A Tropospheric Delay Model for the User of the Wide Area Augmentation
    * System," J. Paul Collins and Richard B. Langley, Technical Report No. 187,
    * Dept. of Geodesy and Geomatics Engineering, University of New Brunswick,
    * 1997. See particularly Appendix C.
    *
    * This model includes a wet and dry component, and was designed for the user
    * without access to measurements of temperature, pressure and relative humidity
    * at ground level. Input of the receiver latitude, day of year and height
    * above the ellipsoid are required, because the mapping functions depend on
    * these quantities. In addition, if the weather (T,P,H) are not explicitly
    * provided, this model interpolates a table of values, using latitude and day
    * of year, to get the ground level weather parameters.
    *
    * Usually, the caller will set the latitude and day of year at the same
    * time the weather is set (if weather is available):
    *
    * @code
    *   NBTropModel nb;
    *   nb.setReceiverLatitude(lat);
    *   nb.setDayOfYear(doy);
    *   nb.setWeather(T,P,H);       // OPTIONAL
    * @endcode
    *
    * Then, when the correction (and/or delay and map) is computed, receiver height
    * should be set before the call to correction(elevation):
    *
    * @code
    *   nb.setReceiverHeight(height);
    *   trop = nb.correction(elevation);
    * @endcode
    *
    * NB in this model, units of 'temp' are degrees Kelvin, and 'humid'
    * is the water vapor partial pressure.
    */
   class NBTropModel : public TropModel
   {
   public:
         /// Empty constructor
      NBTropModel(void);

         /// Create a trop model using the minimum information: latitude and doy.
         /// Interpolate the weather unless setWeather (optional) is called.
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
      NBTropModel(const double& lat,
                  const int& day);

         /// Create a trop model with weather.
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
         /// @param wx the weather to use for this correction.
      NBTropModel(const double& lat,
                  const int& day,
                  const WxObservation& wx)
         noexcept(false);

         /// Create a tropospheric model from explicit weather data
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      NBTropModel(const double& lat,
                  const int& day,
                  const double& T,
                  const double& P,
                  const double& H)
         noexcept(false);

         /// Create a valid model from explicit input
         /// (weather will be estimated internally by this model).
         /// @param ht Height of the receiver in meters.
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
      NBTropModel(const double& ht,
                  const double& lat,
                  const int& day);

         /// Compute and return the full tropospheric delay
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double correction(double elevation) const
         noexcept(false);

         /**
          * Compute and return the full tropospheric delay, given the positions of
          * receiver and satellite and the time tag. This version is most useful
          * within positioning algorithms, where the receiver position and timetag
          * may vary; it computes the elevation (and other receiver location
          * information) and passes them to appropriate set...() routines and the
          * correction(elevation) routine.
          * @param RX  Receiver position
          * @param SV  Satellite position
          * @param tt  Time tag of the signal
          */
      virtual double correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt)
         noexcept(false);

         /// Compute and return the zenith delay for dry component
         /// of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the zenith delay for wet component
         /// of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the mapping function for dry component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);

         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param wx the weather to use for this correction
      virtual void setWeather(const WxObservation& wx)
         noexcept(false);

         /// Define the weather data; typically called just before correction().
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H)
         noexcept(false);

         /// configure the model to estimate the weather using lat and doy
      void setWeather()
         noexcept(false);

         /// Define the receiver height; this required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param ht Height of the receiver in meters.
      void setReceiverHeight(const double& ht);

         /// Define the latitude of the receiver; this is required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param lat Latitude of the receiver in degrees.
      void setReceiverLatitude(const double& lat);

         /// Define the day of year; this is required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param d Day of year.
      void setDayOfYear(const int& d);

   private:
      bool interpolateWeather;      // if true, compute T,P,H from latitude,doy
      double height;                // height (m) of the receiver
      double latitude;              // latitude (deg) of receiver
      int doy;                      // day of year
      bool validWeather;
      bool validRxLatitude;
      bool validRxHeight;
      bool validDOY;

   };    // end class NBTropModel

   //---------------------------------------------------------------------------------
   /** Saastamoinen tropospheric model based on Saastamoinen, J., 'Atmospheric
    * Correction for the Troposphere and Stratosphere in Radio Ranging of
    * Satellites,' Geophysical Monograph 15, American Geophysical Union, 1972,
    * and Ch 9 of McCarthy, D and Petit, G, IERS Conventions (2003), IERS
    * Technical Note 32, IERS, 2004. The mapping functions are from
    * Neill, A.E., 1996, 'Global Mapping Functions for the Atmosphere Delay of
    * Radio Wavelengths,' J. Geophys. Res., 101, pp. 3227-3246 (also see IERS TN 32).
    *
    * This model includes a wet and dry component, and requires input of the
    * geodetic latitude, day of year and height above the ellipsoid of the receiver.
    *
    * Usually, the caller will set the latitude and day of year at the same
    * time the weather is set
    *   SaasTropModel stm;
    *   stm.setReceiverLatitude(lat);
    *   stm.setDayOfYear(doy);
    *   stm.setWeather(T,P,H);
    * Then, when the correction (and/or delay and map) is computed, receiver height
    * should be set before the call to correction(elevation):
    *   stm.setReceiverHeight(height);
    *   trop_corr = stm.correction(elevation);
    *
    * NB in this model, units of 'temp' are degrees Celsius and
    * humid actually stores water vapor partial pressure in mbars
    */
   class SaasTropModel : public TropModel
   {
   public:
         /// Empty constructor
      SaasTropModel(void);

         /// Create a trop model using the minimum information: latitude and doy.
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
      SaasTropModel(const double& lat,
                    const int& day);

         /// Create a trop model with weather.
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
         /// @param wx the weather to use for this correction.
      SaasTropModel(const double& lat,
                    const int& day,
                    const WxObservation& wx)
         noexcept(false);

         /// Create a tropospheric model from explicit weather data
         /// @param lat Latitude of the receiver in degrees.
         /// @param day Day of year.
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      SaasTropModel(const double& lat,
                    const int& day,
                    const double& T,
                    const double& P,
                    const double& H)
         noexcept(false);

         /// Compute and return the full tropospheric delay
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double correction(double elevation) const
         noexcept(false);

         /**
          * Compute and return the full tropospheric delay, given the positions of
          * receiver and satellite and the time tag. This version is most useful
          * within positioning algorithms, where the receiver position and timetag
          * may vary; it computes the elevation (and other receiver location
          * information) and passes them to appropriate set...() routines and the
          * correction(elevation) routine.
          * @param RX  Receiver position
          * @param SV  Satellite position
          * @param tt  Time tag of the signal
          */
      virtual double correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt)
         noexcept(false);

         /// Compute and return the zenith delay for dry component
         /// of the troposphere
      virtual double dry_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the zenith delay for wet component
         /// of the troposphere
      virtual double wet_zenith_delay(void) const
         noexcept(false);

         /// Compute and return the mapping function for dry component of
         /// the troposphere. NB this function will return infinity at zero elevation.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);

         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         /// @param elevation Elevation of satellite as seen at receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);

         /// Re-define the tropospheric model with explicit weather data.
         /// Typically called just before correction().
         /// @param wx the weather to use for this correction
      virtual void setWeather(const WxObservation& wx)
         noexcept(false);

         /// Define the weather data; typically called just before correction().
         /// @param T temperature in degrees Celsius
         /// @param P atmospheric pressure in millibars
         /// @param H relative humidity in percent
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H)
         noexcept(false);

         /// Define the receiver height; this required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param ht Height of the receiver in meters.
      void setReceiverHeight(const double& ht);

         /// Define the latitude of the receiver; this is required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param lat Latitude of the receiver in degrees.
      void setReceiverLatitude(const double& lat);

         /// Define the day of year; this is required before calling
         /// correction() or any of the zenith_delay or mapping_function routines.
         /// @param d Day of year.
      void setDayOfYear(const int& d);

   private:
      double height;                /// height (m) of the receiver above the geoid
      double latitude;              /// latitude (deg) of receiver
      int doy;                      /// day of year
      bool validWeather;
      bool validRxLatitude;
      bool validRxHeight;
      bool validDOY;

   };    // end class SaasTropModel


      //--------------------------------------------------------------------

      /** Tropospheric model implemented in "GPS Code Analysis Tool" (GCAT)
       *  software.
       *
       * This model is described in the book "GPS Data processing: code and
       * phase Algorithms, Techniques and Recipes" by Hernandez-Pajares, M.,
       * J.M. Juan-Zornoza and Sanz-Subirana, J. See Chapter 5.
       *
       * This book and associated software are freely available at:
       *
       * http://gage152.upc.es/~manuel/tdgps/tdgps.html
       *
       * This is a simple but effective model composed of the wet and dry
       * vertical tropospheric delays as defined in Gipsy/Oasis-II GPS
       * analysis software, and the mapping function as defined by Black and
       * Eisner (H. D. Black, A. Eisner. Correcting Satellite Doppler
       * Data for Tropospheric Effects. Journal of  Geophysical Research.
       * Vol 89. 1984.) and used in MOPS (RTCA/DO-229C) standards.
       *
       * Usually, the caller will set the receiver height using
       * setReceiverHeight() method, and then call the correction() method
       * with the satellite elevation as parameter.
       *
       * @code
       *   GCATTropModel gcatTM();
       *   ...
       *   gcatTM.setReceiverHeight(150.0);
       *   trop = gcatTM.correction(elevation);
       * @endcode
       *
       * Another posibility is to set the receiver height when calling
       * the constructor.
       *
       * @code
       *   GCATTropModel gcatTM(150.0);    // Receiver height is 150.0 meters
       *   ...
       *   trop = gcatTM.correction(elevation);
       * @endcode
       */
   class GCATTropModel : public TropModel
   {
   public:


         /// Empty constructor
      GCATTropModel(void)
      { valid = false; };


         /// Constructor to create a GCAT trop model providing  the height
         /// of the receiver above mean sea level (as defined by ellipsoid
         /// model).
         ///
         /// @param ht Height of the receiver above mean sea level, in meters.
      GCATTropModel(const double& ht);


         /** Compute and return the full tropospheric delay. The receiver
          *  height must has been provided before, whether using the
          *  appropriate constructor or with the setReceiverHeight() method
          *
          * @param elevation  Elevation of satellite as seen at receiver, in
          *                   degrees
          */
      virtual double correction(double elevation) const
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite. This version is most useful
          *  within positioning algorithms, where the receiver position may
          *  vary; it computes the elevation (and other receiver location
          *  information as height) and passes them to setReceiverHeight()
          *  method and correction(elevation) method.
          *
          * @param RX  Receiver position
          * @param SV  Satellite position
          */
      virtual double correction( const Position& RX,
                                 const Position& SV )
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the time tag. This version
          *  is most useful within positioning algorithms, where the receiver
          *  position and timetag may vary; it computes the elevation (and
          *  other receiver location information as height) and passes them to
          *  setReceiverHeight() method and correction(elevation) method.
          *
          * @param RX  Receiver position
          * @param SV  Satellite position
          * @param tt  Time. In this model, tt is a dummy parameter kept just
          *            for consistency
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const CommonTime& tt )
         noexcept(false)
      { return correction(RX, SV); };


         /** Compute and return the zenith delay for dry component of the
          *  troposphere.
          */
      virtual double dry_zenith_delay(void) const
         noexcept(false);


         /** Compute and return the zenith delay for wet component of the
          *  troposphere.
          */
      virtual double wet_zenith_delay(void) const
         noexcept(false)
      { return 0.1; };


         /** Compute and return the mapping function for both components of
          *  the troposphere.
          *
          * @param elevation  Elevation of satellite as seen at receiver, in
          *                   degrees
          */
      virtual double mapping_function(double elevation) const
         noexcept(false);


         /** Compute and return the mapping function for dry component
          *  of the troposphere.
          * @param elevation  Elevation of satellite as seen at receiver, in
          *                   degrees
          */
      virtual double dry_mapping_function(double elevation) const
         noexcept(false)
      { return mapping_function(elevation); };


         /** Compute and return the mapping function for wet component
          *  of the troposphere.
          *
          * @param elevation  Elevation of satellite as seen at receiver, in
          *                   degrees
          */
      virtual double wet_mapping_function(double elevation) const
         noexcept(false)
      { return mapping_function(elevation); };


         /** In GCAT tropospheric model, this is a dummy method kept here just
          *  for consistency.
          */
      virtual void setWeather( const double& T,
                               const double& P,
                               const double& H )
         noexcept(false) {};


         /** In GCAT tropospheric model, this is a dummy method kept here just
          *  for consistency.
          */
      virtual void setWeather(const WxObservation& wx)
         noexcept(false) {};


         /** Define the receiver height; this is required before calling
          * correction() or any of the zenith_delay routines.
          *
          * @param ht   Height of the receiver above mean sea level, in meters.
          */
      virtual void setReceiverHeight(const double& ht);


   private:

         /// Receiver height
      double gcatHeight;

   };    // end class GCATTropModel


      //---------------------------------------------------------------------

      /** Tropospheric model implemented in the RTCA "Minimum Operational
       *  Performance Standards" (MOPS), version C.
       *
       * This model is described in the RTCA "Minimum Operational Performance
       * Standards" (MOPS), version C (RTCA/DO-229C), in Appendix A.4.2.4.
       * Although originally developed for SBAS systems (EGNOS, WAAS), it may
       * be suitable for other uses as well.
       *
       * This model needs the day of year, satellite elevation (degrees),
       * receiver height over mean sea level (meters) and receiver latitude in
       * order to start computing.
       *
       * On the other hand, the outputs are the tropospheric correction (in
       * meters) and the sigma-squared of tropospheric delay residual error
       * (meters^2).
       *
       * A typical way to use this model follows:
       *
       * @code
       *   MOPSTropModel mopsTM;
       *   mopsTM.setReceiverLatitude(lat);
       *   mopsTM.setReceiverHeight(height);
       *   mopsTM.setDayOfYear(doy);
       * @endcode
       *
       * Once all the basic model parameters are set (latitude, height and day
       * of year), then we are able to compute the tropospheric correction as
       * a function of elevation:
       *
       * @code
       *   trop = mopsTM.correction(elevation);
       * @endcode
       *
       */
   class MOPSTropModel : public GCATTropModel
   {
   public:

         /// Empty constructor
      MOPSTropModel(void);


         /** Constructor to create a MOPS trop model providing just the height
          *  of the receiver above mean sea level. The other parameters must be
          *  set with the appropriate set methods before calling correction
          *  methods.
          *
          * @param ht   Height of the receiver above mean sea level, in meters.
          */
      MOPSTropModel(const double& ht)
      { setReceiverHeight(ht); };


         /** Constructor to create a MOPS trop model providing the height of
          *  the receiver above mean sea level (as defined by ellipsoid model),
          *  its latitude and the day of year.
          *
          * @param ht   Height of the receiver above mean sea level, in meters.
          * @param lat  Latitude of receiver, in degrees.
          * @param doy  Day of year.
          */
      MOPSTropModel(const double& ht, const double& lat, const int& doy);


         /** Constructor to create a MOPS trop model providing the position of
          *  the receiver and current time.
          *
          * @param RX   Receiver position.
          * @param time Time.
          */
      MOPSTropModel(const Position& RX, const CommonTime& time);


         /** Compute and return the full tropospheric delay. The receiver
          *  height, latitude and Day oy Year must has been set before using
          *  the appropriate constructor or the provided methods.
          *
          * @param elevation   Elevation of satellite as seen at receiver, in
          *                    degrees.
          */
      virtual double correction(double elevation) const
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite.
          *
          * This version is most useful within positioning  algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as height and latitude) and
          * passes them to appropriate methods. You must set time using method
          * setReceiverDOY() before calling this method.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          */
      virtual double correction( const Position& RX,
                                 const Position& SV )
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the time tag.
          *
          * This version is most useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as height and latitude) and
          * passes them to appropriate methods.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          * @param tt  Time (CommonTime object).
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const CommonTime& tt )
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the day of the year.
          *
          * This version is most useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as height and latitude) and
          * passes them to appropriate methods.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          * @param doy Day of year.
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const int& doy )
         noexcept(false);


         /// Compute and return the zenith delay for dry component of the
         /// troposphere.
      virtual double dry_zenith_delay(void) const
         noexcept(false);


         /// Compute and return the zenith delay for wet component of the
         /// troposphere.
      virtual double wet_zenith_delay(void) const
         noexcept(false);


         /** This method configure the model to estimate the weather using
          *  height, latitude and day of year (DOY). It is called automatically
          *  when setting those parameters.
          */
      void setWeather()
         noexcept(false);


         /// In MOPS tropospheric model, this is a dummy method kept here just
         /// for consistency.
      virtual void setWeather( const double& T,
                               const double& P,
                               const double& H )
         noexcept(false) {};


         /// In MOPS tropospheric model, this is a dummy method kept here just
         /// for consistency.
      virtual void setWeather(const WxObservation& wx)
         noexcept(false) {};


         /** Define the receiver height; this is required before calling
          *  correction() or any of the zenith_delay routines.
          *
          * @param ht   Height of the receiver above mean sea level, in meters.
          */
      virtual void setReceiverHeight(const double& ht);


         /** Define the receiver latitude; this is required before calling
          *  correction() or any of the zenith_delay routines.
          *
          * @param lat  Latitude of receiver, in degrees.
          */
      virtual void setReceiverLatitude(const double& lat);


         /** Set the time when tropospheric correction will be computed for, in
          *  days of the year.
          *
          * @param doy  Day of the year.
          */
      virtual void setDayOfYear(const int& doy);


         /** Set the time when tropospheric correction will be computed for, in
          *  days of the year.
          *
          * @param time  Time object.
          */
      virtual void setDayOfYear(const CommonTime& time);


         /** Convenient method to set all model parameters in one pass.
          *
          * @param time  Time object.
          * @param rxPos Receiver position object.
          */
      virtual void setAllParameters( const CommonTime& time,
                                     const Position& rxPos );


         /** Compute and return the sigma-squared value of tropospheric delay
          *  residual error (meters^2).
          *
          * @param elevation  Elevation of satellite as seen at receiver,
          *                   in degrees
          */
      double MOPSsigma2(double elevation)
         noexcept(false);


   private:

      double MOPSHeight;
      double MOPSLat;
      int MOPSTime;
      bool validHeight;
      bool validLat;
      bool validTime;
      MatrixXd avr;
      MatrixXd svr;
      VectorXd fi0;
      VectorXd MOPSParameters;


         // The MOPS tropospheric model needs to compute several extra
         // parameters
      virtual void prepareParameters(void) noexcept(false);


         // The MOPS tropospheric model uses several predefined data tables
      virtual void prepareTables(void);

   };    // end class MOPSTropModel


      //--------------------------------------------------------------------

      /** Tropospheric model based in the Neill mapping functions.
       *
       * This model uses the mapping functions developed by A.E. Niell and
       * published in Neill, A.E., 1996, 'Global Mapping Functions for the
       * Atmosphere Delay of Radio Wavelengths,' J. Geophys. Res., 101,
       * pp. 3227-3246 (also see IERS TN 32).
       *
       * The coefficients of the hydrostatic mapping function depend on the
       * latitude and height above sea level of the receiver station, and on
       * the day of the year. On the other hand, the wet mapping function
       * depends only on latitude.
       *
       * This mapping is independent from surface meteorology, while having
       * comparable accuracy and precision to those that require such data.
       * This characteristic makes this model very useful, and it is
       * implemented in geodetic software such as JPL's Gipsy/OASIS.
       *
       * A typical way to use this model follows:
       *
       * @code
       *   NeillTropModel neillTM;
       *   neillTM.setReceiverLatitude(lat);
       *   neillTM.setReceiverHeight(height);
       *   neillTM.setDayOfYear(doy);
       * @endcode
       *
       * Once all the basic model parameters are set (latitude, height and
       * day of year), then we are able to compute the tropospheric correction
       * as a function of elevation:
       *
       * @code
       *   trop = neillTM.correction(elevation);
       * @endcode
       *
       * @warning The Neill mapping functions are defined for elevation
       * angles down to 3 degrees.
       *
       */
   class NeillTropModel : public TropModel
   {
   public:

         /// Default constructor
      NeillTropModel(void)
      { validHeight=false; validLat=false; validDOY=false; valid=false; };


         /// Constructor to create a Neill trop model providing just the
         /// height of the receiver above mean sea level. The other
         /// parameters must be set with the appropriate set methods before
         /// calling correction methods.
         ///
         /// @param ht   Height of the receiver above mean sea level, in
         ///             meters.
      NeillTropModel(const double& ht)
      { setReceiverHeight(ht); };


         /// Constructor to create a Neill trop model providing the height of
         /// the receiver above mean sea level (as defined by ellipsoid
         /// model), its latitude and the day of year.
         ///
         /// @param ht   Height of the receiver above mean sea level,
         ///             in meters.
         /// @param lat  Latitude of receiver, in degrees.
         /// @param doy  Day of year.
      NeillTropModel( const double& ht,
                      const double& lat,
                      const int& doy )
      { setReceiverHeight(ht); setReceiverLatitude(lat); setDayOfYear(doy); };


         /// Constructor to create a Neill trop model providing the position
         /// of the receiver and current time.
         ///
         /// @param RX   Receiver position.
         /// @param time Time.
      NeillTropModel( const Position& RX,
                      const CommonTime& time );


         /// Compute and return the full tropospheric delay. The receiver
         /// height, latitude and Day oy Year must has been set before using
         /// the appropriate constructor or the provided methods.
         ///
         /// @param elevation Elevation of satellite as seen at receiver,
         ///                  in degrees.
      virtual double correction(double elevation) const
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite.
          *
          * This version is more useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as height and latitude) and
          * passes them to appropriate methods.
          *
          * You must set time using method setReceiverDOY() before calling
          * this method.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          */
      virtual double correction( const Position& RX,
                                 const Position& SV )
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the time tag.
          *
          * This version is more useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as height and latitude), and
          * passes them to appropriate methods.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          * @param tt  Time (CommonTime object).
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const CommonTime& tt )
        noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the day of the year.
          *
          * This version is more useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as height and latitude), and
          * passes them to appropriate methods.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          * @param doy Day of year.
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const int& doy )
         noexcept(false);


         /// Compute and return the zenith delay for dry component of
         /// the troposphere.
      virtual double dry_zenith_delay(void) const
         noexcept(false);


         /// Compute and return the zenith delay for wet component of
         /// the troposphere.
      virtual double wet_zenith_delay(void) const
         noexcept(false)
      { return 0.1; };           // Return a nominal value


         /// Compute and return the mapping function for dry component of
         /// the troposphere.
         ///
         /// @param elevation Elevation of satellite as seen at receiver, in
         ///                  degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);


         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         ///
         /// @param elevation Elevation of satellite as seen at
         ///                  receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);


         /// This method configure the model to estimate the weather using
         /// height, latitude and day of year (DOY). It is called
         /// automatically when setting those parameters.
      void setWeather()
         noexcept(false);


         /// In Neill tropospheric model, this is a dummy method kept here
         /// just for consistency,
      virtual void setWeather( const double& T,
                               const double& P,
                               const double& H )
         noexcept(false) {};


         /// In Neill tropospheric model, this is a dummy method kept here
         /// just for consistency
      virtual void setWeather(const WxObservation& wx)
         noexcept(false) {};


         /// Define the receiver height; this is required before calling
         /// correction() or any of the zenith_delay routines.
         ///
         /// @param ht   Height of the receiver above mean sea level,
         ///             in meters.
      virtual void setReceiverHeight(const double& ht);


         /// Define the receiver latitude; this is required before calling
         /// correction() or any of the zenith_delay routines.
         ///
         /// @param lat  Latitude of receiver, in degrees.
      virtual void setReceiverLatitude(const double& lat);


         /// Set the time when tropospheric correction will be computed for,
         /// in days of the year.
         ///
         /// @param doy  Day of the year.
      virtual void setDayOfYear(const int& doy);


         /// Set the time when tropospheric correction will be computed for,
         /// in days of the year.
         ///
         /// @param time  Time object.
      virtual void setDayOfYear(const CommonTime& time);


         /** Convenient method to set all model parameters in one pass.
          *
          * @param time  Time object.
          * @param rxPos Receiver position object.
          */
      virtual void setAllParameters( const CommonTime& time,
                                     const Position& rxPos );


   private:


      double NeillHeight;
      double NeillLat;
      int NeillDOY;
      bool validHeight;
      bool validLat;
      bool validDOY;


   };    // end class NeillTropModel


      //--------------------------------------------------------------------

      /** Tropospheric model based in the Vienna mapping functions.
       *
       * This model uses the mapping functions developed by Boehm, J., et.al.
       * and published in Boehm, J., et al., 2006, 'Troposphere mapping
       * functions for GPS and very long baseline interferometry from European
       * Centre for Medium-Range Weather Forecasts operational analysis data'
       * J. Geophys. Res., 111, B02406 (also see IERS TN 36).
       *
       * A typical way to use this model follows:
       *
       * @code
       *   ViennaTropModel viennaTM;
       *   viennaTM.loadFile(file);
       *   viennaTM.setReceiverLatitude(lat);
       *   viennaTM.setReceiverLongitude(lon);
       *   viennaTM.setReceiverHeight(height);
       *   viennaTM.setModifiedJulianDate(mjd);
       *
       * @endcode
       *
       * Once all the basic model parameters are set (latitude, longitude,
       * height and modified julian date), then we are able to compute the
       * tropospheric correction as a function of elevation:
       *
       * @code
       *   trop = viennaTM.correction(elevation);
       * @endcode
       *
       */
   class ViennaTropModel : public TropModel
   {
   public:

       struct GPT2Data
       {
           double pgrid[5];
           double Tgrid[5];
           double Qgrid[5];
           double dTgrid[5];
           double undu;
           double Hs;
           double ahgrid[5];
           double awgrid[5];
           double lagrid[5];
           double Tmgrid[5];
       };

   public:

         /// Default constructor
      ViennaTropModel(void)
      {
          validLat=false;
          validLon=false;
          validHeight=false;
          validMJD=false;
          valid=false;
          gpt2DataVec.clear();
      };


         /// Constructor to create a Vienna trop model providing the
         /// latitude, the longitude, the height of the receiver above
         /// ellipsoid (as defined by ellipsoid model), and the modified
         /// julian date.
         ///
         /// @param lat  Latitude of receiver, in degrees.
         /// @param lon  Longitude of receiver, in degrees.
         /// @param ht   Height of the receiver above ellipsoid,
         ///             in meters.
         /// @param mjd  Modified Julian Date.
      ViennaTropModel( const double& lat,
                       const double& lon,
                       const double& ht,
                       const double& mjd )
      {
          setReceiverLatitude(lat);
          setReceiverLongitude(lon);
          setReceiverHeight(ht);
          setModifiedJulianDate(mjd);
      };


         /// Constructor to create a Vienna trop model providing the position
         /// of the receiver and current time.
         ///
         /// @param RX   Receiver position.
         /// @param time Time.
      ViennaTropModel( const Position& RX,
                       const CommonTime& time );


         /// Load GPT2 grid file.
         ///
         /// @param file GPT2 grid file.
      void loadFile(const std::string& file)
         noexcept(false);


         /// Compute and return the full tropospheric delay. The receiver
         /// latitude, longitude, height and modified julian date must has
         /// been set before using the appropriate constructor or the provided
         /// methods.
         ///
         /// @param elevation Elevation of satellite as seen at receiver,
         ///                  in degrees.
      virtual double correction(double elevation) const
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite.
          *
          * This version is more useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as latitude, longitude and
          * height) and passes them to appropriate methods.
          *
          * You must set time using method setModifiedJulianDate() before
          * calling this method.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          */
      virtual double correction( const Position& RX,
                                 const Position& SV )
         noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the time tag.
          *
          * This version is more useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as latitude, longitude and
          * height) and passes them to appropriate methods.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          * @param tt  Time (CommonTime object).
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const CommonTime& tt )
        noexcept(false);


         /** Compute and return the full tropospheric delay, given the
          *  positions of receiver and satellite and the modified julian
          *  date.
          *
          * This version is more useful within positioning algorithms, where
          * the receiver position may vary; it computes the elevation (and
          * other receiver location information as latitude, longitude and
          * height) and passes them to appropriate methods.
          *
          * @param RX  Receiver position.
          * @param SV  Satellite position.
          * @param mjd Modified Julian Date.
          */
      virtual double correction( const Position& RX,
                                 const Position& SV,
                                 const double& mjd )
         noexcept(false);


         /// Compute and return the zenith delay for dry component of
         /// the troposphere.
      virtual double dry_zenith_delay(void) const
         noexcept(false);


         /// Compute and return the zenith delay for wet component of
         /// the troposphere.
      virtual double wet_zenith_delay(void) const
         noexcept(false);


         /// Compute and return the mapping function for dry component of
         /// the troposphere.
         ///
         /// @param elevation Elevation of satellite as seen at receiver, in
         ///                  degrees
      virtual double dry_mapping_function(double elevation) const
         noexcept(false);


         /// Compute and return the mapping function for wet component of
         /// the troposphere.
         ///
         /// @param elevation Elevation of satellite as seen at
         ///                  receiver, in degrees
      virtual double wet_mapping_function(double elevation) const
         noexcept(false);


         /// This method configure the model to estimate the weather using
         /// latitude, longitude, height and modified julian date (MJD). It
         /// is called automatically when setting those parameters.
      void setWeather()
         noexcept(false);


         /// In Vienna tropospheric model, this is a dummy method kept here
         /// just for consistency,
      virtual void setWeather( const double& T,
                               const double& P,
                               const double& H )
         noexcept(false) {};


         /// In Vienna tropospheric model, this is a dummy method kept here
         /// just for consistency
      virtual void setWeather(const WxObservation& wx)
         noexcept(false) {};


         /// Define the receiver latitude; this is required before calling
         /// correction() or any of the zenith_delay routines.
         ///
         /// @param lat  Latitude of receiver, in degrees.
      virtual void setReceiverLatitude(const double& lat);


         /// Define the receiver longitude; this is required before calling
         /// correction() or any of the zenith_delay routines.
         ///
         /// @param lat  Longitude of receiver, in degrees.
      virtual void setReceiverLongitude(const double& lon);


         /// Define the receiver height; this is required before calling
         /// correction() or any of the zenith_delay routines.
         ///
         /// @param ht   Height of the receiver above ellipsoid,
         ///             in meters.
      virtual void setReceiverHeight(const double& ht);


         /// Set the time when tropospheric correction will be computed for,
         /// in Modified Julian Date.
         ///
         /// @param mjd  Modified Julian Date.
      virtual void setModifiedJulianDate(const double& mjd);


         /// Set the time when tropospheric correction will be computed for,
         /// in Modified Julian Date.
         ///
         /// @param time  Time object.
      virtual void setModifiedJulianDate(const CommonTime& time);


         /** Convenient method to set all model parameters in one pass.
          *
          * @param time  Time object.
          * @param rxPos Receiver position object.
          */
      virtual void setAllParameters( const CommonTime& time,
                                     const Position& rxPos );


   private:

      std::vector<GPT2Data> gpt2DataVec;

      double ViennaMJD;
      double ViennaLat;
      double ViennaLon;
      double ViennaHeight;

      bool validMJD;
      bool validLat;
      bool validLon;
      bool validHeight;

      // pressure in hPa
      double pressure;
      // temperature in degree Celsius
      double temperature;
      // temperature lapse rate in degrees per km
      double tLapseRate;
      // mean temperature of the water vapor in degrees Kelvin
      double mtWaterVapor;
      // water vapor pressure in hPa
      double pWaterVapor;
      // hydrostatic mapping function coefficient at zero height (VMF1)
      double hmfCoeff;
      // wet mapping function coefficient (VMF1)
      double wmfCoeff;
      // water vapor decrease factor
      double dfWaterVapor;
      // geoid undulation in m
      double geoidUndu;

   };    // end class ViennaTropModel

}
#endif   // TROPOSPHERIC_MODELS_GPSTK
