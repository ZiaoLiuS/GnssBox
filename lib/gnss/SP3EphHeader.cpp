#pragma ident "$Id$"

/**
 * @file SP3EphHeader.cpp
 * Encapsulate header of SP3 file data, including I/O
 *
 * modify code to be compatible with format sp3d
 *
 * shoujian zhang, 2020
 */

#include "StringUtils.hpp"
#include "SP3EphHeader.hpp"
#include "CivilTime.hpp"
#include "MJD.hpp"
#include "GPSWeekSecond.hpp"

#define debug false

#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;
using namespace utilSpace;
using namespace timeSpace;
using namespace gnssSpace;

namespace gnssSpace
{

    void SP3EphHeader::reallyGetRecord(std::fstream& strm)
        noexcept(false)
    {
        string line;

        getline(strm, line);
        if(debug) std::cout << "SP3 Header Line 1 " << line << std::endl;

        if (line[0]=='#' && line[1]!='#')                  // line 1
        {
            // version character
            if(line[1] == 'a')      version = SP3a;
            else if(line[1] == 'b') version = SP3b;
            else if(line[1] == 'c') version = SP3c;
            else if(line[1] == 'd') version = SP3d;
            else {
                FFStreamError e("Unknown version of SP3: " + line.substr(0,3));
                THROW(e);
            }

            // are V records present?
            if(line[2] == 'V') containsVelocity = true;
            else               containsVelocity = false;

            // parse the rest of the line
            int year = asInt(line.substr(3,4));
            int month = asInt(line.substr(8,2));
            int dom = asInt(line.substr(11,2));
            int hour = asInt(line.substr(14,2));
            int minute = asInt(line.substr(17,2));
            double second = asInt(line.substr(20,11));
            try {
                time = CivilTime(year, month, dom, hour, minute, second).convertToCommonTime();
            }
            catch (Exception& e) {
                FFStreamError fe("Invalid time:" + string(1, line[0]));
                THROW(fe);
            }
            numberOfEpochs = asInt(line.substr(32,7));
            dataUsed = line.substr(40,5);
            coordSystem = line.substr(46,5);
            orbitType = line.substr(52,3);
            agency = line.substr(56,4);
        }
        else
        {
            FFStreamError e("Unknown label in line 1: " + line.substr(0,2));
            THROW(e);
        }

        getline(strm, line);
        if(debug) std::cout << "SP3 Header Line 2 " << line << std::endl;
        if (line[0]=='#' && line[1]=='#')                           // line 2
        {
            epochInterval = asDouble(line.substr(24,14));
        }
        else
        {
            FFStreamError e("Unknown label in line 2: " + line.substr(0,2));
            THROW(e);
        }

        int i, index;
        int numSVs(0), readSVs(0);

        // the map stores them sorted, so use svsAsWritten to determine
        // which SV each accuracy corresponds to.
        vector<SatID> svsAsWritten;
        SatID sat;

        i = 3;

        streampos sp;

        // read in the SV list
        do
        {
            sp = strm.tellg();

            // read a line
            getline(strm, line);

            if(line[0]=='+' && line[1]=='+')
            {
                strm.seekg(sp);
                break;
            }

            if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
            if(line[0]=='+' && line[1]!='+')
            {
                if(i == 3)
                {
                    numSVs = asInt(line.substr(4,2));
                    svsAsWritten.resize(numSVs);
                }
                for(index = 9; index < 60; index += 3)
                {
                    if(readSVs < numSVs)
                    {
                        try 
                        {
                            sat = SatID(line.substr(index,3));
                        }
                        catch (Exception& e) 
                        {
                            FFStreamError ffse(e);
                            THROW(ffse);
                        }
                        svsAsWritten[readSVs] = sat;
                        satList[sat] = 0;
                        readSVs++;
                    }
                }
            }
            else
            {
                FFStreamError e("Unknown 1st char in line "
                                + asString(i) + ": "
                                + string(1, line[0]));
                THROW(e);
            }

            i++;

        } while( true );


        readSVs = 0;

        // read in the accuracy
        do
        {
            sp = strm.tellg();

            getline(strm, line);
            if(line[0]=='%' && line[1]=='c')
            {
                strm.seekg(sp);
                break;
            }

            if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
            if ((line[0]=='+') && (line[1]=='+'))
            {
                for(index = 9; index < 60; index += 3)
                {
                    if (readSVs < numSVs)
                    {
                        satList[svsAsWritten[readSVs]] = asInt(line.substr(index,3));
                        readSVs++;
                    }
                }
            }
            else
            {
                FFStreamError e("Unknown label in line "
                                + asString(i) + ": "
                                + line.substr(0,2));
                THROW(e);
            }

            i++;

        } while( true );


        getline(strm, line);
        if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
        if (version == SP3c || version == SP3d) 
        {
            if(line[0]=='%' && line[1]=='c')                         // line 13
            {
                // file system
                system.fromString(line.substr(3,2));

                // time system
                string ts = upperCase(line.substr(9,3));
                timeSystem.fromString(ts);
            }
            else
            {
                FFStreamError e("Unknown label in line "
                                + asString(i) + ": "
                                + line.substr(0,2));
                THROW(e);
            }
        }
        i++;

        getline(strm, line);
        if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
        i++;

        getline(strm, line);
        if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
        if (version == SP3c || version == SP3d) 
        {
            if (line[0]=='%' && line[1]=='f')                           // line 15
            {
                basePV = asDouble(line.substr(3,10));
                baseClk = asDouble(line.substr(14,12));
            }
            else
            {
                FFStreamError e("Unknown label in line "
                                + asString(i) + ": "
                                + line.substr(0,2));
                THROW(e);
            }
        }
        i++;

        getline(strm, line);                                // line 16
        if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
        i++;

         // read in 2 unused %i lines                             // lines 17,18
        for(int j = 0; j <= 1; j++) 
        {
            getline(strm, line);
            if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
            i++;
        }

         // read in 4 comment lines
        comments.clear();
        do
        {
            sp = strm.tellg();

            getline(strm, line);

            if(line[0] == '*')
            {
                strm.seekg(sp);
                break;
            }

            if(debug) std::cout << "SP3 Header Line " << i << " " << line << std::endl;
            // strip the first 3 characters
            line.erase(0, 3);
            // and add to the comment vector
            comments.push_back(line);

            i++;

        } while( true );


    }  // end SP3EphHeader::reallyGetRecord()


