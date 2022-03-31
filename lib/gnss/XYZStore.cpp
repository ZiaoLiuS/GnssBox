

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>

#include "Exception.hpp"
#include "Xvt.hpp"
#include "XYZStore.hpp"
#include "CommonTime.hpp"
#include "YDSTime.hpp"
#include "StringUtils.hpp"

using namespace std;
using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace gnssSpace;

#define debug 0

namespace gnssSpace
{
    void XYZStore::loadFile(std::string filename)
    {
       ifstream input(filename.c_str());
       if (!input)
       {
          FileMissingException
          fe ( "Cannot Open xyz-formatted file: " + filename );
          THROW(fe);
       }
       
       bool ok(true);
       
       string line;
       
       while( input.peek() != EOF )
       {
          getline( input, line );
    
          if(debug)
              cout << "data line:" << line << endl;
    
          istringstream iss(line);
    
          string station;
          int year, doy;
          double second;
          double x, y, z, vx, vy, vz;
          iss >> station >> year >> doy >> second >> x >> y >> z >> vx >> vy >> vz;
    
          if(debug)
              cout << station << " "
                   << year << " "
                   << doy << " "
                   << second << " "
                   << x << " "
                   << y << " "
                   << z << " "
                   << vx << " "
                   << vy << " "
                   << vz << " "
                   << endl;
    
    
          YDSTime yds(year, doy, second);
          CommonTime epoch = yds.convertToCommonTime();
          
          Xvt xvt;
          xvt.x[0] = x;
          xvt.x[1] = y;
          xvt.x[2] = z;
    
          xvt.v[0] = vx;
          xvt.v[1] = vy;
          xvt.v[2] = vz;
    
          stationXvtData[station][epoch] = xvt;
    
       }
       
       input.close();
       
       if( !ok )
       {
          FileMissingException fe( filename + "is corrupted..." );
          THROW(fe);
       }
       
    } // END of "loadPositionFile"
    
    
    Triple XYZStore::getPos(std::string station, CommonTime& t)
    {
    
        t.setTimeSystem(TimeSystem::Unknown);
    
        Triple Pos;
        string stationUpper = upperCase(station);
        string stationLower= lowerCase(station);
        auto itUpper = stationXvtData.find(stationUpper);
        auto itLower = stationXvtData.find(stationLower);
        if(itUpper != stationXvtData.end())
        {
            epochXvtMap epochXvt;
            epochXvt = (*itUpper).second;
    
            auto itUpper = epochXvt.begin();
    
            CommonTime refEpoch = (*itUpper).first;
            Xvt xvt = (*itUpper).second;
    
            Triple pos = xvt.x;
    
            return pos;
        }
        else if(itLower != stationXvtData.end())
        {
            epochXvtMap epochXvt;
            epochXvt = (*itLower).second;
    
            auto itLower = epochXvt.begin();
    
            CommonTime refEpoch = (*itLower).first;
            Xvt xvt = (*itLower).second;
    
            Triple pos = xvt.x;
            return pos;
        }
        else
        {
            InvalidRequest e(station + " not found.");
            THROW(e);
        }
    
    } // END of "getPos"
    
    Triple XYZStore::getVel(std::string station, CommonTime& t)
    {
        // to be extended.
    }


}
