#pragma once
//============================================================================
// Copyright 
//
// XY CAO, Wuhan University, 2017 
// 
//============================================================================

namespace mathSpace
{
    class CProbability
    {
    public:
    
    	// gamma function, gamma(n/2)
    	double gammaF(int n);
    
    	/* caculate the probability of normal distribution function
    	 * P(-00,u) = p
    	 * @param		u: probalility quantile
    	 * @return		p: the probalility
    	*/
    	double normF( double u);
    
    	/* inverse function of normal distribution
    	 *  P(-00,u) = p
    	 * @param		p: probability
    	 * @return		u: the probalility quantile
    	*/
    	double re_normF( double p);
    
    	/* caculate the probability of X2 distribution function
    	 * chi2(0,x) = p
    	 * @param		n: freedom
    	 * @param		x: right endpoint
    	 * @param		f: dentisy function value
    	 * @return		p: the probalility
    	*/
    	double chi2F(int n, double x, double &f);
    
    	/* caculate the inverse function of X2 distribution
    	 * p=chi2(0,x)
    	 * @param		n: freedom
    	 * @param		p: the probalility
    	 * @return		x: chi2(n,p)
    	*/
    	double re_chi2F(int n, double p);
    
    	/* This functin is used to caculate the probability of F/t function
    	 * Bx(n1/2,n2/2) = p
    	 * @param		n1/n2: integer
    	 * @param		x: right endpoint
    	 * @param		Ux
    	 * @return		p: the probalility
    	*/
    	double BF(int n1, int n2, double x, double &Ux);
    
    	/* Caculate the probability of F function
    	 * F(0,x) = p
    	 * @param		n1/n2: freedom
    	 * @param		x: right endpoint
    	 * @param		f: dentisy function value
    	 * @return		p: the probalility
    	*/
    	double FF(int n1, int n2, double x, double &f);
    
    	/* Caculate the inverse function of F function
    	 * F(0,x) = p
    	 * @param		n1/n2: freedom
    	 * @param		p: the probalility
    	 * @return		u: the probalility quantile
    	*/
    	double re_FF(int n1, int n2, double p);
    
    	/* Caculate the probability of F function
    	 * t(-00,x) = p
    	 * @param		n: freedom
    	 * @param		x: right endpoint
    	 * @param		f: dentisy function value
    	 * @return		p: the probalility
    	*/
    	double tF(int n, double x, double &f);
    
    	/* Caculate the inverse function of t function
    	 * t(-00,x) = p
    	 * @param		n: freedom
    	 * @param		p: the probalility
    	 * @return		u: the probalility quantile
    	*/
    	double re_tF(int n, double p);
    
    };

}

