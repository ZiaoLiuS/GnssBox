//
// Created by liu on 3/30/22.
//

#ifndef GNSSBOX_RTCM3NAVSTORE_HPP
#define GNSSBOX_RTCM3NAVSTORE_HPP

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

#include "Exception.hpp"
#include "StringUtils.hpp"
#include "Xvt.hpp"
#include "XvtStore.hpp"
#include "SatID.hpp"
#include "CommonTime.hpp"
#include "GPSEphemeris.hpp"
#include "BDSEphemeris.hpp"
#include "GalEphemeris.hpp"
#include "GloEphemeris.hpp"
#include "GPSWeekSecond.hpp"
#include "BDSWeekSecond.hpp"

#include "ConvertTime.hpp"


using namespace std;
using namespace utilSpace;
using namespace gnssSpace;

namespace gnssSpace {

    class Rtcm3NavStore : public XvtStore<SatID>{
    public:

        Rtcm3NavStore()
        {};

        Rtcm3NavStore(const std::string& navFile )
        {
            rx3NavFile = navFile;
        };


        void loadGPSEph(GPSEphemeris& gpsEph, string& line, fstream& navFile);
        void loadBDSEph(BDSEphemeris& bdsEph, string& line, fstream& navFile);
        void loadGalEph(GalEphemeris& galEph, string& line, fstream& navFile);
        void loadGloEph(GloEphemeris& gloEph, string& line, fstream& navFile);

        void loadFile(string& file);

        void showEphNum();

        Xvt getXvt(const SatID& sat, const CommonTime& epoch) ;

        GPSEphemeris findGPSEphemeris(const SatID& sat, const CommonTime& epoch);
        BDSEphemeris findBDSEphemeris(const SatID& sat, const CommonTime& epoch);
        GalEphemeris findGalEphemeris(const SatID& sat, const CommonTime& epoch);
        GloEphemeris findGloEphemeris(const SatID& sat, const CommonTime& epoch);



        struct TimeSysCorr
        {
            double A0;
            double A1;
            int refSOW ;
            int refWeek ;
            string geoProvider ;
            int geoUTCid ;
        };

        typedef map<string,vector<double>> ionoCorrMap;
        typedef map<string,TimeSysCorr> timeSysCorrMap;

        string  rx3NavFile;

        double version;                ///< RINEX Version
        std::string fileType;          ///< File type "N...."
        std::string fileSys;           ///< File system string
        std::string fileProgram;       ///< Program string
        std::string fileAgency;        ///< Agency string
        std::string date;              ///< Date string; includes "UTC" at the end
        std::vector<std::string> commentList;  ///< Comment list

        ionoCorrMap ionoCorrData;
        timeSysCorrMap timeSysCorrData;

        long leapSeconds;              ///< Leap seconds
        long leapDelta;                ///< Change in Leap seconds at ref time
        long leapWeek;                 ///< Week number of ref time
        long leapDay;                  ///< Day of week of ref time

        static const std::string stringVersion;      /// "RINEX VERSION / TYPE"
        static const std::string stringRunBy;        /// "PGM / RUN BY / DATE"
        static const std::string stringComment;      /// "COMMENT"
        // R3.x
        static const std::string stringIonoCorr;     /// "IONOSPHERIC CORR"
        static const std::string stringTimeSysCorr;  /// "TIME SYSTEM CORR"
        static const std::string stringLeapSeconds;  /// "LEAP SECONDS"
        static const std::string stringDeltaUTC;     /// "DELTA-UTC: A0,A1,T,W" // R2.11 GPS
        static const std::string stringCorrSysTime;  /// "CORR TO SYSTEM TIME"  // R2.10 GLO
        static const std::string stringDUTC;         /// "D-UTC A0,A1,T,W,S,U"  // R2.11 GEO
        static const std::string stringIonAlpha;     /// "ION ALPHA"            // R2.11
        static const std::string stringIonBeta;      /// "ION BETA"             // R2.11
        static const std::string stringEoH;          /// "END OF HEADER"

        ///tables that record the sat of different sat system
        ///in order to get the eph data num easily
        vector<SatID> satTable;

        map<SatID, std::map<CommonTime, GPSEphemeris>> gpsEphData;
        map<SatID, std::map<CommonTime, BDSEphemeris>> bdsEphData;
        map<SatID, std::map<CommonTime, GalEphemeris>> galEphData;
        map<SatID, std::map<CommonTime, GloEphemeris>> gloEphData;

        /// A debugging function that outputs in human readable form,
        /// all data stored in this object.
        /// @param[in] s the stream to receive the output; defaults to cout
        /// @param[in] detail the level of detail to provide
        void dump(std::ostream& s = std::cout, short detail = 0) const
        {
            cout << ""<< endl;
        };

        void edit(const CommonTime& tmin,
                  const CommonTime& tmax = CommonTime::END_OF_TIME)
        {};

        void clear(void)
        {};

        TimeSystem getTimeSystem(void) const
        {};

        virtual CommonTime getInitialTime(void) const
        {};

        virtual CommonTime getFinalTime(void) const
        {};

        virtual bool hasVelocity(void) const
        {};

        virtual bool isPresent(const SatID& id) const
        {};

        /// destructor
        virtual ~Rx3NavStore()
        {};

    };
}


#endif //GNSSBOX_RTCM3NAVSTORE_HPP
