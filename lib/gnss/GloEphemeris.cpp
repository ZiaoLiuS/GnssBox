
/// @file GloEphemeris.cpp
/// Ephemeris data for GLONASS.

#include <iomanip>
#include "GloEphemeris.hpp"

namespace gnssSpace
{
    void GloEphemeris::printData() const
    {
        cout << "****************************************************************"
             << "************" << endl
             << "GLONASS Broadcast Ephemeris Data: " << endl;
        cout << "Satellite: " << satID.toString() << setfill(' ') << endl;
        cout << "Toc: " << this->CivilToc.year << " " << this->CivilToc.month << " "
             << this->CivilToc.day << " " << this->CivilToc.hour << " "
             << this->CivilToc.minute << " " << this->CivilToc.second << endl;
        cout << scientific << setprecision(8)
             << "TauN: " << setw(16) << TauN << endl
             << "GammaN: " << setw(16) << GammaN << endl
             << "MFtime: " << setw(16) << MFtime << endl;

        cout << "px: " << setw(16) << px << endl
             << "vx:  " << setw(16) << vx << endl
             << "ax: " << setw(16) << ax << endl
             << "health: " << setw(16) << health << endl;

        cout << "py: " << setw(16) << py << endl
             << "vy: " << setw(16) << vy << endl
             << "ay: " << setw(16) << ay << endl
             << "freqNum: " << setw(16) << freqNum << endl;

        cout << "pz: " << setw(16) << pz << endl;
        cout << "vz: " << setw(16) << vz << endl;
        cout << "az: " << setw(16) << az << endl;
        cout << "ageOfInfo: " << setw(16) << ageOfInfo << endl;

        cout << "ctToe: " << ctToe.asString() << endl;
    }


    /// Compute satellite position at the given time.
    Xvt GloEphemeris::svXvt(const CommonTime& epoch) const {
        if (epoch < (ctToe - 3600.0) ||
            epoch >= (ctToe + 3600.0)) {
            cerr << "Requested time is out of ephemeris data" << endl;
        }

        Xvt sv;

        /// If the exact epoch is found, let's return the values
        if (epoch == ctToe)       // exact match for epoch
        {
            sv.x[0] = px * 1.e3;   // m
            sv.x[1] = py * 1.e3;   // m
            sv.x[2] = pz * 1.e3;   // m
            sv.v[0] = vx * 1.e3;  // m/sec
            sv.v[1] = vy * 1.e3;  // m/sec
            sv.v[2] = vz * 1.e3;  // m/sec

            /// In the GLONASS system, 'clkbias' already includes the
            /// relativistic correction, therefore we must substract the late
            /// from the former.
            sv.relcorr = sv.computeRelativityCorrection();
            sv.clkbias = TauN + GammaN * (epoch - ctToe) - sv.relcorr;
            sv.clkdrift = GammaN;
            sv.frame = ReferenceFrame::PZ90;

            return sv;
        }

        double px_tmp( px );   // X coordinate (km)
        double vx_tmp( vx );   // X velocity   (km/s)
        double ax_tmp( ax );   // X acceleration (km/s^2)
        double py_tmp( py );   // Y coordinate
        double vy_tmp( vy );   // Y velocity
        double ay_tmp( ay );   // Y acceleration
        double pz_tmp( pz );   // Z coordinate
        double vz_tmp( vz );   // Z velocity
        double az_tmp( az );   // Z acceleration

        /// We will need some PZ-90 ellipsoid parameters
        PZ90Ellipsoid pz90;
        double we( pz90.angVelocity() );

        /// Get sidereal time at Greenwich at 0 hours UT
        double gst( getSidTime( ctToe ) );
        double s0( gst*PI/12.0 );
        YDSTime ytime( ctToe );
        double numSeconds( ytime.sod );
        double s( s0 + we*numSeconds );
        double cs( std::cos(s) );
        double ss( std::sin(s) );

        /// Initial state matrix
        VectorXd initialState(6), accel(3), dxt1(6), dxt2(6), dxt3(6),
                dxt4(6), tempRes(6);

        // Get the reference state out of GloEphemeris object data. Values
        // must be rotated from PZ-90 to an absolute coordinate system
        // Initial x coordinate (m)
        initialState(0)  = (px_tmp*cs - py_tmp*ss);
        // Initial y coordinate
        initialState(2)  = (px_tmp*ss + py_tmp*cs);
        // Initial z coordinate
        initialState(4)  = pz_tmp;

        // Initial x velocity   (m/s)
        initialState(1)  = (vx_tmp*cs - vy_tmp*ss - we*initialState(2) );
        // Initial y velocity
        initialState(3)  = (vx_tmp*ss + vy_tmp*cs + we*initialState(0) );
        // Initial z velocity
        initialState(5)  = vz_tmp;


        // Integrate satellite state to desired epoch using the given step
        double rkStep( step );

        if ( (epoch - ctToe) < 0.0 ) rkStep = step*(-1.0);
        CommonTime workEpoch( ctToe );

        double tolerance( 1e-9 );
        bool done( false );
        while (!done)
        {

            // If we are about to overstep, change the stepsize appropriately
            // to hit our target final time.
            if( rkStep > 0.0 )
            {
                if( (workEpoch + rkStep) > epoch )
                    rkStep = (epoch - workEpoch);
            }
            else
            {
                if ( (workEpoch + rkStep) < epoch )
                    rkStep = (epoch - workEpoch);
            }

            numSeconds += rkStep;
            s = s0 + we*( numSeconds );
            cs = std::cos(s);
            ss = std::sin(s);

            // Accelerations are computed once per iteration
            accel(0) = ax_tmp*cs - ay_tmp*ss;
            accel(1) = ax_tmp*ss + ay_tmp*cs;
            accel(2) = az_tmp;

            dxt1 = derivative( initialState, accel );
            for( int j = 0; j < 6; ++j )
                tempRes(j) = initialState(j) + rkStep*dxt1(j)/2.0;

            dxt2 = derivative( tempRes, accel );
            for( int j = 0; j < 6; ++j )
                tempRes(j) = initialState(j) + rkStep*dxt2(j)/2.0;

            dxt3 = derivative( tempRes, accel );
            for( int j = 0; j < 6; ++j )
                tempRes(j) = initialState(j) + rkStep*dxt3(j);

            dxt4 = derivative( tempRes, accel );
            for( int j = 0; j < 6; ++j )
                initialState(j) = initialState(j) + rkStep * ( dxt1(j)
                                                               + 2.0 * ( dxt2(j) + dxt3(j) ) + dxt4(j) ) / 6.0;


            // If we are within tolerance of the target time, we are done.
            workEpoch += rkStep;
            if ( std::fabs(epoch - workEpoch ) < tolerance )
                done = true;

        }  // End of 'while (!done)...'


        px_tmp = initialState(0);
        py_tmp = initialState(2);
        pz_tmp = initialState(4);
        vx_tmp = initialState(1);
        vy_tmp = initialState(3);
        vz_tmp = initialState(5);

        sv.x[0] = 1000.0*( px_tmp*cs + py_tmp*ss );         // X coordinate
        sv.x[1] = 1000.0*(-px_tmp*ss + py_tmp*cs);          // Y coordinate
        sv.x[2] = 1000.0*pz_tmp;                        // Z coordinate
        sv.v[0] = 1000.0*( vx_tmp*cs + vy_tmp*ss + we*(sv.x[1]/1000.0) ); // X velocity
        sv.v[1] = 1000.0*(-vx_tmp*ss + vy_tmp*cs - we*(sv.x[0]/1000.0) ); // Y velocity
        sv.v[2] = 1000.0*vz_tmp;                        // Z velocity

        // In the GLONASS system, 'clkbias' already includes the relativistic
        // correction, therefore we must substract the late from the former.
        sv.relcorr = sv.computeRelativityCorrection();
        sv.clkbias = TauN + GammaN * (epoch - ctToe) - sv.relcorr;
        sv.clkdrift = GammaN;
        sv.frame = ReferenceFrame::PZ90;

        // We are done, let's return
        return sv;
    }

