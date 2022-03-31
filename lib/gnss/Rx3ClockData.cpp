/// @file Rx3ClockData.cpp
/// Encapsulate Rinex3Clock file data, including I/O
/// 

#include "Rx3ClockHeader.hpp"
#include "Rx3ClockData.hpp"
#include "StringUtils.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"

#include <fstream>
#include <iostream>

using namespace std;
using namespace utilSpace;
using namespace timeSpace;

#define debug 0

namespace gnssSpace
{

   void Rx3ClockData::reallyPutRecord(std::fstream& strm) 
      noexcept(false)
   {

      // firstly, output the record
      if(!headerWrite)
      {
         strm << header;
         headerWrite = true;
      }

      string line;

      line = datatype;
      line += string(1,' ');

      if(datatype == string("AR")) 
      {
         line += rightJustify(site,4);
      }
      else if(datatype == string("AS")) 
      {
         line += string(1,sat.systemChar());
         line += rightJustify(asString(sat.id),2);
         if(line[4] == ' ') line[4] = '0';
         line += string(1,' ');
      }
      else 
      {
         FFStreamError e("Unknown data type: " + datatype);
         THROW(e);
      }
      line += string(1,' ');

      line += time.asString();

      // must count the data to output
      int n(2);
      if(drift != 0.0) n=3;
      if(sig_drift != 0.0) n=4;
      if(accel != 0.0) n=5;
      if(sig_accel != 0.0) n=6;
      line += rightJustify(asString(n),3);
      line += string(3,' ');

      line += doubleToScientific(bias, 19, 12, 2);
      line += string(1,' ');
      line += doubleToScientific(sig_bias, 19, 12, 2);

      strm << line << endl;

      // continuation line
      if(n > 2) 
      {
         line = doubleToScientific(drift, 19, 12, 2);
         line += string(1,' ');
         if(n > 3) 
         {
            line += doubleToScientific(sig_drift, 19, 12, 2);
            line += string(1,' ');
         }
         if(n > 4) 
         {
            line += doubleToScientific(accel, 19, 12, 2);
            line += string(1,' ');
         }
         if(n > 5) 
         {
            line += doubleToScientific(sig_accel, 19, 12, 2);
            line += string(1,' ');
         }

         strm << line << endl;
      }

   }  // end reallyPutRecord()

