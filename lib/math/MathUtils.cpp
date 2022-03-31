/*
* MathUtils.h
*/
#include "MathUtils.h"

using namespace std;

namespace mathSpace
{

    const double PI = 3.1415926535898;
    
    // Changing Sign
    double sign(double x)
    {
    	return (x <= 0.0) ? -1.0 : 1.0;
    }
    
    // Rounding Values
    double round(double x)
    {
    	return double(std::floor(x + 0.5));
    }
    
    // Swapping values
    void swap(double& a, double& b)
    {
    	double t(a); a = b; b = t;
    }
}
