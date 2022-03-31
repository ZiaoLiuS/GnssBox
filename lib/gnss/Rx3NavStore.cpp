///////////////////////////////////////////////////////////////////////////////
//
// Read and store RINEX formated navigation message (Rinex3Nav) data, 
// following the RINEX 3.02 spec. Support for GNSS GPS, GAL, GLO, BDS, QZS.
//
///////////////////////////////////////////////////////////////////////////////

#include "Rx3NavStore.hpp"

using namespace std;
using namespace gnssSpace;

#define debug 0

namespace gnssSpace
{
    const string Rx3NavStore::stringVersion     = "RINEX VERSION / TYPE";
    const string Rx3NavStore::stringRunBy       = "PGM / RUN BY / DATE";
    const string Rx3NavStore::stringComment     = "COMMENT";
    const string Rx3NavStore::stringIonoCorr    = "IONOSPHERIC CORR";
    const string Rx3NavStore::stringTimeSysCorr = "TIME SYSTEM CORR";
    const string Rx3NavStore::stringLeapSeconds = "LEAP SECONDS";
    //R2.10GLO
    const string Rx3NavStore::stringCorrSysTime = "CORR TO SYSTEM TIME";
    //R2.11GPS
    const string Rx3NavStore::stringDeltaUTC    = "DELTA-UTC: A0,A1,T,W";
    //R2.11GEO
    const string Rx3NavStore::stringDUTC        = "D-UTC A0,A1,T,W,S,U";
    //R2.11
    const string Rx3NavStore::stringIonAlpha    = "ION ALPHA";
    //R2.11
    const string Rx3NavStore::stringIonBeta     = "ION BETA";
    const string Rx3NavStore::stringEoH         = "END OF HEADER";
    
    
    void Rx3NavStore::loadGPSEph(GPSEphemeris& gpsEph, string& line, fstream& navFileStream)
    {
        int prnID = stoi(line.substr(1, 2));
        SatID sat(SatelliteSystem::GPS, prnID);

        ///add each sat into the satTable
        vector<SatID>::iterator result = find(satTable.begin(),satTable.end(),sat);
        if(result==satTable.end())
        {
            satTable.push_back(sat);
        }

        int yr = stoi(line.substr(4, 4));
        int mo = stoi(line.substr(9, 2));
        int day = stoi(line.substr(12, 2));
        int hr = stoi(line.substr(15, 2));
        int min = stoi(line.substr(18, 2));
        double sec = stod(line.substr(21, 2));

        /// Fix RINEX epochs of the form 'yy mm dd hr 59 60.0'
        short ds = 0;
        if (sec >= 60.) {
            ds = sec;
            sec = 0;
        }
        CivilTime cvt(yr, mo, day, hr, min, sec);
        gpsEph.CivilToc = cvt;
        gpsEph.ctToe = cvt.convertToCommonTime();
        if (ds != 0) gpsEph.ctToe += ds;
        gpsEph.ctToe.setTimeSystem(TimeSystem::GPS);

        GPSWeekSecond gws(gpsEph.ctToe);     // sow is system-independent
        gpsEph.Toc = gws.sow;

        gpsEph.af0 = stod(line.substr(23, 19));
        gpsEph.af1 = stod(line.substr(42, 19));
        gpsEph.af2 = stod(line.substr(61, 19));

        ///orbit-1
        int n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.IODE = stod(line.substr(n, 19));
        n += 19;
        gpsEph.Crs = stod(line.substr(n, 19));
        n += 19;
        gpsEph.Delta_n = stod(line.substr(n, 19));
        n += 19;
        gpsEph.M0 = stod(line.substr(n, 19));
        ///orbit-2
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.Cuc = stod(line.substr(n, 19));
        n += 19;
        gpsEph.ecc = stod(line.substr(n, 19));
        n += 19;
        gpsEph.Cus = stod(line.substr(n, 19));
        n += 19;
        gpsEph.sqrt_A = stod(line.substr(n, 19));
        ///orbit-3
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.Toe = stod(line.substr(n, 19));
        n += 19;
        gpsEph.Cic = stod(line.substr(n, 19));
        n += 19;
        gpsEph.OMEGA_0 = stod(line.substr(n, 19));
        n += 19;
        gpsEph.Cis = stod(line.substr(n, 19));
        ///orbit-4
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.i0 = stod(line.substr(n, 19));
        n += 19;
        gpsEph.Crc = stod(line.substr(n, 19));
        n += 19;
        gpsEph.omega = stod(line.substr(n, 19));
        n += 19;
        gpsEph.OMEGA_DOT = stod(line.substr(n, 19));
        ///orbit-5
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.IDOT = stod(line.substr(n, 19));
        n += 19;
        gpsEph.L2Codes = stod(line.substr(n, 19));
        n += 19;
        gpsEph.GPSWeek = stod(line.substr(n, 19));
        n += 19;
        gpsEph.L2Pflag = stod(line.substr(n, 19));
        ///orbit-6
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.URA = stod(line.substr(n, 19));
        n += 19;
        gpsEph.SV_health = stod(line.substr(n, 19));
        n += 19;
        gpsEph.TGD = stod(line.substr(n, 19));
        n += 19;
        gpsEph.IODC = stod(line.substr(n, 19));
        ///orbit-7
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gpsEph.HOWtime = stod(line.substr(n, 19));
        n += 19;
        gpsEph.fitInterval = stod(line.substr(n, 19));
        n += 19;

        /// some process
        /// Some RINEX files have HOW < 0.
        while(gpsEph.HOWtime < 0)
        {
            gpsEph.HOWtime += (long)FULLWEEK;
            gpsEph.GPSWeek--;
        }

        /// In RINEX *files*, weeknum is the week of TOE.
        /// Internally (Rx3NavData), weeknum is week of HOW
        if(gpsEph.HOWtime - gpsEph.Toe > HALFWEEK)
            gpsEph.GPSWeek--;
        else if(gpsEph.HOWtime - gpsEph.Toe < -HALFWEEK)
            gpsEph.GPSWeek++;

        /// Get week for clock, to build Toc
        long adjHOWtime = gpsEph.HOWtime;
        short adjWeeknum = gpsEph.GPSWeek;
        long lToc = (long) gpsEph.Toc;
        if ((gpsEph.HOWtime % SEC_PER_DAY)==0 &&
            ((lToc)%SEC_PER_DAY)==0 &&
            gpsEph.HOWtime == lToc)
        {
            adjHOWtime = gpsEph.HOWtime - 30;
            if (adjHOWtime<0)
            {
                adjHOWtime += FULLWEEK;
                adjWeeknum--;
            }
        }

        double dt = gpsEph.Toc - adjHOWtime;
        int week = gpsEph.GPSWeek;
        if(dt < -HALFWEEK) week++; else if(dt > HALFWEEK) week--;
        gpsEph.ctToc = GPSWeekSecond(week, gpsEph.Toc, TimeSystem::GPS).convertToCommonTime();
        gpsEph.ctToc.setTimeSystem(TimeSystem::GPS);

        gpsEphData[sat][gpsEph.ctToe] = gpsEph;
    }

