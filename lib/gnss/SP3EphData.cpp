/// @file SP3EphData.cpp
/// Encapsulate SP3 file data, including I/O

#include "SP3EphHeader.hpp"
#include "SP3EphData.hpp"
#include "StringUtils.hpp"
#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"

#include <iostream>
#include <fstream>

using namespace std;
using namespace gnssSpace;

namespace gnssSpace
{
   void SP3EphData::reallyGetRecord(std::fstream& strm)
      noexcept(false)
   {

      // If the header hasn't been read, read it...
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

      // status says which records have been written already this call
      // status = 1 if an epoch record was read
      // status = 2 if an P or V record was read
      // status = 3 if an EP or EV record was read
      int status = 0;

      // version to be written out is determined by written header (stored in strm)
      bool isVerA = (header.getVersion() == SP3EphHeader::SP3a);
      bool isVerC = (header.getVersion() == SP3EphHeader::SP3c);
      bool isVerD = (header.getVersion() == SP3EphHeader::SP3d);

      // correlation flag will be set if there is an EP/EV record
      correlationFlag = false;

      // default for the 'c' arrays is all zeros
      sdev[0] = sdev[1] = sdev[2] = sdev[3] = 0;
      sig[0] = sig[1] = sig[2] = sig[3] = 0;
      correlation[0] = correlation[1] = correlation[2]
         = correlation[3] = correlation[4] = correlation[5] = 0;

      // TimeSystem for this stream
      TimeSystem timeSystem;
      timeSystem.fromString(header.timeSystemString());

      // loop until an error occurs, or until the entire record (which may consist
      // of two lines, i.e. current line and the next line) is read.
      while(1) 
      {

         // set the time to be the last epoch read by the stream
         time = currentEpoch;

         // process the lastLine string stored in strm - contains the last line read
         // empty record, for the first time reading
         if(lastLine.size() < 3) 
         {      // 3 b/c it may contain "EOF"
            // nothing in lastLine - do nothing here, get another line
            ;
         }

         // EOF has been read
         else if(lastLine.substr(0,3) == string("EOF")) 
         {
            // if a data record has already been processed during this call,
            // then return here, and let the next call process this EOF.
            // This gives the caller a chance to process the data before hitting EOF.

            // if an epoch record was processed this call, that's an error
            if(status == 1) 
            {
               FFStreamError ffse("EOF was found immediately after epoch line");
               THROW(ffse);
            }

            // some other record was processed this call, so quit
            if(status > 1) 
            {
                reachEOF = true;
                EndOfFile e("end of file");
                THROW(e);
                break;
            }


            // this next read had better fail - if it does not, then there is
            // an "EOF" that is followed by something other than the file EOF.
            getline(strm, lastLine);
            if(strm.eof())
            {
                EndOfFile e("end of file");
                THROW(e);
            }

            // the GetLine succeeded, so this is an error
            FFStreamError err("EOF text found, followed by: " + lastLine);
            THROW(err);
         }

         // Epoch line read
         else if(lastLine[0] == '*') 
         {

            // if an epoch record was already processed this call, that's an error
            // TD consider simply removing this if(status==1) entirely. Some SP3 files
            // particularly those generated from a realtime stream, have consecutive
            // epoch lines. Why would we not just ignore the epoch with no data?
            if(status == 1) 
            {
               FFStreamError ffse("Consecutive epoch records found");
               THROW(ffse);
            }

            // if another record has been process during this call, quit now
            if(status > 1) break;

            // process an epoch record
            status = 1;

            // mark this record as non-data, in case another P|V record is not read
            sat = SatID();

            // warn if the line is short but not too short
            if(lastLine.size() <= 30 && lastLine.size() > 26)
               warnings.push_back(
                  string("Warning (SP3 std): short epoch line: ") + lastLine);

            // throw if the line is short
            if(lastLine.size() <= 26) 
            { 
               // some igs files cut seconds short 30)
               FFStreamError err("Invalid line length " + asString(lastLine.size()));
               THROW(err);
            }

            // parse the epoch line
            RecType = lastLine[0];
            int year = asInt(lastLine.substr(3,4));
            int month = asInt(lastLine.substr(8,2));
            int dom = asInt(lastLine.substr(11,2));
            int hour = asInt(lastLine.substr(14,2));
            int minute = asInt(lastLine.substr(17,2));
            double second = asInt(lastLine.substr(20,10));
            CivilTime t;
            try 
            {
               t = CivilTime(year, month, dom, hour, minute, second, timeSystem);
            }
            catch (Exception& e) 
            {
               FFStreamError fe("Invalid time in:" + lastLine);
               THROW(fe);
            }

            time = currentEpoch = (t).convertToCommonTime();
         }
         // P or V record read
         else if(lastLine[0] == 'P' || lastLine[0] == 'V') 
         {

            // if another record has been process during this call, quit now
            if(status > 0) break;

            // process this P|V
            status = 2;
            RecType = lastLine[0];     // P or V

/*
            // if its version c and the line is short (<80) but valid(>59),
            // then add blanks at the end to make it 80 character.
            if (isVerC && lastLine.size() < 80 && lastLine.size() > 59)
               leftJustify(lastLine,80);

            if (isVerD && lastLine.size() < 80 && lastLine.size() > 59)
               leftJustify(lastLine,80);

            // throw if the line is short
            if ((!isVerC && lastLine.size() < 60) ||
                 (isVerC && lastLine.size() < 80) ) 
            {
               FFStreamError err("Invalid line length ("
                                  + asString(lastLine.size())
                                  + ") for line:\n" + lastLine);
               THROW(err);
            }

            if ((!isVerD && lastLine.size() < 60) ||
                 (isVerD && lastLine.size() < 80) ) 
            {
               FFStreamError err("Invalid line length ("
                                  + asString(lastLine.size())
                                  + ") for line:\n" + lastLine);
               THROW(err);
            }
*/
            numCol = lastLine.size();

            // parse the line
            sat = static_cast<SatID>(SatID(lastLine.substr(1,3)));

            x[0] = asDouble(lastLine.substr(4,14));             // XYZ
            x[1] = asDouble(lastLine.substr(18,14));
            x[2] = asDouble(lastLine.substr(32,14));
            clk  = asDouble(lastLine.substr(46,14));             // Clock

            // the rest is version c only
            if(isVerC || isVerD) 
            {
                if(lastLine.size()>60)
                {
                    sig[0] = asInt(lastLine.substr(61,2));           // sigma XYZ
                    sig[1] = asInt(lastLine.substr(64,2));
                    sig[2] = asInt(lastLine.substr(67,2));
                    sig[3] = asInt(lastLine.substr(70,3));           // sigma clock
                }

                if(RecType == 'P') 
                {                                  // P flags
                   clockEventFlag = clockPredFlag = orbitManeuverFlag = orbitPredFlag = false;
                   if(lastLine[74] == 'E') clockEventFlag = true;
                   if(lastLine[75] == 'P') clockPredFlag = true;
                   if(lastLine[78] == 'M') orbitManeuverFlag = true;
                   if(lastLine[79] == 'P') orbitPredFlag = true;
                }
            }
         }

         // EP or EV correlation record read
         else if(lastLine[0] == 'E' && (lastLine[1] == 'P' || lastLine[1] == 'V'))
         {
            // throw if correlation record did not follow corresponding P|V record
            if(status != 2 || lastLine[1] != RecType) 
            {
               Exception e("correlation EP|V record mismatched with previous record");
               THROW(e);
            }

            // process EP|V record
            status = 3;

            // throw if line is short
            if(lastLine.size()<80) 
            {
               FFStreamError 
                   err("Invalid SP3c correlation line length (" 
                           + asString(lastLine.size()) + ") for line:\n" + lastLine);
               THROW(err);
            }

            // parse the line
            sdev[0] = abs(asInt(lastLine.substr(4,4)));
            sdev[1] = abs(asInt(lastLine.substr(9,4)));
            sdev[2] = abs(asInt(lastLine.substr(14,4)));
            sdev[3] = abs(asInt(lastLine.substr(19,7)));
            correlation[0] = asInt(lastLine.substr(27,8));
            correlation[1] = asInt(lastLine.substr(36,8));
            correlation[2] = asInt(lastLine.substr(45,8));
            correlation[3] = asInt(lastLine.substr(54,8));
            correlation[4] = asInt(lastLine.substr(63,8));
            correlation[5] = asInt(lastLine.substr(72,8));

            // tell the caller that correlation data is now present
            correlationFlag = true;
         }
         else 
         {                              // Unknown record
            FFStreamError err("Unknown record label " + lastLine.substr(0,2));
            THROW(err);
         }


         // read next line into the lastLine
         getline(strm, lastLine);

         if(strm.eof())
         {
             EndOfFile e("end of file");
             THROW(e);
         }

         if(status==3) break;

      }  // end while loop processing records

   }   // end reallyGetRecord()

