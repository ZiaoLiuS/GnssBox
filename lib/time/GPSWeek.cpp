/// @file GPSWeek.cpp


#include "GPSWeek.hpp"
#include "TimeConstants.hpp"
#include "StringUtils.hpp"

using namespace utilSpace;
namespace timeSpace
{
   const int GPSWeek::MAX_WEEK=(CommonTime::END_LIMIT_JDAY-MJD_JDAY-GPS_EPOCH_MJD)/7;

   GPSWeek& GPSWeek::operator=(const GPSWeek& right)
   {
      week = right.week;
      timeSystem = right.timeSystem;
      return *this;
   }

} // namespace
