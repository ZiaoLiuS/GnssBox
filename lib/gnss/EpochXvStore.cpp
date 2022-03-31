
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include "EpochXvStore.hpp"
#include "MJD.hpp"
#include "MiscMath.hpp"

#include "YDSTime.hpp"
#include "StringUtils.hpp"

#define debug 0

using namespace std;
using namespace timeSpace;
using namespace mathSpace;
using namespace gnssSpace;
using namespace utilSpace;

namespace gnssSpace
{

    void EpochXvStore::loadFile(std::string filename)
    {
        if(debug)
        cout << filename << endl;
    
       std::fstream input(filename.c_str(), ios::in);
       if (!input)
       {
          FileMissingException fe ( "Cannot Open kin x file: " + filename );
          THROW(fe);
       }
       
       bool ok(true);
    
       string fileType;
    
       // jump the header
       while( !input.eof() && input.good() )
       {
          if( input.eof() ) break;
          if( input.bad() ) { ok = false; break; }
          
          string line;
          getline( input, line );
    
          // read satellite mission name
          string label = strip(line.substr( 0,30));
          if(label=="SATELLITE NAME")
          {
              markerName = strip(line.substr(31, 7));
              markerName = replaceAll(markerName, " ", "_");
          }
    
          if(label=="FILE TYPE")
          {
              fileType = upperCase(strip(line.substr(31,7)));
          }
    
          if(label == "END OF HEADER" )
          {
              break;
          }
       }
    
       if(debug)
       {
           cout << markerName << endl;
           cout << fileType << endl;
       }
    
       if(fileType != "XYZ")
       {
           cerr << "input file is not LEO xyz-format! " << endl;
           exit(-1);
       }
       
       while( !input.eof() && input.good() )
       {
          if( input.eof() ) break;
          if( input.bad() ) { ok = false; break; }
          
          string line;
          getline( input, line );
          istringstream iss(line);
    
          if(debug)
          cout << line << endl;
    
          int year, doy;
          double second;
          double x, y, z, vx, vy, vz;
          iss >> year >> doy >> second >> x >> y >> z >> vx >> vy >> vz;
          
          YDSTime yds(year, doy, second);
          CommonTime epoch = yds.convertToCommonTime();
    
          if(debug)
          {
              cout << "epoch:" << epoch << endl; 
          }
    
          Xv posVel;
          posVel.x[0] = x;
          posVel.x[1] = y;
          posVel.x[2] = z;
          posVel.v[0] = vx;
          posVel.v[1] = vy;
          posVel.v[2] = vz;
          epochXvData[epoch] = posVel;
    
       }
       
       input.close();
       
       if( !ok )
       {
          FileMissingException fe( filename + "is corrupted..." );
          THROW(fe);
       }
       
    } // END of "loadFile"
    
    
    void EpochXvStore::loadFile(std::string filename,
                          const CommonTime& bTime,
                          const CommonTime& eTime)
    {
       ifstream input(filename.c_str());
       if (!input)
       {
          FileMissingException
          fe ( "Cannot Open kin Attitude file: " + filename );
          THROW(fe);
       }
       
       bool ok(true);
       
       string line;
       
       while( !input.eof() && input.good() )
       {
          if( input.eof() ) break;
          if( input.bad() ) { ok = false; break; }
          
          getline( input, line );
          istringstream iss(line);
    
          int year, doy;
          double second;
          double x, y, z, vx, vy, vz;
          iss >> year >> doy >> second >> x >> y >> z >> vx >> vy >> vz;
          
          YDSTime yds(year, doy, second);
          CommonTime epoch = yds.convertToCommonTime();
          
          Xv posVel;
          if (epoch >= bTime && epoch <= eTime)
          {
             posVel.x[0] = x;
             posVel.x[1] = y;
             posVel.x[2] = z;
             posVel.v[0] = vx;
             posVel.v[1] = vy;
             posVel.v[2] = vz;
             epochXvData[epoch] = posVel;
          }
       }
       
       input.close();
       
       if( !ok )
       {
          FileMissingException fe( filename + "is corrupted..." );
          THROW(fe);
       }
       
    } // END of "loadFile"
    
    
    Triple EpochXvStore::getPos(string& satName, CommonTime& epoch)
    {
    
        if(strip(satName) != markerName)
        {
            cerr << "input marker name does not match the name in the xyz-file!" << endl;
            exit(-1);
        }
    
        if(debug)
        {
            cout << epoch << endl;
        }
    
    
          // The corresponding time in N half.
       Triple Pos;
       std::map< CommonTime, Xv >::iterator it0 = epochXvData.find(epoch);
    
       if (it0 != epochXvData.end())
       {
          Pos[0] = it0->second.x[0];
          Pos[1] = it0->second.x[1];
          Pos[2] = it0->second.x[2];
    
          return Pos;
       }
       else
       {
    //    CommonTime ttag0;
    
          int N(9);
          int Nhalf(std::floor( N/2));
          std::vector<double> times, X, Y, Z;
    
          std::map< CommonTime, Xv >::iterator it1;
          std::map< CommonTime, Xv >::iterator it  = epochXvData.begin();
          std::map< CommonTime, Xv >::iterator it2 = epochXvData.begin();
     
          while(1)
          {
             if( (it->first - epoch) * (it2->first - epoch) <= 0 ) break;
             if( it == epochXvData.end() )
             {
                std::cerr
                << "The Position file doesn't contain the data in current epoch "
                << std::endl;
                break;
                exit(-1);
             }
             it2 = it;
             ++it;
          }
     
          it1 = --it;
     
     
    //    std::cout << "2" << std::endl;
          /************************************************************************/
          for (int i=0; i<Nhalf; i++)
          {
             --it1;
          }
          
          CommonTime firstEpoch = it1->first;
          for( int i=0; i<N; i++)
          {
             times.push_back(it1->first - firstEpoch);
             X.push_back(it1->second.x[0]);
             Y.push_back(it1->second.x[1]);
             Z.push_back(it1->second.x[2]);
             ++it1;
          }
    //    std::cout << "t: " << t << std::endl;
          double dt(epoch-firstEpoch), err;
          Pos[0] = LagrangeInterpolation(times, X, dt, err);
          Pos[1] = LagrangeInterpolation(times, Y, dt, err);
          Pos[2] = LagrangeInterpolation(times, Z, dt, err);
          
    //    std::cout << "3" << std::endl;
    //    std::cout << "Pos: " << Pos << std::endl;
          return Pos;
       }
       
    } // END of "getPos"
    
    
    Triple EpochXvStore::getVel(CommonTime& epoch)
    {
          // The corresponding time in N half.
       Triple Vel;
       std::map< CommonTime, Xv >::iterator it0 = epochXvData.find(epoch);
    
    // std::cout << "Time: " << std::fixed << epoch << std::endl;
    
       if (it0 != epochXvData.end())
       {
          Vel[0] = it0->second.v[0];
          Vel[1] = it0->second.v[1];
          Vel[2] = it0->second.v[2];
    
          return Vel;
       }
       else
       {
          // CommonTime ttag0;
          int N(9);
          int Nhalf(std::floor( N/2));
          std::vector<double> times, VX, VY, VZ;
          std::map< CommonTime, Xv >::iterator it  = epochXvData.begin();
          std::map< CommonTime, Xv >::iterator it2 = epochXvData.begin();
     
          std::map< CommonTime, Xv >::iterator it1;
          while(1)
          {
             if( (it->first - epoch)* (it2->first - epoch) <= 0 ) break;
    
             if( it == epochXvData.end() )
             {
                std::cerr
                << "The Position file doesn't contain the data in current epoch "
                << std::endl;
                break;
             }
             it2 = it;
             ++it;
          }
    
          it1 = --it;
          for (int i=0; i<Nhalf; i++)
          {
             --it1;
          }
          
          CommonTime firstEpoch = it1->first;
          for( int i=0; i<N; i++)
          {
             times.push_back(it1->first - firstEpoch);
             VX.push_back(it1->second.v[0]);
             VY.push_back(it1->second.v[1]);
             VZ.push_back(it1->second.v[2]);
             ++it1;
          }
             // std::cout << "t: " << t << std::endl;
          double dt(epoch-firstEpoch), err;
          Vel[0] = LagrangeInterpolation(times, VX, dt, err);
          Vel[1] = LagrangeInterpolation(times, VY, dt, err);
          Vel[2] = LagrangeInterpolation(times, VZ, dt, err);
          
          return Vel;
       }
    
    } // END of "getVel"


}





















