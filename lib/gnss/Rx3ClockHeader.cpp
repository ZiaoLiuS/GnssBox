
/**
 * @file Rx3ClockHeader.cpp
 * Encapsulate header of RINEX Clock file header data, including I/O
 * 
 */

#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "SystemTime.hpp"
#include "Rx3ClockHeader.hpp"

#define debug 0

#include <fstream>

using namespace std;
using namespace utilSpace;
using namespace timeSpace;

namespace gnssSpace
{

   const string Rx3ClockHeader::versionString =        "RINEX VERSION / TYPE";
   const string Rx3ClockHeader::runByString =          "PGM / RUN BY / DATE";
   const string Rx3ClockHeader::commentString =        "COMMENT";
   const string Rx3ClockHeader::sysString =            "SYS / # / OBS TYPES";
   const string Rx3ClockHeader::timeSystemString =     "TIME SYSTEM ID";
   const string Rx3ClockHeader::leapSecondsString =    "LEAP SECONDS";
   const string Rx3ClockHeader::leapSecondsGNSSString ="LEAP SECONDS GNSS";
   const string Rx3ClockHeader::sysDCBString =         "SYS / DCBS APPLIED";
   const string Rx3ClockHeader::sysPCVString =         "SYS / PCVS APPLIED";
   const string Rx3ClockHeader::numDataString =        "# / TYPES OF DATA";
   const string Rx3ClockHeader::stationNameString =    "STATION NAME / NUM";
   const string Rx3ClockHeader::stationClockRefString ="STATION CLK REF";
   const string Rx3ClockHeader::analysisCenterString = "ANALYSIS CENTER";
   const string Rx3ClockHeader::numClockRefString =    "# OF CLK REF";
   const string Rx3ClockHeader::analysisClkRefrString ="ANALYSIS CLK REF";
   const string Rx3ClockHeader::numReceiversString =   "# OF SOLN STA / TRF";
   const string Rx3ClockHeader::solnStateString =      "SOLN STA NAME / NUM";
   const string Rx3ClockHeader::numSolnSatsString =    "# OF SOLN SATS";
   const string Rx3ClockHeader::prnListString =        "PRN LIST";
   const string Rx3ClockHeader::endOfHeaderString =    "END OF HEADER";

