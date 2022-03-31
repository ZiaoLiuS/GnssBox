
/*
   create by shjzhang
   used to store the reference xyz of the IGS stations

*/

#ifndef XYZStore_HPP
#define XYZStore_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include "CommonTime.hpp"
#include "Triple.hpp"
#include "Xvt.hpp"
#include "MJD.hpp"

using namespace std;
using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;

namespace gnssSpace
{
   typedef std::map<CommonTime, Xvt> epochXvtMap;
   typedef std::map<std::string, epochXvtMap> stationXvtMap;

   class XYZStore
   {
   public:
      
      XYZStore()
      {}
      
      virtual ~XYZStore()
      {}
      
      virtual void loadFile(std::string filename);

      virtual Triple getPos(std::string station, CommonTime& t);
      virtual Triple getVel(std::string station, CommonTime& t);
      
   private:
   
      CommonTime epoch;
      
      double x, y, z, vx, vy, vz;
      
      Xvt xvt;
      
      stationXvtMap stationXvtData;
      
   }; // END of "class XYZStore"
   

} // END of "namespace gnssSpace"










#endif
