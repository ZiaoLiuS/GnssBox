/**
 * @file Rx3ObsHeader.cpp 
 * Encapsulate header of Rinex observation file, including I/O
 *
 * 2020/07/05
 *
 * modify for rinex 3.02, in which c1x should be replaced with c2x.
 *
 * author
 *
 * shoujian
 *  
 */

#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include <string.h>

#include "StringUtils.hpp"
#include "SystemTime.hpp"
#include "Exception.hpp"
#include "Rx3ObsHeader.hpp"

using namespace std;
using namespace utilSpace;
using namespace timeSpace;
using namespace gnssSpace;


namespace gnssSpace
{
    int Rx3ObsHeader::debug = 0;
    
    const string Rx3ObsHeader::hsVersion           = "RINEX VERSION / TYPE";
    const string Rx3ObsHeader::hsRunBy             = "PGM / RUN BY / DATE";
    const string Rx3ObsHeader::hsComment           = "COMMENT";
    const string Rx3ObsHeader::hsMarkerName        = "MARKER NAME";
    const string Rx3ObsHeader::hsMarkerNumber      = "MARKER NUMBER";
    const string Rx3ObsHeader::hsMarkerType        = "MARKER TYPE";
    const string Rx3ObsHeader::hsObserver          = "OBSERVER / AGENCY";
    const string Rx3ObsHeader::hsReceiver          = "REC # / TYPE / VERS";
    const string Rx3ObsHeader::hsAntennaType       = "ANT # / TYPE";
    const string Rx3ObsHeader::hsAntennaPosition   = "APPROX POSITION XYZ";
    const string Rx3ObsHeader::hsAntennaDeltaHEN   = "ANTENNA: DELTA H/E/N";
    const string Rx3ObsHeader::hsAntennaDeltaXYZ   = "ANTENNA: DELTA X/Y/Z";
    const string Rx3ObsHeader::hsAntennaPhaseCtr   = "ANTENNA: PHASECENTER";
    const string Rx3ObsHeader::hsAntennaBsightXYZ  = "ANTENNA: B.SIGHT XYZ";
    const string Rx3ObsHeader::hsAntennaZeroDirAzi = "ANTENNA: ZERODIR AZI";
    const string Rx3ObsHeader::hsAntennaZeroDirXYZ = "ANTENNA: ZERODIR XYZ";
    const string Rx3ObsHeader::hsCenterOfMass      = "CENTER OF MASS: XYZ";
    const string Rx3ObsHeader::hsNumObs            = "# / TYPES OF OBSERV";
    const string Rx3ObsHeader::hsSystemNumObs      = "SYS / # / OBS TYPES";
    const string Rx3ObsHeader::hsWaveFact          = "WAVELENGTH FACT L1/2";
    const string Rx3ObsHeader::hsSigStrengthUnit   = "SIGNAL STRENGTH UNIT";
    const string Rx3ObsHeader::hsInterval          = "INTERVAL";
    const string Rx3ObsHeader::hsFirstTime         = "TIME OF FIRST OBS";
    const string Rx3ObsHeader::hsLastTime          = "TIME OF LAST OBS";
    const string Rx3ObsHeader::hsReceiverOffset    = "RCV CLOCK OFFS APPL";
    const string Rx3ObsHeader::hsSystemDCBSapplied = "SYS / DCBS APPLIED";
    const string Rx3ObsHeader::hsSystemPCVSapplied = "SYS / PCVS APPLIED";
    const string Rx3ObsHeader::hsSystemScaleFac    = "SYS / SCALE FACTOR";
    const string Rx3ObsHeader::hsSystemPhaseShift  = "SYS / PHASE SHIFT";
    const string Rx3ObsHeader::hsGlonassSlotFreqNo = "GLONASS SLOT / FRQ #";
    const string Rx3ObsHeader::hsGlonassCodPhsBias = "GLONASS COD/PHS/BIS";
    const string Rx3ObsHeader::hsLeapSeconds       = "LEAP SECONDS";
    const string Rx3ObsHeader::hsNumSats           = "# OF SATELLITES";
    const string Rx3ObsHeader::hsPrnObs            = "PRN / # OF OBS";
    const string Rx3ObsHeader::hsEoH               = "END OF HEADER";

    Rx3ObsHeader::Rx3ObsHeader() 
        : PisY(false)
    { clear(); }


    void Rx3ObsHeader::clear()
    {
        R2ObsTypes.clear();
        mapSysR2toR3ObsID.clear();
        version = 3.03;
        fileType = "O";          // observation data
        fileSys = "G";           // GPS only by default
        preserveVerType = false; // let the write methods chose the above
        fileSysSat = SatID(-1,SatelliteSystem::GPS);
        fileProgram.clear();
        fileAgency.clear();
        date.clear();
        preserveDate = false;
        commentList.clear();
        markerName.clear();
        markerNumber.clear();
        markerType.clear();
        observer.clear();
        agency.clear();
        recNo.clear();
        recType.clear();
        recVers.clear();
        antNo.clear();
        antType.clear();
        antennaPosition = Triple();
        antennaDeltaHEN = Triple();
        antennaDeltaXYZ = Triple();
        antennaSatSys.clear();
        antennaObsCode.clear();
        antennaPhaseCtr = Triple();
        antennaBsightXYZ = Triple();
        antennaZeroDirAzi = 0.;
        antennaZeroDirXYZ = Triple();
        centerOfMass = Triple();
        mapObsTypes.clear();
        wavelengthFactor[0] = wavelengthFactor[1] = 1;
        extraWaveFactList.clear();
        sigStrengthUnit.clear();
        interval = 0.;
        firstObs = CivilTime();
        lastObs = CivilTime();
        receiverOffset = 0;
        infoDCBS.clear();
        infoPCVS.clear();
        sysSfacMap.clear();
        sysPhaseShift.clear();
        glonassFreqNo.clear();
        glonassCodPhsBias.clear();
        leapSeconds = 0;
        numSVs = 0;
        numObsForSat.clear();
        obsTypeList.clear();
        valid  = 0;
        validEoH = false;

        sysPhaseShiftObsID = TypeID();
        satSysTemp.clear();
        satSysPrev.clear();
        numObs = 0;
        numObsPrev = 0;
        lastPRN = SatID();
        factor = 0;
        factorPrev = 0;
    }


    void Rx3ObsHeader::reallyPutRecord(std::fstream& strm) 
        noexcept(false)
    {
        unsigned long allValid;
        if     (version == 3.00)  allValid = allValid30;
        else if(version == 3.01)  allValid = allValid301;
        else if(version == 3.02)  allValid = allValid302;
        else if(version == 3.03)  allValid = allValid303;
        else if(version == 3.04)  allValid = allValid304;
        else if(version == 3.05)  allValid = allValid305;
        else if(version <  3)     allValid = allValid2;
        else
        {
            RinexObsHeaderError err("Unknown RINEX version: " + asString(version,2));
            err.addText("Make sure to set the version correctly.");
            exit(-1);
        }

        if((valid & allValid) != allValid)
        {
            ostringstream msg;
            msg << endl;
            msg << "Version = " << version << hex << endl;
            if(version == 3.05)
                msg << "allValid305 = 0x" << setw(8) << nouppercase << allValid305 << endl;
            else if(version == 3.04)
                msg << "allValid304 = 0x" << setw(8) << nouppercase << allValid304 << endl;
            else if(version == 3.03)
                msg << "allValid303 = 0x" << setw(8) << nouppercase << allValid303 << endl;
            else if(version == 3.02)
                msg << "allValid302 = 0x" << setw(8) << nouppercase << allValid302 << endl;
            else if(version == 3.01)
                msg << "allValid301 = 0x" << setw(8) << nouppercase << allValid301 << endl;
            else if(version == 3.00)
                msg << " allValid30 = 0x" << setw(8) << nouppercase << allValid30 << endl;
            else
                msg << "  allValid2 = 0x" << setw(8) << nouppercase << allValid2 << endl;
            msg << "      valid = 0x" << setw(8) << nouppercase << valid << endl;
            msg << "Version         " << setw(8) << (valid & validVersion        ) << endl;
            msg << "Run By          " << setw(8) << (valid & validRunBy          ) << endl;
            msg << "Marker Name     " << setw(8) << (valid & validMarkerName     ) << endl;
            //msg << "Marker Type     " << setw(8) << (valid & validMarkerType     ) << endl;
            msg << "Observer        " << setw(8) << (valid & validObserver       ) << endl;
            msg << "Receiver        " << setw(8) << (valid & validReceiver       ) << endl;
            msg << "Antenna Type    " << setw(8) << (valid & validAntennaType    ) << endl;
            msg << "Antenna DHEN    " << setw(8) << (valid & validAntennaDeltaHEN) << endl;
            if(version <  3)
                msg << "# Obs Type      " << setw(8) << (valid & validNumObs) << endl;
            if(version >= 3)
                msg << "Sys Obs Type    " << setw(8) << (valid & validSystemNumObs  ) << endl;
            if(version <  3)
                msg << "Wave Fact       " << setw(8) << (valid & validWaveFact) << endl;
            if(version >= 3.01)
                msg << "Sys Phs Shft    " << setw(8) << (valid & validSystemPhaseShift)<< endl;
            if(version >= 3.01)
                msg << "GLO Freq No     " << setw(8) << (valid & validGlonassSlotFreqNo  ) << endl;
            if(version >= 3.02)
                msg << "GLO Cod-Phs Bias" << setw(8) << (valid & validGlonassCodPhsBias) << endl;
            msg << "Interval        " << setw(8) << (valid & validInterval       ) << endl;
            msg << "First Time      " << setw(8) << (valid & validFirstTime      ) << endl;
            msg << "End Header      " << setw(8) << (validEoH ? "true":"false"   );    // no endl

            RinexObsHeaderError err("Incomplete or invalid header.");
            err.addText("Make sure you set all header valid bits for all of the available data.");
            err.addText(msg.str());
            THROW(err);
        }

        try
        {
            writeHeaderRecords(strm);
        }
        catch(RinexObsHeaderError& e)
        {
            RETHROW(e);
        }
        catch(StringException& e)
        {
            RETHROW(e);
        }

    }  // end reallyPutRecord