    void Rx3NavStore::loadBDSEph(BDSEphemeris& bdsEph, string& line, fstream& navFileStream)
    {
        int prnID = stoi(line.substr(1, 2));
        SatID sat(SatelliteSystem::BDS, prnID);

        if(debug)
        {
            cout << sat << endl;
        }

        /// add each sat into the satTable
        vector<SatID>::iterator result = find(satTable.begin(),satTable.end(),sat);
        if(result==satTable.end())
        {
           satTable.push_back(sat);
        }

        int yr = stoi(line.substr(4, 4));
        int mo = stoi(line.substr(9, 2));
        int day = stoi(line.substr(12, 2));
        int hr = stoi(line.substr(15, 2));
        int min = stoi(line.substr(18, 2));

        if(debug)
            cout << line.substr(21,2) << endl;

        double sec = stod(line.substr(21, 2));

        if(debug)
            cout << "sec:" << sec << endl;


        /// Fix RINEX epochs of the form 'yy mm dd hr 59 60.0'
        short ds = 0;
        if (sec >= 60.) {
            ds = sec;
            sec = 0;
        }

        CivilTime cvt(yr, mo, day, hr, min, sec);
        bdsEph.CivilToc = cvt;
        bdsEph.ctToe = cvt.convertToCommonTime();

        if(debug)
            cout << cvt << endl;

        if (ds != 0) bdsEph.ctToe += ds;
        bdsEph.ctToe.setTimeSystem(TimeSystem::BDT);

        GPSWeekSecond gws(bdsEph.ctToe);     // sow is system-independent
        bdsEph.Toc = gws.sow;

        bdsEph.af0 = stod(line.substr(23, 19));
        bdsEph.af1 = stod(line.substr(42, 19));
        bdsEph.af2 = stod(line.substr(61, 19));

        ///orbit-1
        int n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;
        replace(line.begin(),line.end(),'D','e');
        bdsEph.IODE = stod(line.substr(n, 19));
        n += 19;
        bdsEph.Crs = stod(line.substr(n, 19));
        n += 19;
        bdsEph.Delta_n = stod(line.substr(n, 19));
        n += 19;
        bdsEph.M0 = stod(line.substr(n, 19));

        ///orbit-2
        n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;
        replace(line.begin(),line.end(),'D','e');
        bdsEph.Cuc = stod(line.substr(n, 19));
        n += 19;
        bdsEph.ecc = stod(line.substr(n, 19));
        n += 19;
        bdsEph.Cus = stod(line.substr(n, 19));
        n += 19;
        bdsEph.sqrt_A = stod(line.substr(n, 19));

        ///orbit-3
        n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;
        replace(line.begin(),line.end(),'D','e');
        bdsEph.Toe = stod(line.substr(n, 19));
        n += 19;
        bdsEph.Cic = stod(line.substr(n, 19));
        n += 19;
        bdsEph.OMEGA_0 = stod(line.substr(n, 19));
        n += 19;

        if(debug)
            cout << line.substr(n,19) << endl;

        bdsEph.Cis = stod(line.substr(n, 19));
        
        ///orbit-4
        n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;

        replace(line.begin(),line.end(),'D','e');
        bdsEph.i0 = stod(line.substr(n, 19));
        n += 19;
        bdsEph.Crc = stod(line.substr(n, 19));
        n += 19;
        bdsEph.omega = stod(line.substr(n, 19));
        n += 19;
        bdsEph.OMEGA_DOT = stod(line.substr(n, 19));

        ///orbit-5
        n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;
        replace(line.begin(),line.end(),'D','e');
        bdsEph.IDOT = stod(line.substr(n, 19));
        n += 19;
        double spare1 = stod(line.substr(n, 19));
        n += 19;
        bdsEph.BDSWeek = stod(line.substr(n, 19));
        n += 19;
        double spare2;
        if(line.size()>57+4)
            spare2 = stod(line.substr(n, 19));
        else
            spare2 = 0.0;


        ///orbit-6
        n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;
        replace(line.begin(),line.end(),'D','e');
        bdsEph.URA = stod(line.substr(n, 19));
        n += 19;
        bdsEph.SV_health = stod(line.substr(n, 19));
        n += 19;
        bdsEph.TGD1 = stod(line.substr(n, 19));
        n += 19;
        bdsEph.TGD2 = stod(line.substr(n, 19));

        ///orbit-7
        n = 4;
        getline(navFileStream, line);
        if(debug)
            cout << line << endl;
        replace(line.begin(),line.end(),'D','e');
        bdsEph.HOWtime = stod(line.substr(n, 19));
        n += 19;
        bdsEph.IODC = stod(line.substr(n, 19));
        n += 19;

        /// some process
        /// Some RINEX files have HOW < 0.
        while(bdsEph.HOWtime < 0)
        {
            bdsEph.HOWtime += (long)FULLWEEK;
            bdsEph.BDSWeek--;
        }

        /// In RINEX *files*, weeknum is the week of TOE.
        /// Internally (Rx3NavData), weeknum is week of HOW
        if(bdsEph.HOWtime - bdsEph.Toe > HALFWEEK)
            bdsEph.BDSWeek--;
        else if(bdsEph.HOWtime - bdsEph.Toe < -HALFWEEK)
            bdsEph.BDSWeek++;


        /// Get week for clock, to build Toc
        double dt = bdsEph.Toc - bdsEph.HOWtime;
        int week = bdsEph.BDSWeek;
        if(dt < -HALFWEEK) week++; else if(dt > HALFWEEK) week--;
        BDSWeekSecond bdsws = BDSWeekSecond(week, bdsEph.Toc, TimeSystem::BDT);
        bdsws.adjustToYear(bdsEph.CivilToc.year);
        bdsEph.ctToc = CommonTime(bdsws.convertToCommonTime());

        bdsEphData[sat][bdsEph.ctToe] = bdsEph;
    }

