/// @file GPSEphemeris.cpp Encapsulates the GPS legacy broadcast ephemeris and clock.
/// Inherits OrbitEph, which does most of the work; this class adds health and
/// accuracy information, fit interval, ionospheric correction terms and data
/// flags.


#include <string>
#include "Exception.hpp"
#include "GPSWeekSecond.hpp"
#include "CivilTime.hpp"
#include "MiscMath.hpp"
#include "GPSEphemeris.hpp"

using namespace std;

namespace gnssSpace
{
    void GPSEphemeris::printData() const
    {
        cout << "****************************************************************"
           << "************" << endl
           << "GPS Broadcast Ephemeris Data: " << endl;
        cout   << "Satellite: " <<  satID.toString()<< setfill(' ') << endl;
        cout << "Toc: "<< this->CivilToc.year<<" "<<this->CivilToc.month<<" "
             << this->CivilToc.day<<" "<<this->CivilToc.hour<<" "
             << this->CivilToc.minute<<" "<<this->CivilToc.second<<endl;
        cout << scientific << setprecision(8)
           << "af0: " << setw(16) << af0 << endl
           << "af1: " << setw(16) << af1 << endl
           << "af2: " << setw(16) << af2 << endl;

        cout << "IODE: " << setw(16) <<  IODE << endl
           << "Crs:  " << setw(16) <<  Crs << endl
           << "Delta_n: " << setw(16) << Delta_n << endl
           << "M0: " << setw(16) << M0 << endl;

        cout << "Cuc: " << setw(16) << Cuc << endl
           << "ecc: " << setw(16) << ecc << endl
           << "Cus: " << setw(16) << Cus << endl
           << "sqrt_A: " << setw(16) << sqrt_A << endl;

        cout << "Toe: " << setw(16) << Toe << endl;
        cout << "Cic: " << setw(16) << Cic << endl;
        cout << "OMEGA_0: " << setw(16) << OMEGA_0 << endl;
        cout << "Cis: " << setw(16) << Cis << endl;

        cout << "i0: " << setw(16) << i0 << endl;
        cout << "Crc: " << setw(16) << Crc << endl;
        cout << "omega: " << setw(16) << omega << endl;
        cout << "OMEGA_DOT: " << setw(16) << OMEGA_DOT << endl;

        cout << "IDOT: " << setw(16) << IDOT << endl;
        cout << "Codes_On_L2_Channel: " << setw(16) << L2Codes << endl;
        cout << "GPSWeek: " << setw(16) << GPSWeek << endl;
        cout << "L2P_data_flag: " << setw(16) << L2Pflag << endl;

        cout << "URA: " << setw(16) << URA << endl;
        cout << "SV_health: " << setw(16) << SV_health << endl;
        cout << "TGD: " << setw(16) << TGD << endl;
        cout << "IODC: " << setw(16) << IODC << endl;

        cout << "HOWtime: " << setw(16) << HOWtime << endl;
        cout << "fitInterval: " << setw(16) << fitInterval << endl;

        cout<<"ctToc: "<<ctToc.asString()<<endl;
        cout<< "ctToe: "<<ctToe.asString()<<endl;

    }


    double GPSEphemeris::svClockBias(const CommonTime& t) const
    {
        double dtc, elaptc;
        elaptc = t - ctToc;
        dtc = af0 + elaptc * (af1 + elaptc * af2);
        return dtc;
    }

    double GPSEphemeris::svClockDrift(const CommonTime& t) const
    {
        double drift, elaptc;
        elaptc = t - ctToc;
        drift = af1 + elaptc * af2;
        return drift;
    }

    // Compute satellite relativity correction (sec) at the given time
    // throw Invalid Request if the required data has not been stored.
    double GPSEphemeris::svRelativity(const CommonTime& t) const
    {
        GPSEllipsoid ell;
        ///Semi-major axis
        double A = sqrt_A*sqrt_A;

        ///Computed mean motion (rad/sec)
        double n0 = std::sqrt(ell.gm()/(A*A*A));

        ///Time from ephemeris reference epoch
        double tk = t - ctToe;
        if(tk > 302400)  tk = tk-604800;
        if(tk < -302400) tk = tk+604800;

        ///Corrected mean motion
        double n = n0 + Delta_n;

        ///Mean anomaly
        double Mk = M0 + n*tk;

        ///Kepler's Equation for Eccentric Anomaly
        ///solved by iteration
        double twoPI = 2.0e0 * PI;
        Mk = fmod(Mk, twoPI);
        double Ek = Mk + ecc* ::sin(Mk);
        int loop_cnt = 1;
        double F,G,delea;
        do  {
            F = Mk - (Ek - ecc * ::sin(Ek));
            G = 1.0 - ecc * ::cos(Ek);
            delea = F/G;
            Ek = Ek + delea;
            loop_cnt++;
        } while ((fabs(delea) > 1.0e-11) && (loop_cnt <= 20));

        return (REL_CONST * ecc * SQRT(A) * ::sin(Ek));
    }


