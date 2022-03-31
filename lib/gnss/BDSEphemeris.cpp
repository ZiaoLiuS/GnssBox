/// @file BDSEphemeris.cpp Encapsulates the GPS legacy broadcast ephemeris and clock.
/// Inherits OrbitEph, which does most of the work; this class adds health and
/// accuracy information, fit interval, ionospheric correction terms and data
/// flags.

/// @author 
/// 
/// shoujian zhang, wang yishi, 2021-2022


#include <string>
#include "Exception.hpp"
#include "BDSEphemeris.hpp"
#include "BDSWeekSecond.hpp"
#include "GPSWeekSecond.hpp"
#include "CivilTime.hpp"

#include <Eigen/Eigen>
using namespace Eigen;

using namespace std;

namespace gnssSpace
{
    void BDSEphemeris::printData() const
    {
        cout << "****************************************************************"
             << "************" << endl
             << "BDS Broadcast Ephemeris Data: " << endl;
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
        ///spare
        cout << "GPSWeek: " << setw(16) << BDSWeek << endl;
        ///spare

        cout << "URA: " << setw(16) << URA << endl;
        cout << "SV_health: " << setw(16) << SV_health << endl;
        cout << "TGD1: " << setw(16) << TGD1 << endl;
        cout << "TGD2: " << setw(16) << TGD2 << endl;

        cout << "IODC: " << setw(16) << IODC << endl;
        cout << "HOWtime: " << setw(16) << HOWtime << endl;

        cout<<"ctToc: "<<ctToc.asString()<<endl;
        cout<< "ctToe: "<<ctToe.asString()<<endl;
    }

    double BDSEphemeris::svClockBias(const CommonTime& t) const
    {
        double dtc, elaptc;
        elaptc = t - ctToc;
        dtc = af0 + elaptc * (af1 + elaptc * af2);
        return dtc;
    }

    double BDSEphemeris::svClockDrift(const CommonTime& t) const
    {
        double drift, elaptc;
        elaptc = t - ctToc;
        drift = af1 + elaptc * af2;
        return drift;
    }

    // Compute satellite relativity correction (sec) at the given time
    // throw Invalid Request if the required data has not been stored.
    double BDSEphemeris::svRelativity(const CommonTime& t) const
    {
        WGS84Ellipsoid ell;
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

        return (REL_CONST_BDS * ecc * std::sqrt(A) * ::sin(Ek));
    }