    void Rx3NavStore::loadGalEph(GalEphemeris& galEph, string& line, fstream& navFileStream)
    {
        int prnID = stoi(line.substr(1, 2));
        SatID sat(SatelliteSystem::Galileo, prnID);

        ///add each sat into the satTable
        vector<SatID>::iterator result = find(satTable.begin(),satTable.end(),sat);
        if(result==satTable.end())
        {
            satTable.push_back(sat);
        }

        int yr = stoi(line.substr(4, 4));
        int mo = stoi(line.substr(9, 2));
        int day = stoi(line.substr(12, 2));
        int hr = stoi(line.substr(15, 2));
        int min = stoi(line.substr(18, 2));
        double sec = stod(line.substr(21, 2));

        /// Fix RINEX epochs of the form 'yy mm dd hr 59 60.0'
        short ds = 0;
        if (sec >= 60.) {
            ds = sec;
            sec = 0;
        }
        CivilTime cvt(yr, mo, day, hr, min, sec);
        galEph.CivilToc = cvt;
        galEph.ctToe = cvt.convertToCommonTime();
        if (ds != 0) galEph.ctToe += ds;
        galEph.ctToe.setTimeSystem(TimeSystem::GAL);

        GPSWeekSecond gws(galEph.ctToe);     // sow is system-independent
        galEph.Toc = gws.sow;

        galEph.af0 = stod(line.substr(23, 19));
        galEph.af1 = stod(line.substr(42, 19));
        galEph.af2 = stod(line.substr(61, 19));

        ///orbit-1
        int n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.IODE = stod(line.substr(n, 19));
        n += 19;
        galEph.Crs = stod(line.substr(n, 19));
        n += 19;
        galEph.Delta_n = stod(line.substr(n, 19));
        n += 19;
        galEph.M0 = stod(line.substr(n, 19));
        ///orbit-2
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.Cuc = stod(line.substr(n, 19));
        n += 19;
        galEph.ecc = stod(line.substr(n, 19));
        n += 19;
        galEph.Cus = stod(line.substr(n, 19));
        n += 19;
        galEph.sqrt_A = stod(line.substr(n, 19));
        ///orbit-3
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.Toe = stod(line.substr(n, 19));
        n += 19;
        galEph.Cic = stod(line.substr(n, 19));
        n += 19;
        galEph.OMEGA_0 = stod(line.substr(n, 19));
        n += 19;
        galEph.Cis = stod(line.substr(n, 19));
        ///orbit-4
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.i0 = stod(line.substr(n, 19));
        n += 19;
        galEph.Crc = stod(line.substr(n, 19));
        n += 19;
        galEph.omega = stod(line.substr(n, 19));
        n += 19;
        galEph.OMEGA_DOT = stod(line.substr(n, 19));
        ///orbit-5
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.IDOT = stod(line.substr(n, 19));
        n += 19;
        galEph.dataSource = stod(line.substr(n, 19));
        n += 19;
        galEph.GALWeek = stod(line.substr(n, 19));
        n += 19;
        ///orbit-6
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.URA = stod(line.substr(n, 19));
        n += 19;
        galEph.SV_health = stod(line.substr(n, 19));
        n += 19;
        galEph.TGD1 = stod(line.substr(n, 19));
        n += 19;
        galEph.TGD2 = stod(line.substr(n, 19));
        ///orbit-7
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        galEph.HOWtime = stod(line.substr(n, 19));
        n += 19;

        /// some process
        /// Some RINEX files have HOW < 0.
        while(galEph.HOWtime < 0)
        {
            galEph.HOWtime += (long)FULLWEEK;
            galEph.GALWeek--;
        }

        /// In RINEX *files*, weeknum is the week of TOE.
        /// Internally (Rx3NavData), weeknum is week of HOW
        if(galEph.HOWtime - galEph.Toe > HALFWEEK)
            galEph.GALWeek--;
        else if(galEph.HOWtime - galEph.Toe < -HALFWEEK)
            galEph.GALWeek++;

        /// Get week for clock, to build Toc
        double dt = galEph.Toc - galEph.HOWtime;
        int week = galEph.GALWeek;
        if(dt < -HALFWEEK) week++; else if(dt > HALFWEEK) week--;
        CommonTime gpstoc = GPSWeekSecond(week, galEph.Toc, TimeSystem::GPS).convertToCommonTime();
        galEph.ctToc = gpstoc;
        galEph.ctToc.setTimeSystem(TimeSystem::GAL);

        galEphData[sat][galEph.ctToe] = galEph;
    }