    void SP3EphHeader::reallyPutRecord(std::fstream& strm) 
        noexcept(false)
    {
        try 
        {

            int i,k;
            size_t j;
            string line;
            SatID SVid;
            bool isVerA = (version == SP3a);
            bool isVerC = (version == SP3c);
            bool isVerD = (version == SP3d);

            // line 1
            CivilTime civTime(time);
            line = "#";
            line += versionChar();
            line += (containsVelocity ? "V" : "P");
            line += civTime.asString();
            line += " " + rightJustify(civTime.asString(),11);
            line += " " + rightJustify(asString(numberOfEpochs),7);
            line += " " + rightJustify(dataUsed,5);
            line += " " + rightJustify(coordSystem,5);
            line += " " + rightJustify(orbitType,3);
            line += " " + rightJustify(agency,4);
            strm << line << endl;


            // line 2
            GPSWeekSecond gpsWS(time);
            line = "##";
            line += rightJustify(gpsWS.asString(),5);
            line += rightJustify(gpsWS.asString(),16);
            line += " " + rightJustify(asString(epochInterval,8),14);
            line += " " + MJD(time).asString();
            line += " " + rightJustify(asString(time.getSecondOfDay()/86400.,13),15);
            strm << line << endl;

            // lines 3-7 and 8-12
            //Map<SV,accuracy flag> (all SVs in data)

/*
            std::map<SatID, short>::const_iterator it;
            for(i=3; i<=12; i++) {                 // loop over the lines
                if(i==3) line = "+   " + rightJustify(asString(satList.size()),2) + "   ";
                else if(i < 8) line = "+        ";
                else           line = "++       ";
                k = 0;
                if(i == 3 || i == 8)                // start the iteration
                    it = satList.begin();
                while(k < 17) {                     // there are 17 per line
                    if(it != satList.end()) {
                        if(i < 8) {                 // lines 3-7 - sat id
                            if(!isVerA) {
                            // a satellite in version c -> let j be -1 to mark it
                                SVid = it->first;
                                j = -1;
                            }
                            else j = it->first.id;
                        }
                        else                        // lines 8-12 - accuracy
                            j = it->second;
                        it++;
                    }
                    else j=0;            // no more

                    if(j == -1)          // sat version b or c
                        try {
                            line += rightJustify(SatID(SVid).toString(),3);
                        }
                        catch (Exception& e)
                        {
                            FFStreamError ffse(e);
                            THROW(ffse);
                        }

                    else                 // sat version a, accuracy, or 0
                        line += rightJustify(asString(j),3);
                        k++;
                }
                strm << line << endl;
            }
*/
            
            //////////////////
            //
            //
            // compatible with sp3d
            // sp3d, multi gnss satellite number will be increased from 85 to 999.
            // if satellite number is less  and equal than 85, five comment line will be given.
            // if satellite number is greater than 85, the comment line number will increase 
            // as needed.
            // by shoujian zhang
            //
            //////////////
            int numSatLine;
            if(satList.size() > 85)
            {
                numSatLine = ceil(satList.size()/17.0);
            }
            else
            {
                numSatLine = 5;
            }

            // now, let's define a vector to store the satellite ids.
            std::vector<std::string> satLineVec(numSatLine, "");
            std::vector<std::string> accLineVec(numSatLine, "");

            // at first, write the heading inforamtion for each line
            // and initialize all the satLineVec with '  0'
            for(int i=0; i<numSatLine; i++)
            {
                // sat id line
                if(i==0)
                {
                    satLineVec[i] = "+   " + rightJustify(asString(satList.size()),2) + "   ";
                }
                else
                {
                    satLineVec[i] = "+        ";
                }

                // sat id list initialize
                for(int k=0;k<17;k++)
                {
                    satLineVec[i].append("  0");
                }

                // accuracy line
                accLineVec[i] = "++       ";
                for(int k=0;k<17;k++)
                {
                    accLineVec[i].append("  0");
                }

            }

            // now, let's write the SatID into satLine;
            i=0;
            for(auto it=satList.begin(); it!=satList.end(); it++)
            {
                int numRow = floor(i/17);
                int numCol = i%17;

                // SatID string
                string satStr;
                if(isVerA)
                {
                    satStr = rightJustify(asString((*it).first.id),3);
                }
                else
                {
                    satStr = rightJustify((*it).first.toString(),3);
                }

                // accuracy string
                string accStr;
                accStr = rightJustify(asString((*it).second),3);

                // replace with satStr
                satLineVec[numRow].replace(9+(numCol)*3,3, satStr);
                accLineVec[numRow].replace(9+(numCol)*3,3, accStr);

                // number
                i++;
            }

            // for test
            for(auto it=satLineVec.begin(); it!=satLineVec.end();it++)
            {
                strm << (*it) << endl;
            }

            for(auto it=accLineVec.begin(); it!=accLineVec.end();it++)
            {
                strm << (*it) << endl;
            }

            ////////////////////

            // line 13
            string ft("cc");
            if(!isVerA) {
                ft[0] = system.systemChar();
                ft[1] = ' ';
            }
            strm << "%c " << ft << " cc"
                 << " " << (isVerA ? "ccc" : timeSystemString())
                 << " ccc cccc cccc cccc cccc ccccc ccccc ccccc ccccc" << endl;

            // line 14
            strm << "%c cc cc ccc ccc cccc cccc cccc cccc ccccc ccccc ccccc ccccc" << endl;

            // line 15
            strm << "%f "
                 << ((isVerC|| isVerD) ? rightJustify(asString(basePV,7),10) : " 0.0000000")
                 << " "
                 << ((isVerC|| isVerD) ? rightJustify(asString(baseClk,9),12) : " 0.000000000")
                 << "  0.00000000000  0.000000000000000" << endl;

            // lines 16-18
            strm << "%f  0.0000000  0.000000000  0.00000000000  0.000000000000000" << endl;
            strm << "%i    0    0    0    0      0      0      0      0         0" << endl;
            strm << "%i    0    0    0    0      0      0      0      0         0" << endl;

            // lines 19-22
            j=0;
            for(int i=19; i<=22; i++) 
            {
                line = "/* ";
                if(comments.size()>0) 
                {
                    if(isVerD)
                        line += leftJustify(comments[j++],77);
                    else
                        line += leftJustify(comments[j++],57);
                }
                else 
                {
                    if(isVerD)
                        line += string(77,'C');
                    else
                        line += string(57,'C');
                }
                strm << line << endl;
            }

        }
        catch(Exception& e) { RETHROW(e); }
        catch(std::exception& e) { Exception g(e.what()); THROW(g); }
    }

