/// @file GloEphemeris.hpp
/// Ephemeris data for GLONASS.

#ifndef GLOEPHEMERIS_HPP
#define GLOEPHEMERIS_HPP

#include <iostream>
#include "Triple.hpp"
#include "Xvt.hpp"
#include "CommonTime.hpp"
#include "PZ90Ellipsoid.hpp"
#include "YDSTime.hpp"
#include "SatID.hpp"
#include "CivilTime.hpp"
#include <Eigen/Eigen>

using namespace Eigen;
using namespace timeSpace;

namespace gnssSpace
{
   class GloEphemeris
   {
   public:
       /// Default constructor
       GloEphemeris()
               : step(1.0)
       {};

       /// Destructor.
       virtual ~GloEphemeris() {};

       /// Integration step for Runge-Kutta algorithm (1 second by default)
       double step;

       /// Dump the overhead information to the given output stream.
       /// throw Invalid Request if the required data has not been stored.
       void printData() const;

       /// Compute satellite position at the given time.
       Xvt svXvt(const CommonTime& epoch) const;

       double getSidTime( const CommonTime& time ) const;

       VectorXd derivative( const VectorXd& inState,
                            const VectorXd& accel ) const;

       ///Ephemeris data
       SatID satID;
       CivilTime CivilToc;      ///< Time of clock (year/month/day/hour/min/sec UTC)
       CommonTime ctToe;        ///< Toe in CommonTime form
       double Toc;
       double TauN;             ///< SV clock bias (sec)  clkbias
       double GammaN;           ///< SV relative frequency bias  clkdrift
       double MFtime;           ///< Message frame time (tk+nd*86400)
                                ///< in seconds of the UTC week

       double px;               ///< Satellite position X (km)
       double vx;               ///< velocity X dot (km/sec)
       double ax;               ///< X acceleration (km/sec2)
       double health;           ///< health (0=healthy, 1=unhealthy)

       double py;               ///< Satellite position Y (km)
       double vy;               ///< velocity Y dot (km/sec)
       double ay;               ///< Y acceleration (km/sec2)
       double freqNum;          ///< frequency number(-7...+13)

       double pz;               ///< Satellite position Z (km)
       double vz;               ///< velocity Z dot (km/sec)
       double az;               ///< Z acceleration (km/sec2)
       double ageOfInfo;        ///< Age of oper. information (days)

   };  // End of class 'GloEphemeris'

      //@}

}  // End of namespace gnssSpace

#endif   // GLOEPHEMERIS_HPP