    void Rx3NavStore::loadGloEph(GloEphemeris& gloEph, string& line, fstream& navFileStream)
    {
        int prnID = stoi(line.substr(1, 2));
        SatID sat(SatelliteSystem::GLONASS, prnID);

        ///add each sat into the satTable
        vector<SatID>::iterator result = find(satTable.begin(),satTable.end(),sat);
        if(result==satTable.end())
        {
            satTable.push_back(sat);
        }

        int yr = stoi(line.substr(4, 4));
        int mo = stoi(line.substr(9, 2));
        int day = stoi(line.substr(12, 2));
        int hr = stoi(line.substr(15, 2));
        int min = stoi(line.substr(18, 2));
        double sec = stod(line.substr(21, 2));

        /// Fix RINEX epochs of the form 'yy mm dd hr 59 60.0'
        short ds = 0;
        if (sec >= 60.) {
            ds = sec;
            sec = 0;
        }
        CivilTime cvt(yr, mo, day, hr, min, sec);
        gloEph.CivilToc = cvt;
        gloEph.ctToe = cvt.convertToCommonTime();
        if(ds != 0) gloEph.ctToe += ds;
        gloEph.ctToe.setTimeSystem(TimeSystem::GLO);

        // TOC is the clock time
        GPSWeekSecond gws(gloEph.ctToe);         // sow is system-independent
        gloEph.Toc = gws.sow;

        gloEph.TauN   =      stod(line.substr(23,19));
        gloEph.GammaN =      stod(line.substr(42,19));
        gloEph.MFtime =(long)stod(line.substr(61,19));

        ///orbit-1
        int n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gloEph.px     =        stod(line.substr(n,19)); n+=19;
        gloEph.vx     =        stod(line.substr(n,19)); n+=19;
        gloEph.ax     =        stod(line.substr(n,19)); n+=19;
        gloEph.health = (short)stod(line.substr(n,19));

        ///orbit-2
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gloEph.py     =        stod(line.substr(n,19)); n+=19;
        gloEph.vy     =        stod(line.substr(n,19)); n+=19;
        gloEph.ay     =        stod(line.substr(n,19)); n+=19;
        gloEph.freqNum= (short)stod(line.substr(n,19));

        ///orbit-3
        n = 4;
        getline(navFileStream, line);
        replace(line.begin(),line.end(),'D','e');
        gloEph.pz     =        stod(line.substr(n,19)); n+=19;
        gloEph.vz     =        stod(line.substr(n,19)); n+=19;
        gloEph.az     =        stod(line.substr(n,19)); n+=19;
        gloEph.ageOfInfo =     stod(line.substr(n,19));

        gloEphData[sat][gloEph.ctToe] = gloEph;
    }


