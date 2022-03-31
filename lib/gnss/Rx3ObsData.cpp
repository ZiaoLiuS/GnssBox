/** @file Rx3ObsData.cpp
 * Encapsulate RINEX 3 observation file data, including I/O.
 *
 * remove RinexDatum/ObsID/RinexObsID 
 * 
 * use TypeID as basic structrues, which are easy to read and use.
 *
 * modified by shoujian zhang
 *
 * 2020/08/03
 */

#include <fstream>
#include <algorithm>
#include "StringUtils.hpp"
#include "CivilTime.hpp"
#include "TypeID.hpp"
#include "Rx3ObsData.hpp"

using namespace std;
using namespace utilSpace;
using namespace timeSpace;
using namespace gnssSpace;

#define debug 0

namespace gnssSpace
{


   void Rx3ObsData::writeRecordVer2( std::fstream& strm ) 
      noexcept(false)
   {
         // is there anything to write?
      if( (epochFlag==0 || epochFlag==1 || epochFlag==6)
          && (numSVs==0 || stvData.empty()) ) 
          return;

      if( epochFlag>=2
          && epochFlag<=5
          && auxHeader.numberHeaderRecordsToBeWritten()==0 ) 
          return;

         // first the epoch line to 'line'
      string line;
      if(currEpoch == CommonTime::BEGINNING_OF_TIME)
         line = string(26, ' ');
      else
      {
         CivilTime civTime(currEpoch);
         line  = string(1, ' ');
         line += rightJustify(asString<short>(civTime.year),2);
         line += string(1, ' ');
         line += rightJustify(asString<short>(civTime.month),2);
         line += string(1, ' ');
         line += rightJustify(asString<short>(civTime.day),2);
         line += string(1, ' ');
         line += rightJustify(asString<short>(civTime.hour),2);
         line += string(1, ' ');
         line += rightJustify(asString<short>(civTime.minute),2);
         line += rightJustify(asString(civTime.second, 7),11);
         line += string(2, ' ');
         line += rightJustify(asString<short>(epochFlag), 1);
         line += rightJustify(asString<short>(numSVs), 3);
      }

      // write satellite ids to 'line'
      const size_t maxPrnsPerLine = 12;
      size_t satsWritten = 0;

      satTypeValueMap::const_iterator itr(stvData.begin());

      if( epochFlag==0 || epochFlag==1 || epochFlag==6 )
      {
        
         // now, let's print out 
         while( itr != stvData.end() && satsWritten < maxPrnsPerLine )
         {
            line += itr->first.toString();
            satsWritten++;
            itr++;
         }

            // add clock offset
         if( clockOffset != 0.0 )
         {
            line += string(68 - line.size(), ' ');
            line += rightJustify( asString(clockOffset, 9), 12 );
         }

            // continuation lines
         while( satsWritten != stvData.size() )
         {
            if((satsWritten % maxPrnsPerLine) == 0)
            {
               strm << line << endl;
               line  = string(32, ' ');
            }
            line += itr->first.toString();
            satsWritten++;
            itr++;
         }

      }  // End of 'if( epochFlag==0 || epochFlag==1 || ...'

         // write the epoch line
      strm << line << endl;

      // write the auxiliary header records, if any
      if( epochFlag >= 2 && epochFlag <= 5 )
      {
         try
         {
            auxHeader.writeHeaderRecords(strm);
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
      // write out data
      else if( epochFlag == 0 || epochFlag == 1 || epochFlag == 6 )
      {
         size_t i;
         const int maxObsPerLine(5);

         // loop over satellites in R3 stvData data
         for( itr = stvData.begin(); itr != stvData.end(); ++itr )
         {
            SatID sat(itr->first);               // current satellite
            string sys(string(1,sat.systemChar()));   // system

            int obsWritten(0);
            line = string("");

            typeValueMap typeObs = itr->second;
            typeValueMap typeLLI = stvDataLLI[sat];
            typeValueMap typeSSI = stvDataSSI[sat];

               // loop over R2 obstypes
            for( i=0; i<(*pHeader).R2ObsTypes.size(); i++ )
            {
                  // get the R3 obs ID from the map
               TypeID obsid;
               obsid = (*pHeader).mapSysR2toR3ObsID[sys][(*pHeader).R2ObsTypes[i]];

               if(debug)
               {
                   cout << obsid << endl;
               }

                  // need a continuation line?
               if( obsWritten != 0 && (obsWritten % maxObsPerLine) == 0 )
               {
                  strm << line << endl;
                  line = string("");
               }

                  // write the line
               if (typeObs.find(obsid) != typeObs.end())
               {
                  double data = typeObs[obsid];

                  // now, convert data from meters to cycles;
                  string typeStr = obsid.asString();


                  // carrier-phase
                  if(typeStr[0]=='L')
                  {
                      double wavelength(0.0);

                      // carrier-band
                      int n;

                      if(typeStr[1] == 'A')
                          n = 1 ;
                      else 
                          n= asInt(string(1, typeStr[1]));

                      // GLONASS
                      if(typeStr[3]== 'R')
                      {
                          Rx3ObsHeader::GLOFreqNumMap::iterator it=(*pHeader).glonassFreqNo.find(sat);
                          if( it != (*pHeader).glonassFreqNo.end())
                          {
                              // glonass slot 
                              int k = it->second;
                              wavelength = getWavelength(sat,n,k);
                          }
                          else
                          {
                              continue;
                          }
                      }
                      else
                      {
                          wavelength = getWavelength(sat, n);
                      }

                      if(wavelength==0.0) continue;

                      if(debug) 
                         cout << typeStr << "wavelength" 
                              << setprecision(12)
                              << wavelength << endl;

                      // convert cycles to meters
                      data = data/wavelength;

                  }

                  if(debug)
                  {
                      cout << data << endl;
                  }

                  line += rightJustify(asString(data, 3), 14);

                  double lli = typeLLI[obsid];
                  line += rightJustify(asString<short>(lli),1);

                  double ssi = typeSSI[obsid];
                  line += rightJustify(asString<short>(ssi),1);
               }

               obsWritten++;

            }  // End of 'for( i=0; i<(*pHeader).R2ObsTypes.size(); i++ )'

            strm << line << endl;

         }  // End of 'for( itr = stvData.begin(); itr != stvData.end();...'

      }  // Ebf of 'else if( epochFlag == 0 || epochFlag == 1 || ...'

   }  // End of function 'Rx3ObsData::writeRecordVer2()'

   void Rx3ObsData::writeRecord(std::fstream& strm) 
      noexcept(false)
   {

      // warning:
      // you must write the header firstly, outside this class

         // is there anything to write?
      if( (epochFlag == 0 || epochFlag == 1 || epochFlag == 6)
          && (numSVs==0 || stvData.empty()))
      {
         return;
      }

      if(debug)
      {
          cout << (*pHeader).version << endl;
      }

         // call the version for RINEX ver 2
      if((*pHeader).version < 3)
      {
         try
         {
            writeRecordVer2(strm);
         }
         catch(Exception& e)
         {
            RETHROW(e);
         }
         return;
      }

      string line;

         // first the epoch line
      line  = ">";
      line += writeTime(currEpoch);
      line += string(2, ' ');
      line += rightJustify(asString<short>(epochFlag), 1);
      line += rightJustify(asString<short>(numSVs   ), 3);
      line += string(6, ' ');
      if(clockOffset != 0.0) // optional data; need to test for its existence
         line += rightJustify(asString(clockOffset, 12), 15);

      strm << line << endl;
      line.erase();

      if(epochFlag == 0 || epochFlag == 1 || epochFlag == 6)
      {
         // now, let's print out 
         satTypeValueMap::const_iterator itr = stvData.begin();
         while(itr != stvData.end())
         {
            line = itr->first.toString();

            SatID sat(itr->first);               // current satellite
            string sys(string(1,sat.systemChar()));   // system

            // get the order of the obs types.
            std::vector<TypeID> typeVec = (*pHeader).mapObsTypes[sys];

            typeValueMap typeObs = itr->second;
            typeValueMap typeLLI = stvDataLLI[sat];
            typeValueMap typeSSI = stvDataSSI[sat];

            for(auto typeIt=typeVec.begin(); typeIt!=typeVec.end(); typeIt++)
            {

                double data = typeObs[(*typeIt)] ;

                // now, convert data from meters to cycles;
                string typeStr = (*typeIt).asString();

                // carrier-phase
                if(typeStr[0]=='L')
                {
                    double wavelength(0.0);

                    int n;

                    if(typeStr[1] == 'A')
                        n = 1 ;
                    else 
                        n= asInt(string(1, typeStr[1]));

                    // GLONASS
                    if(typeStr[3]== 'R')
                    {
                        Rx3ObsHeader::GLOFreqNumMap::iterator it=(*pHeader).glonassFreqNo.find(sat);
                        if( it != (*pHeader).glonassFreqNo.end())
                        {
                            // glonass slot 
                            int k = it->second;
                            wavelength = getWavelength(sat,n,k);
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        wavelength = getWavelength(sat, n);
                    }

                    if(wavelength==0.0) continue;

                    if(debug) 
                    cout << typeStr << "wavelength" 
                           << setprecision(12)
                        << wavelength << endl;

                    // convert cycles to meters
                    data = data/wavelength;

                }

                // F14.3 data
                line += rightJustify(asString(data, 3), 14);

                double lli = typeLLI[(*typeIt)];
                line += rightJustify(asString<short>(lli),1);

                double ssi = typeSSI[(*typeIt)];
                line += rightJustify(asString<short>(ssi),1);
            }

               // write the data line out
            strm << line << endl;
            line.erase();

            itr++;
         } // end loop over sats and data
      }

         // write the auxiliary (*pHeader) records, if any
      else if(epochFlag >= 2 && epochFlag <= 5)
      {
         try
         {
            auxHeader.writeHeaderRecords(strm);
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

   }   // end Rx3ObsData::writeRecord


   void Rx3ObsData::readRecordVer2(std::fstream& strm)
      noexcept(false)
   {
      static CommonTime previousTime(CommonTime::BEGINNING_OF_TIME);

         // get the epoch line and check
      string line;
      while(line.empty())        // ignore blank lines in place of epoch lines
      {
          getline(strm,line);

          if(strm.eof())
          {
              EndOfFile err("EOF encountered!");
              THROW(err);
          }
      }

      stripTrailing(line);

      if(line.size()>80 || line[0] != ' ' || line[3] != ' ' || line[6] != ' ')
      {
         FFStreamError e("Bad epoch line: >" + line + "<");
         THROW(e);
      }

         // process the epoch line, including SV list and clock bias
      epochFlag = asInt(line.substr(28,1));
      if((epochFlag < 0) || (epochFlag > 6))
      {
         FFStreamError e("Invalid epoch flag: " + asString(epochFlag));
         THROW(e);
      }

         // Not all epoch flags are required to have a time.
         // Specifically, 0,1,5,6 must have an epoch time; it is
         // optional for 2,3,4.  If there is and epoch time, parse it
         // and load it in the member "currEpoch".
         // If epoch flag=0, 1, 5, or 6 and there is NO epoch time, then throw.
         // If epoch flag=2, 3, or 4 and there is no epoch time,
         // use the time of the previous record.
      bool noEpochTime = (line.substr(0,26) == string(26, ' '));
      if(noEpochTime && (epochFlag==0 || epochFlag==1 ||
                         epochFlag==5 || epochFlag==6 ))
      {
         FFStreamError e("Required epoch time missing: " + line);
         THROW(e);
      }
      else if(noEpochTime)
      {
         currEpoch = CommonTime::BEGINNING_OF_TIME;
      }
      else
      {
         try
         {
               // check if the spaces are in the right place - an easy
               // way to check if there's corruption in the file
            if((line[0] != ' ') || (line[3] != ' ') || (line[6] != ' ') ||
               (line[9] != ' ') || (line[12] != ' ') || (line[15] != ' '))
            {
               FFStreamError e("Invalid time format");
               THROW(e);
            }

               // if there's no time, just use a bad time
            if(line.substr(0,26) == string(26, ' '))
            {
               currEpoch = CommonTime::BEGINNING_OF_TIME;
               //currEpoch = previousTime; ??
            }
            else
            {
               int year, month, day, hour, min;
               double sec;
               int yy = (static_cast<CivilTime>((*pHeader).firstObs)).year/100;
               yy *= 100;

               year  = asInt(   line.substr(1,  2 ));
               month = asInt(   line.substr(4,  2 ));
               day   = asInt(   line.substr(7,  2 ));
               hour  = asInt(   line.substr(10, 2 ));
               min   = asInt(   line.substr(13, 2 ));
               sec   = asDouble(line.substr(15, 11));

                  // Real Rinex has epochs 'yy mm dd hr 59 60.0'
                  // surprisingly often....
               double ds(0);
               if(sec >= 60.)
               {
                  ds=sec;
                  sec=0.0;
               }
               CivilTime rv(yy+year, month, day, hour, min, sec,
                            TimeSystem::GPS);
               if(ds != 0) rv.second += ds;

               currEpoch = rv.convertToCommonTime();
            }
         }
            // string exceptions for substr are caught here
         catch(std::exception &e)
         {
            FFStreamError err("std::exception: " + string(e.what()));
            THROW(err);
         }
         catch(Exception& e)
         {
            FFStreamError err(e);
            THROW(err);
         }

      }

         // number of satellites
      numSVs = asInt(line.substr(29,3));

         // clock offset
      if(line.size() > 68 )
         clockOffset = asDouble(line.substr(68, 12));
      else
         clockOffset = 0.0;

         // Read the observations ...
      if(epochFlag==0 || epochFlag==1 || epochFlag==6)
      {
            // first read the SatIDs off the epoch line
         int isv, ndx, line_ndx;
         string satsys;
         SatID sat;
         vector<SatID> satIndex(numSVs);
         for(isv=1, ndx=0; ndx<numSVs; isv++, ndx++)
         {
            if(!(isv % 13))
            {  
               // get a new continuation line
               getline(strm, line);
               if(strm.eof())
               {
                   EndOfFile err("EOF encountered!");
                   THROW(err);
               }

               if(debug)
                   cout << line <<endl;

               stripTrailing(line);
               isv = 1;
               if(line.size() > 80)
               {
                  FFStreamError err("Invalid line size:" +
                                    asString(line.size()));
                  THROW(err);
               }
            }

               // read the sat id
            try
            {
               sat = SatID(line.substr(30+isv*3-1, 3));
               satIndex[ndx] = sat;
            }
            catch (Exception& e)
            {
               FFStreamError ffse(e);
               THROW(ffse);
            }
         }  // end loop over numSVs

         // number of R2 OTs in (*pHeader)
         unsigned numObs((*pHeader).R2ObsTypes.size());

         ///>> clear stvData firstly!
         stvData.clear();
         stvDataLLI.clear();
         stvDataSSI.clear();

            // loop over all sats, reading obs data
         for(isv=0; isv < numSVs; isv++)
         {
            sat = satIndex[isv];                   // sat for this data
            satsys = asString(sat.systemChar());   // system for this sat

            typeValueMap typeObs;
            typeValueMap typeLLI;
            typeValueMap typeSSI;

               // loop over data in the line
            for(ndx=0, line_ndx=0; ndx < numObs; ndx++, line_ndx++)
            {
               if(! (line_ndx % 5))
               {  
                  // get a new line
                  getline(strm, line);
                  if(strm.eof())
                  {
                      EndOfFile err("EOF encountered!");
                      THROW(err);
                  }
                  if(debug)
                      cout << line << endl;

                  stripTrailing(line);
                  line.resize(80, ' ');            // pad just in case
                  line_ndx = 0;
                  if(line.size() > 80)
                  {
                     FFStreamError err("Invalid line size:" +
                                       asString(line.size()));
                     THROW(err);
                  }
               }

                  // does this R2 OT map into a valid R3 ObsID?
               string R2ot((*pHeader).R2ObsTypes[ndx]);
               string R3ot(asString((*pHeader).mapSysR2toR3ObsID[satsys][R2ot]));

               if(R3ot != string("   "))
               {
                  string str(line.substr(line_ndx*16, 16));

                  // observation
                  string tmpStr = str.substr(0, 14);
                  double data = asDouble(tmpStr);

                  // carrier-phase
                  if(R3ot[0]=='L')
                  {
                      double wavelength(0.0);

                      // carrier-band
                      int n;

                      if(R3ot[1] == 'A')
                          n = 1 ;
                      else 
                          n= asInt(string(1, R3ot[1]));


                      // GLONASS
                      if(R3ot[3]== 'R')
                      {
                          Rx3ObsHeader::GLOFreqNumMap::iterator it=(*pHeader).glonassFreqNo.find(sat);
                          if( it != (*pHeader).glonassFreqNo.end())
                          {
                              // glonass slot 
                              int k = it->second;
                              if(debug)
                                  cout << "glonass slot:" << k << endl;

                              wavelength = getWavelength(sat,n,k);

                              if(n == 1)
                              {
                                  typeObs[TypeID::wavelengthL1R] = wavelength;
                              }
                              else if(n == 2 )
                              {
                                  typeObs[TypeID::wavelengthL2R] = wavelength;
                              }
                          }
                          else
                          {
                              continue;
                          }
                      }
                      else
                      {
                          wavelength = getWavelength(sat, n);
                      }

                      if(wavelength == 0.0) continue;

                      if(debug) 
                         cout << R3ot << "wavelength" 
                              << setprecision(12)
                              << wavelength << endl;

                      // convert cycles to meters
                      data = data * wavelength;
                  }

                  // LLI
                  tmpStr = str.substr(14, 1);
                  double lli = asInt(tmpStr);

                  // SSI
                  tmpStr = str.substr(15, 1);
                  double ssi = asInt(tmpStr);

                  // ObsType
                  TypeID obsType(R3ot);

                  typeObs[obsType] = data;
                  typeLLI[obsType] = lli;
                  typeSSI[obsType] = ssi;
               }
            }

            // insert current satellite data 
            stvData[sat] = typeObs;
            stvDataLLI[sat] = typeLLI;
            stvDataSSI[sat] = typeSSI;

         }  // end loop over sats to read obs data

      }

         // ... or the auxiliary (*pHeader) information
      else if(numSVs > 0)
      {
         auxHeader.clear();
         for(int i=0; i<numSVs; i++)
         {

            // read a line
            getline(strm, line);
            if(strm.eof())
            {
                EndOfFile err("EOF encountered!");
                THROW(err);
            }
            if(debug)
                cout << line << endl;

            stripTrailing(line);
            try
            {
               auxHeader.parseHeaderRecord(line);
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
      }

   }  // end void Rx3ObsData::readRecordVer2(std::fstream& strm)


   void Rx3ObsData::readRecord(std::fstream& strm)
      noexcept(false)
   {

/*
         // If the (*pHeader) hasn't been read, read it.
      if(!(*pHeader)Read) 
      {
  
          strm.clear();//必须先调用这个，清除flag
          strm.seekg(0);//才能用这个设定到文件开头
          strm >> (*pHeader);
  
          (*pHeader)Read = true;
      }
*/
      if(pHeader==NULL)
      {
          cerr << " Rx3ObsData:: you must read rinex header and set into this class firstly! " << endl;
          exit(-1);
      }

         // call the version for RINEX ver 2
      if( (*pHeader).version < 3)
      {
         try
         {
            readRecordVer2(strm);
         }
         catch(EndOfFile& e)
         {
            RETHROW(e);
         }
         catch(Exception& e)
         {
            RETHROW(e);
         }

         // return
         return;
      }

      string line;
      
      // read a line
      getline(strm, line);
      if(strm.eof())
      {
          EndOfFile err("EOF encountered!");
          THROW(err);
      }
      if(debug)
          cout << line << endl;

      stripTrailing(line, " ");

         // Check and parse the epoch line -----------------------------------
         // Check for epoch marker ('>') and following space.
      if(line[0] != '>' || line[1] != ' ')
      {
         FFStreamError e("Bad epoch line: >" + line + "<");
         THROW(e);
      }

      epochFlag = asInt(line.substr(31,1));
      if(epochFlag < 0 || epochFlag > 6)
      {
         FFStreamError e("Invalid epoch flag: " + asString(epochFlag));
         THROW(e);
      }

      TimeSystem timeSys = (*pHeader).firstObs.timeSystem;
      currEpoch = parseTime(line, (*pHeader), timeSys);

      numSVs = asInt(line.substr(32,3));

      if(line.size() > 41)
         clockOffset = asDouble(line.substr(41,15));
      else
         clockOffset = 0.0;

      // Read the observations: SV ID and data ----------------------------
      if(epochFlag == 0 || epochFlag == 1 || epochFlag == 6)
      {

         stvData.clear();
         stvDataLLI.clear();
         stvDataSSI.clear();


         vector<SatID> satIndex(numSVs);
         for(int isv = 0; isv < numSVs; isv++)
         {
            getline(strm, line);
            if(strm.eof())
            {
                EndOfFile err("EOF encountered!");
                THROW(err);
            }

            stripTrailing(line, " ");

            if(debug)
                cout<< line << endl;

            // get the SV ID
            try
            {
               satIndex[isv] = SatID(line.substr(0,3));
            }
            catch (Exception& e)
            {
               FFStreamError ffse(e);
               THROW(ffse);
            }

            SatID sat = SatID(satIndex[isv]);

               // get the # data items (# entries in ObsType map of
               // maps from header)
            string gnss = asString(satIndex[isv].systemChar());
            int size = (*pHeader).mapObsTypes[gnss].size();

               // Some receivers leave blanks for missing Obs (which
               // is OK by RINEX 3).  If the last Obs are the ones
               // missing, it won't necessarily be padded with spaces,
               // so the parser will break.  This adds the padding to
               // let the parser do its job and interpret spaces as
               // zeroes.
            size_t minSize = 3 + 16*size;
            if(line.size() < minSize)
               line += string(minSize-line.size(), ' ');

            // get the data (# entries in ObsType map of maps from header)
            typeValueMap typeObs;
            typeValueMap typeLLI;
            typeValueMap typeSSI;
            for(int i = 0; i < size; i++)
            {
               size_t pos = 3 + 16*i;

               std::string str = line.substr(pos,16);

               // ObsType
               string R3ot = (*pHeader).mapObsTypes[gnss][i].asString();  

               // observation
               string tmpStr = str.substr(0, 14);

               double data = asDouble(tmpStr);

               // carrier-phase
               if(R3ot[0]=='L')
               {
                   double wavelength(0.0);

                   // carrier-band
                   int n;

                   if(R3ot[1] == 'A')
                       n = 1 ;
                   else 
                       n= asInt(string(1, R3ot[1]));


                   // GLONASS
                   if(R3ot[3]== 'R')
                   {
                       Rx3ObsHeader::GLOFreqNumMap::iterator it=(*pHeader).glonassFreqNo.find(sat);
                       if( it != (*pHeader).glonassFreqNo.end())
                       {
                           // glonass slot 
                           int k = it->second;
                           if(debug)
                               cout << "glonass slot:" << k << endl;

                           wavelength = getWavelength(sat,n,k);


                           if(n == 1)
                           {
                               typeObs[TypeID::wavelengthL1R] = wavelength;
                           }
                           else if(n == 2 )
                           {
                               typeObs[TypeID::wavelengthL2R] = wavelength;
                           }
                       }
                       else
                       {
                           continue;
                       }
                   }
                   else
                   {
                       wavelength = getWavelength(sat, n);
                   }

                   if(wavelength==0.0) continue;

                   if(debug) 
                   cout << R3ot << " wavelength" 
                       << setprecision(12) 
                       << wavelength << endl;

                   // convert cycles to meters
                   data = data * wavelength;
               }

               // LLI
               tmpStr = str.substr(14, 1);
               double lli = asInt(tmpStr);

               // SSI
               tmpStr = str.substr(15, 1);
               double ssi = asInt(tmpStr);


               TypeID obsType(R3ot);
               typeObs[obsType] = data;
               typeLLI[obsType] = lli;
               typeSSI[obsType] = ssi;

            }

            // insert current satellite data into stvData
            stvData[satIndex[isv]] = typeObs;
            stvDataLLI[satIndex[isv]] = typeLLI;
            stvDataSSI[satIndex[isv]] = typeSSI;

         }

      }

         // ... or the auxiliary header information
      else if(numSVs > 0)
      {
         auxHeader.clear();
         for(int i = 0; i < numSVs; i++)
         {
            getline(strm, line);
            if(strm.eof())
            {
                EndOfFile err("EOF encountered!");
                THROW(err);
            }
            stripTrailing(line);

            if(debug)
                cout << line << endl;
            try
            {
               auxHeader.parseHeaderRecord(line);
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
      }

      return;

   } // end of readRecord()


   CommonTime Rx3ObsData::parseTime( const string& line,
                                          const Rx3ObsHeader& hdr,
                                          const TimeSystem& ts) const
      noexcept(false)
   {
      try
      {
            // check if the spaces are in the right place - an easy
            // way to check if there's corruption in the file
         if( (line[ 1] != ' ') || (line[ 6] != ' ') || (line[ 9] != ' ') ||
             (line[12] != ' ') || (line[15] != ' ') || (line[18] != ' ') ||
             (line[29] != ' ') || (line[30] != ' '))
         {
            FFStreamError e("Invalid time format");
            THROW(e);
         }

            // if there's no time, just return a bad time
         if(line.substr(2,27) == string(27, ' '))
            return CommonTime::BEGINNING_OF_TIME;

         int year, month, day, hour, min;
         double sec;

         year  = asInt(   line.substr( 2,  4));
         month = asInt(   line.substr( 7,  2));
         day   = asInt(   line.substr(10,  2));
         hour  = asInt(   line.substr(13,  2));
         min   = asInt(   line.substr(16,  2));
         sec   = asDouble(line.substr(19, 11));

            // Real Rinex has epochs 'yy mm dd hr 59 60.0' surprisingly often.
         double ds = 0;
         if(sec >= 60.)
         {
            ds = sec;
            sec = 0.0;
         }

         CommonTime rv = CivilTime(year,month,day,hour,min,sec).convertToCommonTime();
         if(ds != 0) rv += ds;

         rv.setTimeSystem(ts);

         return rv;
      }
         // string exceptions for substr are caught here
      catch (std::exception &e)
      {
         FFStreamError err("std::exception: " + string(e.what()));
         THROW(err);
      }
      catch (Exception& e)
      {
         FFStreamError err(e);
         THROW(err);
      }
   }  // end parseTime

   string Rx3ObsData::writeTime(const CommonTime& ct) const
      noexcept(false)
   {
      if(ct == CommonTime::BEGINNING_OF_TIME)
         return string(26, ' ');

      CivilTime civtime(ct);
      string line;

      line  = string(1, ' ');
      line += rightJustify(asString<short>(civtime.year    ), 4);
      line += string(1, ' ');
      line += rightJustify(asString<short>(civtime.month   ), 2, '0');
      line += string(1, ' ');
      line += rightJustify(asString<short>(civtime.day     ), 2, '0');
      line += string(1, ' ');
      line += rightJustify(asString<short>(civtime.hour    ), 2, '0');
      line += string(1, ' ');
      line += rightJustify(asString<short>(civtime.minute  ), 2, '0');
      line += rightJustify(asString       (civtime.second,7),11);

      return line;
   }  // end writeTime

   string Rx3ObsData::timeString() const noexcept(false)
   {
      return writeTime(currEpoch);
   }

   void Rx3ObsData::dumpRAW(ostream& s, int detail) const
   {
      if(stvData.empty())
         return;

      s << "Dump of Rx3ObsData - time: " << writeTime(currEpoch)
        << " epochFlag: " << " " << epochFlag
        << " numSVs: " << numSVs
        << fixed << setprecision(9) << " clk offset: " << clockOffset << endl;

      if(epochFlag == 0 || epochFlag == 1)
      {
          // dump 
          for(auto jt=stvData.begin(); jt != stvData.end(); jt++)
          {
             SatID sat(jt->first);
             typeValueMap tvData = jt->second;
             SysTypesMap::const_iterator sysIt 
                 = (*pHeader).mapObsTypes.find(sat.toString().substr(0,1));
             if( sysIt !=(*pHeader).mapObsTypes.end())
             {
                vector<TypeID> types = sysIt->second;
                s << " " << sat.toString() << fixed << setprecision(3);

                for(auto vt=types.begin(); vt!= types.end(); vt++)
                {
                    if(detail)
                    {
                        s << " " << (*vt) ;
                    }

                    s << " " << setw(13)  << tvData((*vt));
                }

                s << endl;
             }

          }
      }
      else
      {
         s << "aux. header info:\n";
         auxHeader.dump(s);
      }
   }

   void Rx3ObsData::readRecordByTime(fstream &strm, CommonTime current) noexcept(false) {
       double Tolerance=5;
       CommonTime roverTime=current;
       CommonTime lastEpoch=currEpoch;
       while(true)
       {
           if(abs(lastEpoch-roverTime)<=Tolerance)
           {
               break;
           }
           else if(roverTime-lastEpoch>Tolerance)
           {
               readRecord(strm);
           }
           else
           {
               /// 同步失败
               bool flag = 1;
               throw flag;
           }
           lastEpoch = currEpoch;


       }


   }


    // end dump

}