   void SP3EphData::reallyPutRecord(std::fstream& strm) 
      noexcept(false)
   {

      // firstly, output the record
      if(!headerWrite)
      {
         strm << header;
         headerWrite = true;
      }

      string line;

      // version to be written out is determined by written header (stored in strm)
      bool isVerA = (header.getVersion() == SP3EphHeader::SP3a);
      bool isVerC = (header.getVersion() == SP3EphHeader::SP3c);
      bool isVerD = (header.getVersion() == SP3EphHeader::SP3d);

      // output Epoch Header Record
      if(reachEOF)
      {
         line = "EOF";
      }
      else if(RecType == '*') 
      {
         CivilTime civTime(time);
         line = "* ";
         line += civTime.asString();
         line += " " + rightJustify(civTime.asString(),11);
      }

      // output Position and Clock OR Velocity and Clock Rate Record
      else 
      {
         line = RecType;                                    // P or V
         if(isVerA) 
         {
            if(sat.system != SatelliteSystem::GPS) 
            {
               FFStreamError fse("Cannot output non-GPS to SP3a");
               THROW(fse);
            }
            line += rightJustify(asString(sat.id),3);
         }
         else
         {
            line += static_cast<SatID>(sat).toString();  // sat ID
         }

         line += rightJustify(asString(x[0],6),14);         // XYZ
         line += rightJustify(asString(x[1],6),14);
         line += rightJustify(asString(x[2],6),14);
         line += rightJustify(asString(clk,6),14);          // Clock

         if(isVerC || isVerD) 
         {
            if(numCol>60)
            {
               line += rightJustify(asString(sig[0]),3);       // sigma XYZ
               line += rightJustify(asString(sig[1]),3);
               line += rightJustify(asString(sig[2]),3);
               line += rightJustify(asString(sig[3]),4);       // sigma Clock

               if(RecType == 'P') 
               {                            // flags or blanks
                  line += string(" ");
                  line += (clockEventFlag ? string("E") : string(" "));
                  line += (clockPredFlag ? string("P") : string(" "));
                  line += string("  ");
                  line += (orbitManeuverFlag ? string("M") : string(" "));
                  line += (orbitPredFlag ? string("P") : string(" "));
               }
            }

         }

         // if version is 'c' and correlation flag is set,
         // then output the P|V Correlation Record
         if((isVerC || isVerD) && correlationFlag) 
         {
            // first output the P|V record you just built
            strm << line << endl;

            // now build and output the correlation record
            if(RecType == 'P')                                 // P or V
               line = "EP ";
            else
               line = "EV ";
            line += rightJustify(asString(sdev[0]),5);         // stddev X
            line += rightJustify(asString(sdev[1]),5);         // stddev Y
            line += rightJustify(asString(sdev[2]),5);         // stddev Z
            line += rightJustify(asString(sdev[3]),8);         // stddev Clk

            for(int i=0; i<6; i++)                             // correlations
               line += rightJustify(asString(correlation[i]),9);
         }
      }

      // write the line just built
      strm << line << endl;

   }  // end reallyPutRecord()