      // This function computes the number of valid header records
      // which writeHeaderRecords will write.
      // NB not used in Rinex3Obs....
    int Rx3ObsHeader::numberHeaderRecordsToBeWritten(void) const throw()
    {
        int n = 0;

        if(valid & validVersion          ) n++;
        if(valid & validRunBy            ) n++;
        if(valid & validComment          ) n += commentList.size();
        if(valid & validMarkerName       ) n++;
        if(valid & validMarkerNumber     ) n++;
        if(version >= 3 && (valid & validMarkerType)) n++;
        if(valid & validObserver         ) n++;
        if(valid & validReceiver         ) n++;
        if(valid & validAntennaType      ) n++;
        if(valid & validAntennaPosition  ) n++;
        if(valid & validAntennaDeltaHEN  ) n++;
        if(version >= 3 && (valid & validAntennaDeltaXYZ)) n++;
        if(version >= 3 && (valid & validAntennaPhaseCtr)) n++;
        if(version >= 3 && (valid & validAntennaBsightXYZ)) n++;
        if(version >= 3 && (valid & validAntennaZeroDirAzi)) n++;
        if(version >= 3 && (valid & validAntennaZeroDirXYZ)) n++;
        if(version >= 3 && (valid & validCenterOfMass)) n++;
        if(version < 3 && (valid & validNumObs) && R2ObsTypes.size() != 0)
            n += 1 + (R2ObsTypes.size()-1)/9;
        if(version >= 3 && (valid & validSystemNumObs) && numObs != 0)
            n += 1 + (numObs-1)/9;
        if(version < 3 && (valid & validWaveFact))
        {
            n++;
            if(extraWaveFactList.size() != 0) n += extraWaveFactList.size();
        }
        if(version >= 3 && (valid & validSigStrengthUnit)) n++;
        if(valid & validInterval         ) n++;
        if(valid & validFirstTime        ) n++;
        if(valid & validLastTime         ) n++;
        if(valid & validReceiverOffset   ) n++;
        if(version >= 3 && (valid & validSystemDCBSapplied)) n++;
        if(version >= 3 && (valid & validSystemPCVSapplied)) n++;
        if(version >= 3 && (valid & validSystemScaleFac)) n++;
        if(version >= 3.01 && (valid & validSystemPhaseShift)) n++;   // one per system at least
        if(version >= 3.01 && (valid & validGlonassSlotFreqNo)) n++;  // TODO: continuation lines...
        if(version >= 3.02 && (valid & validGlonassCodPhsBias)) n++;
        if(valid & validLeapSeconds      ) n++;
        if(valid & validNumSats          ) n++;
        if(valid & validPrnObs           )
            n += numObsForSat.size() * (1+numObsForSat.begin()->second.size()/9);
        if(validEoH                      ) n++;

        return n;

    }  // end numberHeaderRecordsToBeWritten


