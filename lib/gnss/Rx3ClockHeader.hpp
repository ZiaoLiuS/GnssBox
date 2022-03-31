
/**
 * @file Rx3ClockHeader.hpp
 * Encapsulate header of Rinex3Clock file data, including I/O
 *
 * modified from old Rinex3ClockHeader.
 * 
 * 2020-10-31
 * modify for rinex-3.04
 * 
 * author: shoujian zhang
 */

#ifndef Rx3ClockHeader_HPP
#define Rx3ClockHeader_HPP

#include <string>
#include <vector>
#include <map>
#include "SatID.hpp"
#include "TimeSystem.hpp"

using namespace utilSpace;
using namespace timeSpace;

namespace gnssSpace
{
      /// This class models the header for a RINEX Clock file.
      /// see rx3_clock_test.cpp
    class Rx3ClockHeader 
    {
    public:

         /// constructor
        Rx3ClockHeader()
            : version(3.0), leapSeconds(0), timeSystem(TimeSystem::Any),
              pcvsSystem(1, SatelliteSystem::GPS),
              numSolnStations(0), numSolnSatellites(0),
              firstLine(true)
        {}

         /// @name RINEX Clock Header Formatting Strings
         //@{
        static const std::string versionString;         ///< "RINEX VERSION / TYPE"
        static const std::string runByString;           ///< "PGM / RUN BY / DATE"
        static const std::string commentString;         ///< "COMMENT"
        static const std::string sysString;             ///< "SYS / # / OBS TYPES"
        static const std::string timeSystemString;      ///< "TIME SYSTEM ID"
        static const std::string leapSecondsString;     ///< "LEAP SECONDS"
        static const std::string leapSecondsGNSSString; ///< "LEAP SECONDS GNSS"
        static const std::string sysDCBString;          ///< "SYS / DCBS APPLIED"
        static const std::string sysPCVString;          ///< "SYS / PCVS APPLIED"
        static const std::string numDataString;         ///< "# / TYPES OF DATA"
        static const std::string stationNameString;     ///< "STATION NAME / NUM"
        static const std::string stationClockRefString; ///< "STATION CLK REF"
        static const std::string analysisCenterString;  ///< "ANALYSIS CENTER"
        static const std::string numClockRefString;     ///< "# OF CLK REF"
        static const std::string analysisClkRefrString; ///< "ANALYSIS CLK REF"
        static const std::string numReceiversString;    ///< "# OF SOLN STA / TRF"
        static const std::string solnStateString;       ///< "SOLN STA NAME / NUM"
        static const std::string numSolnSatsString;     ///< "# OF SOLN SATS"
        static const std::string prnListString;         ///< "PRN LIST"
        static const std::string endOfHeaderString;     ///< "END OF HEADER"
         //@}

         /// Validity bits for the RINEX Clock Header (** optional)
        enum validBits
        {
            versionValid            = 0x01, ///< "RINEX VERSION / TYPE"
            runByValid              = 0x02, ///< "PGM / RUN BY / DATE"
            commentValid            = 0x04, ///< "COMMENT" **
            sysValid                = 0x08, ///< "SYS / # / OBS TYPES" **

            timeSystemValid        = 0x010, ///< "TIME SYSTEM ID" **
            leapSecondsValid       = 0x020, ///< "LEAP SECONDS" **
            sysDCBValid            = 0x040, ///< "SYS / DCBS APPLIED" **
            sysPCVValid            = 0x080, ///< "SYS / PCVS APPLIED" **

            numDataValid          = 0x0100, ///< "# / TYPES OF DATA"
            stationNameValid      = 0x0200, ///< "STATION NAME / NUM" **
            stationClockRefValid  = 0x0400, ///< "STATION CLK REF" **
            analysisCenterValid   = 0x0800, ///< "ANALYSIS CENTER"

            numClockRefValid     = 0x01000, ///< "# OF CLK REF" **
            analysisClkRefrValid = 0x02000, ///< "ANALYSIS CLK REF" **
            numReceiversValid    = 0x04000, ///< "# OF SOLN STA / TRF"
            solnStateValid       = 0x08000, ///< "SOLN STA NAME / NUM"

