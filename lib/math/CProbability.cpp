#include "CProbability.hpp"
#include <math.h>
#include <cmath>

namespace mathSpace
{
    
    double CProbability::gammaF(int n)
    {
    	int i; double k;
    	
    	if( n % 2)  // odd
    	{
    		k = 1.772453850905516;
    		i = 1;
    	}
    	else // even
    	{
    		k = 1.0;
    		i = 2;
    	}
    
    	while( i <= (n-2) )
    	{
    		k *= i/2.0;
    		i += 2;
    	}
    
    	return k;
    
    } // End method 'CProbability::gammaF'
    
    
    double CProbability::normF(double u)
    {
    	// avoid overflow
    	if(u<-5.0) return 0.0;
    
    	if(u>5.0) return 1.0;
    
    	double y = fabs(u)/sqrt(2.0);
    
    	double p = 1.0 + y*(0.0705230784 + y*(0.0422820123 + 
    		y*(0.0092705272+y*(0.0001520143+y*(0.0002765672+y*0.0000430638)))));
    
    
    	double er = 1.0 -pow(p,-16.0);
    
    	p = (u<0.0)?(0.5-0.5*er):(0.5+0.5*er);
    
    	return p;
    
    } // End method 'CProbability::normF'
    
    
    double CProbability::re_normF(double p)
    {
    	if(p == 0.5) return 0.0;
    
    	if(p > 0.9999997) return 5.0;
    
    	if(p < 0.0000003) return -5.0;
    
    	if(p < 0.5) return -re_normF(1.0-p);
    
    	double y = -log(4.0*p*(1.0-p));
    
    	y = y*(1.570796288 + y*(0.3706987906e-1
    		+ y*(-0.8364353589e-3 + y*( -0.2250947176e-3
    		+ y*(0.6841218299e-5 + y*(0.5824238515e-5
    		+ y*(-0.1045274970e-5 + y*(0.8360937017e-7
    		+ y*(-0.3231081277e-8 + y*(0.3657763036e-10
    		+ y*0.6936233982e-12))))))))));
    
    	return sqrt(y);
    
    } // End method 'CProbability::re_normF'
    
    
    