   void Rx3NavStore::loadFile(string& file)
   {

       rx3NavFile = file;
       if(rx3NavFile.size()==0)
       {
           cout<<"the nav file path is empty!"<<endl;
           exit(-1);
       }

       fstream navFileStream(rx3NavFile.c_str(),ios::in);
       int lineNumber(0);


       ///first, we should read nav head
       while (1)
       {
           string line;
           getline(navFileStream, line);

           if(debug)
               cout << "Rx3NavStore:" << line << endl;

           stripTrailing(line);

           if(line.length() == 0) continue;
           else if(line.length() < 60 || line.length() > 80)
           {
               cout<<"line.length is fault"<<endl;
               FFStreamError e("Invalid line length");
               THROW(e);
           }

           lineNumber++;

           string thisLabel(line, 60, 20);

           /// following is huge if else else ... endif for each record type
           if(thisLabel == stringVersion)
           {
               /// "RINEX VERSION / TYPE"
               version = stod(line.substr( 0,20));

               fileType = strip(line.substr(20,20));
               if(version >= 3)
               {                        // ver 3
                   if(fileType[0] != 'N' && fileType[0] != 'n')
                   {
                       FFStreamError e("File type is not NAVIGATION: " + fileType);
                       THROW(e);
                   }
                   fileSys = strip(line.substr(40,20));   // not in ver 2

               }
               fileType = "NAVIGATION";
           }
           else if(thisLabel == stringRunBy)
           {
               /// "PGM / RUN BY / DATE"
               fileProgram = strip(line.substr( 0,20));
               fileAgency = strip(line.substr(20,20));
               // R2 may not have 'UTC' at end
               date = strip(line.substr(40,20));
           }
           else if(thisLabel == stringComment)
           {
               /// "COMMENT"
               commentList.push_back(strip(line.substr(0,60)));
           }
           else if(thisLabel == stringIonoCorr)
           {
               /// "IONOSPHERIC CORR"
               string ionoCorrType = strip(line.substr(0,4));
               vector<double> ionoCorrCoeff;
               for(int i=0; i < 4; i++)
               {
                   double ionoCorr = stod(line.substr(5 + 12*i, 12));
                   ionoCorrCoeff.push_back(ionoCorr);
               }

               ionoCorrData[ionoCorrType]=ionoCorrCoeff;
           }
           else if(thisLabel == stringTimeSysCorr)
           {
               /// "TIME SYSTEM CORR"
               string timeSysCorrType = strip(line.substr(0,4));

               TimeSysCorr timeSysCorrValue;
               timeSysCorrValue.A0 = stod(line.substr(5,17));
               timeSysCorrValue.A1 = stod(line.substr(22,16));
               timeSysCorrValue.refSOW = stoi(line.substr(38,7));
               timeSysCorrValue.refWeek = stoi(line.substr(45,5));
               timeSysCorrValue.geoProvider = string(" ");
               timeSysCorrValue.geoUTCid = 0;

               timeSysCorrData[timeSysCorrType] = timeSysCorrValue;
           }
           else if(thisLabel == stringLeapSeconds)
           {
               /// "LEAP SECONDS"
               leapSeconds = asInt(line.substr(0,6));
               leapDelta = asInt(line.substr(6,6));      // R3 only
               leapWeek = asInt(line.substr(12,6));      // R3 only
               leapDay = asInt(line.substr(18,6));       // R3 only
           }
           else if(thisLabel == stringEoH)
           {
               /// "END OF HEADER"
               break;
           }
           else
           {
               cout<<thisLabel<<" is Unknown or unsupported label"<<endl;
               exit(-1);
           }
       }

       ///now, start read nav data
       while(navFileStream.peek()!=EOF)
       {
           string line;
           getline(navFileStream,line);

           if(debug)
               cout << "Rx3NavStore:" << line << endl;

           replace(line.begin(),line.end(),'D','e');

           if(line[0]=='G')
           {
               GPSEphemeris gpsEph;
               loadGPSEph(gpsEph, line, navFileStream);
           }
           if(line[0]=='E')
           {
               GalEphemeris galEph;
               loadGalEph(galEph, line, navFileStream);
           }
           if(line[0]=='R')
           {
               GloEphemeris gloEph;
               loadGloEph(gloEph, line, navFileStream);
           }
           if(line[0]=='C')
           {
               BDSEphemeris bdsEph;

               if(debug)
               {
                   cout << "before loadBDSEph" << endl;
               }

               loadBDSEph(bdsEph, line, navFileStream);

           }
       }
   }