    Xvt BDSEphemeris::svXvt(const SatID& sat, const CommonTime& t) const
    {
        Xvt sv;
        WGS84Ellipsoid ell;

        ///Semi-major axis
        double A = sqrt_A*sqrt_A;

        ///Computed mean motion (rad/sec)
        double n0 = std::sqrt(ell.gm()/(A*A*A));

        ///Time from ephemeris reference epoch
        ///remind here, t must be BDT
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

        ///BDS contains GEO/IGSO/MEO satellites
        ///the process of GEO satellites is different
        ///the prn between 1-5 and 59-61 is GEO satellites
        if(sat.id<=5 ||sat.id>=59)
        {
            double OMEGA_k = OMEGA_0 + OMEGA_DOT*tk
                             - ell.angVelocity() * Toe;

            double sinOMG_k = ::sin(OMEGA_k);
            double cosOMG_k = ::cos(OMEGA_k);
            double cosik = ::cos(ik);
            double sinik = ::sin(ik);

            double xgk  =  xip*cosOMG_k  -  yip*cosik*sinOMG_k;
            double ygk  =  xip*sinOMG_k  +  yip*cosik*cosOMG_k;
            double zgk  =                   yip*sinik;

            Eigen::MatrixXd Rx(3,3);
            Rx(0,0)= 1.0;
            Rx(0,1)= 0.0;
            Rx(0,2)= 0.0;
            Rx(1,0)= 0.0;
            Rx(1,1)= ::cos((-5)*PI/180);
            Rx(1,2)= ::sin((-5)*PI/180);
            Rx(2,0)= 0.0;
            Rx(2,1)= - ::sin((-5)*PI/180);
            Rx(2,2)= ::cos((-5)*PI/180);

            Eigen::MatrixXd Rz(3,3);
            Rz(0,0)= ::cos(ell.angVelocity()*tk);
            Rz(0,1)= ::sin(ell.angVelocity()*tk);
            Rz(0,2)= 0.0;
            Rz(1,0)= - ::sin(ell.angVelocity()*tk);
            Rz(1,1)= ::cos(ell.angVelocity()*tk);
            Rz(1,2)= 0.0;
            Rz(2,0)= 0.0;
            Rz(2,1)= 0.0;
            Rz(2,2)= 1.0;

            Eigen::MatrixXd XGK(3,1);
            XGK(0,0)=xgk;
            XGK(1,0)=ygk;
            XGK(2,0)=zgk;

            Eigen::MatrixXd Xk(3,1);

            Xk = Rz * Rx * XGK;

            sv.x[0] = Xk(0,0);
            sv.x[1] = Xk(1,0);
            sv.x[2] = Xk(2,0);

            /// derivatives of true anamoly and arg of latitude
            double dek,dlk,div,duv,drv,dxp,dyp;
            dek = n / G;
            dlk = sqrt_A * q * std::sqrt(ell.gm()) / (rk*rk);

            div = IDOT - 2.0e0 * dlk * (Cis*cos2phi_k - Cic*sin2phi_k);
            duv = dlk*(1.e0+ 2.e0 * (Cus*cos2phi_k - Cuc*sin2phi_k));
            drv = A * ecc * dek * sinEk + 2.e0 * dlk * (Crs * cos2phi_k - Crc * sin2phi_k);
            dxp = drv * ::cos(uk) - rk * ::sin(uk)*duv;
            dyp = drv * ::sin(uk) + rk * ::cos(uk)*duv;

            /// Time-derivative of Rz matrix
            Eigen::MatrixXd dmatZ(3,3);
            dmatZ(0,0) =  ::sin(ell.angVelocity()*tk) * -ell.angVelocity();
            dmatZ(0,1) = -::cos(ell.angVelocity()*tk) * -ell.angVelocity();
            dmatZ(0,2) =   0.0;
            dmatZ(1,0) =  ::cos(ell.angVelocity()*tk) * -ell.angVelocity();
            dmatZ(1,1) =  ::sin(ell.angVelocity()*tk) * -ell.angVelocity();
            dmatZ(1,2) =   0.0;
            dmatZ(2,0) =   0.0;
            dmatZ(2,1) =   0.0;
            dmatZ(2,2) =   0.0;

            /// Time-derivative of X,Y,Z in interial form
            Eigen::MatrixXd dIntPos(3,1);
            dIntPos(0,0) = - xip * sinOMG_k * OMEGA_DOT
                           + dxp * cosOMG_k
                           - yip * (cosik * cosOMG_k * OMEGA_DOT
                                    -sinik * sinOMG_k * div )
                           - dyp * cosik * sinOMG_k;
            dIntPos(1,0) =   xip * cosOMG_k * OMEGA_DOT
                             + dxp * sinOMG_k
                             - yip * (cosik * sinOMG_k * OMEGA_DOT
                                      +sinik * cosOMG_k * div )
                             + dyp * cosik * cosOMG_k;
            dIntPos(2,0) = yip * cosik * div + dyp * sinik;

            Eigen::MatrixXd V(3,1);
            V = Rz * Rx * dIntPos +
                    dmatZ * Rx * XGK;

            sv.v[0] = V(0,0);
            sv.v[1] = V(1,0);
            sv.v[2] = V(2,0);

            return sv;
        }

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

    bool BDSEphemeris::isValid(const CommonTime& ct) const
    {
        if(ct < beginValid || ct > endValid) return false;
        return true;
    }

} // end namespace