    double CProbability::chi2F(int n, double x, double&f)
    {
    	double iai;
    	double p,Ux;
    	double pi = 3.14159265358979312;
    
    	double y = x/2.0;
    	if( n%2 ) // odd
    	{
    		Ux = sqrt(y/pi)*exp(-y);
    		p = 2.0*normF(sqrt(x))-1.0;
    		iai = 0.5;
    	}
    	else // even
    	{
    		Ux = y*exp(-y);
    		p = 1.0 - exp(-y);
    		iai = 1.0;
    	}
    
    	while(iai != 0.5*n)
    	{
    		p = p-Ux/iai;
    		Ux = Ux*y/iai;
    		iai += 1.0;
    	}
    
    	f = Ux/x;
    
    	return p;
    
    } // End method 'CProbability::chi2F'
    
    
    double CProbability::re_chi2F(int n, double p)
    {
    	if(p >0.9999999) p = 0.9999999;
    
    	if(n == 1)
    	{
    		double x = re_normF( (1.0-p)/2.0 );
    		return x*x;
    	}
    
    	if( n ==2)
    	{
    		return -2.0*log(1.0-p);
    	}
    
    	double u = re_normF(p);
    	double w = 2.0/(9.0*n);
    	double x0 = 1.0 - w + u*sqrt(w);
    	x0 = n*x0*x0*x0;
    
    	while(1)
    	{
    		double f;
    		double pp = chi2F(n,x0,f);
    		if(f + 1.0 == 1.0) return x0;
    		double xx = x0-(pp-p)/f;
    		if(fabs(x0-xx)<0.001) return xx;
    
    		x0 = xx;
    
    	}
    } // End method 'CProbability::re_chi2F'
    
    
    double CProbability::BF( int n1, int n2, double x, double &Ux)
    {
    	int m1,m2;
    	double Ix;
    
    	double pi = 3.14159265358979312;
    
    	if( n1%2 && n2%2 ) // both odd
    	{
    		Ux = std::sqrt( x*(1.0-x) )/pi;
    		Ix = 1.0 - 2.0*std::atan( std::sqrt( (1.0 - x)/x ) )/pi;
    		m1 = m2 = 1;
    	}
    
    	if( n1%2 && ! (n2%2) ) // odd even
    	{
    		Ux = std::sqrt( x )*(1.0-x) /2.0;
    		Ix = std::sqrt( x );
    		m1 = 1;
    		m2 = 2;
    	}
    
    	if( !( n1%2) &&  (n2%2) ) // even odd
    	{
    		Ux = x*std::sqrt( 1.0 - x ) /2.0;
    		Ix = 1.0 - std::sqrt( 1.0 - x );
    		m1 = 2;
    		m2 = 1;
    	}
    
    	if( !( n1%2) &&  !(n2%2) ) // even even
    	{
    		Ux = x*( 1.0 - x ) ;
    		Ix = x;
    		m1 = 2;
    		m2 = 2;
    	}
    
    	while( m2 != n2)
    	{
    		Ix = Ix + 2.0*Ux/m2;
    		Ux = Ux*( 1.0 + m1/( m2 + 0.0) )*(1.0 -x);
    		m2 = m2 + 2;
    	}
    
    	while( m1 != n1)
    	{
    		Ix = Ix - 2.0*Ux/m1;
    		Ux = Ux*( 1.0 + m2/( m1 + 0.0) )*x;
    		m1 = m1 + 2;
    	}
    
    	return Ix;
    
    } // End method 'CProbability::BF'
    
    
    double CProbability::FF( int n1, int n2, double x, double &f)
    {
    	double y = n1*x/(n2 + n1*x);
    	double Ux;
    	double p = CProbability::BF(n1,n2,y, Ux);
    
    	f = Ux/x;
    	 return p;
    
    } // End method 'CProbability::FF'
    
    
    double CProbability::re_FF( int n1, int n2, double p)
    {
    	double d,f0;
    
    	double a = 2.0/(9.0*n1);
    	double b = 2.0/(9.0*n2);
    	double a1 = 1.0 - a;
    	double b1 = 1.0 - b;
    	double u = re_normF(p);
    	double e= b1*b1 - b*u*u;
    
    
    	if( e<=0.8)
    	{
    		a1 = 2.0*pow( n2 + 0.0, 0.5*n2 - 1.0);
    		double a2 = pow(n1 + 0.0, 0.5*n2);
    		double B = gammaF(n1)*gammaF(n2)/gammaF( n1+ n2);
    		double f = a1/a2/B/(1.0 - p);
    		f0 = pow(f,2.0/n2);
    	}
    	else
    	{
    		f0 = ( a1*b1 + u*std::sqrt( a1 *a1*b + a*e) )/2;
    		f0 = f0*f0*f0;
    	}
    
    	if( f0 < 0.0) f0 = 0.01;
    
    	while(1)
    	{
    		double pp = FF( n1, n2, f0, d);
    		double df  = ( p - pp )/d;
    
    		while( std::fabs(f0) < std::fabs( df) )  df /= 2.0;
    		f0 = f0 + df;
    
    		if( std::fabs(df)/f0 < 0.0001 || std::fabs(df) < 0.0001)
    		{
    			return f0;
    		}
    
    	} // end while
    
    } // End method 'CProbability::FF'
    
    
    double CProbability::tF( int nn, double t, double &f)
    {
    	if( t + 1.0 == 1.0) t = 1.0e-9;
    
    	double x = t*t/(nn + t*t);
    
    	double P = 0.5*BF( 1, nn, x, f);
    
    	if( x< 0.0) 
    		P = 0.5 - P;
    	else 
    		P = 0.5 + P;
    
    	f = f/std::fabs(t);
    
    	return P;
    
    } // End method 'CProbability::tF'
    
    
    double CProbability::re_tF( int n, double p)
    {
    	if( p < 0.5)
    	{
    		return -re_tF(n,1.0-p);
    	}
    
    	double x;
    	double pi = 3.14159265358979312;
    
    	if( n ==1 )
    	{
    		x= tan( pi*( p - 0.5) );
    	}
    	else if( n ==2)
    	{
    		double u = 2.0 *p - 1.0;
    		u = u*u;
    		x = sqrt(2.0*u/( 1.0 - u));
    	}
    	else
    	{
    		x = re_normF(p)*sqrt( n/( n-2.0) );
    
    		while(1)
    		{
    			double f;
    			double F = tF( n, x, f);
    			double dx = ( F-p)/f;
    			x = x - dx;
    			if( fabs(dx)<0.001) break;
    		}
    	}
    
    	return x;
    
    } // End method 'CProbability::re_tF'

}
