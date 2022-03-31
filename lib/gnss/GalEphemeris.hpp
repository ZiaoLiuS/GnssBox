
//////////////////////////////////
//
// modified from the gpstk, redesign the interface
//
// @author
// 
// shoujian zhang, yishi wang, 2021-2022
//
//////////////////////////////////

#ifndef GALORBITEPH_HPP
#define GALORBITEPH_HPP

#include <string>
#include <cmath>

#include "Exception.hpp"
#include "CommonTime.hpp"
#include "CivilTime.hpp"
#include "SatID.hpp"
#include "Xvt.hpp"
#include "GALEllipsoid.hpp"

using namespace timeSpace;

namespace gnssSpace
{
      /// @ingroup GNSSEph
      //@{

   class GalEphemeris
   {
   public:
       /// Default constuctor
       GalEphemeris(void)
               : beginValid(CommonTime::END_OF_TIME),
                 endValid(CommonTime::BEGINNING_OF_TIME)
       {
           beginValid.setTimeSystem(TimeSystem::GAL);
           endValid.setTimeSystem(TimeSystem::GAL);
           ctToe.setTimeSystem(TimeSystem::GAL);
           ctToc.setTimeSystem(TimeSystem::GAL);
           transmitTime.setTimeSystem(TimeSystem::GAL);
       }

       /// Destructor
       virtual ~GalEphemeris(void) {}

       /// Dump the overhead information to the given output stream.
       /// throw Invalid Request if the required data has not been stored.
       void printData() const;

       /// Compute the satellite clock bias (seconds) at the given time
       /// throw Invalid Request if the required data has not been stored.
       double svClockBias(const CommonTime& t) const;

       /// Compute the satellite clock drift (sec/sec) at the given time
       /// throw Invalid Request if the required data has not been stored.
       double svClockDrift(const CommonTime& t) const;

       /// Compute satellite relativity correction (sec) at the given time
       /// throw Invalid Request if the required data has not been stored.
       double svRelativity(const CommonTime& t) const;

       /// Compute satellite position at the given time.
       Xvt svXvt(const CommonTime& t) const;

       bool isValid(const CommonTime& ct) const;

       ///Ephemeris data
       ///   SV/EPOCH/SV CLK
       SatID satID;               ///< Define satellite system and specific SV
       CivilTime CivilToc;
       double Toc;             ///< Time of clock (year/month/day/hour/min/sec  GAL)
       double af0;                ///< SV clock bias(seconds)
       double af1;                ///< SV clock drift(sec/sec)
       double af2;                ///< SV clock drift rate (sec/sec2)

       ///   BROADCAST ORBIT-1
       double IODE;               ///< IODE Issue of Data, Ephemeris
       double Crs;                ///< (meters)
       double Delta_n;            ///< Mean Motion Difference From Computed Value(semi-circles/sec)
       double M0;                 ///< Mean Anomaly at Reference Time(semi-circles)

       ///   BROADCAST ORBIT-2
       double Cuc;                ///< (radians)
       double ecc;                ///< Eccentricity
       double Cus;                ///< (radians)
       double sqrt_A;             ///< Square Root of the Semi-Major Axis(sqrt(m))

       ///   BROADCAST ORBIT-3
       double Toe;                ///< Time of Ephemeris(sec of BDT week)
       double Cic;                ///< (radians)
       double OMEGA_0;            ///< Longitude of Ascending Node of Orbit Plane(semi-circles)
       double Cis;                ///< (radians)

       ///   BROADCAST ORBIT-4
       double i0;                 ///< Inclination Angle at Reference Time(semi-circles)
       double Crc;                ///< (meters)
       double omega;              ///< Argument of Perigee(semi-circles)
       double OMEGA_DOT;          ///< Rate of Right Ascension(semi-circles/sec)

       ///   BROADCAST ORBIT-5
       double IDOT;               ///< Rate of Inclination Angle(semi-circles/sec)
       double dataSource;
       double GALWeek;            ///< to go with TOE, Continuous, not mod 1024
       ///spare

       ///   BROADCAST ORBIT-6
       double URA;                ///< SV accuracy(meters) SISA
       double SV_health;          ///< bits 17-22 w 3 sf 1
       double TGD1;               ///< (E5a/E1 seconds)
       double TGD2;               ///< (E5b/E1 seconds)

       ///   BROADCAST ORBIT-7
       long HOWtime;              ///< Transmission time of message， sec of GALWeek


       ///member data
       CommonTime ctToc;          ///< Toc in CommonTime form
       CommonTime ctToe;          ///< Toe in CommonTime form
       CommonTime transmitTime;   ///< Transmission time in CommonTime form
       CommonTime beginValid;     ///< Time at beginning of validity
       CommonTime endValid;       ///< Time at end of fit validity


   private:
       /// Get the fit interval in hours from the fit interval flag and the IODC
       static short getFitInterval(const short IODC, const short fitIntFlag);


   }; // end class BDSEphemeris

      //@}

} // end namespace

#endif // GALORBITEPH_HPP
