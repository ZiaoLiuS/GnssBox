/// @file TimeConstants.hpp  Define constants related to time, both in general and
///    for the different GNSS.

#ifndef TIMECONSTANTS_HPP
#define TIMECONSTANTS_HPP

namespace timeSpace
{
      /// Add this offset to convert Modified Julian Date to Julian Date.
   const double MJD_TO_JD = 2400000.5;
      /// 'Julian day' offset from MJD
   const long MJD_JDAY = 2400001L;
      /// Modified Julian Date of UNIX epoch (Jan. 1, 1970).
   const long UNIX_MJD = 40587L;

      /// Seconds per half week.
   const long HALFWEEK = 302400L;
      /// Seconds per whole week.
   const long FULLWEEK = 604800L;

      /// Seconds per day.
   const long SEC_PER_DAY = 86400L;
      /// Days per second.
   const double DAY_PER_SEC = 1.0/SEC_PER_DAY;

      /// Milliseconds in a second.
   const long MS_PER_SEC = 1000L;
      /// Seconds per millisecond.
   const double SEC_PER_MS = 1.0/MS_PER_SEC;

      /// Milliseconds in a day.
   const long MS_PER_DAY = MS_PER_SEC * SEC_PER_DAY;
      /// Days per milliseconds.
   const double DAY_PER_MS = 1.0/MS_PER_DAY;

   // system-specific constants

   // GPS -------------------------------------------
      /// 'Julian day' of GPS epoch (Jan. 6, 1980).
   const double GPS_EPOCH_JD = 2444244.5;
      /// Modified Julian Date of GPS epoch (Jan. 6, 1980).
   const long GPS_EPOCH_MJD = 44244L;
      /// Weeks per GPS Epoch
   const long GPS_WEEK_PER_EPOCH = 1024L;

      /// Zcounts in a  day.
   const long ZCOUNT_PER_DAY = 57600L;
      /// Days in a Zcount
   const double DAY_PER_ZCOUNT = 1.0/ZCOUNT_PER_DAY;
      /// Zcounts in a week.
   const long ZCOUNT_PER_WEEK = 403200L;
      /// Weeks in a Zcount.
   const double WEEK_PER_ZCOUNT = 1.0/ZCOUNT_PER_WEEK;

   // GAL -------------------------------------------
      /// 'Julian day' of GAL epoch (Aug 22 1999)
   const double GAL_EPOCH_JD = 2451412.5;
      /// Modified Julian Date of GAL epoch (Aug 22 1999)
   const long GAL_EPOCH_MJD = 51412L;
      /// Weeks per GAL Epoch
   const long GAL_WEEK_PER_EPOCH = 4096L;

   // QZS -------------------------------------------
      /// 'Julian day' of QZS epoch (Jan. 1, 1980).
   const double QZS_EPOCH_JD = 2444244.5;
      /// Modified Julian Date of QZS epoch (Jan. 1, 1980).
   const long QZS_EPOCH_MJD = 44244L;
      /// Weeks per QZS Epoch
   const long QZS_WEEK_PER_EPOCH = 65535L;         // effectively no rollover

   // BDS -------------------------------------------
      /// 'Julian day' of BDS epoch (Jan. 1, 2006).
   const double BDS_EPOCH_JD = 2453736.5;
      /// Modified Julian Date of BDS epoch (Jan. 1, 2006).
   const long BDS_EPOCH_MJD = 53736L;
      /// Weeks per BDS Epoch
   const long BDS_WEEK_PER_EPOCH = 8192L;

} // namespace

#endif // TIMECONSTANTS_HPP