      // --------------------------------------------------------------------------------
    void Rx3ClockHeader::reallyGetRecord(std::fstream& strm)
        noexcept(false)
    {
        // clear the storage
        clear();

        string line;
        while(!(valid & endOfHeaderValid)) 
        {
            getline(strm, line);
            stripTrailing(line);

            if(debug) 
                cout << "Rinex3Clock Header Line " << line << endl;

            if(line.length() == 0) 
                continue;
            else if(line.length() < 60 || line.length() > 120) 
            {
                FFStreamError e("Invalid line length");
                THROW(e);
            }

            // first let's read the rinex clock version
            if(firstLine)
            {
                version = asDouble(strip(line.substr(0,9)));
                firstLine = false;
            }

            if(debug)
            {
                cout << "version:" << version << endl; 
            }

            // parse the line
            try 
            {
                string label;
                if(version >= 3.04)
                {
                   label = strip(line.substr(65, 20));
                }
                else
                {
                   label = strip(line.substr(60, 20));
                }

                if(debug)
                {
                    cout << label << endl;
                }

                if(label == versionString) 
                {
                    if(version >= 3.04)
                    {
                        if(line[21] != 'C') 
                        {
                            FFStreamError e("Invalid file type: " + line.substr(21,1));
                            THROW(e);
                        }
                    }
                    else
                    {
                        if(line[20] != 'C') 
                        {
                            FFStreamError e("Invalid file type: " + line.substr(20,1));
                            THROW(e);
                        }
                    }
                    // TD system
                    valid |= versionValid;
                }
                else if(label == runByString) 
                {
                    if(version>=3.04)
                    {
                        program = strip(line.substr(0,21));
                        runby = strip(line.substr(21,21));
                    }
                    else
                    {
                        program = strip(line.substr(0,20));
                        runby = strip(line.substr(20,20));
                    }
                    //date = strip(line.substr(40,20));
                    valid |= runByValid;
                }
                else if(label == commentString) 
                {
                    if(version>=3.04)
                    {
                        commentList.push_back(strip(line.substr(0,66)));
                    }
                    else
                    {
                        commentList.push_back(strip(line.substr(0,60)));
                    }
                    valid |= commentValid;
                }
                else if(label == sysString) 
                {
                    ; // TD ??
                    valid |= sysValid;
                }
                else if(label == timeSystemString) 
                {
                    string ts(upperCase(line.substr(3,3)));
                    timeSystem.fromString(ts);
                    valid |= timeSystemValid;
                }
                else if(label == leapSecondsString) 
                {
                    leapSeconds = asInt(line.substr(0,6));
                    valid |= leapSecondsValid;
                }
                else if(label == leapSecondsGNSSString) 
                {
                    leapSeconds = asInt(line.substr(0,6));
                    valid |= leapSecondsValid;
                }
                else if(label == sysDCBString) 
                {
                    //if(line[0] == 'G')
                    //   dcbSystem = SatID(-1,SatelliteSystem::GPS);
                    //else if(line[0] == 'R')
                    //   dcbSystem = SatID(-1,SatelliteSystem::Glonass);
                    //else {
                    //   FFStreamError e("Invalid dcb system : " + line.substr(0,1));
                    //   THROW(e);
                    //}
                    valid |= sysDCBValid;
                }
                else if(label == sysPCVString) 
                {
                    if(line[0] == 'G')
                        pcvsSystem = SatID(-1,SatelliteSystem::GPS);
                    else if(line[0] == 'R')
                        pcvsSystem = SatID(-1,SatelliteSystem::GLONASS);
                    else if(line[0] == 'C')
                        pcvsSystem = SatID(-1,SatelliteSystem::BDS);
                    else if(line[0] == 'E')
                        pcvsSystem = SatID(-1,SatelliteSystem::Galileo);
                    else if(line[0] == 'S')
                        pcvsSystem = SatID(-1,SatelliteSystem::SBAS);
                    else if(line[0] == 'I')
                        pcvsSystem = SatID(-1,SatelliteSystem::IRNSS);
                    else if(line[0] == 'J')
                        pcvsSystem = SatID(-1,SatelliteSystem::QZSS);
                    pcvsProgram = strip(line.substr(1,17));
                    pcvsSource = strip(line.substr(20,40));
                    valid |= sysPCVValid;
                }
                else if(label == numDataString) 
                {
                    int n(asInt(line.substr(0,6)));
                    for(int i=0; i<n; ++i)
                        dataTypes.push_back(line.substr(10+i*6,2));
                    valid |= numDataValid;
                }
                else if(label == stationNameString) 
                {
                    //string label(strip(line.substr(0,4)));
                    //stationID[label] = strip(line.substr(5,20));
                    valid |= stationNameValid;
                }
                else if(label == stationClockRefString) 
                {
                    valid |= stationClockRefValid;
                }
                else if(label == analysisCenterString) 
                {
                    analCenterDesignator = strip(line.substr(0,3));
                    analysisCenter = strip(line.substr(5,55));
                    valid |= analysisCenterValid;
                }
                else if(label == numClockRefString) 
                {
                    valid |= numClockRefValid;
                }
                else if(label == analysisClkRefrString) 
                {
                    valid |= analysisClkRefrValid;
                }
                else if(label == numReceiversString) 
                {
                    numSolnStations = asInt(line.substr(0,6));
                    terrRefFrame = strip(line.substr(10,50));
                    valid |= numReceiversValid;
                }
                else if(label == solnStateString) 
                {
                    string label;
                    if(version >=3.04)
                    {
                        label = (strip(line.substr(0,9)));
                        stationID[label] = strip(line.substr(10,19));
                        stationX[label] = strip(line.substr(29,12));
                        stationY[label] = strip(line.substr(41,12));
                        stationZ[label] = strip(line.substr(53,12));
                    }
                    else
                    {
                        label = (strip(line.substr(0,4)));
                        stationID[label] = strip(line.substr(4,19));
                        stationX[label] = strip(line.substr(23,12));
                        stationY[label] = strip(line.substr(35,12));
                        stationZ[label] = strip(line.substr(47,12));
                    }
                    valid |= solnStateValid;
                }
                else if(label == numSolnSatsString) 
                {
                    numSolnSatellites = asInt(line.substr(0,6));
                    valid |= numSolnSatsValid;
                }
                else if(label == prnListString) 
                {
                    int i,prn;
                    string label;
                    if(version>=3.04)
                    {
                        for(i=0; i<16; ++i) 
                        {
                            label = line.substr(4*i,3);
                            if(label == string("   ")) break;
                            prn = asInt(line.substr(4*i+1,2));
                            if(line[4*i] == 'G')
                                satList.push_back(SatID(prn,SatelliteSystem::GPS));
                            else if(line[4*i] == 'R')
                                satList.push_back(SatID(prn,SatelliteSystem::GLONASS));
                            else if(line[4*i] == 'E')
                                satList.push_back(SatID(prn,SatelliteSystem::Galileo));
                            else if(line[4*i] == 'C')
                                satList.push_back(SatID(prn,SatelliteSystem::BDS));
                            else if(line[4*i] == 'J')
                                satList.push_back(SatID(prn,SatelliteSystem::QZSS));
                            else if(line[4*i] == 'I')
                                satList.push_back(SatID(prn,SatelliteSystem::IRNSS));
                            else 
                            {
                                FFStreamError e("Invalid sat (PRN LIST): /" + label + "/");
                                THROW(e);
                            }
                        }
                    }
                    else
                    {
                        for(i=0; i<15; ++i) 
                        {
                            label = line.substr(4*i,3);
                            if(label == string("   ")) break;
                            prn = asInt(line.substr(4*i+1,2));
                            if(line[4*i] == 'G')
                                satList.push_back(SatID(prn,SatelliteSystem::GPS));
                            else if(line[4*i] == 'R')
                                satList.push_back(SatID(prn,SatelliteSystem::GLONASS));
                            else if(line[4*i] == 'E')
                                satList.push_back(SatID(prn,SatelliteSystem::Galileo));
                            else if(line[4*i] == 'C')
                                satList.push_back(SatID(prn,SatelliteSystem::BDS));
                            else if(line[4*i] == 'J')
                                satList.push_back(SatID(prn,SatelliteSystem::QZSS));
                            else if(line[4*i] == 'I')
                                satList.push_back(SatID(prn,SatelliteSystem::IRNSS));
                            else 
                            {
                                FFStreamError e("Invalid sat (PRN LIST): /" + label + "/");
                                THROW(e);
                            }
                        }
                    }

                    // TD how to check numSolnSatsValid == satList.size() ?
                    valid |= prnListValid;
                }
                else if(label == endOfHeaderString) 
                {
                    valid |= endOfHeaderValid;
                }
                else 
                {
                    FFStreamError e("Invalid line label: " + label);
                    THROW(e);
                }

                if(debug) cout << "Valid is " << hex << valid << fixed << endl;

            }  // end parsing the line
            catch(FFStreamError& e) { RETHROW(e); }

        }  // end while end-of-header not found

        if(debug) cout << "Header read; Valid is " << hex << valid << fixed << endl;

         // is this header valid?
        if( (valid & allRequiredValid) != allRequiredValid) 
        {
            cout << "Header is invalid on input (valid is x" << hex << valid
                 << dec << ").\n";
            dumpValid(cout);
            FFStreamError e("Invalid header");
            THROW(e);
        }

    }  // end Rx3ClockHeader::reallyGetRecord()