   void SP3EphData::dump(ostream& s, bool includeC) const throw()
   {
      // dump record type (PV*), sat id, and current epoch
      s << RecType << " " << static_cast<SatID>(sat).toString() << " "
         << (CivilTime(time)).asString()
         << " = "
         << (GPSWeekSecond(time)).asString();

      if(RecType != '*') {                   // not epoch line
         s << fixed << setprecision(6)
           << " X=" << setw(14) << x[0]      // XYZ
           << " Y=" << setw(14) << x[1]
           << " Z=" << setw(14) << x[2]
           << " C=" << setw(14) << clk;      // clk

         if(includeC) {
            s << " sX=" << setw(2) << sig[0]    // sigma XYZ
              << " sY=" << setw(2) << sig[1]
              << " sZ=" << setw(2) << sig[2]
              << " sC=" << setw(3) << sig[3];   // sigma clock

            if(RecType == 'P')                  // flags
              s << " " << (clockEventFlag ? "clockEvent" : "-")
                << " " << (clockPredFlag ? "clockPrediction" : "-")
                << " " << (orbitManeuverFlag ? "orbitManeuver" : "-")
                << " " << (orbitPredFlag ? "orbitPrediction" : "-");

            if(correlationFlag)                 // stddevs and correlations
               s << endl
                 << "    and E" << RecType
                 << " cXX=" << setw(4) << sdev[0]
                 << " cYY=" << setw(4) << sdev[1]
                 << " cZZ=" << setw(4) << sdev[2]
                 << " cCC=" << setw(7) << sdev[3]
                 << " cXY=" << setw(8) << correlation[0]
                 << " cXZ=" << setw(8) << correlation[1]
                 << " cXC=" << setw(8) << correlation[2]
                 << " cYZ=" << setw(8) << correlation[3]
                 << " cYC=" << setw(8) << correlation[4]
                 << " cZC=" << setw(8) << correlation[5];
         }
      }

      s << endl;

   }  // end dump()

} // namespace