   void Rx3ClockData::reallyGetRecord(std::fstream& strm)
      noexcept(false)
   {
      
      // If the header hasn't been read, read it...
      if(debug)
      {
          cout << "headerRead:" << headerRead << endl;
      }

      if(!headerRead)
      {
         try
         {
            strm.clear();//必须先调用这个，清除flag
            strm.seekg(0);//才能用这个设定到文件开头
            strm >> header;
         }
         catch(std::exception& e)
         {
            FFStreamError fse(string("std::exception reading header ") + e.what());
            THROW(fse);
         }
         catch(FFStreamError& fse)
         {
             RETHROW(fse);
         }

         headerRead = true;
      }

      clear();

      string line;
      getline(strm, line);
      if(debug)
      {
          cout << "Rx3ClockData:" << line << endl;
      }

      if(strm.eof())
      {
          EndOfFile e("end of file");
          THROW(e);
      }

      stripTrailing(line);
      if(line.length() < 59) 
      {
         FFStreamError e("Short line : " + line);
         THROW(e);
      }

      //cout << "Data Line: /" << line << "/" << endl;
      datatype = line.substr(0,2);
      site = line.substr(3,4);
      if(datatype == string("AS")) 
      {
         strip(site);
         int prn(asInt(site.substr(1,2)));
         if(site[0] == 'G') sat = SatID(prn,SatelliteSystem::GPS);
         else if(site[0] == 'R') sat = SatID(prn,SatelliteSystem::GLONASS);
         else if(site[0] == 'E') sat = SatID(prn,SatelliteSystem::Galileo);
         else if(site[0] == 'C') sat = SatID(prn,SatelliteSystem::BDS);
         else if(site[0] == 'J') sat = SatID(prn,SatelliteSystem::QZSS);
         else if(site[0] == 'I') sat = SatID(prn,SatelliteSystem::IRNSS);
         else {
            FFStreamError e("Invalid sat : /" + site + "/");
            THROW(e);
         }
         site = string();
      }

      if(debug)
      {
         cout << "header.version:" << header.version << endl;
      }

      if(header.version >= 3.04)
      {
         time = CivilTime(asInt(line.substr( 8+5,4)),
                          asInt(line.substr(12+5,3)),
                          asInt(line.substr(15+5,3)),
                          asInt(line.substr(18+5,3)),
                          asInt(line.substr(21+5,3)),
                          asDouble(line.substr(24+5,10)),
                          TimeSystem::Any).convertToCommonTime();

         if(debug)
         {
             cout << YDSTime(time) << endl;
         }

         int n(asInt(line.substr(34+5,3)));
         bias = asDouble(line.substr(40+5,19));

         if(debug)
         {
             cout << setiosflags(ios::fixed) 
                  << setprecision(19) << "bias:" << bias << endl;
         }

         if(n > 1 && line.length() >= 59+5) 
             sig_bias = asDouble(line.substr(60+5,19));

         if(n > 2) 
         {
            getline(strm, line);
            if(strm.eof())
            {
                EndOfFile e("end of file");
                THROW(e);
            }

            stripTrailing(line);
            if(int(line.length()) < (n-2)*20-1) 
            {
               FFStreamError e("Short line : " + line);
               THROW(e);
            }

            drift = asDouble(line.substr( 0,19));
            if(n > 3) 
                sig_drift = asDouble(line.substr(20,19));
            if(n > 4) 
                accel     = asDouble(line.substr(40,19));
            if(n > 5) 
                sig_accel = asDouble(line.substr(60,19));
         }
      }
      else
      {
         time = CivilTime(asInt(line.substr( 8,4)),
                        asInt(line.substr(12,3)),
                        asInt(line.substr(15,3)),
                        asInt(line.substr(18,3)),
                        asInt(line.substr(21,3)),
                        asDouble(line.substr(24,10)),
                        TimeSystem::Any).convertToCommonTime();

         int n(asInt(line.substr(34,3)));
         bias = asDouble(line.substr(40,19));
         if(n > 1 && line.length() >= 59) 
             sig_bias = asDouble(line.substr(60,19));

         if(n > 2) 
         {
            getline(strm, line);
            if(strm.eof())
            {
                EndOfFile e("end of file");
                THROW(e);
            }

            stripTrailing(line);
            if(int(line.length()) < (n-2)*20-1) 
            {
               FFStreamError e("Short line : " + line);
               THROW(e);
            }

            drift = asDouble(line.substr( 0,19));
            if(n > 3) 
                sig_drift = asDouble(line.substr(20,19));
            if(n > 4) 
                accel     = asDouble(line.substr(40,19));
            if(n > 5) 
                sig_accel = asDouble(line.substr(60,19));
         }
      }

   }   // end reallyGetRecord()

   void Rx3ClockData::dump(ostream& s) const throw()
   {
      // dump record type, sat id / site, current epoch, and data
      s << " " << datatype;
      if(datatype == string("AR")) s << " " << site;
      else s << " " << sat.toString();
      s << " " << time.asString();
      s << scientific << setprecision(12)
         << " " << setw(19) << bias
         << " " << setw(19) << sig_bias;
      if(drift != 0.0) s << " " << setw(19) << drift; else s << " 0.0";
      if(sig_drift != 0.0) s << " " << setw(19) << sig_drift; else s << " 0.0";
      if(accel != 0.0) s << " " << setw(19) << accel; else s << " 0.0";
      if(sig_accel != 0.0) s << " " << setw(19) << sig_accel; else s << " 0.0";
      s << endl;

   }  // end dump()

} // namespace

//------------------------------------------------------------------------------------