    void Rx3ClockHeader::reallyPutRecord(std::fstream& strm) 
        noexcept(false)
    {
        try 
        {
            // is this header valid?
            if( (valid & allRequiredValid) != allRequiredValid) 
            {
                FFStreamError e("Invalid header");
                THROW(e);
            }

            size_t i;
            string line;
            try 
            {
                line = rightJustify(asString(version,2), 9);
                line += string(11,' ');
                line += string("CLOCK") + string(15,' ');
                line += string("GPS") + string(17,' ');      // TD fix
                line += versionString;         // "RINEX VERSION / TYPE"
                strm << line << endl;

                line = leftJustify(program,20);
                line += leftJustify(runby,20);
                CommonTime dt = SystemTime().convertToCommonTime();
                string dat = dt.asString();
                line += leftJustify(dat, 20);
                line += runByString;           // "PGM / RUN BY / DATE"
                strm << line << endl;

                if(valid & sysValid) 
                {
                    line = string(60,' ');  // TD
                    line += sysString;          // "SYS / # / OBS TYPES"
                    strm << line << endl;
                }

                if(valid & timeSystemValid) 
                {
                    line = string(60,' ');  // TD
                    line += timeSystemString;      // "TIME SYSTEM ID"
                    strm << line << endl;
                }

                for(i=0; i<commentList.size(); ++i) 
                {
                    line = leftJustify(commentList[i],60);
                    line += commentString;         // "COMMENT"
                    strm << line << endl;
                }

                if(valid & leapSecondsValid) 
                {
                    line = rightJustify(asString(leapSeconds), 6);
                    line += string(54,' ');
                    line += leapSecondsString;     // "LEAP SECONDS"
                    strm << line << endl;
                }

                if(valid & sysDCBValid) 
                {
                    line = string(60,' ');  // TD
                    line += sysDCBString;          // "SYS / DCBS APPLIED"
                    strm << line << endl;
                }

                if(valid & sysPCVValid) 
                {
                    line = string("  ");
                    line[0] = pcvsSystem.systemChar();
                    line += leftJustify(pcvsProgram,17);
                    line += string(" ");
                    line += leftJustify(pcvsSource,40);
                    line += sysPCVString;          // "SYS / PCVS APPLIED"
                    strm << line << endl;
                }

                line = rightJustify(asString(dataTypes.size()), 6);
                for(i=0; i<dataTypes.size(); ++i)
                    line += string(4,' ') + dataTypes[i];
                line += string(60-line.size(),' ');
                line += numDataString;         // "# / TYPES OF DATA"
                strm << line << endl;

                //TD line += stationNameString;     // "STATION NAME / NUM"
                //strm << line << endl;

                //TD line += stationClockRefString; // "STATION CLK REF"
                //strm << line << endl;

                line = analCenterDesignator;
                line += string(2,' ');
                line += leftJustify(analysisCenter,55);
                line += analysisCenterString;  // "ANALYSIS CENTER"
                strm << line << endl;

                //line += numClockRefString;     // "# OF CLK REF"
                //strm << line << endl;

                //line += analysisClkRefrString; // "ANALYSIS CLK REF"
                //strm << line << endl;

                line = rightJustify(asString(numSolnStations), 6);
                line += string(4,' ');
                line += leftJustify(terrRefFrame,50);
                line += numReceiversString;    // "# OF SOLN STA / TRF"
                strm << line << endl;

                map<string,string>::const_iterator it, jt;
                for(it=stationID.begin(); it != stationID.end(); ++it) 
                {
                    string label(it->first),field;
                    line = label;
                    line += string(1,' ');
                    line += leftJustify(it->second,20);
                    jt = stationX.find(label);
                    field = jt->second;
                    line += rightJustify(field, 11);
                    line += string(1,' ');
                    jt = stationY.find(label);
                    field = jt->second;
                    line += rightJustify(field, 11);
                    line += string(1,' ');
                    jt = stationZ.find(label);
                    field = jt->second;
                    line += rightJustify(field, 11);
                    line += solnStateString;       // "SOLN STA NAME / NUM"
                    strm << line << endl;
                }

                line = rightJustify(asString(numSolnSatellites), 6);
                line += string(54,' ');
                line += numSolnSatsString;     // "# OF SOLN SATS"
                strm << line << endl;

                line = string();
                for(i=0; i<satList.size(); ++i) 
                {
                    string satstr(" ");
                    satstr[0] = satList[i].systemChar();
                    satstr += rightJustify(asString(satList[i].id), 2);
                    if(satstr[1] == ' ') satstr[1] = '0';
                    line += satstr + string(1,' ');
                    if(((i+1) % 15) == 0 || i==satList.size()-1) {
                        line += string(60-line.size(),' ');
                        line += prnListString;         // "PRN LIST"
                        strm << line << endl;
                        line = string();
                    }
                }

                line = string(60,' ');
                line += endOfHeaderString;     // "END OF HEADER"
                strm << line << endl;
            }
            catch(FFStreamError& e) 
            { 
                RETHROW(e); 
            }
            catch(StringException& e) 
            { 
                RETHROW(e); 
            }

        }
        catch(Exception& e) 
        { 
            RETHROW(e); 
        }
        catch(exception& e) 
        { 
            Exception g(e.what()); 
            THROW(g); 
        }
    }


