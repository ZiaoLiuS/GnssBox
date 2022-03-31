#pragma once

/*
* ARLambda.h
* Ambiguity Resolution using MLAMBDA
*/

#ifndef ARLAMBDA_H_
#define ARLAMBDA_H_

#include "MathUtils.h"
#include <Eigen/Eigen>

namespace gnssSpace
{
    using namespace Eigen;
    using namespace mathSpace;

    class ARLambda
    {
    public:

    	ARLambda()
            :squaredRatio(0)
        {};

    	~ARLambda()
        {};
    
    	Eigen::VectorXd resolve(Eigen::VectorXd& ambFloat, 
                                Eigen::MatrixXd& ambCov);
    
    	bool isFixed(double threshhold = 10.0)
        {
    		return (squaredRatio>threshhold) ? true : false;
    	}
    
    	double squaredRatio;
    
    protected:
    
    	// lambda/mlambda integer least-square estimation
    	// a     Float parameters (n x 1)
    	// Q     Covariance matrix of float parameters (n x n)
    	// F     Fixed solutions (n x m)
    	// s     Sum of squared residulas of fixed solutions (1 x m)
    	// m     Number of fixed solutions
    	//      status (0:ok,other:error)
    	int lambda(Eigen::VectorXd& a, 
                   Eigen::MatrixXd& Q, 
                   Eigen::MatrixXd& F, 
                   Eigen::VectorXd& s, 
                   const int& m = 2);
    
    	// Q = L'*diag(D)*L
    	int factorize(Eigen::MatrixXd& Q, 
                      Eigen::MatrixXd& L, 
                      Eigen::VectorXd& D);

    	void gauss( Eigen::MatrixXd& L, 
                    Eigen::MatrixXd& Z, 
                    int i, 
                    int j);

    	void permute(Eigen::MatrixXd& L, 
                     Eigen::VectorXd& D, 
                     int j, 
                     double del, 
                     Eigen::MatrixXd& Z);

    	void reduction(Eigen::MatrixXd& L, 
                       Eigen::VectorXd& D, 
                       Eigen::MatrixXd& Z);
    
    	// Modified lambda (mlambda) search
    	virtual int search(Eigen::MatrixXd& L, 
                           Eigen::VectorXd& D, 
                           Eigen::VectorXd& zs, 
                           Eigen::MatrixXd& zn, 
                           Eigen::VectorXd& s, 
                           const int& m = 2);
    
    };
    
}
#endif /* ARLAMBDA_H_ */
