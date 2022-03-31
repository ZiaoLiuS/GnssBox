/// @file MathBase.hpp
/// Basic math #defines (ABS, SQRT, etc)

#ifndef MATH_BASE_HPP
#define MATH_BASE_HPP

//@fix MSVC doesnt like std::sqrt or std::abs, and disabling
//extensions allows abs(double) to be used instead of fabs()
#ifdef _MSC_VER
#undef _MSC_EXTENSIONS
#endif
#include <cmath>
#ifdef _MSC_VER
#define _MSC_EXTENSIONS
#endif

namespace mathSpace
{
// do Doxygen elsewhere
#ifdef _MSC_VER
#define ABS(x)  ::abs(x)
#define SQRT(x) ::sqrt(x)
#define MAX(x,y) std::max(x,y)
#else
#define ABS(x)  std::abs(x)
#define SQRT(x) std::sqrt(x)
#define MAX(x,y) std::max(x,y)
#endif

}  // namespace mathSpace

#endif
