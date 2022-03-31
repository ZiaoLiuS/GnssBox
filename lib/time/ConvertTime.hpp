#pragma once
//////////////////////////////////////////////////////////
//
// function
//
//   convert time system
//
// author
//
//   shoujian zhang & yishi wang, wuhan university, 2022
//
//////////////////////////////////////////////////////////

#include<iostream>
#include<string>
#include<cmath>

#include "TimeSystem.hpp"
#include "ConvertCalendar.hpp"
#include "CommonTime.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"

using namespace std;
using namespace timeSpace;
using namespace utilSpace;

namespace gnssSpace
{
    CommonTime convertTimeSystem(const CommonTime& inCommonTime,
                             const TimeSystem& targetSys);

    CivilTime convertTimeSystem(const CivilTime& inCivilTime,
                            const TimeSystem& targetSys);

    YDSTime convertTimeSystem(const YDSTime& inYDSTime,
                          const TimeSystem& targetSys);

    /// Return the number of leap seconds between UTC and TAI, that is the
    /// difference in time scales UTC-TAI, at an epoch defined by year/month/day.
    /// NB. Input day in a floating quantity and thus any epoch may be represented;
    /// this is relevant the period 1960 to 1972, when UTC-TAI was not integral.
    /// NB. GPS = TAI-19sec and so GPS-UTC = getLeapSeconds()-19 == dtLS.
    /// NB. GLO = UTC = GPS - dtLS. but not incl. RINEX::TIME SYSTEM CORR::GPUT
    /// NB. GLO is actually UTC(SU) Moscow
    /// NB. GAL = GPS = UTC + dtLS this does not incl. RINEX::TIME SYSTEM CORR::GAUT
    /// NB. BDT = GPS - 15 but this does not include RINEX::TIME SYSTEM CORR::BDUT
    /// NB. BDT is actually UTC(NTSC) China
    /// @param  yr, mon, day give the day of interest
    double getLeapSeconds(const int yr, const int mon, const double day);

    /// Compute the conversion (in seconds) from one time system (inTS) to another
    /// (outTS), given the year and month of the time to be converted.
    /// Result is to be added to the first time (inTS) to yield the second (outTS),
    /// that is t(outTS) = t(inTS) + correction(inTS,outTS).
    /// NB. caller must not forget to change to outTS after adding this correction.
    /// @param TimeSystem inTS, input system
    /// @param TimeSystem outTS, output system
    /// @param int year, year of the time to be converted.
    /// @param int month, month (1-12) of the time to be converted.
    /// @return double dt, correction (sec) to be added to t(in) to yield t(out).
    /// @throw if input system(s) are invalid or Unknown.
    double Correction(const TimeSystem& inTS, const TimeSystem& outTS,
                             const int year, const int month, const double day);
}