            numSolnSatsValid    = 0x010000, ///< "# OF SOLN SATS"
            prnListValid        = 0x020000, ///< "PRN LIST"
            endOfHeaderValid    = 0x040000, ///< "END OF HEADER"

            //allRequiredValid    = 0x07F9A3, ///< this mask if for all required fields
            //allRequiredValid    = 0x07F903, ///< this mask if for all required fields
            //allRequiredValid    = 0x07C903, ///< this mask if for all required fields
            allRequiredValid    = 0x074903, ///< this mask if for all required fields
            allValid            = 0x07FFFF  ///< all the bits
        };

         /// destructor
        virtual ~Rx3ClockHeader() {}

         // The next four lines is our common interface
         /// Rx3ClockHeader is a "header" so this function always returns true.
        virtual bool isHeader() const { return true; }

         /** Dump information about the header to an ostream.
          * @param[in] os ostream to receive the output; defaults to std::cout
          * @param[in] detail integer level of detail to provide;
          *   allowed values are:
          *    0: all the header string except stations and satellites, but
          *       their num.
          *    1: above plus all the stations and satellites
          *    2: above plus all invalid header strings (dumpValid) */
        virtual void dump(std::ostream& s=std::cout, short detail = 0) const throw();

         /// Dump validity bits -> header strings
        void dumpValid(std::ostream& s=std::cout) const throw();

        double version;                        ///< Rinex3Clock Version or file format
        std::string program;                   ///< Program name
        std::string runby;                     ///< Run by string
        std::vector<std::string> dataTypes;    ///< list of data types
        int leapSeconds;                       ///< Leap seconds
        TimeSystem timeSystem;                 ///< Time system

        std::string analCenterDesignator;      ///< Analysis center designator (3 char)
        std::string analysisCenter;            ///< Analysis center
        std::string terrRefFrame;              ///< Terr Ref Frame or SINEX solution
        SatID pcvsSystem;                 ///< system (G=GPS, R=GLO) for PCVs
        std::string pcvsProgram;               ///< program used to apply PCVs
        std::string pcvsSource;                ///< source of applied PCVs

        int numSolnStations;                   ///< Number of stations in the solution
        std::map<std::string,std::string> stationID; ///< 4-char name, station id

        // NB these coordinates are often more than 32 bits -- cannot store as number!
        std::map<std::string,std::string> stationX;  ///< name, station X coord in mm
        std::map<std::string,std::string> stationY;  ///< name, station Y coord in mm
        std::map<std::string,std::string> stationZ;  ///< name, station Z coord in mm

        int numSolnSatellites;                  ///< Number of satellites in the soln
        std::vector<SatID> satList;        ///< List of sats (PRN LIST)

        std::vector<std::string> commentList;   ///< comments

        unsigned long valid;                    ///< valid bits for this header

        // read first line to judge the version
        bool firstLine;


        /// clear out the member data
        void clear(void)
        {
            version = 3.0;
            program = std::string();
            runby = std::string();
            dataTypes.clear();
            leapSeconds = 0;
            analysisCenter = std::string();
            terrRefFrame = std::string();
            timeSystem = TimeSystem::Any;
            pcvsSystem = SatID(-1, SatelliteSystem::GPS);
            pcvsProgram = std::string();
            pcvsSource = std::string();
            numSolnStations = 0;
            stationID.clear();
            stationX.clear();
            stationY.clear();
            stationZ.clear();
            numSolnSatellites = 0;
            satList.clear();
            commentList.clear();

            valid = 0;
        }

        virtual void reallyPutRecord(std::fstream& strm) 
            noexcept(false);

        virtual void reallyGetRecord(std::fstream& strm)
            noexcept(false);

    }; // end class Rx3ClockHeader


        // global re-define the operator >> for reading from file stream
    inline std::fstream& operator>>(std::fstream& strm, Rx3ClockHeader& hdr)
    {
        hdr.reallyGetRecord(strm);
        return strm;
    }

    // global re-define the operator << for reading from file stream
    inline std::fstream& operator<<(std::fstream& strm, Rx3ClockHeader& hdr)
    {
        hdr.reallyPutRecord(strm);
        return strm;
    }


}  // namespace

#endif // Rx3ClockHeader_HPP