      // This function writes all valid header records.
    void Rx3ObsHeader::writeHeaderRecords(std::fstream& strm) const
        noexcept(false)
    {

        string line;

        if(valid & validVersion)
        {
            line  = rightJustify(asString(version,2), 9);
            line += string(11, ' ');

            if((fileType[0] != 'O') && (fileType[0] != 'o'))
            {
                RinexObsHeaderError err("File type is not Observation: " + fileType);
                THROW(err);
            }

            if (preserveVerType)
            {
                line += leftJustify(fileType, 20);
                line += leftJustify(fileSys, 20);
            }
            else
            {
                if(fileSysSat.system == SatelliteSystem::Unknown)
                {
                    RinexObsHeaderError err("Invalid satellite system");
                    THROW(err);
                }

                line += leftJustify(string("OBSERVATION DATA"), 20);
                string str;
                if(fileSysSat.system == SatelliteSystem::Mixed)
                    str = "MIXED";
                else
                {
                    SatID sat(fileSysSat);
                    str = sat.systemChar();
                    str = str + " (" + sat.systemString() + ")";
                }
                line += leftJustify(str, 20);
            }
            line += hsVersion;
            strm << line << endl;
        }
        if(valid & validRunBy)
        {
            line  = leftJustify(fileProgram, 20);
            line += leftJustify(fileAgency , 20);
            if (preserveDate)
            {
                line += leftJustify(date, 20);
            }
            else
            {
                SystemTime sysTime;
                string curDate;
                curDate = sysTime.convertToCommonTime().asString();
                line += leftJustify(curDate, 20);
            }
            line += hsRunBy;
            strm << line << endl;
        }
        if(valid & validComment)
        {
            vector<string>::const_iterator itr = commentList.begin();
            while (itr != commentList.end())
            {
                line  = leftJustify((*itr), 60);
                line += hsComment;
                strm << line << endl;
                itr++;
            }
        }
        if(!R2DisambiguityMap.empty())
        {
            DisAmbMap::const_iterator itr = R2DisambiguityMap.begin();
            while (itr != R2DisambiguityMap.end())
            {
                line  = leftJustify(itr->first.substr(0,1)
                                + " Obs Type " + itr->first.substr(1)
                                + " originated from  " + itr->second, 60);
                line += hsComment;
                strm << line << endl;
                itr++;
            }
        }
        if(valid & validMarkerName)
        {
            line  = leftJustify(markerName, 60);
            line += hsMarkerName;
            strm << line << endl;
        }
        if(valid & validMarkerNumber)
        {
            line  = leftJustify(markerNumber, 20);
            line += string(40, ' ');
            line += hsMarkerNumber;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validMarkerType))
        {
            line  = leftJustify(markerType, 20);
            line += string(40, ' ');
            line += hsMarkerType;
            strm << line << endl;
        }
        if(valid & validObserver)
        {
            line  = leftJustify(observer, 20);
            line += leftJustify(agency  , 40);
            line += hsObserver;
            strm << line << endl;
        }
        if(valid & validReceiver)
        {
            line  = leftJustify(recNo  , 20);
            line += leftJustify(recType, 20);
            line += leftJustify(recVers, 20);
            line += hsReceiver;
            strm << line << endl;
        }
        if(valid & validAntennaType)
        {
            line  = leftJustify(antNo  , 20);
            line += leftJustify(antType, 20);
            line += string(20, ' ');
            line += hsAntennaType;
            strm << line << endl;
        }
        if(valid & validAntennaPosition)
        {
            line  = rightJustify(asString(antennaPosition[0], 4), 14);
            line += rightJustify(asString(antennaPosition[1], 4), 14);
            line += rightJustify(asString(antennaPosition[2], 4), 14);
            line += string(18, ' ');
            line += hsAntennaPosition;
            strm << line << endl;
        }
        if(valid & validAntennaDeltaHEN)
        {
            line  = rightJustify(asString(antennaDeltaHEN[0], 4), 14);
            line += rightJustify(asString(antennaDeltaHEN[1], 4), 14);
            line += rightJustify(asString(antennaDeltaHEN[2], 4), 14);
            line += string(18, ' ');
            line += hsAntennaDeltaHEN;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validAntennaDeltaXYZ))
        {
            line  = rightJustify(asString(antennaDeltaXYZ[0], 4), 14);
            line += rightJustify(asString(antennaDeltaXYZ[1], 4), 14);
            line += rightJustify(asString(antennaDeltaXYZ[2], 4), 14);
            line += string(18, ' ');
            line += hsAntennaDeltaXYZ;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validAntennaPhaseCtr))
        {
            line  =  leftJustify(antennaSatSys , 1);
            line += string(1, ' ');
            line += rightJustify(antennaObsCode, 3);
            line += rightJustify(asString(antennaPhaseCtr[0], 4),  9);
            line += rightJustify(asString(antennaPhaseCtr[1], 4), 14);
            line += rightJustify(asString(antennaPhaseCtr[2], 4), 14);
            line += string(18, ' ');
            line += hsAntennaPhaseCtr;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validAntennaBsightXYZ))
        {
            line  = rightJustify(asString(antennaBsightXYZ[0], 4), 14);
            line += rightJustify(asString(antennaBsightXYZ[1], 4), 14);
            line += rightJustify(asString(antennaBsightXYZ[2], 4), 14);
            line += string(18, ' ');
            line += hsAntennaBsightXYZ;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validAntennaZeroDirAzi))
        {
            line  = rightJustify(asString(antennaZeroDirAzi, 4), 14);
            line += string(46, ' ');
            line += hsAntennaZeroDirAzi;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validAntennaZeroDirXYZ))
        {
            line  = rightJustify(asString(antennaZeroDirXYZ[0], 4), 14);
            line += rightJustify(asString(antennaZeroDirXYZ[1], 4), 14);
            line += rightJustify(asString(antennaZeroDirXYZ[2], 4), 14);
            line += string(18, ' ');
            line += hsAntennaZeroDirXYZ;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validCenterOfMass))
        {
            line  = rightJustify(asString(centerOfMass[0], 4), 14);
            line += rightJustify(asString(centerOfMass[1], 4), 14);
            line += rightJustify(asString(centerOfMass[2], 4), 14);
            line += string(18, ' ');
            line += hsCenterOfMass;
            strm << line << endl;
        }

        if(version < 3 && (valid & validNumObs))          // R2 only
        {
            if(R2ObsTypes.empty())
            {
                RinexObsHeaderError er("Header contains no R2ObsTypes. "
                                       "You must run prepareVer2Write before outputting an R2 file");
                THROW(er);
            }

            // write out RinexObsTypes
            const int maxObsPerLine = 9;
            int obsWritten = 0;
            line = ""; // make sure the line contents are reset.

            for(size_t i=0; i<R2ObsTypes.size(); i++)
            {
                string val;
                // the first line needs to have the # of obs
                if(obsWritten == 0)
                    line  = rightJustify(asString(R2ObsTypes.size()), 6);
                // if you hit 9, write out the line and start a new one
                else if((obsWritten % maxObsPerLine) == 0)
                {
                    line += hsNumObs;
                    strm << line << endl;
                    line  = string(6, ' ');
                }
                val = R2ObsTypes[i];
                line += rightJustify(val, 6);
                obsWritten++;
            }

            line += string(60 - line.size(), ' ');
            line += hsNumObs;
            strm << line << endl;
        }

        if(version >= 3 && (valid & validSystemNumObs))
        {
            static const int maxObsPerLine = 13;

            map<string,vector<TypeID> >::const_iterator mapIter;
            for(mapIter = mapObsTypes.begin();
                mapIter != mapObsTypes.end();
                mapIter++)
            {
                int obsWritten = 0;
                line = ""; // make sure the line contents are reset

                vector<TypeID> ObsTypeList = mapIter->second;

                for(size_t i = 0; i < ObsTypeList.size(); i++)
                {
                    // the first line needs to have the GNSS type and # of obs
                    if(obsWritten == 0)
                    {
                        line  =  leftJustify(mapIter->first, 1);
                        line += string(2, ' ');
                        line += rightJustify(asString(ObsTypeList.size()), 3);
                    }
                    // if you hit 13, write out the line and start a new one
                    else if((obsWritten % maxObsPerLine) == 0)
                    {
                        line += string(2, ' ');
                        line += hsSystemNumObs;
                        strm << line << endl;
                        line  = string(6, ' ');
                    }
                    line += string(1, ' ');
                    line += rightJustify(ObsTypeList[i].asString(), 3);
                    obsWritten++;
                }
                line += string(60 - line.size(), ' ');
                line += hsSystemNumObs;
                strm << line << endl;
            }
        }
        if(version < 3 && (valid & validWaveFact))
        {
            line  = rightJustify(asString<short>(wavelengthFactor[0]),6);
            line += rightJustify(asString<short>(wavelengthFactor[1]),6);
            line += string(48, ' ');
            line += hsWaveFact;
            strm << line << endl;

            // handle continuation lines
            if(!extraWaveFactList.empty())
            {
                vector<ExtraWaveFact>::const_iterator itr = extraWaveFactList.begin();

                while (itr != extraWaveFactList.end())
                {
                    const int maxSatsPerLine = 7;
                    short satsWritten = 0, satsLeft = (*itr).satList.size(), satsThisLine;
                    vector<SatID>::const_iterator vecItr = (*itr).satList.begin();

                    while ((vecItr != (*itr).satList.end()))
                    {
                        if(satsWritten == 0)
                        {
                            line  = rightJustify(asString<short>((*itr).wavelengthFactor[0]),6);
                            line += rightJustify(asString<short>((*itr).wavelengthFactor[1]),6);
                            satsThisLine = (satsLeft > maxSatsPerLine ? maxSatsPerLine : satsLeft);
                            line += rightJustify(asString<short>(satsThisLine),6);
                        }

                        try
                        {
                            line += string(3, ' ') + SatID(*vecItr).toString();
                        }
                        catch (Exception& e)
                        {
                            RinexObsHeaderError ffse(e);
                            THROW(ffse);
                        }
                        satsWritten++;
                        satsLeft--;
                        if(satsWritten==maxSatsPerLine || satsLeft==0)
                        {
                            // output a complete line
                            line += string(60 - line.size(), ' ');
                            line += hsWaveFact;
                            strm << line << endl;
                            satsWritten = 0;
                        }
                        vecItr++;
                    }
                    itr++;
                }
            }
        }
        if(version >= 3 && valid & validSigStrengthUnit)
        {
            line  = leftJustify(sigStrengthUnit, 20);
            line += string(40, ' ');
            line += hsSigStrengthUnit;
            strm << line << endl;
        }
        if(valid & validInterval)
        {
            line  = rightJustify(asString(interval, 3), 10);
            line += string(50, ' ');
            line += hsInterval;
            strm << line << endl;
        }
        if(valid & validFirstTime)
        {
            line  = writeTime(firstObs);
            line += string(60 - line.size(), ' ');
            line += hsFirstTime;
            strm << line << endl;
        }
        if(valid & validLastTime)
        {
            line  = writeTime(lastObs);
            line += string(60 - line.size(), ' ');
            line += hsLastTime;
            strm << line << endl;
        }
        if(valid & validReceiverOffset)
        {
            line  = rightJustify(asString(receiverOffset), 6);
            line += string(54, ' ');
            line += hsReceiverOffset;
            strm << line << endl;
        }
        if(version >= 3 && (valid & validSystemDCBSapplied))
        {
            for(size_t i = 0; i < infoDCBS.size(); i++)
            {
                line  = leftJustify(infoDCBS[i].satSys,  1);
                line += string(1, ' ');
                line += leftJustify(infoDCBS[i].name  , 17);
                line += string(1, ' ');
                line += leftJustify(infoDCBS[i].source, 40);
                line += hsSystemDCBSapplied;
                strm << line << endl;
            }
        }
        if(version >= 3 && (valid & validSystemPCVSapplied))
        {
            for(size_t i = 0; i < infoPCVS.size(); i++)
            {
                line  = leftJustify(infoPCVS[i].satSys,  1);
                line += string(1, ' ');
                line += leftJustify(infoPCVS[i].name  , 17);
                line += string(1, ' ');
                line += leftJustify(infoPCVS[i].source, 40);
                line += hsSystemPCVSapplied;
                strm << line << endl;
            }
        }
        if(version >= 3 && (valid & validSystemScaleFac))
        {
            static const int maxObsPerLine = 12;

            static const int size = 4;
            static const int factors[size] = {1,10,100,1000};
            vector<string> obsTypes;

            // loop over GNSSes
            map<string, ScaleFacMap>::const_iterator mapIter;
            for(mapIter = sysSfacMap.begin();
                mapIter != sysSfacMap.end();
                mapIter++)
            {
                map<TypeID, int>::const_iterator iter;
//                map<TypeID, int>::const_iterator iter;

                for(int i = 0; i < size; i++) // loop over possible factors (above)
                {
                    int count = 0;
                    obsTypes.clear(); // clear the list of Obs Types we're going to make

                    for(iter = mapIter->second.begin();      // loop over scale factor map
                        iter != mapIter->second.end();
                        iter++)
                    {
                        if(iter->second == factors[i] )
                        {
                            count++;
                            obsTypes.push_back(iter->first.asString());
                        }
                    }

                    if(count == 0 ) continue;

                    line  = leftJustify(mapIter->first       , 1);
                    line += string(1, ' ');
                    line += rightJustify(asString(factors[i]), 4);
                    line += string(2, ' ');
                    line += rightJustify(asString(count     ), 2);

                    for(int j = 0; j < count; j++)
                    {
                        if(j > maxObsPerLine-1 && (j % maxObsPerLine) == 0 )
                        {
                            // need continuation; end current line
                            line += string(2, ' ');
                            line += hsSystemScaleFac;
                            strm << line << endl;
                            line  = string(10, ' ');
                        }
                        line += string(1, ' ');
                        line += rightJustify(obsTypes[j], 3);
                    }
                    int space = 60 - 10 - 4*(count % maxObsPerLine);
                    line += string(space, ' ');
                    line += hsSystemScaleFac;
                    strm << line << endl;
                }
            }
        }

        if(version >= 3.01 && (valid & validSystemPhaseShift))
        {
            map<string, map<TypeID, map<SatID,double> > >::const_iterator it;
            for(it=sysPhaseShift.begin();
                it!=sysPhaseShift.end();
                ++it)
            {
                string sys(it->first);
                map<TypeID, map<SatID,double> >::const_iterator jt(it->second.begin());
                if(jt == it->second.end())
                {
                    line  = sys;
                    line += string(60-line.length(), ' ');
                    line += hsSystemPhaseShift;
                    strm << line << endl;
                }
                else
                {
                    for( ; jt!=it->second.end(); ++jt)
                    {
                        TypeID obsid(jt->first);
//                        TypeID obsid(jt->first);
                        SatID sat(jt->second.begin()->first);
                        double corr(jt->second.begin()->second);
                        line = sys + " ";
                        line += leftJustify(obsid.asString(),3) + " ";
                        line += rightJustify(asString(corr,5),8);
                        if(sat.id == -1)
                        {
                            line += string(60-line.length(), ' ');
                            line += hsSystemPhaseShift;
                            strm << line << endl;
                        }
                        else
                        {
                            // list of sats
                            setfill('0');
                            line += string("  ") + rightJustify(asString(jt->second.size()),2);
                            setfill(' ');

                            int n(0);
                            map<SatID,double>::const_iterator kt,lt;
                            for(kt=jt->second.begin(); kt!=jt->second.end(); ++kt)
                            {
                                line += string(" ") + kt->first.toString();
                                if(++n == 10 || ++(lt=kt) == jt->second.end())
                                {
                                    // end this line
                                    line += string(60-line.length(), ' ');
                                    line += hsSystemPhaseShift;
                                    strm << line << endl;
                                    n = 0;
                                    // are there more for a continuation line?
                                    if(lt != jt->second.end())
                                        line = string(18,' ');
                                } // if(++n == 10 || ++(lt=kt) == jt->second.end())
                            } // for(kt=jt->second.begin(); kt!=jt->second.end(); ++kt)
                        } // else
                    } // for( ; jt!=it->second.end(); ++jt)
                } // else
            } // for(it=sysPhaseShift.begin(); it!=sysPhaseShift.end(); ++it)
        } // if(version >= 3.01 && (valid & validSystemPhaseShift))

        if(version >= 3.01)
        {
            if ((valid & validGlonassSlotFreqNo))
            {
                size_t n(0), nsat(glonassFreqNo.size());
                line = rightJustify(asString(nsat), 3) + " ";
                GLOFreqNumMap::const_iterator it, kt;
                for (it = glonassFreqNo.begin(); it != glonassFreqNo.end(); ++it)
                {
                    line += it->first.toString();
                    line += rightJustify(asString(it->second), 3) + " ";
                    if (++n == 8 || ++(kt = it) == glonassFreqNo.end())
                    {
                        // write it
                        line += string(60 - line.length(), ' ');
                        line += hsGlonassSlotFreqNo;
                        strm << line << endl;
                        n = 0;
                        // are there more for a continuation line?
                        if (kt != glonassFreqNo.end())
                            line = string(4, ' ');
                    }
                }
            }
            else if(mapObsTypes.find("R") != mapObsTypes.end())
            {
                RinexObsHeaderError err("Glonass Slot Freq No required for files containing Glonass Observations ");
                THROW(err);
            }
        }
        if(version >= 3.02)
        {
            if ((valid & validGlonassCodPhsBias))
            {
                line.clear();
                GLOCodPhsBias::const_iterator it;
                const string labs[4] = {"C1C", "C1P", "C2C", "C2P"};
                for (int i = 0; i < 4; i++)
                {
                    TypeID obsid(TypeID( labs[i] + "R"));
                    it = glonassCodPhsBias.find(obsid);
                    double bias = 0.0;
                    if (it != glonassCodPhsBias.end())
                        bias = it->second;
                    line += " " + labs[i] + " " + rightJustify(asString(bias, 3), 8);
                }
                line += string(60 - line.length(), ' ');
                line += hsGlonassCodPhsBias;
                strm << line << endl;
            }
            else if(mapObsTypes.find("R") != mapObsTypes.end())
            {
                RinexObsHeaderError err("Glonass Code Phase Bias required for files containing Glonass Observations ");
                THROW(err);
            }
        }
        if(valid & validLeapSeconds)
        {
            line  = rightJustify(asString(leapSeconds), 6);
            line += string(54, ' ');
            line += hsLeapSeconds;
            strm << line << endl;
        }
        if(valid & validNumSats)
        {
            line  = rightJustify(asString(numSVs), 6);
            line += string(54, ' ');
            line += hsNumSats;
            strm << line << endl;
        }
        if(valid & validPrnObs)
        {
            static const int maxObsPerLine = 9;
            map<SatID, vector<int> >::const_iterator itr(numObsForSat.begin());
            // loop over satellites
            while(itr != numObsForSat.end())
            {
                int numObsWritten = 0;                  // # of counts written for this sat
                SatID sat(itr->first);             // the sat
                const vector<int>& numObs(itr->second); // the vector of ints stored
                vector<int> vec;                        // the vector of ints to write

                if(version >= 3)
                    vec = numObs;
                else
                {
                    // fill in zeros for version 2
                    int j;
                    size_t i;
                    string sys(string(1,sat.systemChar()));
                    map<string, map<string, TypeID> >::const_iterator jt(mapSysR2toR3ObsID.find(sys));
                    const map<string, TypeID> mapVec(jt->second);
                    map<string, TypeID>::const_iterator kt;
                    for(i=0,j=0; i<R2ObsTypes.size(); i++)
                    {
                        kt = mapVec.find(R2ObsTypes[i]);
                        string obsid(kt->second.asString());
                        if(obsid == string("   ")) vec.push_back(0.0);
                        else                       vec.push_back(numObs[j++]);
                    }
                }

                vector<int>::const_iterator vecItr(vec.begin());
                while (vecItr != vec.end())
                {
                    if(numObsWritten == 0)
                    {
                        // start of line
                        try
                        {
                            line = string(3, ' ') + sat.toString();      // '   G01'
                        }
                        catch (Exception& e)
                        {
                            RinexObsHeaderError ffse(e);
                            RETHROW(ffse);
                        }
                    }
                    else if((numObsWritten % maxObsPerLine) == 0)
                    {
                        // end of line
                        line += hsPrnObs;
                        strm << line << endl;
                        line  = string(6, ' ');
                    }

                    line += rightJustify(asString(*vecItr), 6);        // add num obs to line
                    ++vecItr;
                    ++numObsWritten;
                }

                // finish last line
                line += string(60 - line.size(), ' ');
                line += hsPrnObs;
                strm << line << endl;
                itr++;
            }
        }
        if(validEoH)
        {
            line  = string(60, ' ');
            line += hsEoH;
            strm << line << endl;
        }

    } // end writeHeaderRecords


      // This function parses a single header record.
    void Rx3ObsHeader::parseHeaderRecord(string& line)
        noexcept(false)
    {
        int i;
        string label(line, 60, 20);

        if(debug)
        {
            cout << "Rx3ObsHeader:"  << line << endl;
        }

        if(label == hsVersion)
        {
            version  = asDouble(line.substr( 0,20));

            fileType = strip(   line.substr(20,20));
            fileSys  = strip(   line.substr(40,20));

            if(fileSys[0] != 'M' && fileSys[0] != 'm')
            {
                SatID sat;
                sat.fromString(fileSys);
                fileSysSat = SatID(sat);
            }
            else
                fileSysSat = SatID(-1,SatelliteSystem::Mixed);

            if(fileType[0] != 'O' && fileType[0] != 'o')
            {
                RinexObsHeaderError err("This isn't a RINEX 3 Obs file.");
                THROW(err);
            }

            valid |= validVersion;
        }
        else if(label == hsRunBy)
        {
            fileProgram = strip(line.substr( 0,20));
            fileAgency  = strip(line.substr(20,20));
            date        = strip(line.substr(40,20));
            valid |= validRunBy;
        }
        else if(label == hsComment)
        {
            commentList.push_back(strip(line.substr(0,60)));
            valid |= validComment;
        }
        else if(label == hsMarkerName)
        {
            markerName = strip(line.substr(0,60));
            // add by shjzhang, 2021
            markerName = replaceAll(markerName, " ", "_" );
            valid |= validMarkerName;
        }
        else if(label == hsMarkerNumber)
        {
            markerNumber = strip(line.substr(0,20));
            valid |= validMarkerNumber;
        }
        else if(label == hsMarkerType)
        {
            markerType = strip(line.substr(0,20));
            valid |= validMarkerType;
        }
        else if(label == hsObserver)
        {
            observer = strip(line.substr( 0,20));
            agency   = strip(line.substr(20,40));
            valid |= validObserver;
        }
        else if(label == hsReceiver)
        {
            recNo   = strip(line.substr( 0,20));
            recType = strip(line.substr(20,20));
            recVers = strip(line.substr(40,20));
            valid |= validReceiver;
        }
        else if(label ==hsAntennaType)
        {
            antNo   = strip(line.substr( 0,20));
            antType = strip(line.substr(20,20));
            valid |= validAntennaType;
        }
        else if(label == hsAntennaPosition)
        {
            antennaPosition[0] = asDouble(line.substr( 0,14));
            antennaPosition[1] = asDouble(line.substr(14,14));
            antennaPosition[2] = asDouble(line.substr(28,14));
            valid |= validAntennaPosition;
        }
        else if(label == hsAntennaDeltaHEN)
        {
            antennaDeltaHEN[0] = asDouble(line.substr( 0,14));
            antennaDeltaHEN[1] = asDouble(line.substr(14,14));
            antennaDeltaHEN[2] = asDouble(line.substr(28,14));
            valid |= validAntennaDeltaHEN;
        }
        else if(label == hsAntennaDeltaXYZ)
        {
            antennaDeltaXYZ[0] = asDouble(line.substr( 0,14));
            antennaDeltaXYZ[1] = asDouble(line.substr(14,14));
            antennaDeltaXYZ[2] = asDouble(line.substr(28,14));
            valid |= validAntennaDeltaXYZ;
        }
        else if(label == hsAntennaPhaseCtr)
        {
            antennaSatSys  = strip(line.substr(0,2));
            antennaObsCode = strip(line.substr(2,3));
            antennaPhaseCtr[0] = asDouble(line.substr( 5, 9));
            antennaPhaseCtr[1] = asDouble(line.substr(14,14));
            antennaPhaseCtr[2] = asDouble(line.substr(28,14));
            valid |= validAntennaPhaseCtr;
        }
        else if(label == hsAntennaBsightXYZ)
        {
            antennaBsightXYZ[0] = asDouble(line.substr( 0,14));
            antennaBsightXYZ[1] = asDouble(line.substr(14,14));
            antennaBsightXYZ[2] = asDouble(line.substr(28,14));
            valid |= validAntennaBsightXYZ;
        }
        else if(label == hsAntennaZeroDirAzi)
        {
            antennaZeroDirAzi = asDouble(line.substr(0,14));
            valid |= validAntennaBsightXYZ;
        }
        else if(label == hsAntennaZeroDirXYZ)
        {
            antennaZeroDirXYZ[0] = asDouble(line.substr( 0,14));
            antennaZeroDirXYZ[1] = asDouble(line.substr(14,14));
            antennaZeroDirXYZ[2] = asDouble(line.substr(28,14));
            valid |= validAntennaBsightXYZ;
        }
        else if(label == hsCenterOfMass)
        {
            centerOfMass[0] = asDouble(line.substr( 0,14));
            centerOfMass[1] = asDouble(line.substr(14,14));
            centerOfMass[2] = asDouble(line.substr(28,14));
            valid |= validCenterOfMass;
        }
        else if(label == hsNumObs)        // R2 only
        {
            if(version >= 3)
            {
                RinexObsHeaderError err("RINEX 2 record in RINEX 3 file: "+label);
                THROW(err);
            }

            // process the first line
            if(!(valid & validNumObs))
            {
                numObs = asInt(line.substr(0,6));
                valid |= validNumObs;
            }

            const int maxObsPerLine = 9;
            for(i = 0; (R2ObsTypes.size() < numObs) && (i < maxObsPerLine); i++)
                R2ObsTypes.push_back(line.substr(i * 6 + 10, 2));
        }
        else if(label == hsSystemNumObs)
        {
            if(version < 3)
            {
                RinexObsHeaderError err("RINEX 3 record in RINEX 2 file: "+label);
                THROW(err);
            }

            string satSys = strip(line.substr(0,1));
            if (satSys != "")
            {
                numObs = asInt(line.substr(3,3));
                valid |= validSystemNumObs;
                satSysPrev = satSys;
            }
            else
                satSys = satSysPrev;

            try
            {
                const int maxObsPerLine = 13;
                for (int i=0; i < maxObsPerLine && mapObsTypes[satSys].size() < numObs; i++)
                {
                    // for 3.02, both C1x and C2x should be treated as c2x in 3.03 and 3.04, 
                    // see the wavelength values in constants.hpp
                    // shoujian zhang.
                    std::string typeStr = (line.substr(4 * i + 7, 3)+satSys);
                    if(debug)
                    {
                        cout << version << " "
                             << satSys  << " "
                             << endl;
                    }

                    if(version==3.02 && satSys == "C")
                    {
                        if(typeStr[1] == '1')
                        {
                            typeStr[1] = '2';
                        }
                    }

                    if(debug)
                    {
                        cout << "Rx3ObsHeader:" << typeStr << endl;
                    }

                    // insert into mapObsTypes
                    mapObsTypes[satSys].push_back(TypeID(typeStr));

                }
            }
            catch(InvalidParameter& ip)
            {
                RinexObsHeaderError fse("InvalidParameter: "+ip.what());
                THROW(fse);
            }
        }
        else if(label == hsWaveFact)         // R2 only
        {
            // first time reading this
            if(!(valid & validWaveFact))
            {
                wavelengthFactor[0] = asInt(line.substr(0,6));
                wavelengthFactor[1] = asInt(line.substr(6,6));
                valid |= validWaveFact;
            }
            else
            {
                // additional wave fact lines
                const int maxSatsPerLine = 7;
                int Nsats;
                ExtraWaveFact ewf;
                ewf.wavelengthFactor[0] = asInt(line.substr(0,6));
                ewf.wavelengthFactor[1] = asInt(line.substr(6,6));
                Nsats = asInt(line.substr(12,6));

                if(Nsats > maxSatsPerLine)   // > not >=
                {
                    RinexObsHeaderError e("Invalid number of Sats for " + hsWaveFact);
                    THROW(e);
                }

                for(i = 0; i < Nsats; i++)
                {
                    try
                    {
                        SatID prn(line.substr(21+i*6,3));
                        ewf.satList.push_back(prn);
                    }
                    catch (Exception& e)
                    {
                        RinexObsHeaderError ffse(e);
                        RETHROW(ffse);
                    }
                }

                extraWaveFactList.push_back(ewf);
            }
        }
        else if(label == hsSigStrengthUnit)
        {
            sigStrengthUnit = strip(line.substr(0,20));
            valid |= validSigStrengthUnit;
        }
        else if(label == hsInterval)
        {
            interval = asDouble(line.substr(0,10));
            valid |= validInterval;
        }
        else if(label == hsFirstTime)
        {
            firstObs = parseTime(line);
            if(firstObs.timeSystem == TimeSystem::Unknown)
                firstObs.timeSystem = (TimeSystem::GPS);
            valid |= validFirstTime;
        }
        else if(label == hsLastTime)
        {
            lastObs = parseTime(line);
            valid |= validLastTime;
        }
        else if(label == hsReceiverOffset)
        {
            receiverOffset = asInt(line.substr(0,6));
            valid |= validReceiverOffset;
        }
        else if(label == hsSystemDCBSapplied)
        {
            Rinex3CorrInfo tempInfo;
            tempInfo.satSys = strip(line.substr( 0, 1));
            tempInfo.name   = strip(line.substr( 2,17));
            tempInfo.source = strip(line.substr(20,40));
            infoDCBS.push_back(tempInfo);
            valid |= validSystemDCBSapplied;
        }
        else if(label == hsSystemPCVSapplied)
        {
            Rinex3CorrInfo tempInfo;
            tempInfo.satSys = strip(line.substr( 0, 1));
            tempInfo.name   = strip(line.substr( 2,17));
            tempInfo.source = strip(line.substr(20,40));
            infoPCVS.push_back(tempInfo);
            valid |= validSystemPCVSapplied;
        }
        else if(label == hsSystemScaleFac)
        {
            static const int maxObsPerLine = 12;

            satSysTemp = strip(line.substr(0,1));
            factor     = asInt(line.substr(2,4));
            numObs     = asInt(line.substr(8,2));

            int startPosition = 0;

            if(satSysTemp == "" )
            {
                // it's a continuation line; use prev. info., end pt. to start
                satSysTemp = satSysPrev;
                factor     = factorPrev;
                numObs     = numObsPrev;

                startPosition = sysSfacMap[satSysTemp].size();
            }

            // 0/blank numObs means factor applies to all obs types
            // in appropriate obsTypeList
            if(numObs == 0)
                numObs = mapObsTypes[satSysTemp].size();

            ScaleFacMap tempSfacMap = sysSfacMap[satSysTemp];
            for(i = startPosition;
                (i < numObs) && ((i % maxObsPerLine) < maxObsPerLine);
                i++)
            {
                int position = 4*(i % maxObsPerLine) + 10 + 1;
                TypeID tempType(strip(line.substr(position,3) + satSysTemp));
                tempSfacMap.insert(make_pair(tempType,factor));
            }
            sysSfacMap[satSysTemp] = tempSfacMap;

            ScaleFacMap::const_iterator iter;
            ScaleFacMap tempmap;
            tempmap = sysSfacMap[satSysTemp];

            // save values in case next line is a continuation line
            satSysPrev = satSysTemp;
            factorPrev = factor;
            numObsPrev = numObs;

            valid |= validSystemScaleFac;
        }
        else if(label == hsSystemPhaseShift ||
                label == "SYS / PHASE SHIFTS") ///< "SYS / PHASE SHIFT"    R3.01
            // Add "SYS / PHASE SHIFTS" for exceptional cases.
        {
            SatID sat;
            // system
            satSysTemp = strip(line.substr(0,1));

            if(satSysTemp.empty())
            {
                // continuation line
                satSysTemp = satSysPrev;

                if(sysPhaseShift[satSysTemp].find(sysPhaseShiftObsID)
                                    == sysPhaseShift[satSysTemp].end())
                {
                    //RinexObsHeaderError e("SYS / PHASE SHIFT: unexpected continuation line");
                    //THROW(e);
                    // lenient - some writers have only a single blank record
                    return;
                }

                map<SatID,double>& satcorrmap(sysPhaseShift[satSysTemp][sysPhaseShiftObsID]);
                double cor(sysPhaseShift[satSysTemp][sysPhaseShiftObsID].begin()->second);
                for(i=0; i<10; i++)
                {
                    string str = strip(line.substr(19+4*i,3));
                    if(str.empty()) break;
                    sat = SatID(str);
                    satcorrmap.insert(make_pair(sat,cor));
                }
            }
            else
            {
                // not a cont. line
                sat.fromString(satSysTemp);
                if(sysPhaseShift.find(satSysTemp) == sysPhaseShift.end())
                {
                    map<TypeID, map<SatID, double> > obssatcormap;
                    sysPhaseShift.insert(make_pair(satSysTemp,obssatcormap));
                }

                // obs id
                string str = strip(line.substr(2,3));

                // obsid and correction may be blank <=> unknown: ignore this
                if(!str.empty())
                {
                    // modify by shoujian zhang
                    string typeStr = str+satSysTemp;
                    if(version==3.02 && satSysTemp == "C")
                    {
                        if(typeStr[1] == '1')
                        {
                            typeStr[1] = '2';
                        }
                    }
                    TypeID obsid(typeStr);
                    double cor(asDouble(strip(line.substr(6,8))));
                    int nsat(asInt(strip(line.substr(16,2))));
                    if(nsat > 0)
                    {
                        // list of sats
                        map<SatID,double> satcorrmap;
                        for(i=0; i<(nsat < 10 ? nsat : 10); i++)
                        {
                            sat = SatID(strip(line.substr(19+4*i,3)));
                            satcorrmap.insert(make_pair(sat,cor));
                        }
                        sysPhaseShift[satSysTemp].insert(make_pair(obsid,satcorrmap));
                        if(nsat > 10)        // expect continuation
                            sysPhaseShiftObsID = obsid;
                    }
                    else
                    {
                        // no sat, just system
                        map<SatID,double> satcorrmap;
                        satcorrmap.insert(make_pair(sat,cor));
                        sysPhaseShift[satSysTemp].insert(make_pair(obsid,satcorrmap));
                    }
                }

                // save for continuation lines
                satSysPrev = satSysTemp;

                valid |= validSystemPhaseShift;
            }
        }
        else if(label == hsGlonassSlotFreqNo)
        {
            //map<SatID,int> glonassFreqNo;
            int tmp;
            SatID sat;
            string str(strip(line.substr(0,3)));

            for(i=0; i<8; i++)
            {
                str = strip(line.substr(4+i*7,3));
                if(str.empty()) break;
                sat = SatID(str);
                str = strip(line.substr(8+i*7,2));
                tmp = asInt(str);
                glonassFreqNo.insert(make_pair(sat,tmp));
            }

            valid |= validGlonassSlotFreqNo;
        }
        else if(label == hsGlonassCodPhsBias ||
                label == hsGlonassCodPhsBias+"#")
        {
            // several IGS MGEX 3.02 files do the extra "#"
            //std::map<TypeID,double> glonassCodPhsBias; ///< "GLONASS COD/PHS/BIS"            R3.02
            for(i=0; i<4; i++)
            {
                string str(strip(line.substr(i*13+1,3)));
                if(str.empty()) continue;
                TypeID obsid(str+"R");
                double bias(asDouble(strip(line.substr(i*13+5,8))));
                glonassCodPhsBias[obsid] = bias;
            }
            valid |= validGlonassCodPhsBias;
        }
        else if(label == hsLeapSeconds)
        {
            leapSeconds = asInt(line.substr(0,6));
            valid |= validLeapSeconds;
        }
        else if(label == hsNumSats)
        {
            numSVs = asInt(line.substr(0,6)) ;
            valid |= validNumSats;
        }
        else if(label == hsPrnObs)
        {
            // this assumes 'PRN / # OF OBS' comes after '# / TYPES OF OBSERV' or 'SYS / # / OBS TYPES'
            static const int maxObsPerLine = 9;

            int j,otmax;
            SatID PRN;
            string prn, GNSS;
            vector<int> numObsList;

            prn = strip(line.substr(3,3));

            if(prn == "" ) // this is a continuation line; use last PRN
            {
                PRN = lastPRN;
                GNSS = PRN.systemChar();
                if(version < 3)
                    otmax = R2ObsTypes.size();
                else
                {
                    if(mapObsTypes.find(GNSS) == mapObsTypes.end())
                    {
                        Exception e("PRN/#OBS for system " +
                                     PRN.toString() +
                                    " not found in SYS/#/OBS");
                        THROW(e);
                    }
                    otmax = mapObsTypes[GNSS].size();
                }

                numObsList = numObsForSat[PRN]; // grab the existing list

                for(j=0,i=numObsList.size(); j<maxObsPerLine && i<otmax; i++,j++)
                    numObsList.push_back(asInt(line.substr(6*j+6,6)));

                numObsForSat[PRN] = numObsList;
            }
            else             // this is a new PRN line
            {
                PRN = SatID(prn);
                GNSS = PRN.systemChar();
                if(version < 3)
                    otmax = R2ObsTypes.size();
                else
                {
                    if(mapObsTypes.find(GNSS) == mapObsTypes.end())
                    {
                        Exception e("PRN/#OBS for system " +
                                     PRN.toString() +
                                     " not found in SYS/#/OBS");
                        THROW(e);
                    }
                    otmax = mapObsTypes[GNSS].size();
                }

                for(i=0; i<maxObsPerLine && i<otmax; i++)
                    numObsList.push_back(asInt(line.substr(6*i+6,6)));

                numObsForSat[PRN] = numObsList;

                lastPRN = PRN;
            }

            valid |= validPrnObs;
        }
        else if(label == hsEoH)
        {
            validEoH = true;
        }
        else
        {
            RinexObsHeaderError e("Unidentified label: >" + label + "<");
            THROW(e);
        }
    } // end of parseHeaderRecord


      // This function parses the entire header from the given stream
    void Rx3ObsHeader::reallyGetRecord(std::fstream& strm)
        noexcept(false)
    {
         // Since we're reading a new header, we need to reinitialize
         // all our list structures. All the other objects should be
         // ok.  This also applies if we threw an exception the first
         // time we read the header and are now re-reading it.  Some
         // of these could be full and we need to empty them.
        clear();

        string line;

        while (!validEoH)
        {
            getline(strm, line);
            if(strm.eof())
            {
                EndOfFile err("EOF encountered!");
                THROW(err);
            }

            if(debug)
            {
                cout << line << endl;
                cout << "length:" << line.length() << endl;
            }

            stripTrailing(line);
            if(line.length() == 0)
            {
                RinexObsHeaderError err("No data read");
                cerr << err << endl;
                exit(-1);
            }
            else if(line.length() < 60 || line.length() > 80)
            {
                RinexObsHeaderError err("Invalid line length");
                cout  << line << endl;
                cerr << err << endl;
                exit(-1);
            }

            try
            {
                parseHeaderRecord(line);
            }
            catch(RinexObsHeaderError& e)
            {
                RETHROW(e);
            }

        } // end while(not end of header)

        // if RINEX 2, define mapObsTypes from R2ObsTypes and
        // system(s) this may have to be corrected later using
        // wavelengthFactor also define mapSysR2toR3ObsID in case
        // version 2 is written out later
        if(version < 3)
        {
            // try to determine systems included in the file
            vector<string> syss;                // 1-char strings "G" "R" "E" ...
            if(numObsForSat.size() > 0)
            {
                // get syss from PRN/#OBS
                map<SatID, vector<int> >::const_iterator it;
                for(it=numObsForSat.begin(); it != numObsForSat.end(); ++it)
                {
                    string sys(string(1,(it->first).systemChar()));
                    if(find(syss.begin(),syss.end(),sys) == syss.end())
                        syss.push_back(sys);
                }
            }
            else if(fileSysSat.system != SatelliteSystem::Mixed)
            {
                // only one system in this file
                syss.push_back(string(1,SatID(fileSysSat).systemChar()));
            }
            else
            {
                // have to replicate obs type list for all RINEX2 systems
                syss.push_back("G");
                syss.push_back("R");
                syss.push_back("S");    // ??
                syss.push_back("E");
            }

            // given systems and list of R2ObsTypes, compute
            // mapObsTypes and mapSysR2toR3ObsID
            mapSysR2toR3ObsID.clear();
            for(size_t i=0; i<syss.size(); i++)
            {
                const string s(syss[i]);
                vector<TypeID> obsids;

                try
                {
                    if      (s=="G")
                        obsids = mapR2ObsToR3Obs_G();
                    else if (s=="R")
                        obsids = mapR2ObsToR3Obs_R();
                    else if (s=="E")
                        obsids = mapR2ObsToR3Obs_E();
                    else if (s=="S")
                        obsids = mapR2ObsToR3Obs_S();
                }
                catch(RinexObsHeaderError fse)
                {
                    RETHROW(fse);
                }

                // TD if GPS and have wavelengthFactors, add more
                // ObsIDs with tc=N

                mapObsTypes[syss[i]] = obsids;
            }

            // modify numObsForSat if necessary
            map<SatID, vector<int> >::const_iterator it(numObsForSat.begin());
            for( ; it != numObsForSat.end(); ++it)
            {
                SatID sat(it->first);
                string sys;
                sys = sat.systemChar();
                vector<int> vec;
                for(size_t i=0; i<R2ObsTypes.size(); i++)
                {
                    if(mapSysR2toR3ObsID[sys][R2ObsTypes[i]].asString() == string("   "))
                        ;
                    else
                        vec.push_back(it->second[i]);
                }
                numObsForSat[sat] = vec;
            }
        }

        // after get mapObsTypes, we will extract codeSysObsTypes and phaseSysObsTypes
        // for later data processing. by shoujian zhang.
        

        // Since technically the Phase Shift record is required in ver 3.01,
        // create SystemPhaseShift record(s) if not present.
        //map<string, map<TypeID, map<SatID,double> > > sysPhaseShift;
        if(version >= 3.01 && (valid & validSystemNumObs)
            && !(valid & validSystemPhaseShift))
        {
            // loop over obs types to get systems
            map<string,vector<TypeID> >::const_iterator iter;
            for(iter=mapObsTypes.begin(); iter != mapObsTypes.end(); iter++)
            {
                string sys(iter->first);
                if(sysPhaseShift.find(sys) == sysPhaseShift.end())
                {
                    map<TypeID, map<SatID, double> > dummy;
                    sysPhaseShift.insert(make_pair(sys,dummy));
                }
            }
            valid |= validSystemPhaseShift;
        }

         // is the header valid?
        unsigned long allValid;
        if     (version <  3  )  allValid = allValid2;
        else if(version == 3.0)  allValid = allValid30;
        else if(version == 3.01) allValid = allValid301;
        else if(version == 3.02) allValid = allValid302;
        else if(version == 3.03) allValid = allValid303;
        else if(version == 3.04) allValid = allValid304;
        else if(version == 3.05) allValid = allValid305;
        else
        {
            cerr << ("Unknown or unsupported RINEX version " + asString(version));
            exit(-1);
        }

        if((valid & allValid) != allValid)
        {
            cerr << ("Unknown or unsupported RINEX version " + asString(version));
            exit(-1);
        }

         // determine the time system of epochs in this file; cf. R3.02 Table A2
         // 1.determine time system from time tag in TIME OF FIRST OBS record
         // 2.if not given, determine from type in RINEX VERSION / TYPE record
         // 3.(if the type is MIXED, the time system in firstObs is required by RINEX)
        TimeSystem timeSys = firstObs.timeSystem;
        if( timeSys == TimeSystem::Any ||
            timeSys == TimeSystem::Unknown )
        {
            if(fileSysSat.system == SatelliteSystem::GPS)
            {
                timeSys = TimeSystem::GPS;
                firstObs.timeSystem = (TimeSystem::GPS);
            }
            else if(fileSysSat.system == SatelliteSystem::GLONASS)
            {
                timeSys = TimeSystem::UTC;
                firstObs.timeSystem=(TimeSystem::UTC);
            }
            else if(fileSysSat.system == SatelliteSystem::Galileo)
            {
                timeSys = TimeSystem::GAL;
                firstObs.timeSystem=(TimeSystem::GAL);
            }
            else if(fileSysSat.system == SatelliteSystem::QZSS)
            {
                timeSys = TimeSystem::QZS;
                firstObs.timeSystem=(TimeSystem::QZS);
            }
            else if(fileSysSat.system == SatelliteSystem::BDS)
            {
                timeSys = TimeSystem::BDT;
                firstObs.timeSystem=(TimeSystem::BDT);
            }
            else if(fileSysSat.system == SatelliteSystem::Mixed)
            {
                // lenient
                timeSys = TimeSystem::GPS;
                firstObs.timeSystem=(TimeSystem::GPS);
                // RINEX 3 requires this
                //RinexObsHeaderError e("TimeSystem in MIXED files must be given by first obs");
                //THROW(e);
            }
            else
            {
                cerr << "Rx3ObsHeader:" << ("Unknown file system type") << endl;; 
            }
        }

    } // end reallyGetRecord


      // This method maps v2.11 GPS observation types to the v3 equivalent.
      // Since only GPS and only v2.11 are of interest, only L1/L2/L5
      // are considered.
    vector<TypeID> Rx3ObsHeader::mapR2ObsToR3Obs_G()
        noexcept(false)
    {
        vector<TypeID> obsids;

        // Assume D1, S1, and L1 come from C/A unless P is being treated as Y and P1 is present
        // Furthermore, if P1 is present and P is NOT being treated as Y, assume that P1
        // is some Z-mode or equivalent "smart" codeless process.
        //
        // Condition           Result
        // PisY   P1?
        //    N    Y     L1,D1,S1 considered C,  P1 becomes C1W
        //    N    N     L1,D1,S1 considered C
        //    Y    Y     L1,D1,S1 considered Y,  P1 becomes C1Y
        //    Y    N     L1,D1,S1 considered C
        //
        bool hasL1P = find(R2ObsTypes.begin(),R2ObsTypes.end(), string("P1")) != R2ObsTypes.end();
        string code1 = "C";
        string code1P = "W";
        if (PisY && hasL1P)
        {
            code1 = "Y";
            code1P = "Y";
        }

        // Assume D2, S2, and L2 come from Y if P is being treated as Y and P2 is present
        // codeless unless L2C is tracked.
        // If BOTH C2 and P2 are present, and P is NOT being treated as Y, assume C2
        // is code tracking the open signal and that P2 is codelessly tracking an
        // authorized signal.
        //
        // Condition           Result
        // PisY   C2?   P2?
        //    N    Y     N     L2,D2,S2 considered X,
        //    N    Y     Y     L2,D2,S2 considered W,  P2 becomes C2W**
        //    N    N     Y     L2,D2,S2 considered W,  P2 becomes C2W
        //    N    N     N     L2,D2,S2 considered X*
        //    Y    Y     N     L2,D2,S2 considered X
        //    Y    Y     Y     L2,D2,S2 considered Y,  P2 becomes C2Y
        //    Y    N     Y     L2,D2,S2 considered Y,  P2 becomes C2Y
        //    Y    N     N     L2,D2,S2 considered X*
        // * - Probably not a reasonable set of conditions.  It implies no L2 pseudoranges
        //     were collected on any tracking code.
        // **- Interesting case.  Currently presence of C2 in the header means
        //     that the data MAY be present.  However, since only some of the GPS
        //     SVs have L2C, the C2 data field will frequently be empty.
        //     Therefore, we'll go with "W" if P2 is present.  The other option
        //     would be to add smarts to the SV-by-SV record reading process to
        //     coerce this to X if there are actually data in the C2 field at
        //     the time the observations are read.  That would really do violence
        //     to the existing logic.  Better to hope for a transition to Rinex 3
        //     before this becomes a real issue.
        //
        // N.B.:  This logic (both for P1 and P2) assumes P is NEVER P.  If we want to allow for
        // live sky (or simulator capture) P code, we'll have to add more logic
        // to differentate between PisY, PisW, and PisP. That will have to be
        // "beyond RINEX v2.11" extra-special handling.
        //
        bool hasL2P = find(R2ObsTypes.begin(),R2ObsTypes.end(), string("P2")) != R2ObsTypes.end();
        bool hasL2C = find(R2ObsTypes.begin(),R2ObsTypes.end(), string("C2")) != R2ObsTypes.end();

        string code2 = "X";   // Correct condition as long as P2 is not in the list
        string code2P = "X";  // Condition is irrelvant unless P2 is in the list
        if (hasL2P)
        {
            if (PisY)
            {
                code2  = "Y";
                code2P = "Y";
            }
            else
            {
                code2 = "W";
                code2P = "W";
            }
        }
        string syss("G");
        for(size_t j=0; j<R2ObsTypes.size(); ++j)
        {
            string ot(R2ObsTypes[j]);
            string obsid;

            if      (ot == "C1") obsid += "C1C";
            else if (ot == "P1") obsid += "C1" + code1P;
            else if (ot == "L1") obsid += "L1" + code1;
            // rinex 2.20 for LEO satellite rinex data 
            else if (ot == "LA") obsid += "LA" + code1;
            else if (ot == "D1") obsid += "D1" + code1;
            else if (ot == "SA") obsid += "SA" + code1;
            else if (ot == "S1") obsid += "S1" + code1;

            else if (ot == "C2") obsid += "C2X";
            else if (ot == "P2") obsid += "C2" + code2P;
            else if (ot == "L2") obsid += "L2" + code2;
            else if (ot == "D2") obsid += "D2" + code2;
            else if (ot == "S2") obsid += "S2" + code2;

            else if (ot == "C5") obsid += "C5X";
            else if (ot == "L5") obsid += "L5X";
            else if (ot == "D5") obsid += "D5X";
            else if (ot == "S5") obsid += "S5X";

            // If the obs type isn't valid for GPS, skip it.
            else 
                continue;

            // system id is placed at the last position
            obsid +=syss;

            try
            {
                TypeID OT(obsid);
                obsids.push_back(OT);
                mapSysR2toR3ObsID[syss][ot] = OT; //map<string, map<string, TypeID> >
            }
            catch(Exception& e)
            {
                cerr << "Rx3ObsData:" << e << endl;
                exit(-1);
            }
        }

        return obsids;
    }


      // This method maps v2.11 GLONASS observation types to the v3 equivalent.
      // Since only GLONASS and only v2.11 are of interest, only L1/L2
      // are considered.
    vector<TypeID> Rx3ObsHeader::mapR2ObsToR3Obs_R( )
        noexcept(false)
    {
        vector<TypeID> obsids;

         // Assume D1, S1, and L1 come from C/A
         // This assumes that any files claiming to track GLONASS P1 is
         // actually doing so with a codeless technique.  There is no RINEX V3
         // "C1W" for GLONASS, so we'll leave P1 as C1P as the closest approximation.
        bool hasL1P = find(R2ObsTypes.begin(),R2ObsTypes.end(), string("P1")) != R2ObsTypes.end();
        string code1 = "C";

         // Assume D2, S2, and L2 come from C/A.  Same logic as above.
        bool hasL2P = find(R2ObsTypes.begin(),R2ObsTypes.end(), string("P2")) != R2ObsTypes.end();
        string code2 = "C";

        string syss("R");
        for(size_t j=0; j<R2ObsTypes.size(); ++j)
        {
            string ot(R2ObsTypes[j]);
            string obsid;

            if      (ot == "C1") obsid += "C1C";
            else if (ot == "P1") obsid += "C1P";
            else if (ot == "L1") obsid += "L1" + code1;
            else if (ot == "D1") obsid += "D1" + code1;
            else if (ot == "S1") obsid += "S1" + code1;

            else if (ot == "C2") obsid += "C2C";
            else if (ot == "P2") obsid += "C2P";
            else if (ot == "L2") obsid += "L2" + code2;
            else if (ot == "D2") obsid += "D2" + code2;
            else if (ot == "S2") obsid += "S2" + code2;

            // If the obs type isn't valid for GLONASS, skip it.
            else continue;

            obsid+=syss;

            try
            {
                TypeID OT(obsid);
                obsids.push_back(OT);
                mapSysR2toR3ObsID[syss][ot] = OT; //map<string, map<string, TypeID> >
            }
            catch(InvalidParameter& ip)
            {
                RinexObsHeaderError fse("InvalidParameter: "+ip.what());
                THROW(fse);
            }
        }
        return obsids;
    }


      // This method maps v2.11 Galileo observation types to the v3 equivalent.
      // Since only Galileo and only v2.11 are of interest no L2 types
      // are considered.  Furthermore, Rinex v2.11 states that there is no
      // P for Galileo.  (Where that leaves the PRS is a good question.)
      //
      // In RINEX v3, there are 3-5 tracking codes defined for each carrier.
      // Given the current lack of experience, the code makes some
      // guesses on what the v2.11 translations should mean.
    vector<TypeID> Rx3ObsHeader::mapR2ObsToR3Obs_E()
        noexcept(false)
    {
        vector<TypeID> obsids;

        string code1 = "B";  // Corresponds to the open service
        string code5 = "I";  // Corresponds to the open service
        string code7 = "X";  // Corresponds to I + Q tracking
        string code8 = "X";  // Corresponds to I + Q tracking
        string code6 = "X";  // Corresponds to B + C tracking

        string syss("E");
        for(size_t j=0; j<R2ObsTypes.size(); ++j)
        {
            string ot(R2ObsTypes[j]);
            string obsid;
            if      (ot == "C1") obsid += "C1" + code1;
            else if (ot == "L1") obsid += "L1" + code1;
            else if (ot == "D1") obsid += "D1" + code1;
            else if (ot == "S1") obsid += "S1" + code1;

            else if (ot == "C5") obsid += "C5" + code5;
            else if (ot == "L5") obsid += "L5" + code5;
            else if (ot == "D5") obsid += "D5" + code5;
            else if (ot == "S5") obsid += "S5" + code5;

            else if (ot == "C6") obsid += "C6" + code6;
            else if (ot == "L6") obsid += "L6" + code6;
            else if (ot == "D6") obsid += "D6" + code6;
            else if (ot == "S6") obsid += "S6" + code6;

            else if (ot == "C7") obsid += "C7" + code7;
            else if (ot == "L7") obsid += "L7" + code7;
            else if (ot == "D7") obsid += "D7" + code7;
            else if (ot == "S7") obsid += "S7" + code7;

            else if (ot == "C8") obsid += "C8" + code8;
            else if (ot == "L8") obsid += "L8" + code8;
            else if (ot == "D8") obsid += "D8" + code8;
            else if (ot == "S8") obsid += "S8" + code8;

            // If the obs type isn't valid for Galileo, skip it.
            else continue;

            obsid+=syss;

            try
            {
                TypeID OT(obsid);
                obsids.push_back(OT);
                mapSysR2toR3ObsID[syss][ot] = OT; //map<string, map<string, TypeID> >
            }
            catch(InvalidParameter& ip)
            {
                RinexObsHeaderError fse("InvalidParameter: "+ip.what());
                THROW(fse);
            }
        }
        return obsids;
    }


      // This method maps v2.11 SBAS observation types to the v3 equivalent.
      // Since only SBAS and only v2.11 are of interest only L1/L5
      // are considered.
    vector<TypeID> Rx3ObsHeader::mapR2ObsToR3Obs_S()
        noexcept(false)
    {
        vector<TypeID> obsids;

        string code1 = "C";  // Only option
        string code5 = "X";  // Corresponds to I + Q tracking

        string syss("S");
        for(size_t j=0; j<R2ObsTypes.size(); ++j)
        {
            string ot(R2ObsTypes[j]);
            string obsid;

            if      (ot == "C1") obsid += "C1" + code1;
            else if (ot == "L1") obsid += "L1" + code1;
            else if (ot == "D1") obsid += "D1" + code1;
            else if (ot == "S1") obsid += "S1" + code1;

            else if (ot == "C5") obsid += "C5" + code5;
            else if (ot == "L5") obsid += "L5" + code5;
            else if (ot == "D5") obsid += "D5" + code5;
            else if (ot == "S5") obsid += "S5" + code5;

            // If the obs type isn't valid for SBAS, skip it.
            else continue;

            obsid+=syss;

            try
            {
                TypeID OT(obsid);
                obsids.push_back(OT);
                mapSysR2toR3ObsID[syss][ot] = OT; //map<string, map<string, TypeID> >
            }
            catch(InvalidParameter& ip)
            {
                RinexObsHeaderError fse("InvalidParameter: "+ip.what());
                THROW(fse);
            }
        }
        return obsids;
    }


    CivilTime Rx3ObsHeader::parseTime(const string& line) const
    {
        int year, month, day, hour, min;
        double sec;
        string tsys;
        TimeSystem ts;

        year  = asInt(   line.substr(0,   6));
        month = asInt(   line.substr(6,   6));
        day   = asInt(   line.substr(12,  6));
        hour  = asInt(   line.substr(18,  6));
        min   = asInt(   line.substr(24,  6));
        sec   = asDouble(line.substr(30, 13));
        tsys  =          line.substr(48,  3) ;

        ts.fromString(tsys);

        return CivilTime(year, month, day, hour, min, sec, ts);
    } // end parseTime


    string Rx3ObsHeader::writeTime(const CivilTime& civtime) const
    {
        string line;

        line  = rightJustify(asString<short>(civtime.year    )   ,  6);
        line += rightJustify(asString<short>(civtime.month   )   ,  6);
        line += rightJustify(asString<short>(civtime.day     )   ,  6);
        line += rightJustify(asString<short>(civtime.hour    )   ,  6);
        line += rightJustify(asString<short>(civtime.minute  )   ,  6);
        line += rightJustify(asString(       civtime.second,7)   , 13);
        line += rightJustify((civtime.timeSystem).asString(),  8);

        return line;
    } // end writeTime



    void Rx3ObsHeader::dump(ostream& s) const
    {
        size_t i;

        string str;
        if(fileSysSat.system == SatelliteSystem::Mixed)
            str = "MIXED";
        else
        {
            SatID sat(fileSysSat);
            str = sat.systemChar();
            str = str + " (" + sat.systemString() + ")";
        }

        s << "---------------------------------- REQUIRED "
          << "----------------------------------" << endl;
        s << "Rinex Version " << fixed << setw(5) << setprecision(2) << version
          << ",  File type " << fileType << ",  System " << str << "." << endl;
        s << "Prgm: " << fileProgram << ",  Run: " << date
          << ",  By: " << fileAgency << endl;
         //s << "Marker name: " << markerName << ", ";
        s << "Marker type: " << markerType << "." << endl;
        s << "Observer : " << observer << ",  Agency: " << agency << endl;
        s << "Rec#: " << recNo << ",  Type: " << recType
          << ",  Vers: " << recVers << endl;
        s << "Antenna # : " << antNo << ",  Type : " << antType << endl;
        s << "Position      (XYZ,m) : " << setprecision(4) << antennaPosition
          << "." << endl;
        s << "Antenna Delta (HEN,m) : " << setprecision(4) << antennaDeltaHEN
          << "." << endl;
        map<string,vector<TypeID> >::const_iterator iter;
        for(iter = mapObsTypes.begin(); iter != mapObsTypes.end(); iter++)
        {
            SatID rsid;
            rsid.fromString(iter->first);
            s << rsid.systemString() << " Observation types ("
              << iter->second.size() << "):" << endl;
            for(i = 0; i < iter->second.size(); i++)
                s << " Type #" << setw(2) << setfill('0') << i+1 << setfill(' ')
                  << " (" << iter->second[i].asString() << ") "
                  << endl;
        }

        s << "R2ObsTypes: ";
        for (StringVec::const_iterator i=R2ObsTypes.begin(); i != R2ObsTypes.end(); i++)
            s << *i << " ";
        s << endl;
        for (VersionObsMap::const_iterator i = mapSysR2toR3ObsID.begin();
             i != mapSysR2toR3ObsID.end();
             i++)
        {
            s << "mapSysR2toR3ObsID[" << i->first << "] ";
            for (TypeIDMap::const_iterator j = i->second.begin(); j != i->second.end(); j++)
                s << j->first << ":" << j->second.asString() << " ";
            s << endl;
        }

        s << "Time of first obs "
          << firstObs.asString() << endl;

        unsigned long allValid = 0;
        if     (version == 3.0)   allValid = allValid30;
        else if(version == 3.01)  allValid = allValid301;
        else if(version == 3.02)  allValid = allValid302;
        else if(version == 3.03)  allValid = allValid303;
        else if(version == 3.04)  allValid = allValid304;
        else if(version == 3.05)  allValid = allValid305;

        s << "(This header is ";
        if((valid & allValid) == allValid)
            s << "VALID)" << endl;
        else
        {
            s << "NOT VALID";
            s << " RINEX " << setprecision(2) << version << ")" << endl;
            s << "valid    = " << hex << setw(8) << valid << endl;
            s << "allValid = " << hex << setw(8) << allValid << endl;
            s << "~v & aV  = " << hex << setw(8) << (~valid & allValid) << endl << dec;

            s << "Invalid header records:" << endl;
            if(!(valid & validVersion)) s << " Version / Type\n";
            if(!(valid & validRunBy)) s << " Pgm / Run By / Date\n";
            if(!(valid & validMarkerName)) s << " Marker Name\n";
            //if(version >= 3 && !(valid & validMarkerType)) s << "Marker Type\n";
            // Not defined in R2 and not required in > 3, see Table A2 in R3 doc:
            // "Record required except for GEODETIC and NON_GEODETIC marker types"
            if(!(valid & validObserver)) s << " Observer / Agency\n";
            if(!(valid & validReceiver)) s << " Receiver # / Type\n";
            if(!(valid & validAntennaType)) s << " Antenna Type\n";
            if(!(valid & validAntennaPosition)) s << " Antenna Position\n";
            if(!(valid & validAntennaDeltaHEN)) s << " Antenna Delta HEN\n";
            if(version < 3 && !(valid & validNumObs)) s << " # / TYPES OF OBSERV\n";
            if(version >= 3 && !(valid & validSystemNumObs  )) s << " Sys / # / Obs Type\n";
            if(!(valid & validFirstTime)) s << " Time of First Obs\n";
            if(version >= 3.01 && !(valid & validSystemPhaseShift)) s << " SYS / PHASE SHIFT\n";
            if(version >= 3.01 && !(valid & validGlonassSlotFreqNo)) s << " GLONASS SLOT / FRQ #\n";
            if(version >= 3.02 && !(valid & validGlonassCodPhsBias)) s << " GLONASS COD/PHS/BIS\n";
            if(!(validEoH)) s << " END OF HEADER\n";
            s << "END Invalid header records." << endl;
        }

        s << "---------------------------------- OPTIONAL "
          << "----------------------------------" << endl;
        if(valid & validMarkerNumber     )
            s << "Marker number : " << markerNumber << endl;
        if(valid & validMarkerType       )
            s << "Marker type : " << markerType << endl;
        if(valid & validAntennaDeltaXYZ  )
            s << "Antenna Delta    (XYZ,m) : "
              << setprecision(4) << antennaDeltaXYZ   << endl;
        if(valid & validAntennaPhaseCtr  )
            s << "Antenna PhaseCtr (XYZ,m) : "
              << setprecision(4) << antennaPhaseCtr   << endl;
        if(valid & validAntennaBsightXYZ )
            s << "Antenna B.sight  (XYZ,m) : "
              << setprecision(4) << antennaBsightXYZ  << endl;
        if(valid & validAntennaZeroDirAzi)
            s << "Antenna ZeroDir  (deg)   : "
              << setprecision(4) << antennaZeroDirAzi << endl;
        if(valid & validAntennaZeroDirXYZ)
            s << "Antenna ZeroDir  (XYZ,m) : "
              << setprecision(4) << antennaZeroDirXYZ << endl;
        if(valid & validCenterOfMass     )
            s << "Center of Mass   (XYZ,m) : "
              << setprecision(4) << antennaPhaseCtr   << endl;
        if(valid & validSigStrengthUnit  )
            s << "Signal Strenth Unit = " << sigStrengthUnit << endl;
        if(valid & validInterval         )
            s << "Interval = "
              << fixed << setw(7) << setprecision(3) << interval << endl;
        if(valid & validLastTime         )
            s << "Time of Last Obs "
              << lastObs.asString() << endl;
        if(valid & validReceiverOffset   )
            s << "Clock offset record is present and offsets "
              << (receiverOffset ? "ARE" : "are NOT") << " applied." << endl;
        if(version < 3 && (valid & validWaveFact)) // TD extraWaveFactList
            s << "Wavelength factor L1: " << wavelengthFactor[0]
              << " L2: " << wavelengthFactor[1] << endl;
        if(valid & validSystemDCBSapplied)
        {
            for(i = 0; i < infoDCBS.size(); i++)
            {
                SatID rsid;
                rsid.fromString(infoDCBS[i].satSys);
                s << "System DCBS Correction Applied to " << rsid.systemString()
                  << " data using program " << infoDCBS[i].name << endl;
                s << " from source " << infoDCBS[i].source << "." << endl;
            }
        }
        if(valid & validSystemPCVSapplied)
        {
            for(i = 0; i < infoPCVS.size(); i++)
            {
                SatID rsid;
                rsid.fromString(infoPCVS[i].satSys);
                s << "System PCVS Correction Applied to " << rsid.systemString()
                  << " data using program " << infoPCVS[i].name << endl;
                s << " from source " << infoPCVS[i].source << "." << endl;
            }
        }
        if(valid & validSystemScaleFac   )
        {
            map<string, ScaleFacMap>::const_iterator mapIter;
            // loop over GNSSes
            for(mapIter = sysSfacMap.begin(); mapIter != sysSfacMap.end(); mapIter++)
            {
                SatID rsid;
                rsid.fromString(mapIter->first);
                s << rsid.systemString() << " scale factors applied:" << endl;
                map<TypeID,int>::const_iterator iter;
                // loop over scale factor map
                for(iter = mapIter->second.begin(); iter != mapIter->second.end(); iter++)
                    s << "   " << iter->first.asString() << " " << iter->second << endl;
            }
        }
        if(valid & validSystemPhaseShift )
        {
            map<string, map<TypeID, map<SatID,double> > >::const_iterator it;
            for(it=sysPhaseShift.begin(); it!=sysPhaseShift.end(); ++it)
            {
                string sys(it->first);
                map<TypeID, map<SatID, double> >::const_iterator jt;
                jt = it->second.begin();
                if(jt == it->second.end())
                    s << "Phase shift correction for system " << sys << " is empty." << endl;
                for( ; jt!=it->second.end(); ++jt)
                {
                    map<SatID,double>::const_iterator kt;
                    for(kt=jt->second.begin(); kt!=jt->second.end(); ++kt)
                        s << "Phase shift correction for system " << sys << ": "
                          << fixed << setprecision(5)
                          << setw(8) << kt->second << " cycles applied to obs type "
                          << jt->first.asString() << " "
                          << SatID(sys).systemString() << endl;
                }
            }
        }
        if(valid & validGlonassSlotFreqNo)
        {
            int n(0);
            map<SatID,int>::const_iterator it;
            s << "GLONASS frequency channels:\n";
            for(it=glonassFreqNo.begin(); it!=glonassFreqNo.end(); ++it)
            {
                s << " " << it->first.toString() << " " << setw(2) << it->second;
                if(++n > 1 && (n%8)==0) s << endl;
            }
            if((n%8) != 0) s << endl;
        }
        if(valid & validGlonassCodPhsBias)
        {
            map<TypeID,double>::const_iterator it;
            s << "GLONASS Code-phase biases:\n" << fixed << setprecision(3);
            for(it=glonassCodPhsBias.begin(); it!=glonassCodPhsBias.end(); ++it)
                s << " " << it->first.asString() << " " << setw(8) << it->second;
            s << endl;
        }
        if(valid & validLeapSeconds)
            s << "Leap seconds: " << leapSeconds << endl;
        if(valid & validNumSats)
            s << "Number of Satellites with data : " << numSVs << endl;
        if(valid & validPrnObs)
        {
            SatID sat, sys(-1,SatelliteSystem::Unknown);
            s << " PRN and number of observations for each obs type:" << endl;
            map<SatID, vector<int> >::const_iterator it = numObsForSat.begin();
            while (it != numObsForSat.end())
            {
                sat = it->first;
                if(sat.system != sys.system)
                {
                    // print a header: SYS  OT  OT  OT ...
                    s << " " << sat.systemString3() << " ";
                    iter = mapObsTypes.find(string(1,sat.systemChar()));
                    const vector<TypeID>& vec(iter->second);
                    for(i=0; i<vec.size(); i++)
                        s << setw(7) << vec[i].asString();
                    s << endl;
                    sys = sat;
                }
                vector<int> obsvec = it->second;
                s << " " << sat.toString() << " ";
                for(i = 0; i < obsvec.size(); i++)           // print the numbers of obss
                    s << " " << setw(6) << obsvec[i];
                s << endl;
                it++;
            }
        }
        if(commentList.size())
        {
            if(!(valid & validComment)) s << " Comment list is NOT valid" << endl;
            s << "Comments (" << commentList.size() << ") :" << endl;
            for(i=0; i<commentList.size(); i++) s << commentList[i] << endl;
        }

        s << "-------------------------------- END OF HEADER "
          << "--------------------------------" << endl;
    } // end dump


      /* This method returns the numerical index of a given observation
       *
       * @param type String representing the observation type.
       */
    size_t Rx3ObsHeader::getObsIndex( const string& type ) const
        noexcept(false)
    {
        string newType(type);

         // 'old-style' type: Let's change it to 'new style'.
        if( newType.size() == 2 )
        {
            if     ( newType == "C1" ) newType = "C1C";
            else if( newType == "P1" ) newType = "C1P";
            else if( newType == "L1" ) newType = "L1P";
            else if( newType == "D1" ) newType = "D1P";
            else if( newType == "S1" ) newType = "S1P";
            else if( newType == "C2" ) newType = "C2C";
            else if( newType == "P2" ) newType = "C2P";
            else if( newType == "L2" ) newType = "L2P";
            else if( newType == "D2" ) newType = "D2P";
            else if( newType == "S2" ) newType = "S2P";
            else
            {
                InvalidRequest exc("Invalid type.");
                THROW(exc);
            }
        }

         // Add GNSS code. By default the system is GPS
        if( newType.size() == 3 )
        {
            newType = newType + "G" ;
        }

         // Extract the GNSS from the newType
        string sys( newType, 3, 1 );
        return getObsIndex(sys, TypeID(newType));
    }


    size_t Rx3ObsHeader::getObsIndex(const string& sys,
                                        const TypeID& obsID) const
        noexcept(false)
    {

         // find the GNSS in the map
        SysTypesMap::const_iterator it = mapObsTypes.find(sys);

        if (it == mapObsTypes.end())
        {
            InvalidRequest ir("GNSS system " + sys + " not stored.");
            THROW(ir);
        }

        const TypeIDVec& rov = it->second;
        for (size_t i=0; i<rov.size(); i++)
        {
            if (rov[i] == obsID)
                return i;
        }

        InvalidRequest ir(obsID.asString() + " is not stored in system " + sys + ".");
        THROW(ir);
        return 0;
    }


    bool Rx3ObsHeader::compare(const Rx3ObsHeader& right,
                                  std::vector<std::string>& diffs,
                                  const std::vector<std::string>& inclExclList,
                                  bool incl)
    {
         // map header token to comparison result
        std::map<std::string,bool> lineMap;
        std::map<std::string,bool>::const_iterator lmi;
         // Put the comments in a sorted set, we don't really care
         // about the ordering.
        std::set<std::string>
            lcomments(commentList.begin(), commentList.end()),
            rcomments(right.commentList.begin(), right.commentList.end());
        std::set<TypeID>
            lobs(obsTypeList.begin(), obsTypeList.end()),
            robs(right.obsTypeList.begin(), right.obsTypeList.end());
         // Compare everything first...
         // deliberately ignoring valid flags

         // only comparing first character of file type because that's
         // all that matters according to RINEX
        lineMap[hsVersion] =
            ((version == right.version) &&
             (fileType[0] == right.fileType[0]) &&
             (fileSysSat.system == right.fileSysSat.system));
        lineMap[hsRunBy] =
            ((fileProgram == right.fileProgram) &&
             (fileAgency == right.fileAgency) &&
             (date == right.date));
        lineMap[hsComment] = (lcomments == rcomments);
        lineMap[hsMarkerName] = (markerName == right.markerName);
        lineMap[hsMarkerNumber] = (markerNumber == right.markerNumber);
        lineMap[hsMarkerType] = (markerType == right.markerType);
        lineMap[hsObserver] =
            ((observer == right.observer) &&
             (agency == right.agency));
        lineMap[hsReceiver] =
            ((recNo == right.recNo) &&
             (recType == right.recType) &&
             (recVers == right.recVers));
        lineMap[hsAntennaType] =
            ((antNo == right.antNo) &&
             (antType == right.antType));
        lineMap[hsAntennaPosition] =
            (antennaPosition == right.antennaPosition);
        lineMap[hsAntennaDeltaHEN] =
            (antennaDeltaHEN == right.antennaDeltaHEN);
        lineMap[hsAntennaDeltaXYZ] =
            (antennaDeltaXYZ == right.antennaDeltaXYZ);
        lineMap[hsAntennaPhaseCtr] =
            (antennaPhaseCtr == right.antennaPhaseCtr);
        lineMap[hsAntennaBsightXYZ] =
            (antennaBsightXYZ == right.antennaBsightXYZ);
        lineMap[hsAntennaZeroDirAzi] =
            (antennaZeroDirAzi == right.antennaZeroDirAzi);
        lineMap[hsAntennaZeroDirXYZ] =
            (antennaZeroDirXYZ == right.antennaZeroDirXYZ);
        lineMap[hsCenterOfMass] = (centerOfMass == right.centerOfMass);
        lineMap[hsNumObs] = (lobs == robs);
        lineMap[hsSystemNumObs] = true;
        lineMap[hsWaveFact] =
            (memcmp(wavelengthFactor, right.wavelengthFactor,
                 sizeof(wavelengthFactor)) == 0);
        lineMap[hsSigStrengthUnit] =
            (sigStrengthUnit == right.sigStrengthUnit);
        lineMap[hsInterval] = (interval == right.interval);
        lineMap[hsFirstTime] = (firstObs == right.firstObs);
        lineMap[hsLastTime] = (lastObs == right.lastObs);
        lineMap[hsReceiverOffset] = (receiverOffset == right.receiverOffset);
        lineMap[hsSystemDCBSapplied] = true;
        lineMap[hsSystemPCVSapplied] = true;
        lineMap[hsSystemScaleFac] = true;
        lineMap[hsSystemPhaseShift] = true;
        lineMap[hsGlonassSlotFreqNo] = true;
        lineMap[hsGlonassCodPhsBias] = true;
        lineMap[hsLeapSeconds] = (leapSeconds == right.leapSeconds);
        lineMap[hsNumSats] = (numSVs == right.numSVs);
        lineMap[hsPrnObs] = true;
         // ...then filter by inclExclList later
        if (incl)
        {
            std::map<std::string,bool> oldLineMap(lineMap);
            std::map<std::string,bool>::const_iterator olmi;
            lineMap.clear();
            for (unsigned i = 0; i < inclExclList.size(); i++)
            {
                if ((olmi = oldLineMap.find(inclExclList[i])) != oldLineMap.end())
                {
                    lineMap[olmi->first] = olmi->second;
                }
            }
        }
        else
        {
            // exclude, remove items in inclExclList
            for (unsigned i = 0; i < inclExclList.size(); i++)
            {
                lineMap.erase(inclExclList[i]);
            }
        }
         // check the equality of the final remaining set of header lines
        bool rv = true;
        for (lmi = lineMap.begin(); lmi != lineMap.end(); lmi++)
        {
            if (!lmi->second)
            {
                diffs.push_back(lmi->first);
                rv = false;
            }
        }
        return rv;
    } // bool Rx3ObsHeader::compare


} // namespace gnssSpace
