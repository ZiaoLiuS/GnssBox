#ifndef NEUUtil_HPP
#define NEUUtil_HPP

// coordSpace
#include "Triple.hpp"
#include "Xvt.hpp"
#include "Position.hpp"
#include "constants.hpp"
#include <Eigen/Eigen>


using namespace std;
using namespace mathSpace;
using namespace coordSpace;
using namespace gnssSpace;
using namespace Eigen;

namespace coordSpace
{

       /// A utility for converting from Cartesian in XYZ to North-East_Up (NEU)
    class NEUUtil
    {
       public:
    
             // default Constructors
          NEUUtil()
          {};
    
          /**
           * Given a location as a (geodetic) latitude and longitude
           * the constructor creates the appropriate rotation matrix
           * from XYZ to NEU and retains it for later use.
           * @param refLatRad geodetic latitude of point of interest (radians)
           * @param refLonRad longitude of point of interest (radians).
           */
    
          NEUUtil( const double refLatRad,
                   const double refLonRad);
    
          // common constructor
          NEUUtil( const Position& refPos)
          {
              setRefPosition(refPos);
          };
    
             // Methods
          /**
           * Convert from a vector in ECEF XYZ to ECEF NEU using the
           * current rotation matrix.
           * @param inV,inVec,in vector of interest in ECEF XYZ.
           * @return Same type as input but with the vector in ECEF NEU
           */
          VectorXd convertToNEU( const VectorXd& inV ) const;
          Triple   convertToNEU( const Triple& inVec ) const;
    
          /**
           * convert from a vector in neu to xyz using current rotation matrix
           * @param inV,inVec,in vector of interest in ECEF NEU.
           * @return Same type as input but with the vector in ECEF XYZ
           */
          VectorXd convertToXYZ( const VectorXd& inV ) const;
          Triple   convertToXYZ( const Triple& inVec ) const;
    
          /**
           * Update the rotation matrix to the new location without creating
           * a new object
           * @param refGdLatRad geodetic latitude of point of interest (radians)
           * @param refLonRad longitude of point of interest (radians).
           */
          void updatePosition( const double refLatRad,
                               const double refLonRad );
    
          void setRefPosition( const double refLatRad,
                               const double refLonRad )
          {
              compute(refLatRad, refLonRad);
          };
    
          void setRefPosition( const Position& refPos)
          {
              double refLatRad = refPos.getGeodeticLatitude() * DEG_TO_RAD;
              double refLonRad = refPos.getLongitude() * DEG_TO_RAD;
              compute(refLatRad, refLonRad);
          };
    
          // Utilities
       protected:
    
          void compute( const double refLat,
                        const double refLon);
    
          MatrixXd rotMat;
          MatrixXd rotMatNEU2XYZ;
    
    };

}

#endif