    double GloEphemeris::getSidTime( const CommonTime& time ) const
    {

        // The following algorithm is based on the paper:
        // Aoki, S., Guinot,B., Kaplan, G. H., Kinoshita, H., McCarthy, D. D.
        //    and P.K. Seidelmann. 'The New Definition of Universal Time'.
        //    Astronomy and Astrophysics, 105, 359-361, 1982.

        // Get the Julian Day at 0 hours UT (jd)
        YDSTime ytime( time );
        double year( static_cast<double>(ytime.year) );
        int doy( ytime.doy );
        int temp( static_cast<int>(floor(365.25 * (year - 1.0))) + doy );

        double jd( static_cast<double>(temp)+ 1721409.5 );

        // Compute the Julian centuries (36525 days)
        double jc( (jd - 2451545.0)/36525.0 );

        // Compute the sidereal time, in seconds
        double sid( 24110.54841 + jc * ( 8640184.812866
                                         + jc * ( 0.093104 - jc * 0.0000062 ) ) );

        sid = sid / 3600.0;
        sid = fmod(sid, 24.0);
        if( sid < 0.0 ) sid = sid + 24.0;

        return sid;

    }; // End of method 'GloEphemeris::getSidTime()'

    // Function implementing the derivative of GLONASS orbital model.
    VectorXd GloEphemeris::derivative( const VectorXd& inState,
                                             const VectorXd& accel )
    const
    {

        // We will need some important PZ90 ellipsoid values
        PZ90Ellipsoid pz90;
        const double j20( pz90.j20() );
        const double mu( pz90.gm_km() );
        const double ae( pz90.a_km() );

        // Let's start getting the current satellite position and velocity
        double  x( inState(0) );          // X coordinate
        //double vx( inState(1) );          // X velocity
        double  y( inState(2) );          // Y coordinate
        //double vy( inState(3) );          // Y velocity
        double  z( inState(4) );          // Z coordinate
        //double vz( inState(5) );          // Z velocity

        double r2( x*x + y*y + z*z );
        double r( std::sqrt(r2) );
        double xmu( mu/r2 );
        double rho( ae/r );
        double xr( x/r );
        double yr( y/r );
        double zr( z/r );
        double zr2( zr*zr );
        double k1(j20*xmu*1.5*rho*rho);
        double  cm( k1*(1.0-5.0*zr2) );
        double cmz( k1*(3.0-5.0*zr2) );
        double k2(cm-xmu);

        double gloAx( k2*xr + accel(0) );
        double gloAy( k2*yr + accel(1) );
        double gloAz( (cmz-xmu)*zr + accel(2) );

        VectorXd dxt = VectorXd::Zero(6);

        // Let's insert data related to X coordinates
        dxt(0) = inState(1);       // Set X'  = Vx
        dxt(1) = gloAx;            // Set Vx' = gloAx

        // Let's insert data related to Y coordinates
        dxt(2) = inState(3);       // Set Y'  = Vy
        dxt(3) = gloAy;            // Set Vy' = gloAy

        // Let's insert data related to Z coordinates
        dxt(4) = inState(5);       // Set Z'  = Vz
        dxt(5) = gloAz;            // Set Vz' = gloAz

        return dxt;

    }  // End of method 'GloEphemeris::derivative()'



}  // End of namespace gnssSpace
