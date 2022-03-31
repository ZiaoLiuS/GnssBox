
//=========================                    
// class to convert xyz2neu and neu2xyz
//=========================                    

// coordSpace
#include "NEUUtil.hpp"
#include <cmath>

using namespace utilSpace;
using namespace mathSpace;
namespace coordSpace
{

    using std::sin;
    using std::cos;
    
    NEUUtil::NEUUtil(const double refLatRad,
                     const double refLonRad)
    {
       compute( refLatRad, refLonRad );
    }
    
    //
    void NEUUtil::compute( const double refLat,
                           const double refLon )
    {
       rotMat.resize(3,3);
       rotMat (0,0)       = -sin(refLat)*cos(refLon);
       rotMat (0,1)       = -sin(refLat)*sin(refLon);
       rotMat (0,2)       =  cos(refLat);
    
       rotMat (1,0)       = -sin(refLon);
       rotMat (1,1)       =  cos(refLon);
       rotMat (1,2)       =  0.0;
    
       rotMat (2,0)       =  cos(refLat)*cos(refLon);
       rotMat (2,1)       =  cos(refLat)*sin(refLon);
       rotMat (2,2)       =  sin(refLat);
    
       rotMatNEU2XYZ.resize(3,3);
    
       rotMatNEU2XYZ(0,0) = -sin(refLat)*cos(refLon);
       rotMatNEU2XYZ(0,1) = -sin(refLon);
       rotMatNEU2XYZ(0,2) =  cos(refLat)*cos(refLon);
    
       rotMatNEU2XYZ(1,0) = -sin(refLat)*sin(refLon);
       rotMatNEU2XYZ(1,1) =  cos(refLon);
       rotMatNEU2XYZ(1,2) =  cos(refLat)*sin(refLon);
    
       rotMatNEU2XYZ(2,0) =  cos(refLat);
       rotMatNEU2XYZ(2,1) =  0.0; 
       rotMatNEU2XYZ(2,2) =  sin(refLat);
    
    }
    
    void NEUUtil::updatePosition( const double refLatRad,
                                  const double refLonRad )
    {
       compute( refLatRad, refLonRad );
    }
    
    
    VectorXd NEUUtil::convertToNEU( const VectorXd& inV ) const
    {
       VectorXd outV(3);
    
       if (inV.size()!=3)
       {
          Exception e("Incompatible dimensions for Vector");
          THROW(e);
       }
       outV = rotMat * inV;
       return(outV);
    }
    
    Triple NEUUtil::convertToNEU( const Triple& inVec ) const
    {
       Vector3d v;
       v[0] = inVec[0];
       v[1] = inVec[1];
       v[2] = inVec[2];
    
       VectorXd vOut = convertToNEU( v );
       Triple outVec( vOut[0], vOut[1], vOut[2] );
       return(outVec);
    }
    
    VectorXd NEUUtil::convertToXYZ( const VectorXd& inV ) const
    {
       VectorXd outV(3);
    
       if (inV.size()!=3)
       {
          Exception e("Incompatible dimensions for Vector");
          THROW(e);
       }
       outV = rotMatNEU2XYZ * inV;
       return(outV);
    }
    
    Triple NEUUtil::convertToXYZ( const Triple& inVec ) const
    {
       VectorXd v(3);
       v[0] = inVec[0];
       v[1] = inVec[1];
       v[2] = inVec[2];
    
       VectorXd vOut = convertToXYZ( v );
       Triple outVec( vOut[0], vOut[1], vOut[2] );
       return(outVec);
    }

}     // end namespace coordSpace