    void SP3EphHeader::dump(ostream& s) const throw()
    {
        s << "SP3 Header: version " << versionString() << " containing ";
        if(containsVelocity) s << "positions and velocities.";
        else s << "positions only.";
        CivilTime ct(time);
        s << endl;
        s << " Time tag : " << ct.asString() << endl;
        s << " Timespacing is " << epochInterval
          << " sec, and the number of epochs is " << numberOfEpochs << endl;
        s << " Data used as input : " << dataUsed << endl;
        s << " Coordinate system : " << coordSystem << endl;
        s << " Orbit estimate type : " << orbitType << endl;
        s << " Agency : " << agency << endl;
        if(version == SP3c || version == SP3d) {
            s << " File type: '" << system.systemChar() << "' which is "
              << system.systemString() << endl;
            s << " Time System: " << timeSystemString() << endl;
            s << " Base for Pos/Vel =" << fixed << setw(10) << setprecision(7)
              << basePV << endl;
            s << " Base for Clk/Rate =" << setw(12) << setprecision(9)
              << baseClk << endl;
        }

        s << " List of satellite PRN/accuracy (" << satList.size() << " total) :\n";
        int i=0;
        std::map<SatID,short>::const_iterator it=satList.begin();
        while(it != satList.end()) {
            s << " " << it->first << "/" << it->second;
            if(!(++i % 8)) s << endl;
            it++;
        }
        if(i % 8) s << endl;

        s << " Comments:\n";
        for(size_t j=0; j<comments.size(); j++) s << "    " << comments[j] << endl;

        s << "End of SP3 header" << endl;

    }  // end SP3EphHeader::dump()

}  // namespace
