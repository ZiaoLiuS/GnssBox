
/// @file SP3EphStore.cpp
/// Store a tabular list of position and clock bias (perhaps also velocity and clock
/// drift) data from SP3 file(s) for several satellites; access the tables to compute
/// values at any timetag, within the limits of the data, from this table via
/// interpolation.
/// An option allows assigning the clock store to RINEX clock files, with separate
/// interpolation algorithm.

#include <iostream>
#include <fstream>

#include "Exception.hpp"
#include "SatID.hpp"
#include "CommonTime.hpp"
#include "StringUtils.hpp"

#include "SP3EphHeader.hpp"
#include "SP3EphData.hpp"

#include "Rx3ClockHeader.hpp"
#include "Rx3ClockData.hpp"

#include "FileStore.hpp"
#include "ClockSatStore.hpp"
#include "PositionSatStore.hpp"

#include "SP3EphStore.hpp"

using namespace std;

#define debug 0

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
namespace gnssSpace
{
    // Returns the position, velocity, and clock offset of the indicated
    // object in ECEF coordinates (meters) at the indicated time.
    // param[in] sat the satellite of interest
    // param[in] ttag the time to look up
    // return the Xvt of the object at the indicated time
    // throw InvalidRequest If the request can not be completed for any
    //    reason, this is thrown. The text may have additional
    //    information as to why the request failed.
    Xvt SP3EphStore::getXvt(const SatID &sat, const CommonTime &ttag)
        noexcept(false)
    {
        PositionRecord prec;
        ClockRecord crec;

        try
        { prec = posStore.getValue(sat, ttag); }
        catch (InvalidRequest &e)
        {RETHROW(e); }

        try
        { crec = clkStore.getValue(sat, ttag); }
        catch (InvalidRequest &e)
        {RETHROW(e); }

        try
        {
            Xvt retXvt;
            for (int i = 0; i < 3; i++)
            {
                retXvt.x[i] = prec.Pos[i] * 1000.0;    // km -> m
                retXvt.v[i] = prec.Vel[i] * 0.1;       // dm/s -> m/s
            }
            if (useSP3clock)
            {                            // SP3
                retXvt.clkbias = crec.bias * 1.e-6;       // microsec -> sec
                retXvt.clkdrift = crec.drift * 1.e-6;     // microsec/sec -> sec/sec
            } else
            {                                       // RINEX clock
                retXvt.clkbias = crec.bias;               // sec
                retXvt.clkdrift = crec.drift;             // sec/sec
            }

            // compute relativity correction, in seconds
            retXvt.computeRelativityCorrection();

            return retXvt;
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }

    // Determine the earliest time for which this object can successfully
    // determine the Xvt for any object.
    // return the earliest time in the table
    // throw InvalidRequest if there is no data.
    CommonTime SP3EphStore::getInitialTime() const noexcept(false)
    {
        try
        {
            if (useSP3clock) return posStore.getInitialTime();

            CommonTime tc, tp;
            try
            { tc = clkStore.getInitialTime(); }
            catch (InvalidRequest &e)
            { tc = CommonTime::BEGINNING_OF_TIME; }
            try
            { tp = posStore.getInitialTime(); }
            catch (InvalidRequest &e)
            { tp = CommonTime::BEGINNING_OF_TIME; }
            return (tc > tp ? tc : tp);
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }

    // Determine the latest time for which this object can successfully
    // determine the Xvt for any object.
    // return the latest time in the table
    // throw InvalidRequest if there is no data.
    CommonTime SP3EphStore::getFinalTime() const noexcept(false)
    {
        try
        {
            if (useSP3clock) return posStore.getFinalTime();

            CommonTime tc, tp;
            try
            { tc = clkStore.getFinalTime(); }
            catch (InvalidRequest &e)
            { tc = CommonTime::END_OF_TIME; }
            try
            { tp = posStore.getFinalTime(); }
            catch (InvalidRequest &e)
            { tp = CommonTime::END_OF_TIME; }
            return (tc > tp ? tp : tc);
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }

// end of XvtStore interface

    // Return the position for the given satellite at the given time
    // param[in] sat the SatID of the satellite of interest
    // param[in] ttag the time (CommonTime) of interest
    // return Triple containing the position ECEF XYZ meters
    // throw InvalidRequest if result cannot be computed, for example because
    //  a) the time t does not lie within the time limits of the data table
    //  b) checkDataGap is true and there is a data gap
    //  c) checkInterval is true and the interval is larger than maxInterval
    Triple SP3EphStore::getPosition(const SatID sat, const CommonTime ttag)
    const noexcept(false)
    {
        try
        {
            PositionRecord prec;
            prec = posStore.getValue(sat, ttag);
            for (int i = 0; i < 3; i++)
                prec.Pos[i] *= 1000.0;    // km -> m
            return prec.Pos;
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }

    // Return the velocity for the given satellite at the given time
    // param[in] sat the SatID of the satellite of interest
    // param[in] ttag the time (CommonTime) of interest
    // return Triple containing the velocity ECEF XYZ meters/second
    // throw InvalidRequest if result cannot be computed, for example because
    //  a) the time t does not lie within the time limits of the data table
    //  b) checkDataGap is true and there is a data gap
    //  c) checkInterval is true and the interval is larger than maxInterval
    Triple SP3EphStore::getVelocity(const SatID sat, const CommonTime ttag)
    const noexcept(false)
    {
        try
        {
            PositionRecord prec;
            prec = posStore.getValue(sat, ttag);
            for (int i = 0; i < 3; i++)
                prec.Vel[i] *= 0.1;    // dm/s -> m/s
            return prec.Vel;
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }

    // Get the earliest time of data in the store for the given satellite.
    // Return the first time
    // Throw InvalidRequest if there is no data
    CommonTime SP3EphStore::getInitialTime(const SatID &sat)
    const noexcept(false)
    {
        try
        {
            if (useSP3clock) return posStore.getInitialTime(sat);

            CommonTime tc, tp;
            try
            { tc = clkStore.getInitialTime(sat); }
            catch (InvalidRequest &e)
            { tc = CommonTime::BEGINNING_OF_TIME; }
            try
            { tp = posStore.getInitialTime(sat); }
            catch (InvalidRequest &e)
            { tp = CommonTime::BEGINNING_OF_TIME; }
            return (tc > tp ? tc : tp);
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }

    // Get the latest time of data in the store for the given satellite.
    // Return the latest time
    // Throw InvalidRequest if there is no data
    CommonTime SP3EphStore::getFinalTime(const SatID &sat)
    const noexcept(false)
    {
        try
        {
            if (useSP3clock) return posStore.getFinalTime(sat);

            CommonTime tc, tp;
            try
            { tc = clkStore.getFinalTime(sat); }
            catch (InvalidRequest &e)
            { tc = CommonTime::END_OF_TIME; }
            try
            { tp = posStore.getFinalTime(sat); }
            catch (InvalidRequest &e)
            { tp = CommonTime::END_OF_TIME; }
            return (tc > tp ? tp : tc);
        }
        catch (InvalidRequest &e)
        {RETHROW(e); }
    }


    // This is a private utility routine used by the loadFile and loadSP3File routines.
    // Store position (velocity) and clock data from SP3 files in clock and position
    // stores. Also update the FileStore with the filename and SP3 header.
    void SP3EphStore::loadSP3Store(const string &filename, bool fillClockStore)
    noexcept(false)
    {
        try
        {
            // open the input stream
            std::fstream strm(filename.c_str());

            if (!strm)
            {
                Exception e("File " + filename + " could not be opened");
                THROW(e);
            }

            // declare header and data
            SP3EphHeader head;

            // read the SP3 ephemeris header
            try
            {
                strm >> head;
            }
            catch (Exception &e)
            {
                e.addText("Error reading header of file " + filename + e.getText());
                RETHROW(e);
            }
            //cout << "Read header" << endl; head.dump();

            // check/save TimeSystem to storeTimeSystem
            if (head.timeSystem != TimeSystem::Any && head.timeSystem != TimeSystem::Unknown)
            {
                // if store time system has not been set, do so
                if (storeTimeSystem == TimeSystem::Any)
                {
                    // NB. store-, pos- and clk- TimeSystems must always be the same
                    storeTimeSystem = head.timeSystem;
                    posStore.setTimeSystem(head.timeSystem);
                    clkStore.setTimeSystem(head.timeSystem);
                }

                    // if store system has been set, and it doesn't agree, throw
                else if (storeTimeSystem != head.timeSystem)
                {
                    InvalidRequest ir("Time system of file " + filename
                                      + " (" + head.timeSystem.asString()
                                      + ") is incompatible with store time system ("
                                      + storeTimeSystem.asString() + ").");
                    THROW(ir);
                }

            }  // end if header time system is set

            // save in FileStore
            SP3Files.addFile(filename, head);

            // define SP3EphData
            SP3EphData data;

            // read data
            bool isC(head.version == SP3EphHeader::SP3c);
            bool goNext, haveP, haveV, haveEP, haveEV, predP, predC;
            int i;
            CommonTime ttag;
            SatID sat;
            PositionRecord prec;
            ClockRecord crec;

            prec.Pos = prec.sigPos = prec.Vel = prec.sigVel
                    = prec.Acc = prec.sigAcc = Triple(0, 0, 0);

            if (fillClockStore)
            {
                crec.bias = crec.drift = crec.sig_bias = crec.sig_drift = 0.0;
                crec.accel = crec.sig_accel = 0.0;
            }

            try
            {
                haveP = haveV = haveEP = haveEV = predP = predC = false;
                goNext = true;

                while (1)
                {
                    try
                    {
                        strm >> data;
                    }
                    catch (EndOfFile &e)
                    {
                        break;
                    }

                    //cout << "Read data " << data.RecType
                    //<< " at " << printTime(data.time,"%Y %m %d %H %M %S") << endl;

                    while (1)
                    {
                        if (data.RecType == '*')
                        {
                            // epoch
                            if (haveP || haveV)
                            {
                                goNext = false;
                            }
                            else
                            {
                                ttag = data.time;
                                goNext = true;
                            }
                        } 
                        else if (data.RecType == 'P' && !data.correlationFlag)
                        {
                            // P
                            //cout << "P record: "; data.dump(cout); cout << endl;
                            if (haveP)
                                goNext = false;
                            else
                            {
                                sat = data.sat;
                                for (i = 0; i < 3; i++)
                                {
                                    prec.Pos[i] = data.x[i]; // km
                                    if (isC && data.sig[i] >= 0)
                                        prec.sigPos[i] = ::pow(head.basePV, data.sig[i]); // mm
                                    else
                                        prec.sigPos[i] = 0.0;
                                }

                                if (fillClockStore)
                                {
                                    crec.bias = data.clk; // microsec
                                    if (isC && data.sig[3] >= 0) // picosec -> msec
                                        crec.sig_bias = ::pow(head.baseClk, data.sig[3]) * 1.e-6;
                                }

                                if (data.orbitPredFlag) predP = true;
                                if (data.clockPredFlag) predC = true;

                                haveP = true;
                            }
                        } 
                        else if (data.RecType == 'V' && !data.correlationFlag)
                        {
                            // V
                            //cout << "V record: "; data.dump(cout); cout << endl;
                            if (haveV)
                                goNext = false;
                            else
                            {
                                for (i = 0; i < 3; i++)
                                {
                                    prec.Vel[i] = data.x[i]; // dm/s
                                    if (isC && data.sig[i] >= 0)
                                        prec.sigVel[i] =
                                                ::pow(head.basePV, data.sig[i]);  // 10-4mm/s
                                    else
                                        prec.sigVel[i] = 0.0;
                                }

                                if (fillClockStore)
                                {
                                    crec.drift = data.clk * 1.e-4; // 10-4micros/s -> micors/s
                                    if (isC && data.sig[3] >= 0)      // 10-4picos/s  -> micros/s
                                        crec.sig_drift = ::pow(head.baseClk, data.sig[3]) * 1.e-10;
                                }

                                if (data.orbitPredFlag)
                                    predP = true;
                                if (data.clockPredFlag)
                                    predC = true;

                                haveV = true;
                            }
                        } 
                        else if (data.RecType == 'P' && data.correlationFlag)
                        {
                            // EP
                            //cout << "EP record: "; data.dump(cout); cout << endl;
                            if (haveEP)
                                goNext = false;
                            else
                            {
                                for (i = 0; i < 3; i++)
                                    prec.sigPos[i] = data.sdev[i];
                                if (fillClockStore)
                                    crec.sig_bias = data.sdev[3] * 1.e-6;// picosec -> microsec

                                if (data.orbitPredFlag) predP = true;
                                if (data.clockPredFlag) predC = true;

                                haveEP = true;
                            }
                        } 
                        else if (data.RecType == 'V' && data.correlationFlag)
                        {
                            // EV
                            //cout << "EV record: "; data.dump(cout); cout << endl;
                            if (haveEV)
                                goNext = false;
                            else
                            {
                                for (i = 0; i < 3; i++)
                                    prec.sigVel[i] = data.sdev[i]; // 10-4mm/s

                                if (fillClockStore)
                                    crec.sig_drift = data.sdev[3] * 1.0e-10;// 10-4ps/s->micros/s

                                if (data.orbitPredFlag)
                                    predP = true;
                                if (data.clockPredFlag)
                                    predC = true;

                                haveEV = true;
                            }
                        } 
                        else
                        {
                            //cout << "other record (" << data.RecType << "):\n";
                            //data.dump(cout); cout << endl;
                            //throw?
                            goNext = true;
                        }

                        //cout << "goNext is " << (goNext ? "T":"F") << endl;
                        if (goNext)
                            break;

                        if (rejectBadPosFlag &&
                            (prec.Pos[0] == 0.0 ||
                             prec.Pos[1] == 0.0 ||
                             prec.Pos[2] == 0.0))
                        {
                            //cout << "Bad position" << endl;
                            haveP = haveV = haveEV = haveEP = false; // bad position record
                        } 
                        else if (fillClockStore && rejectBadClockFlag
                                   && crec.bias >= 999999.)
                        {
                            //cout << "Bad clock" << endl;
                            haveP = haveV = haveEV = haveEP = false; // bad clock record
                        } 
                        else
                        {
                            //cout << "Add rec: " << sat << " " << ttag << " " << prec<<endl;
                            if (!rejectPredPosFlag || !predP)
                                posStore.addPositionRecord(sat, ttag, prec);
                            if (fillClockStore && (!rejectPredClockFlag || !predC))
                                clkStore.addClockRecord(sat, ttag, crec);

                            // prepare for next
                            haveP = haveV = haveEP = haveEV = predP = predC = false;
                            prec.Pos = prec.Vel = prec.sigPos = prec.sigVel = Triple(0, 0, 0);
                            if (fillClockStore)
                                crec.bias = crec.drift = crec.sig_bias = crec.sig_drift = 0.0;
                        }

                        goNext = true;

                    }  // end while loop (loop twice)
                }  // end read loop

                if (haveP || haveV)
                {
                    if (rejectBadPosFlag &&
                        (prec.Pos[0] == 0.0 ||
                         prec.Pos[1] == 0.0 ||
                         prec.Pos[2] == 0.0))
                    {
                        //cout << "Bad last rec: position" << endl;
                        ;
                    } 
                    else if (fillClockStore && rejectBadClockFlag && crec.bias >= 999999.)
                    {
                        //cout << "Bad last rec: clock" << endl;
                        ;
                    } 
                    else
                    {
                        //cout << "Add last rec: "<< sat <<" "<< ttag <<" "<< prec << endl;
                        if (!rejectPredPosFlag || !predP)
                            posStore.addPositionRecord(sat, ttag, prec);
                        if (fillClockStore && (!rejectPredClockFlag || !predC))
                            clkStore.addClockRecord(sat, ttag, crec);
                    }
                }
            }
            catch (Exception &e)
            {
                e.addText("Error reading data of file " + filename);
                RETHROW(e);
            }

            // close
            strm.close();

        }
        catch (Exception &e)
        {
            RETHROW(e);
        }
        catch (...)
        {
            gnssSpace::Exception exc("Unknown exception");
            THROW(exc);
        }
    }


    // Load an SP3 ephemeris file; may set the velocity and acceleration flags.
    // If the clock store uses RINEX clock files, this ignores the clock data.
    void SP3EphStore::loadSP3File(const std::string &filename)
    noexcept(false)
    {
        try
        {
            loadSP3Store(filename, useSP3clock);
        }
        catch (Exception &e)
        {
            RETHROW(e);
        }
    }

    // Load a RINEX clock file; may set the 'have' bias and drift flags
    void SP3EphStore::loadRinexClockFile(const std::string &filename)
    noexcept(false)
    {
        try
        {
            if (useSP3clock) useRinexClockData();

            // open the input stream
            std::fstream strm(filename.c_str());
            if (!strm)
            {
                Exception e("File " + filename + " could not be opened");
                THROW(e);
            }

            // declare header and data
            Rx3ClockHeader head;
            Rx3ClockData data;

            // read the RINEX clock header
            try
            {
                strm >> head;
            }
            catch (Exception &e)
            {
                e.addText("Error reading header of file " + filename);
                RETHROW(e);
            }

            if(debug)
            {
                cout << "Read header" << endl; 
                head.dumpValid();
            }

            // check/save TimeSystem to storeTimeSystem
            if (head.timeSystem != TimeSystem::Any &&
                head.timeSystem != TimeSystem::Unknown)
            {
                // if store time system has not been set, do so
                if (storeTimeSystem == TimeSystem::Any)
                {
                    // NB. store-, pos- and clk- TimeSystems must always be the same
                    storeTimeSystem = head.timeSystem;
                    posStore.setTimeSystem(head.timeSystem);
                    clkStore.setTimeSystem(head.timeSystem);
                }

                    // if store system has been set, and it doesn't agree, throw
                else if (storeTimeSystem != head.timeSystem)
                {
                    InvalidRequest ir("Time system of file " + filename
                                      + " (" + head.timeSystem.asString()
                                      + ") is incompatible with store time system ("
                                      + storeTimeSystem.asString() + ").");
                    THROW(ir);
                }
            }  // end if header time system is set

                // there is no way to determine the time system....this is a problem TD
                // TD SP3EphStore::fixTimeSystem() ??
            else
            {
                head.timeSystem = TimeSystem::GPS;
                storeTimeSystem = head.timeSystem;
                posStore.setTimeSystem(head.timeSystem);
                clkStore.setTimeSystem(head.timeSystem);
            }

            // save in FileStore
            clkFiles.addFile(filename, head);

            // read data
            try
            {
                while(1) 
                {
                    try
                    {
                        strm >> data;
                    }
                    catch(FFStreamError& e)
                    {
                        continue;
                    }
                    catch(EndOfFile& e)
                    {
                        break;
                    }

                    if(debug)
                    {
                        data.dump(cout);
                    }

                    if (data.datatype == std::string("AS"))
                    {
                        data.time.setTimeSystem(head.timeSystem);
                        // add this data
                        ClockRecord rec;
                        rec.bias = data.bias;
                        rec.sig_bias = data.sig_bias;
                        rec.drift = data.drift;
                        rec.sig_drift = data.sig_drift;
                        rec.accel = data.accel;
                        rec.sig_accel = data.sig_accel;
                        clkStore.addClockRecord(data.sat, data.time, rec);
                    }
                }
            }
            catch (Exception &e)
            {
                e.addText("Error reading data of file " + filename);
                RETHROW(e);
            }

            strm.close();

        }
        catch (EndOfFile &e)
        {
            return;
        }
        catch (Exception &e)
        {
            RETHROW(e);
        }
    }

    //@}

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
}  // End of namespace gnssSpace