    void Rx3ClockHeader::dump(ostream& os, short detail) const throw()
    {
       size_t i;
       os << "Dump Rx3Clock Header:\n";
       os << " Version = " << fixed << setprecision(2) << version
          << " Prgm /" << program << "/ Run By /" << runby << "/" << endl;
       os << " There are " << dataTypes.size() << " data types, as follows:";
       for(i=0; i<dataTypes.size(); ++i)
          os << " " << dataTypes[i];
       os << endl;
       os << " Leap seconds is " << leapSeconds << endl;
       os << " Analysis center: /" << analCenterDesignator
          << "/ /" << analysisCenter << "/" << endl;
       os << " Terrestrial Reference Frame " << terrRefFrame << endl;
       os << " PCVs: " << pcvsSystem << " /" << pcvsProgram << "/ /"
          << pcvsSource << "/" << endl;
       os << " Comments:\n";
       for(i=0; i<commentList.size(); ++i)
          os << "    " << commentList[i] << endl;
       os << " There are " << stationID.size() << " stations." << endl;
       os << " There are " << satList.size() << " satellites." << endl;
       if(detail > 0) {
          os << " Stations:  identifier     X(mm)       Y(mm)       Z(mm)\n";
          map<string,string>::const_iterator it, jt;
          for(it=stationID.begin(); it!=stationID.end(); ++it) {
             string label(it->first),field;
             os << "     " << label << "   " << it->second;
             jt = stationX.find(label);
             field = jt->second;
             os << rightJustify(field,12);
             jt = stationY.find(label);
             field = jt->second;
             os << rightJustify(field,12);
             jt = stationZ.find(label);
             field = jt->second;
             os << rightJustify(field,12) << endl;
          }
          os << " Sat list:\n";
          for(i=0; i<satList.size(); ++i) {
             os << " " << satList[i];
             if(((i+1)%15) == 0 || i == satList.size()-1) os << endl;
          }
  
          if(detail >= 2) dumpValid(os);
       }
  
       os << "End of Rinex3Clock header dump." << endl;
  
    }  // end Rx3ClockHeader::dump()