   void Rx3NavStore::showEphNum()
   {
       int count(0);

       for(auto it:satTable)
       {
           if(it.system==SatelliteSystem::GPS)
           {
               count += gpsEphData[it].size();
               cout<<" "<<it<<" has "<<gpsEphData[it].size() << "eph data"<<endl;
           }
           if(it.system==SatelliteSystem::BDS)
           {
               count += bdsEphData[it].size();
               cout<<" "<<it<<" has "<<bdsEphData[it].size() << "eph data"<<endl;
           }
           if(it.system==SatelliteSystem::Galileo)
           {
               count += galEphData[it].size();
               cout<<" "<<it<<" has "<<galEphData[it].size() << "eph data"<<endl;
           }
           if(it.system==SatelliteSystem::GLONASS)
           {
               count += gloEphData[it].size();
               cout<<" "<<it<<" has "<<gloEphData[it].size() << "eph data"<<endl;
           }
       }
       cout<<"total eph num is: "<<count<<endl;
   }

   Xvt Rx3NavStore::getXvt(const SatID& sat, const CommonTime& epoch) 
   {
      Xvt xvt;
      CommonTime realEpoch;
      TimeSystem ts;
      if(sat.system == SatelliteSystem::GPS)
      {
          ts = TimeSystem::GPS;
          realEpoch = convertTimeSystem(epoch, ts);
          GPSEphemeris gpsEph = findGPSEphemeris(sat, realEpoch);
          xvt = gpsEph.svXvt(realEpoch);
      }
      else if(sat.system == SatelliteSystem::BDS)
      {
          ts = TimeSystem::BDT;
          realEpoch = convertTimeSystem(epoch, ts);
          BDSEphemeris bdsEph = findBDSEphemeris(sat, realEpoch);
          xvt = bdsEph.svXvt(sat, realEpoch);
      }
      else if(sat.system == SatelliteSystem::Galileo)
      {
          ts = TimeSystem::GAL;
          realEpoch = convertTimeSystem(epoch, ts);
          GalEphemeris galEph = findGalEphemeris(sat, realEpoch);
          xvt = galEph.svXvt(realEpoch);
      }
      else if(sat.system == SatelliteSystem::GLONASS)
      {
          ts = TimeSystem::GLO;
          realEpoch = convertTimeSystem(epoch, ts);
          GloEphemeris gloEph = findGloEphemeris(sat, realEpoch);
          xvt = gloEph.svXvt(realEpoch);
      }

      return xvt;
   };

