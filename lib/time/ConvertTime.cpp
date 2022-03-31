#include "Exception.hpp"
#include "ConvertTime.hpp"
#include "TimeSystem.hpp"

using namespace timeSpace;

namespace gnssSpace
{
   CommonTime convertTimeSystem(const CommonTime& inCommonTime,
                            const TimeSystem& targetSys)
   {
       CommonTime outCommonTime(inCommonTime);
       TimeSystem fromSys(inCommonTime.getTimeSystem());

       // is a conversion necessary?
       if(fromSys == targetSys)
           return outCommonTime;

       // first correct for leap seconds
       const CivilTime cvt(inCommonTime);
       double dt = Correction(fromSys, 
                              targetSys, 
                              cvt.year, 
                              cvt.month, 
                              cvt.day);

       outCommonTime += dt;
       // the corrected timetag: now only the system, not the value, matters
       outCommonTime.setTimeSystem(targetSys);

       return outCommonTime;      // never reached, satisfy some compilers
   }

   CivilTime convertTimeSystem(const CivilTime& inCivilTime,
                           const TimeSystem& targetSys)
   {

       CommonTime inCommonTime = inCivilTime.convertToCommonTime();
       CommonTime outCommonTime = convertTimeSystem(inCommonTime, targetSys);
       CivilTime outCivlTime(outCommonTime);
       return outCivlTime;
   }

   YDSTime convertTimeSystem(const YDSTime& inYDSTime,
                         const TimeSystem& targetSys)
   {
       CommonTime inCommonTime = inYDSTime.convertToCommonTime();
       CommonTime outCommonTime = convertTimeSystem(inCommonTime, targetSys);
       YDSTime outYDSTime(outCommonTime);
       return outYDSTime;
   }

   // The table 'leaps' must be modified when a new leap second is announced.
   // Return the number of leap seconds between UTC and TAI, that is the
   // difference in time scales UTC-TAI at an epoch defined by (year, month, day).
   // NB. Input day in a floating quantity and thus any epoch may be represented;
   // this is relevant the period 1960 to 1972, when UTC-TAI was not integral.
   // NB. GPS = TAI - 19sec and so GPS-UTC = getLeapSeconds()-19.
   double getLeapSeconds(const int year,
                         const int month,
                         const double day)
   {
      // Leap second data --------------------------------------------------------
      // number of changes before leap seconds (1960-1971) - this should never change.
      static const int NPRE=14;

      // epoch year, epoch month(1-12), delta t(sec), rate (sec/day) for [1960,1972).
      static const struct {
         int year, month;
         double delt, rate;
      } preleap[NPRE] = {
         { 1960,  1,  1.4178180, 0.0012960 },
         { 1961,  1,  1.4228180, 0.0012960 },
         { 1961,  8,  1.3728180, 0.0012960 },
         { 1962,  1,  1.8458580, 0.0011232 },
         { 1963, 11,  1.9458580, 0.0011232 },
         { 1964,  1,  3.2401300, 0.0012960 },
         { 1964,  4,  3.3401300, 0.0012960 },
         { 1964,  9,  3.4401300, 0.0012960 },
         { 1965,  1,  3.5401300, 0.0012960 },
         { 1965,  3,  3.6401300, 0.0012960 },
         { 1965,  7,  3.7401300, 0.0012960 },
         { 1965,  9,  3.8401300, 0.0012960 },
         { 1966,  1,  4.3131700, 0.0025920 },
         { 1968,  2,  4.2131700, 0.0025920 }
      };

      // Leap seconds history
      // ***** This table must be updated for new leap seconds **************
      static const struct {
         int year, month, nleap;
      } leaps[] = {
         { 1972,  1, 10 },
         { 1972,  7, 11 },
         { 1973,  1, 12 },
         { 1974,  1, 13 },
         { 1975,  1, 14 },
         { 1976,  1, 15 },
         { 1977,  1, 16 },
         { 1978,  1, 17 },
         { 1979,  1, 18 },
         { 1980,  1, 19 },
         { 1981,  7, 20 },
         { 1982,  7, 21 },
         { 1983,  7, 22 },
         { 1985,  7, 23 },
         { 1988,  1, 24 },
         { 1990,  1, 25 },
         { 1991,  1, 26 },
         { 1992,  7, 27 },
         { 1993,  7, 28 },
         { 1994,  7, 29 },
         { 1996,  1, 30 },
         { 1997,  7, 31 },
         { 1999,  1, 32 },
         { 2006,  1, 33 },
         { 2009,  1, 34 },
         { 2012,  7, 35 },
         { 2015,  7, 36 },
         { 2017,  1, 37 },// leave the last comma!
         // add new entry here, of the form:
         // { year, month(1-12), leap_sec }, // leave the last comma!
      };

      // the number of leaps (do not change this)
      static const int NLEAPS = sizeof(leaps)/sizeof(leaps[0]);

      // last year in leaps
      //static const int MAXYEAR = leaps[NLEAPS-1].year;

      // END static data -----------------------------------------------------

      // search for the input year, month
      if(year < 1960)                        // pre-1960 no deltas
         ;
      else if(month < 1 || month > 12)       // blunder, should never happen - throw?
         ;
      else if(year < 1972) {                 // [1960-1972) pre-leap
         for(int i=NPRE-1; i>=0; i--) {
            if(preleap[i].year > year ||
               (preleap[i].year == year && preleap[i].month > month)) continue;

            // found last record with < rec.year >= year and rec.month >= month
            // watch out - cannot use CommonTime here
            int iday(day);
            double dday(iday-int(day));
            if(iday == 0) { iday = 1; dday = 1.0-dday; }
            long JD0 = convertCalendarToJD(year,month,iday);
            long JD = convertCalendarToJD(preleap[i].year,preleap[i].month,1);
            return (preleap[i].delt + (double(JD0-JD)+dday)*preleap[i].rate);
         }
      }
      else {                                    // [1972- leap seconds
         for(int i=NLEAPS-1; i>=0; i--) {
            if(leaps[i].year > year ||
               (leaps[i].year == year && leaps[i].month > month)) continue;
            return double(leaps[i].nleap);
         }
      }

      return 0.0;
   }

