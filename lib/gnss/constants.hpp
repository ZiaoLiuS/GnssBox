
/**
 * @file constants.hpp
 * Constants.
 * 
 * 2020/08/06
 * add constrans defined by rinex3.04 
 * add IRNSS 
 * 
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "Exception.hpp"
#include "SatelliteSystem.hpp"

using namespace std;

using namespace utilSpace;

namespace gnssSpace
{
      /**
       * @addtogroup geodeticgroup
       * @name GNSS Constants
       * Time constants are in TimeConstants.hpp
       */
      //@{

      /** The maximum number of active satellites forGPS/GAL/GLO/BDS/QZSS constellation. */
    const long MAX_PRN_GPS    = 32;
    const long MAX_PRN_GAL    = 36;
    const long MAX_PRN_GLO    = 32;
    const long MAX_PRN_BDS    = 60;
    const long MAX_PRN_QZS    = 7;

    // ---------------- independent of GNSS ----------------------
      /// GPS value of PI; also specified by GAL
    const double PI        = 3.141592653589793238462643383280;
      /// GPS value of PI*2
    const double TWO_PI    = 6.283185307179586476925286766559;
      /// GPS value of PI**0.5
    const double SQRT_PI   = 1.772453850905516027298167483341;
      /// relativity constant (sec/sqrt(m))
    const double REL_CONST = -4.442807633e-10;

        /// relativity constant for BDS (sec/sqrt(m))
    const double REL_CONST_BDS = -4.442807309e-10;

      /// m/s, speed of light; this value defined by GPS but applies to GAL and GLO.
    const double C_MPS = 2.99792458e8;

    /// Ionospheric constant k
    const double C2_FACT   = 40.3e+16;


      /// Conversion Factor from degrees to radians (unit: degrees^-1)
    static const double DEG_TO_RAD   = 1.745329251994329576923691e-2;
      /// Conversion Factor from radians to degrees (unit: degrees)
    static const double RAD_TO_DEG   = 57.29577951308232087679815;
      /// Conversion Factor from arcseconds to radians (unit: arcseconds^-1)
    static const double AS_TO_RAD    = 4.848136811095359935899141e-6;
      /// Conversion Factor from radians to arcseconds (unit: arcseconds)
    static const double RAD_TO_AS    = 206264.8062470963551564734;
      /// Conversion Factor from seconds of time to radians (unit: seconds^-1)
    static const double SEC_TO_RAD   = 7.272205216643039903848712e-5;
      /// Conversion Factor from radians to seconds of time (unit: seconds)
    static const double RAD_TO_SEC   = 43200.0/PI;
      /// Arcseconds in a full circle
    static const double TURNAS       = 1296000.0;
      /// Seconds per day
    static const double DAY_TO_SEC   = 86400.0;
      /// Days per Julian Century
    static const double JC           = 36525.0;
      /// Modified Julian Date of the J2000 Epoch
    static const double MJD_J2000    = 51544.5;
      /// Julian Date of Modified Julian Date zero
    static const double JD_TO_MJD    = 2400000.5;
      /// Reference epoch (J2000.0), Julian Data
    static const double JD_J2000     = 2451545.0;


    // IERS 2010 Numerical Standards
    //-------------------------------------------------------------------------

    /// Constant of gravitation (m^3/kg/s^2)
    static const double G            = 6.67428e-11;

    /// Astronomical unit (m)
    static const double AU           = 1.49597870700e11;

      /// Solar radiation pressure at 1 AU (N/m^2)
     static const double P_SRP        = 4.560e-6;


      /// Geocentric gravitational constant
    static const double GM_EARTH     = 3.986004418e14;
      /// Equatorial radius of the Earth (m)
    static const double RE_EARTH     = 6378136.6;
      /// Nominal mean angular velocity of the Earth (rad/s)
    static const double OMEGA_EARTH  = 7.292115e-5;
      /// Flattening factor of the Earth
    static const double FLAT_EARTH   = 1.0/298.25642;
      /// Mean equatorial gravity (m/s^2)
    static const double GE_EARTH     = 9.7803278;
      /// Dynamical form factor of the Earth
    static const double J2_EARTH     = 1.0826359e-3;

      /// Heliocentric gravitational constant
    static const double GM_SUN       = 1.32712442099e20;
      /// Mean radius of the Sun (m)
    static const double RE_SUN       = 6.95700e+8;

      /// Moon-Earth mass ratio
    static const double RATIO_ME     = 0.0123000383;
    static const double GM_MOON      = GM_EARTH * RATIO_ME;
      /// Mean radius of the Moon (m)
    static const double RE_MOON      = 1738000.0;

      /// Sun-Mercury mass ratio
    static const double RATIO_SMe    = 6.0236e6;
    static const double GM_MERCURY   = GM_SUN / RATIO_SMe;

      /// Sun-Venus mass ratio
    static const double RATIO_SVe    = 4.08523719e5;
    static const double GM_VENUS     = GM_SUN / RATIO_SVe;

      /// Sun-Mars mass ratio
    static const double RATIO_SMa    = 3.09870359e6;
    static const double GM_MARS      = GM_SUN / RATIO_SMa;

      /// Sun-Jupiter mass ratio
    static const double RATIO_SJ     = 1.047348644e3;
    static const double GM_JUPITER   = GM_SUN / RATIO_SJ;

      /// Sun-Saturn mass ratio
    static const double RATIO_SSa    = 3.4979018e3;
    static const double GM_SATURN    = GM_SUN / RATIO_SSa;

      /// Sun-Uranus mass ratio
    static const double RATIO_SU     = 2.290298e4;
    static const double GM_URANUS    = GM_SUN / RATIO_SU;

      /// Sun-Neptune mass ratio
    static const double RATIO_SN     = 1.941226e4;
    static const double GM_NEPTUNE   = GM_SUN / RATIO_SN;

      /// Sun-Pluto mass ratio
    static const double RATIO_SP     = 1.36566e8;
    static const double GM_PLUTO     = GM_SUN / RATIO_SP;


    // ---------------- GPS --------------------------------------
      /// Hz, GPS Oscillator or chip frequency
    const double OSC_FREQ_GPS  = 10.23e6;
      /// Hz, GPS chip rate of the P & Y codes
    const double PY_CHIP_FREQ_GPS = OSC_FREQ_GPS;
      /// Hz, GPS chip rate of the C/A code
    const double CA_CHIP_FREQ_GPS = OSC_FREQ_GPS / 10.0;
      /// Hz, GPS Base freq w/o relativisitic effects
    const double RSVCLK_GPS    = 10.22999999543e6;

      /// GPS L1 carrier frequency in Hz
    const double L1_FREQ_GPS   = 1575.42e6;
      /// GPS L2 carrier frequency in Hz
    const double L2_FREQ_GPS   = 1227.60e6;
      /// GPS L5 carrier frequency in Hz
    const double L5_FREQ_GPS   = 1176.45e6;

      /// GPS L1 carrier wavelength in meters
    const double L1_WAVELENGTH_GPS  = 0.190293672798;
      /// GPS L2 carrier wavelength in meters
    const double L2_WAVELENGTH_GPS  = 0.244210213425;
      /// GPS L5 carrier wavelength in meters
    const double L5_WAVELENGTH_GPS  = 0.254828048791;

       // =   (f1^2*lambda1 * N1 - f2^2*lambda2 *N2)/(f1^2-f2^2)
       // =   (f1*N1 - f2*N2) * clight/(f1^2-f2^2)
       // =   (f1*N1 - f2*N1 + f2*N1 -f2*N2) * clight/(f1^2-f2^2)
       // = ( (f1-f2)*N1 + f2*Nw ) * clight/(f1^2-f2^2)
       // =   clight/(f1+f2)*N1 + f2*clight/(f1^2-f2^2)*Nw
       // =   lambda1*f1/(f1+f2)*N1 + lambda2*f2*f2/(f1^2-f2^2)*Nw
       // =   lambda1*f1/(f1+f2)*N1 + lambda1*f1*f2/(f1^2-f2^2)*Nw
       // 
       // lambLCN1 = lambda1*f1/(f1+f2)
       // lambLCNw = lambda2*f2^2/(f1^2-f2^2)

      /// GPS LC (L1+L2) carrier wavelength in meters
    const double LC12_WAVELENGTH_GPS= 0.106953378142;
      /// GPS LC (L1+L5) carrier wavelength in meters
    const double LC15_WAVELENGTH_GPS= 0.108941359149;
    const double LC25_WAVELENGTH_GPS= 0.124703087706;

      /// GPS WL (L1+L2) carrier wavelength in meters
    const double WL12_WAVELENGTH_GPS = 0.8619184003220;
      /// GPS WL (L1+L5) carrier wavelength in meters
    const double WL15_WAVELENGTH_GPS = 0.7514160413064;
    const double WL25_WAVELENGTH_GPS = 5.8610451221896;

      /// GPS L1 frequency in units of oscillator frequency
    const double L1_MULT_GPS   = 154.0;
      /// GPS L2 frequency in units of oscillator frequency
    const double L2_MULT_GPS   = 120.0;
      /// GPS L5 frequency in units of oscillator frequency
    const double L5_MULT_GPS   = 115.0;

      /// GPS Gamma constant, L1 and L2
    const double GAMMA_GPS_L1L2 = 1.646944444444444444;

      /// GPS Gamma constant, L1 and L5
    const double GAMMA_GPS_L1L5 = 1.793270321;

    const double GAMMA_GPS_L2L5 = 1.088846881;

     /// Reference Semi-major axis. From IS-GPS-800 Table 3.5-2 in meters.
    const double A_REF_GPS = 26559710.0;

     /// Omega reference value from Table 30-I converted to radians
    const double OMEGADOT_REF_GPS = -2.6e-9 * PI;

     /// Beta limit for GPS eclipse, unit: degrees
    const double BETA_GPS = 13.25;

    inline
    short getLegacyFitInterval(const short iodc, const short fiti)
        noexcept(false)
    {
         /* check the IODC */
        if (iodc < 0 || iodc > 1023)
        {
            /* error in iodc, return minimum fit */
            return 4;
        }

        if ( ( ( (fiti == 0) && (iodc & 0xFF) < 240 )
            || (iodc & 0xFF) > 255 ) )
        {
            /* fit interval of 4 hours */
            return 4;
        }
        else if (fiti == 1)
        {
            if( ((iodc & 0xFF) < 240 || (iodc & 0xFF) > 255))
            {
                /* fit interval of 6 hours */
                return 6;
            }
            else if(iodc >=240 && iodc <=247)
            {
                /* fit interval of 8 hours */
                return 8;
            }
            else if( ( (iodc >= 248) && (iodc <= 255) ) || iodc == 496 )
            {
                /* fit interval of 14 hours */
                return 14;
            }
            else if((iodc >= 497 && iodc <=503) || (iodc >= 1021 && iodc <= 1023))
            {
                /* fit interval of 26 hours */
                return 26;
            }
            else if(iodc >= 504 && iodc <=510)
            {
                /* fit interval of 50 hours */
                return 50;
            }
            else if( iodc == 511 || ( (iodc >= 752) && (iodc <= 756) ) )
            {
                /* fit interval of 74 hours */
                return 74;
            }
            else if(iodc == 757)
            {
                /* fit interval of 98 hours */
                return 98;
            }
            else
            {
                InvalidRequest exc("Invalid IODC Value For sv Block");
                THROW(exc);
            }
        }
        else
        {
            /* error in ephemeris/iodc, return minimum fit */
            return 4;
        }

        return 0; // never reached
    }


    // ---------------- GLONASS ----------------------------------
      /// GLO Fundamental chip rate in Hz.
    const double OSC_FREQ_GLO  = 5.11e6;
      /// GLO Chip rate of the P & Y codes in Hz.
    const double PY_CHIP_FREQ_GLO = OSC_FREQ_GLO;
      /// GLO Chip rate of the C/A code in Hz.
    const double CA_CHIP_FREQ_GLO = OSC_FREQ_GLO / 10.0;

      /// GLO Fundamental oscillator freq in Hz.
    const double PSC_FREQ_GLO  = 5.00e6;
      /// GLO Base freq w/o relativisitic effects in Hz.
    const double RSVCLK_GLO    = 4.99999999782e6;

      // GLO Frequency(Hz) f1 is 1602.0e6 + n*562.5e3 Hz = 9 * (178 + n*0.0625) MHz
      //                   f2    1246.0e6 + n*437.5e3 Hz = 7 * (178 + n*0.0625) MHz
      // where n is the time- and satellite-dependent 'frequency channel' -7 <= n <= 7
      /// GLO L1 carrier base frequency in Hz.
    const double L1_FREQ_GLO       = 1602.0e6;
      /// GLO L1 carrier frequency step size in Hz.
    const double L1_FREQ_STEP_GLO  = 562.5e3;

      /// GLO L2 carrier base frequency in Hz.
    const double L2_FREQ_GLO       = 1246.0e6;
      /// GLO L2 carrier frequency step size in Hz.
    const double L2_FREQ_STEP_GLO  = 437.5e3;

      /// GLO L1 multiplier.
    const double L1_MULT_GLO   = 320.4;

      /// GLO L2 multiplier.
    const double L2_MULT_GLO   = 249.2;

       /// GLONASS Gamma constant
    const double GAMMA_GLO = 81.0/49.0;

         /// GLONASS L3 carrier frequency in Hz.
   const double L3_FREQ_GLO   = 1202.025e6;
     /// GLONASS L3 carrier wavelength in meters
   const double L3_WAVELENGTH_GLO = C_MPS/L3_FREQ_GLO;

     /// GLO L4
   const double L4_FREQ_GLO   = 1600.995e6;
   const double L4_WAVELENGTH_GLO = C_MPS/L4_FREQ_GLO;

     /// GLO L6
   const double L6_FREQ_GLO   = 1248.06e6;
   const double L6_WAVELENGTH_GLO = C_MPS/L6_FREQ_GLO;
    

      /// Constant for the max array index in SV accuracy table.
    const int SV_ACCURACY_GLO_INDEX_MAX = 15;
      /// Map from SV accuracy/URA flag to NOMINAL accuracy values in m.
      /// Further details in ICD-GLO-v5.0, Table 4.4 in Section 4.4.
    const double SV_ACCURACY_GLO_INDEX[] = { 1.0,  2.0,   2.5,   4.0,   5.0,
                                             7.0, 10.0,  12.0,  14.0,  16.0,
                                            32.0, 64.0, 128.0, 256.0, 512.0,
                                             9.999999999999e99               };


    // ---------------- Galileo ----------------------------------

      /// GAL L5 (E5a) carrier frequency in Hz
    const double L5_FREQ_GAL   = 1176.45e6;
      /// GAL L8 (E5a+E5b) carrier frequency in Hz
    const double L8_FREQ_GAL   = 1191.795e6;
      /// GAL L7 (E5b) carrier frequency in Hz
    const double L7_FREQ_GAL   = 1207.140e6;
      /// GAL L6 (E6) carrier frequency in Hz
    const double L6_FREQ_GAL   = 1278.75e6;
      /// GAL L1 (E1) carrier frequency in Hz
    const double L1_FREQ_GAL   = 1575.42e6;

      /// GAL L1 carrier wavelength in meters
    const double L1_WAVELENGTH_GAL  = C_MPS/L1_FREQ_GAL;
    const double L6_WAVELENGTH_GAL  = C_MPS/L6_FREQ_GAL;
    const double L7_WAVELENGTH_GAL  = C_MPS/L7_FREQ_GAL;
    const double L8_WAVELENGTH_GAL  = C_MPS/L8_FREQ_GAL;
    const double L5_WAVELENGTH_GAL  = C_MPS/L5_FREQ_GAL;

      /// GAL LC (L1+L5) carrier wavelength in meters
    const double LC15_WAVELENGTH_GAL = 0.108941359148506;
    const double LC17_WAVELENGTH_GAL = 0.107739800040250;
    const double LC18_WAVELENGTH_GAL = 0.108337248099623;
    const double LC16_WAVELENGTH_GAL = 0.105036650935297;

      /// GAL WL (L1+L5) carrier wavelength in meters
    const double WL15_WAVELENGTH_GAL =  0.751416041306364;
    const double WL17_WAVELENGTH_GAL =  0.814034044748561;
    const double WL18_WAVELENGTH_GAL =  0.781472682958618;
    const double WL16_WAVELENGTH_GAL =  1.010525021067179;

    const double WL57_WAVELENGTH_GAL = -9.768408536982731;
    const double WL58_WAVELENGTH_GAL =-19.536817073965461;
    const double WL56_WAVELENGTH_GAL = -2.930522561094819;

    const double WL78_WAVELENGTH_GAL = 19.536817073965461;
    const double WL76_WAVELENGTH_GAL = -4.186460801564027;

    const double WL86_WAVELENGTH_GAL = -3.447673601288023;

      /// GAL L5 frequency in units of oscillator frequency
    const double L5_MULT_GAL   = 115.0;
      /// GAL L8 frequency in units of oscillator frequency
    const double L8_MULT_GAL   = 116.5;
      /// GAL L7 frequency in units of oscillator frequency
    const double L7_MULT_GAL   = 118.0;
      /// GAL L6 frequency in units of oscillator frequency
    const double L6_MULT_GAL   = 125.0;
      /// GAL L1 frequency in units of oscillator frequency
    const double L1_MULT_GAL   = 154.0;

      /// Galileo Gamma constant, E1 and E5a
    const double GAMMA_GAL_L1L5 = 1.793270321;
      /// Galileo Gamma constant, E1 and E5a+E5b
    const double GAMMA_GAL_L1L8 = 1.747388974;
      /// Galileo Gamma constant, E1 and E5b
    const double GAMMA_GAL_L1L7 = 1.703246194;
      /// Galileo Gamma constant, E1 and E6
    const double GAMMA_GAL_L1L6 = 1.517824000;

    // ---------------- Geostationary (SBAS) ---------------------
      /// GEO L1 carrier frequency in Hz
    const double L1_FREQ_GEO   = L1_FREQ_GPS;
      /// GEO L5 carrier frequency in Hz.
    const double L5_FREQ_GEO   = L5_FREQ_GPS;

      /// GEO L1 carrier wavelength in meters
    const double L1_WAVELENGTH_GEO  = L1_WAVELENGTH_GPS;
      /// GEO L5 carrier wavelength in meters.
    const double L5_WAVELENGTH_GEO  = L5_WAVELENGTH_GPS;


    // ---------------- BeiDou-2/-3 ----------------------------------

      /// BDS L1 frequency in units of oscillator frequency
    const double L2_MULT_BDS   = 152.6;
    const double L7_MULT_BDS   = 118.0;
    const double L6_MULT_BDS   = 124.0;

    // BDS-3 L1/L2/L5/L7/L6
    // BDS-2 L2/L7/L6
    const double L5_FREQ_BDS   = 1176.450e6; // B2a (BDS-3)
    const double L8_FREQ_BDS   = 1191.795e6; // B2=B21+B2b/2
    const double L7_FREQ_BDS   = 1207.140e6; // B2b (BDS-3/BDS-2)
    const double L6_FREQ_BDS   = 1268.520e6; // B3  (BDS-3/BDS-2)
    const double L2_FREQ_BDS   = 1561.098e6; // B1I (BDS-3/BDS-2)
    const double L1_FREQ_BDS   = 1575.420e6; // B1C (BDS-3)

    const double L1_WAVELENGTH_BDS  = C_MPS/L1_FREQ_BDS;
    const double L2_WAVELENGTH_BDS  = C_MPS/L2_FREQ_BDS;
    const double L6_WAVELENGTH_BDS  = C_MPS/L6_FREQ_BDS;
    const double L7_WAVELENGTH_BDS  = C_MPS/L7_FREQ_BDS;
    const double L8_WAVELENGTH_BDS  = C_MPS/L8_FREQ_BDS;
    const double L5_WAVELENGTH_BDS  = C_MPS/L5_FREQ_BDS;

      /// BDS LC (L2+L7) carrier wavelength in meters.
    const double LC27_WAVELENGTH_BDS = 0.108297212161671;
    const double LC26_WAVELENGTH_BDS = 0.105948031854476;

    const double WL27_WAVELENGTH_BDS = 0.846971838466711;
    const double WL26_WAVELENGTH_BDS = 1.024658238145014;

    const double GAMMA_BDS_L1L2 = 1.018432791852536;
    const double GAMMA_BDS_L1L5 = 1.793270321361056;
    const double GAMMA_BDS_L1L7 = 1.703246193622520;
    const double GAMMA_BDS_L1L8 = 1.747388973825267;
    const double GAMMA_BDS_L1L6 = 1.542403746097812;

      /// BDS Gamma constant, B1 and B2
    const double GAMMA_BDS_L2L7 = 1.67241884515943;
    const double GAMMA_BDS_L2L6 = 1.51448751300728;

    const double GAMMA_BDS_L2L1 = 0.981900826446280;
    const double GAMMA_BDS_L2L5 = 1.760813610586011;
    const double GAMMA_BDS_L2L8 = 1.715762677522149;


    // ---------------- QZSS ----------------------------------
      /// QZS L1 carrier frequency in Hz.
    const double L1_FREQ_QZS   = L1_FREQ_GPS;
      /// QZS L2 carrier frequency in Hz.
    const double L2_FREQ_QZS   = L2_FREQ_GPS;
      /// QZS L5 carrier frequency in Hz.
    const double L5_FREQ_QZS   = L5_FREQ_GPS;
      /// QZS LEX(6) carrier frequency in Hz.
    const double L6_FREQ_QZS   = L6_FREQ_GAL;

      /// QZS L1 carrier wavelength in meters.
    const double L1_WAVELENGTH_QZS   = L1_WAVELENGTH_GPS;
      /// QZS L2 carrier wavelength in meters.
    const double L2_WAVELENGTH_QZS   = L2_WAVELENGTH_GPS;
      /// QZS L5 carrier wavelength in meters.
    const double L5_WAVELENGTH_QZS   = L5_WAVELENGTH_GPS;
      /// QZS LEX(6) carrier wavelength in meters.
    const double L6_WAVELENGTH_QZS   = L6_WAVELENGTH_GAL;

    // ---------------- IRNSS ----------------------------------

    const double L5_FREQ_IRNSS   = L5_FREQ_GPS;
    const double L9_FREQ_IRNSS   = 2492.028e6;

    const double L5_WAVELENGTH_IRNSS   = L5_WAVELENGTH_GPS;
    const double L9_WAVELENGTH_IRNSS   = C_MPS/L9_FREQ_IRNSS;



    // ---------------- Convenience routines ---------------------
    /// Compute wavelength for the given satellite system (sat.id is ignored) at the
    /// given RINEX frequency band n(=1,2,5,6,7,8). Return 0 if the frequency n is
    /// not valid for the system.
    /// Calls for system GLO must include the frequency channel number N (-7<=N<=7).
    inline double getWavelength(const SatelliteSystem& sys, const int& n, const int N=0)
        throw()
    {
        if(n==0)
        {
            cerr << "getWavelength():frequency no must be positive integer!" << endl;
            exit(-1);
        }

        switch(sys.system) {
            case SatelliteSystem::GPS:
                     if(n == 1) return L1_WAVELENGTH_GPS;
                else if(n == 2) return L2_WAVELENGTH_GPS;
                else if(n == 5) return L5_WAVELENGTH_GPS;
                break;
            case SatelliteSystem::GLONASS:
                     if(n == 1) return (C_MPS/(L1_FREQ_GLO + N*L1_FREQ_STEP_GLO));
                else if(n == 2) return (C_MPS/(L2_FREQ_GLO + N*L2_FREQ_STEP_GLO));
                else if(n == 3) return L3_WAVELENGTH_GLO; // CDMA signal
                else if(n == 4) return L4_WAVELENGTH_GLO; // CDMA signal
                else if(n == 6) return L6_WAVELENGTH_GLO; // CDMA signal
                break;
            case SatelliteSystem::Galileo:
                     if(n == 1) return L1_WAVELENGTH_GAL;
                else if(n == 5) return L5_WAVELENGTH_GAL;
                else if(n == 6) return L6_WAVELENGTH_GAL;
                else if(n == 7) return L7_WAVELENGTH_GAL;
                else if(n == 8) return L8_WAVELENGTH_GAL;
                break;
            case SatelliteSystem::SBAS:
                     if(n == 1) return L1_WAVELENGTH_GEO;
                else if(n == 5) return L5_WAVELENGTH_GEO;
                break;
            case SatelliteSystem::BDS:
                     if(n == 1) return L1_WAVELENGTH_BDS;
                else if(n == 2) return L2_WAVELENGTH_BDS;
                else if(n == 5) return L5_WAVELENGTH_BDS;
                else if(n == 7) return L7_WAVELENGTH_BDS;
                else if(n == 8) return L8_WAVELENGTH_BDS;
                else if(n == 6) return L6_WAVELENGTH_BDS;
                break;
            case SatelliteSystem::QZSS:
                     if(n == 1) return L1_WAVELENGTH_QZS;
                else if(n == 2) return L2_WAVELENGTH_QZS;
                else if(n == 5) return L5_WAVELENGTH_QZS;
                else if(n == 6) return L6_WAVELENGTH_QZS;
                break;
            case SatelliteSystem::IRNSS:
                     if(n == 5) return L5_WAVELENGTH_IRNSS;
                else if(n == 9) return L9_WAVELENGTH_IRNSS;
                break;
        }

        return 0.0;
    }

    // ---------------- Convenience routines ---------------------
    /// Compute wavelength for the given satellite system (sat.id is ignored) at the
    /// given RINEX frequency band n(=1,2,5,6,7,8). Return 0 if the frequency n is
    /// not valid for the system.
    /// Calls for system GLO must include the frequency channel number N (-7<=N<=7).
    inline double getFreq(const SatelliteSystem& sys, const int& n, const int N=0)
        throw()
    {
        switch(sys.system) {
            case SatelliteSystem::GPS:
                     if(n == 1) return L1_FREQ_GPS;
                else if(n == 2) return L2_FREQ_GPS;
                else if(n == 5) return L5_FREQ_GPS;
                break;
            case SatelliteSystem::GLONASS:
                     if(n == 1) return ((L1_FREQ_GLO + N*L1_FREQ_STEP_GLO));
                else if(n == 2) return ((L2_FREQ_GLO + N*L2_FREQ_STEP_GLO));
                else if(n == 3) return L3_FREQ_GLO; // CDMA signal
                else if(n == 4) return L4_FREQ_GLO; // CDMA signal
                else if(n == 6) return L6_FREQ_GLO; // CDMA signal
                break;
            case SatelliteSystem::Galileo:
                     if(n == 1) return L1_FREQ_GAL;
                else if(n == 5) return L5_FREQ_GAL;
                else if(n == 6) return L6_FREQ_GAL;
                else if(n == 7) return L7_FREQ_GAL;
                else if(n == 8) return L8_FREQ_GAL;
                break;
            case SatelliteSystem::SBAS:
                     if(n == 1) return L1_FREQ_GEO;
                else if(n == 5) return L5_FREQ_GEO;
                break;
            case SatelliteSystem::BDS:
                     if(n == 1) return L1_FREQ_BDS;
                else if(n == 2) return L2_FREQ_BDS;
                else if(n == 5) return L5_FREQ_BDS;
                else if(n == 7) return L7_FREQ_BDS;
                else if(n == 8) return L8_FREQ_BDS;
                else if(n == 6) return L6_FREQ_BDS;
                break;
            case SatelliteSystem::QZSS:
                     if(n == 1) return L1_FREQ_QZS;
                else if(n == 2) return L2_FREQ_QZS;
                else if(n == 5) return L5_FREQ_QZS;
                else if(n == 6) return L6_FREQ_QZS;
                break;
            case SatelliteSystem::IRNSS:
                     if(n == 5) return L5_FREQ_IRNSS;
                else if(n == 9) return L9_FREQ_IRNSS;
                break;
        }

        return 0.0;
    }


    /// Compute beta(a,b), the ratio of 2 frequencies fb/fa for the given satellite
    /// system (sat.id is ignored). Return 0 if either of the input n's are not valid
    /// RINEX bands (n=1,2,5,6,7,or 8) for the system.
    inline double getBeta(const SatelliteSystem& sys, const int& na, const int& nb) throw()
    {
        double wla = getWavelength(sys,na);
        double wlb = getWavelength(sys,nb);
        if(wla == 0.0 || wlb == 0.0) return 0.0;
        return wlb/wla;
    }

    /// Compute alpha (also called gamma) = (beta^2-1) = ((fa/fb)^2-1)
    /// for 2 frequencies fa,fb for the given satellite system (sat.id is ignored).
    /// Return 0 if either of the input n's are not valid RINEX bands (n=1,2,5,6,7,8)
    /// for the satellite system.
    inline double getAlpha(const SatelliteSystem& sys, const int& na, const int& nb) throw()
    {
        double beta(getBeta(sys,na,nb));
        if(beta == 0.0) return 0.0;
        return (beta*beta-1.0);
    }

      //@}

} // namespace

#endif //CONSTANTS_HPP