    Xvt GPSEphemeris::svXvt(const CommonTime& t) const
    {
        Xvt sv;
        GPSEllipsoid ell;

        ///Semi-major axis
        double A = sqrt_A*sqrt_A;

        ///Computed mean motion (rad/sec)
        double n0 = std::sqrt(ell.gm()/(A*A*A));

        ///Time from ephemeris reference epoch
        double tk = t - ctToe;
        if(tk > 302400)  tk = tk-604800;
        if(tk < -302400) tk = tk+604800;

        ///Corrected mean motion
        double n = n0 + Delta_n;

        ///Mean anomaly
        double Mk = M0 + n*tk;

        ///Kepler's Equation for Eccentric Anomaly
        ///solved by iteration
        double twoPI = 2.0e0 * PI;
        Mk = fmod(Mk, twoPI);
        double Ek = Mk + ecc* ::sin(Mk);
        int loop_cnt = 1;
        double F,G,delea;
        do  {
            F = Mk - (Ek - ecc * ::sin(Ek));
            G = 1.0 - ecc * ::cos(Ek);
            delea = F/G;
            Ek = Ek + delea;
            loop_cnt++;
        } while ((fabs(delea) > 1.0e-11) && (loop_cnt <= 20));

        ///compute clock corrections
        sv.relcorr = svRelativity(t);
        sv.clkbias = svClockBias(t);
        sv.clkdrift = svClockDrift(t);
        sv.frame = ReferenceFrame::WGS84;

        ///True Anomaly
        double q = std::sqrt(1.0 - ecc*ecc);
        double sinEk = ::sin(Ek);
        double cosEk = ::cos(Ek);

        double GSTA = q*sinEk;
        double GCTA = cosEk - ecc;
        double vk = atan2(GSTA, GCTA);

        ///Eccentric Anomaly
        //Ek = std::acos((ecc+ ::cos(vk))/(1+ecc* ::cos(vk)));

        ///Argument of Latitude
        double phi_k = vk + omega;
        double cos2phi_k = ::cos(2.0*phi_k);
        double sin2phi_k = ::sin(2.0*phi_k);

        double duk = cos2phi_k*Cuc + sin2phi_k*Cus;
        double drk = cos2phi_k*Crc + sin2phi_k*Crs;
        double dik = cos2phi_k*Cic + sin2phi_k*Cis;

        double uk = phi_k + duk;
        double rk = A*(1.0 - ecc*cosEk) + drk;
        double ik = i0 + dik + IDOT*tk;

        ///Positions in orbital plane.
        double xip = rk * ::cos(uk);
        double yip = rk * ::sin(uk);

        ///Corrected longitude of ascending node.
        double OMEGA_k = OMEGA_0 + (OMEGA_DOT - ell.angVelocity())*tk
                        - ell.angVelocity() * Toe;

        ///Earth-fixed coordinates.
        double sinOMG_k = ::sin(OMEGA_k);
        double cosOMG_k = ::cos(OMEGA_k);
        double cosik = ::cos(ik);
        double sinik = ::sin(ik);

        double xef  =  xip*cosOMG_k  -  yip*cosik*sinOMG_k;
        double yef  =  xip*sinOMG_k  +  yip*cosik*cosOMG_k;
        double zef  =                   yip*sinik;
        sv.x[0] = xef;
        sv.x[1] = yef;
        sv.x[2] = zef;

        /// Compute velocity of rotation coordinates
        double dek,dlk,div,domk,duv,drv,dxp,dyp;
        dek = n * A / rk;
        dlk = sqrt_A * q * std::sqrt(ell.gm()) / (rk*rk);
        div = IDOT - 2.0e0 * dlk * (Cic*sin2phi_k - Cis*cos2phi_k);
        domk = OMEGA_DOT - ell.angVelocity();
        duv = dlk*(1.e0+ 2.e0 * (Cus*cos2phi_k - Cuc*sin2phi_k));
        drv = A * ecc * dek * sinEk - 2.e0 * dlk * (Crc * sin2phi_k - Crs * cos2phi_k);
        dxp = drv * ::cos(uk) - rk * ::sin(uk)*duv;
        dyp = drv * ::sin(uk) + rk * ::cos(uk)*duv;

        /// Calculate velocities
        double vxef = dxp*cosOMG_k - xip * sinOMG_k * domk - dyp * cosik * sinOMG_k
               + yip * (sinik * sinOMG_k*div - cosik * cosOMG_k*domk);
        double vyef = dxp*sinOMG_k + xip * cosOMG_k * domk + dyp * cosik * cosOMG_k
               - yip * (sinik * cosOMG_k*div + cosik * sinOMG_k*domk);
        double vzef = dyp * sinik + yip * cosik * div;

        sv.v[0] = vxef;
        sv.v[1] = vyef;
        sv.v[2] = vzef;

        return sv;
    }

    bool GPSEphemeris::isValid(const CommonTime& ct) const
    {
        if(ct < beginValid || ct > endValid) return false;
        return true;
    }


} // end namespace