    void Rx3ClockHeader::dumpValid(ostream& os) const throw()
    {
        if( (valid & allValid) == allValid) return;
        string tag("  Invalid or missing header line: ");
        os << "Dump invalid or missing header records:\n";
        if(!(valid & versionValid)) os << tag << versionString << endl;
        if(!(valid & runByValid)) os << tag << runByString << endl;
        if(!(valid & commentValid)) os << tag << commentString << endl;
        if(!(valid & sysValid)) os << tag << sysString << endl;
        if(!(valid & timeSystemValid)) os << tag << timeSystemString << endl;
        if(!(valid & leapSecondsValid)) os << tag << leapSecondsString << endl;
        if(!(valid & sysDCBValid)) os << tag << sysDCBString << endl;
        if(!(valid & sysPCVValid)) os << tag << sysPCVString << endl;
        if(!(valid & numDataValid)) os << tag << numDataString << endl;
        if(!(valid & stationNameValid)) os << tag << stationNameString << endl;
        if(!(valid & stationClockRefValid)) os << tag << stationClockRefString << endl;
        if(!(valid & analysisCenterValid)) os << tag << analysisCenterString << endl;
        if(!(valid & numClockRefValid)) os << tag << numClockRefString << endl;
        if(!(valid & analysisClkRefrValid)) os << tag << analysisClkRefrString << endl;
        if(!(valid & numReceiversValid)) os << tag << numReceiversString << endl;
        if(!(valid & solnStateValid)) os << tag << solnStateString << endl;
        if(!(valid & numSolnSatsValid)) os << tag << numSolnSatsString << endl;
        if(!(valid & prnListValid)) os << tag << prnListString << endl;
        if(!(valid & endOfHeaderValid)) os << tag << endOfHeaderString << endl;
        os << "End of invalid or missing dump" << endl;
    }

}  // namespace