   // Compute the conversion (in seconds) from one time system (inTS) to another
   // (outTS), given the year and month of the time to be converted.
   // Result is to be added to the first time (inTS) to yield the converted (outTS),
   // that is t(outTS) = t(inTS) + correction(inTS,outTS).
   // NB. the caller must not forget to change to outTS after adding this correction.
   // @param TimeSystem inTS, input system
   // @param TimeSystem outTS, output system
   // @param int year, year of the time to be converted.
   // @param int month, month (1-12) of the time to be converted.
   // @return double dt, correction (sec) to be added to t(in) to yield t(out).
   // @throw if input system(s) are invalid or Unknown.
   double Correction(const TimeSystem& inTS,
                     const TimeSystem& outTS,
                     const int year,
                     const int month,
                     const double day)
   {
      double dt(0.0);

      // identity
      if(inTS == outTS)
         return dt;

      // cannot convert unknowns
      if(inTS == TimeSystem::Unknown || outTS == TimeSystem::Unknown) {
         Exception e("Cannot compute correction for TimeSystem::Unknown");
         THROW(e);
      }

      // compute TT-TDB here; ref Astronomical Almanac B7
      double TDBmTT(0.0);
      if(inTS == TimeSystem::TDB || outTS == TimeSystem::TDB) {
         int iday = int(day);
         long jday = convertCalendarToJD(year, month, iday) ;
         double frac(day-iday);
         double TJ2000(jday-2451545.5+frac);     // t-J2000
         //       0.0001657 sec * sin(357.53 + 0.98560028 * TJ2000 deg)
         frac = ::fmod(0.017201969994578 * TJ2000, 6.2831853071796);
         TDBmTT = 0.001657 * ::sin(6.240075674 + frac);
         //        0.000022 sec * sin(246.11 + 0.90251792 * TJ2000 deg)
         frac = ::fmod(0.015751909262251 * TJ2000, 6.2831853071796);
         frac = 0.015751909262251 * TJ2000;
         TDBmTT += 0.000022  * ::sin(4.295429822 + frac);
      }

      // -----------------------------------------------------------
      // conversions: first convert inTS->TAI ...
      // TAI = GPS + 19s
      // TAI = GAL + 19s
      // TAI = UTC + getLeapSeconds()
      // TAI = GLO + getLeapSeconds()
      // TAI = UT1 - UT1mUTC + getLeapSeconds()
      // TAI = BDT + 33s
      // TAI = TT - 32.184s
      // TAI = TDB - TDBmTT - 32.184s
      if(inTS == TimeSystem::GPS ||       // GPS -> TAI
         inTS == TimeSystem::GAL)         // GAL -> TAI
         dt = 19.;
      else if(inTS == TimeSystem::UTC ||  // UTC -> TAI
              inTS == TimeSystem::GLO)    // GLO -> TAI
         dt = getLeapSeconds(year, month, day);
//    else if(inTS == UT1)
//       dt = -UT1mUTC + getLeapSeconds(year, month, day);
      else if(inTS == TimeSystem::BDT)    // BDT -> TAI         // RINEX 3.02 seems to say this
         dt = 33.;
      else if(inTS == TimeSystem::TAI)    // TAI -> TAI
         dt = 0.;
      else if(inTS == TimeSystem::TT)     // TT -> TAI
         dt = -32.184;
      else if(inTS == TimeSystem::TDB)    // TDB -> TAI
         dt = -32.184 - TDBmTT;
      else {                              // other
         Exception e("Invalid input TimeSystem " + inTS.asString());
         THROW(e);
      }

      // -----------------------------------------------------------
      // ... then convert TAI->outTS
      // GPS = TAI - 19s
      // GAL = TAI - 19s
      // UTC = TAI - getLeapSeconds()
      // GLO = TAI - getLeapSeconds()
      // UT1 = TAI - getLeapSeconds() + UT1mUTC
      // BDT = TAI - 33s
      // TT = TAI + 32.184s
      // TDB = TAI + 32.184s + TDBmTT
      if(outTS == TimeSystem::GPS ||      // TAI -> GPS
         outTS == TimeSystem::GAL)        // TAI -> GAL
         dt -= 19.;
      else if(outTS == TimeSystem::UTC || // TAI -> UTC
              outTS == TimeSystem::GLO)   // TAI -> GLO
         dt -= getLeapSeconds(year, month, day);
//      esle if(outTS == TimeSystem::UT1)
//         dt -= getLeapSeconds(year, month, day) - UT1mUTC;
      else if(outTS == TimeSystem::BDT)   // TAI -> BDT
         dt -= 33.;
      else if(outTS == TimeSystem::TAI)   // TAI
         dt -= 0.;
      else if(outTS == TimeSystem::TT)    // TAI -> TT
         dt += 32.184;
      else if(outTS == TimeSystem::TDB)   // TAI -> TDB
         dt += 32.184 + TDBmTT;
      else {                              // other
         Exception e("Invalid output TimeSystem " + outTS.asString());
         THROW(e);
      }

      return dt;
   }

}



