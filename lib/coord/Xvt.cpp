#pragma ident "$Id$"

/**
 * @file Xvt.cpp
 * Position and velocity as Triples, clock bias and drift as doubles.
 */

#include <iostream>
#include "Xvt.hpp"
#include "constants.hpp"

using namespace mathSpace;
using namespace gnssSpace;

namespace coordSpace
{

    // compute the relativity correction
    double Xvt::computeRelativityCorrection(void)
    {
       relcorr = -2.0*( (x[0]/C_MPS)*(v[0]/C_MPS)
                       +(x[1]/C_MPS)*(v[1]/C_MPS)
                       +(x[2]/C_MPS)*(v[2]/C_MPS) );
       return relcorr;
    }
    
    // Function to find the range and position from a ground
    // location, rxPos, to the spacecraft position (*this).x
    // Use the pseudorange corrected for SV clock effects to get a
    // rough time of flight (dt).  Account directly for Earth
    // rotation, then compute a rough receiver bias by differencing
    // the initial time of flight with the new estimate.  Then
    // correct the rotation by a small amount.
    double Xvt::preciseRho(const Triple& rxPos,
                                  const EllipsoidModel& ellips,
                                  double correction) const 
       throw()
    {
       // Compute initial time of flight estimate using the
       // geometric range at transmit time.  This fails to account
       // for the rotation of the earth, but should be good to
       // within about 40 m
       double sr1 = rxPos.slantRange(x);
       double dt = sr1 / ellips.c();
    
       // compute rotation angle in the time of signal transit
       double rotation_angle = -ellips.angVelocity() * dt;
    
       // rotate original GS coordinates to new values to correct for
       // rotation of ECEF frame
       // Ref: Satellite Geodesy, Gunter Seeber, 1993, pg 291  and the 
       // ICD-GPS-200 sheet 102 May 1993 version
       //   xnew[0]=xg[0]*cos(rotation_angle)-xg[1]*sin(rotation_angle);
       //   xnew[1]=xg[1]*cos(rotation_angle)+xg[0]*sin(rotation_angle);
       //   xnew[2]=xg[2];
       // since cosine and sine are small, approximate by the first
       // order terms in an expansion.
       Triple xnew;
       for (int i = 0; i < 2; i++)
       {
          xnew[0] = x[0] - x[1] * rotation_angle;
          xnew[1] = x[1] + x[0] * rotation_angle;
          xnew[2] = x[2];
    
          // Compute geometric slant range from ground station to
          // the rotated new coord's
          sr1 = rxPos.slantRange(xnew);
       
          // Recompute the time of flight (dt) based on PR, with the
          // time of flight based on geometric range.  Note that
          // this is a really unneeded, in that the change in PR is
          // < 40 m, hence the change in tof is < 20 ns
          dt = sr1 / ellips.c();
       
          // Compute new rotation in this time 
          rotation_angle = -ellips.angVelocity() * dt;  
       }
    
       // Account for SV clock drift, relativity and user input correction
       double rho = sr1 - (clkbias + relcorr) * ellips.c() - correction;
    
       return rho;
    
    } // end of preciseRho()

}

