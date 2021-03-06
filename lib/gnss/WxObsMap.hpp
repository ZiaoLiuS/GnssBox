#pragma ident "$Id$"


#ifndef WXOBSMAP_HPP
#define WXOBSMAP_HPP

/**
 * @file WxObsMap.hpp
 * A class encapsulating and managing wx observations data.
 */


#include <map>
#include <iostream>

#include "CommonTime.hpp"

using namespace utilSpace;
using namespace timeSpace;

namespace gnssSpace
{
   /// A Single Weather Observation.
   struct WxObservation
   {
      /// Default Constructor
      WxObservation() throw()
         : t(CommonTime::END_OF_TIME), temperatureSource(noWx),
           pressureSource(noWx), humiditySource(noWx)
      {}

      /** Constructor
       * @param t the time of the observation
       * @param temp the recorded temperature (deg. Centigrade)
       * @param pres the recorded pressure (millibars)
       * @param humid the recorded relative humidity (%)
       */
      WxObservation(const CommonTime& t, 
                    float temp, 
                    float pres, 
                    float humid)
         throw()
         :t(t),
          temperature(temp), pressure(pres),
          humidity(humid), temperatureSource(obsWx),
          pressureSource(obsWx),  humiditySource(obsWx)
      {}


      CommonTime t;  ///< Time that this data was collected, in Rx GPS time.
      float  temperature;  ///< degrees Centigrade
      float  pressure;     ///< millibars
      float  humidity;     ///< percent

      /// An enumeration of the various weather sources.
      enum EWxSrc 
      {
         noWx,  ///< No weather measurements were available.
         defWx, ///< Default weather was used.
         obsWx  ///< The Observed Weather was used.
      };
      
      enum EWxSrc temperatureSource; ///< source of the temperature meas.
      enum EWxSrc pressureSource;    ///< source of the pressure meas.
      enum EWxSrc humiditySource;    ///< source of the humidity meas.

      /** Return whether all weather values in this object are valid.
       * @return whether all weather values in this object are valid
       */
      bool isAllValid() const throw();

      /** Friendly Output Operator.
       * @param s the output stream to which data is sent
       * @param obs the WxObservation from which the data comes
       * @return a reference to the modified ostream
       */
      friend std::ostream& operator<<(std::ostream& s, 
                                      const WxObservation& obs) throw();
   };


   /// This is a map of weather observations over time.  The key 
   /// should be the same time as the time of the epoch.
   typedef std::map<CommonTime, WxObservation> WxObsMap;

   /// This is a time history weather data from a single site
   struct WxObsData
   {
      /// Constructor
      WxObsData() throw()
         :firstTime(CommonTime::END_OF_TIME), 
          lastTime(CommonTime::BEGINNING_OF_TIME) {}
     
      // First and last time of any data in this object
      CommonTime firstTime; ///< Time of the oldest data in this object.
      CommonTime lastTime;  ///< Time of the youngest data in this object.
   
      /// This is the identifier of the site.
      unsigned rxId;

      /// The actual data.
      WxObsMap obs;

      /** Get the last WxObservation made before time t.
       * @return the WxObservation coming before time t
       */
      WxObservation getMostRecent(const CommonTime& t) const throw();
      
      /** Insert a WxObservation.
       * @param obs the WxObservation to insert.
       */
      void insertObservation(const WxObservation& obs) throw();
      
      /**
       * Removes all stored #WxObservation objects older than time \a t.
       * \param t remove #WxObservation objects older than this
       */
      void flush(const CommonTime& t) throw();

      /**
       * Find a #WxObservation object for time \a t.
       * A #WxObservation object will be retured. If interpolation is used,
       * the first object immediately before or at time \a t and the first
       * object immediately after time \a t will be linearly interpolated
       * between to return a data point at time \a t. If there is not enough
       * data to interpolate, the nearest real observation will be returned.
       * Any object returned or used in the interpolation scheme must fit
       * within the specified time interval around \a t (if \a iv is one
       * hour, a point must be within an hour before or after time \a t.)
       * \param t the time of interest
       * \param interpolate true if interpolation between points is wanted
       * \param iv time interval (seconds, before and after) around \a t
       * \return a #WxObservation object
       * \exception ObjectNotFound a #WxObservation object not available
       */
      WxObservation getWxObservation(const CommonTime& t,
                                     unsigned iv = 3600,
                                     bool interpolate = true) const
         noexcept(false);
   };
} // namespace
#endif 