    GPSEphemeris Rx3NavStore::findGPSEphemeris(const SatID& sat , const CommonTime& epoch)
    {
        GPSWeekSecond targetWS(epoch);
        for(auto it:gpsEphData[sat])
        {
            GPSWeekSecond ws(it.first);
            double diff = ws.sow - targetWS.sow;
            if( diff > -7200 && diff <7200)
            {
                return it.second;
            }
        }
    }

    BDSEphemeris Rx3NavStore::findBDSEphemeris(const SatID& sat , const CommonTime& epoch)
    {
        GPSWeekSecond targetWS(epoch);
        for(auto it:bdsEphData[sat])
        {
            GPSWeekSecond ws(it.first);
            double diff = ws.sow - targetWS.sow;
            if( diff > -7200 && diff <7200)
            {
                return it.second;
            }
        }
    }

    GalEphemeris Rx3NavStore::findGalEphemeris(const SatID& sat , const CommonTime& epoch)
    {
        GPSWeekSecond targetWS(epoch);
        for(auto it:galEphData[sat])
        {
            GPSWeekSecond ws(it.first);
            double diff = ws.sow - targetWS.sow;
            if( diff > -7200 && diff <7200)
            {
                return it.second;
            }
        }
    }

    GloEphemeris Rx3NavStore::findGloEphemeris(const SatID& sat , const CommonTime& epoch)
    {
        GPSWeekSecond targetWS(epoch);
        for(auto it:gloEphData[sat])
        {
            GPSWeekSecond ws(it.first);
            double diff = ws.sow - targetWS.sow;
            if( diff > -1800 && diff <1800)
            {
                return it.second;
            }
        }
    }

}  // namespace gnssSpace
