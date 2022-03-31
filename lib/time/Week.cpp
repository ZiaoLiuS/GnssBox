/// @file Week.cpp


#include "Week.hpp"
#include "TimeConstants.hpp"

using namespace utilSpace;
namespace timeSpace
{
   Week& Week::operator=(const Week& right)
   {
      week = right.week;
      timeSystem = right.timeSystem;
      return *this;
   }

} // namespace
