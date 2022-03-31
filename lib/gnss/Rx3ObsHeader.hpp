
/**
 * @file Rx3ObsHeader.hpp
 * 
 * modified from gnssSpace::Rinex3ObsHeader, but we delete the 
 * comlicated fftextstream/ffdata class to make the 
 * code simple to use.
 * 
 * shoujian zhang, 2020-07-01, at wuhan.
 */

#ifndef Rx3ObsHeader_HPP
#define Rx3ObsHeader_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <iomanip>

#include "CivilTime.hpp"
#include "Triple.hpp"
#include "SatID.hpp"
#include "TypeID.hpp"

using namespace utilSpace;
using namespace timeSpace;
using namespace mathSpace;

namespace gnssSpace
{


      /**  
       *  RINEX OBS header implementation
       *
       * For each header line type, the following are listed in the
       * table below:
       *   1. The label
       *   2. Its usage in RINEX 2
       *   3. Its usage in RINEX 3
       *   4. The name for validity checking and header variable
       *      storage (see below)
       *   5. Variables (data members) storing the header information
       *
       * | Header Field         | v2  | v3  | Name              | Variables         |
       * | :------------------- | :-- | :-- | :---------------- | :---------------- |
       * | RINEX VERSION / TYPE | req | req | Version           | version           |
       * |                    ^ |   ^ |   ^ |                 ^ | fileType          |
       * |                    ^ |   ^ |   ^ |                 ^ | fileSys           |
       * |                    ^ |   ^ |   ^ |                 ^ | fileSysSat        |
       * |                    ^ |   ^ |   ^ |                 ^ | preserveVerType   |
       * | PGM / RUN BY / DATE  | req | req | RunBy             | fileProgram       |
       * |                    ^ |   ^ |   ^ |                 ^ | fileAgency        |
       * |                    ^ |   ^ |   ^ |                 ^ | date              |
       * |                    ^ |   ^ |   ^ |                 ^ | preserveDate      |
       * | MARKER NAME          | req | req | MarkerName        | markerName        |
       * | MARKER NUMBER        | opt | opt | MarkerNumber      | markerNumber      |
       * | MARKER TYPE          |  -  | req | MarkerType        | markerType        |
       * | OBSERVER / AGENCY    | req | req | Observer          | observer          |
       * |                    ^ |   ^ |   ^ |                 ^ | agency            |
       * | REC # / TYPE / VERS  | req | req | Receiver          | recNo             |
       * |                    ^ |   ^ |   ^ |                 ^ | recType           |
       * |                    ^ |   ^ |   ^ |                 ^ | recVers           |
       * | ANT # / TYPE         | req | req | AntennaType       | antNo             |
       * |                    ^ |   ^ |   ^ |                 ^ | antType           |
       * | APPROX POSITION XYZ  | req | req | AntennaPosition   | antennaPosition   |
       * | ANTENNA: DELTA H/E/N | req | req | AntennaDeltaHEN   | antennaDeltaHEN   |
       * | ANTENNA: DELTA X/Y/Z |  -  | opt | AntennaDeltaXYZ   | antennaDeltaXYZ   |
       * | ANTENNA: PHASECENTER |  -  | opt | AntennaPhaseCtr   | antennaSatSys     |
       * |                    ^ |   ^ |   ^ |                 ^ | antennaObsCode    |
       * |                    ^ |   ^ |   ^ |                 ^ | antennaPhaseCtr   |
       * | ANTENNA: B.SIGHT XYZ |  -  | opt | AntennaBsightXYZ  | antennaBsightXYZ  |
       * | ANTENNA: ZERODIR AZI |  -  | opt | AntennaZeroDirAzi | antennaZeroDirAzi |
       * | ANTENNA: ZERODIR XYZ |  -  | opt | AntennaZeroDirXYZ | antennaZeroDirXYZ |
       * | CENTER OF MASS: XYZ  |  -  | opt | CenterOfMass      | centerOfMass      |
       * | # / TYPES OF OBSERV  | req |  -  | NumObs            | |
       * | SYS / # / OBS TYPES  |  -  | req | SystemNumObs      | mapObsTypes       |
       * | WAVELENGTH FACT L1/2 | opt |  -  | WaveFact          | wavelengthFactor  |
       * |                    ^ |   ^ |   ^ |                 ^ | extraWaveFactList |
       * | SIGNAL STRENGTH UNIT |  -  | opt | SigStrengthUnit   | sigStrengthUnit   |
       * | INTERVAL             | opt | opt | Interval          | interval          |
       * | TIME OF FIRST OBS    | req | req | FirstTime         | firstObs          |
       * | TIME OF LAST OBS     | opt | opt | LastTime          | lastObs           |
       * | RCV CLOCK OFFS APPL  | opt | opt | ReceiverOffset    | receiverOffset    |
       * | SYS / DCBS APPLIED   |  -  | opt | SystemDCBSapplied | infoDCBS          |
       * | SYS / PCVS APPLIED   |  -  | opt | SystemPCVSapplied | infoPCVS          |
       * | SYS / SCALE FACTOR   |  -  | opt | SystemScaleFac    | sysSfacMap        |
       * | SYS / PHASE SHIFT    |  -  | req | SystemPhaseShift  | sysPhaseShift     |
       * | GLONASS SLOT / FRQ # |  -  | req | GlonassSlotFreqNo | glonassFreqNo     |
       * | GLONASS COD/PHS/BIS  |  -  | req | GlonassCodPhsBias | glonassCodPhsBias |
       * | LEAP SECONDS         | opt | opt | LeapSeconds       | leapSeconds       |
       * | # OF SATELLITES      | opt | opt | NumSats           | numSVs            |
       * | PRN / # OF OBS       | opt | opt | PrnObs            | numObsForSat      |
       * | END OF HEADER        | req | req | EoH (header only) | -                 |
       *
       * The "Name" column in the above table is used for both
       * validity flags and header strings.  For the header strings,
       * prepend "hs", e.g. "hsVersion" will give you the header field
       * label.  For validity flags, prepend valid,
       * e.g. "validVersion" would indicate the presence of the "RINEX
       * VERSION / TYPE" header record.
       */

      /**
       * This class models the header for a RINEX 3 Observation File.
       * see the test program in /tests/rinex_obs_hdr_test.cpp
       *
       * RINEX 2 is also supported.
       */

    typedef std::map<std::string, std::map<char, std::string> > stringCharStringMap;


    class Rx3ObsHeader 
    {
    public:

         /// A Simple Constructor.
        Rx3ObsHeader();

        // Clear (empty out) header, setting all data members to
        // default values 
        void clear();

         /// @name RINEX observation file header formatting strings
         ///@{
        static const std::string hsVersion;           ///< RINEX VERSION / TYPE
        static const std::string hsRunBy;             ///< PGM / RUN BY / DATE
        static const std::string hsComment;           ///< COMMENT
        static const std::string hsMarkerName;        ///< MARKER NAME
        static const std::string hsMarkerNumber;      ///< MARKER NUMBER
        static const std::string hsMarkerType;        ///< MARKER TYPE
        static const std::string hsObserver;          ///< OBSERVER / AGENCY
        static const std::string hsReceiver;          ///< REC # / TYPE / VERS
        static const std::string hsAntennaType;       ///< ANT # / TYPE
        static const std::string hsAntennaPosition;   ///< APPROX POSITION XYZ
        static const std::string hsAntennaDeltaHEN;   ///< ANTENNA: DELTA H/E/N
        static const std::string hsAntennaDeltaXYZ;   ///< ANTENNA: DELTA X/Y/Z
        static const std::string hsAntennaPhaseCtr;   ///< ANTENNA: PHASECENTER
        static const std::string hsAntennaBsightXYZ;  ///< ANTENNA: B.SIGHT XYZ
        static const std::string hsAntennaZeroDirAzi; ///< ANTENNA: ZERODIR AZI
        static const std::string hsAntennaZeroDirXYZ; ///< ANTENNA: ZERODIR XYZ
        static const std::string hsCenterOfMass;      ///< CENTER OF MASS: XYZ
        static const std::string hsNumObs;            ///< # / TYPES OF OBSERV
        static const std::string hsSystemNumObs;      ///< SYS / # / OBS TYPES
        static const std::string hsWaveFact;          ///< WAVELENGTH FACT L1/2
        static const std::string hsSigStrengthUnit;   ///< SIGNAL STRENGTH UNIT
        static const std::string hsInterval;          ///< INTERVAL
        static const std::string hsFirstTime;         ///< TIME OF FIRST OBS
        static const std::string hsLastTime;          ///< TIME OF LAST OBS
        static const std::string hsReceiverOffset;    ///< RCV CLOCK OFFS APPL
        static const std::string hsSystemDCBSapplied; ///< SYS / DCBS APPLIED
        static const std::string hsSystemPCVSapplied; ///< SYS / PCVS APPLIED
        static const std::string hsSystemScaleFac;    ///< SYS / SCALE FACTOR
        static const std::string hsSystemPhaseShift;  ///< SYS / PHASE SHIFT
        static const std::string hsGlonassSlotFreqNo; ///< GLONASS SLOT / FRQ #
        static const std::string hsGlonassCodPhsBias; ///< GLONASS COD/PHS/BIS
        static const std::string hsLeapSeconds;       ///< LEAP SECONDS
        static const std::string hsNumSats;           ///< # OF SATELLITES
        static const std::string hsPrnObs;            ///< PRN / # OF OBS
        static const std::string hsEoH;               ///< END OF HEADER
         ///@}

         /** Validity bits for the RINEX Observation Header - please
          * keep ordered as strings above */
        enum validBits
        {
            validVersion           =        0x1, ///< RINEX VERSION / TYPE
            validRunBy             =        0x2, ///< PGM / RUN BY / DATE
            validComment           =        0x4, ///< COMMENT
            validMarkerName        =        0x8, ///< MARKER NAME
            validMarkerNumber      =       0x10, ///< MARKER NUMBER
            validMarkerType        =       0x20, ///< MARKER TYPE
            validObserver          =       0x40, ///< OBSERVER / AGENCY
            validReceiver          =       0x80, ///< REC # / TYPE / VERS
            validAntennaType       =      0x100, ///< ANT # / TYPE
            validAntennaPosition   =      0x200, ///< APPROX POSITION XYZ
            validAntennaDeltaHEN   =      0x400, ///< ANTENNA: DELTA H/E/N
            validAntennaDeltaXYZ   =      0x800, ///< ANTENNA: DELTA X/Y/Z
            validAntennaPhaseCtr   =     0x1000, ///< ANTENNA: PHASECENTER
            validAntennaBsightXYZ  =     0x2000, ///< ANTENNA: B.SIGHT XYZ
            validAntennaZeroDirAzi =     0x4000, ///< ANTENNA: ZERODIR AZI
            validAntennaZeroDirXYZ =     0x8000, ///< ANTENNA: ZERODIR XYZ
            validCenterOfMass      =    0x10000, ///< CENTER OF MASS: XYZ
            validNumObs            =    0x20000, ///< # / TYPES OF OBSERV
            validSystemNumObs      =    0x20000, ///< SYS / # / OBS TYPES
            validWaveFact          =    0x40000, ///< WAVELENGTH FACT L1/2
            validSigStrengthUnit   =    0x40000, ///< SIGNAL STRENGTH UNIT
            validInterval          =    0x80000, ///< INTERVAL
            validFirstTime         =   0x100000, ///< TIME OF FIRST OBS
            validLastTime          =   0x200000, ///< TIME OF LAST OBS
            validReceiverOffset    =   0x400000, ///< RCV CLOCK OFFS APPL
            validSystemDCBSapplied =   0x800000, ///< SYSTEM DCBS APPLIED
            validSystemPCVSapplied =  0x1000000, ///< SYSTEM PCVS APPLIED
            validSystemScaleFac    =  0x2000000, ///< SYSTEM SCALE FACTOR
            validSystemPhaseShift  =  0x4000000, ///< SYS / PHASE SHIFT
            validGlonassSlotFreqNo =  0x8000000, ///< GLONASS SLOT / FRQ #
            validGlonassCodPhsBias = 0x10000000, ///< GLONASS COD/PHS/BIS
            validLeapSeconds       = 0x20000000, ///< LEAP SECONDS
            validNumSats           = 0x40000000, ///< # OF SATELLITES
            validPrnObs            = 0x80000000, ///< PRN / # OF OBS

            /// This mask is for all required valid fields - RINEX 2
            allValid2              = 0x001207CB,

            /// RINEX 3.0 for moving receivers -- make default
            allValid30             = 0x001205CB,

            // NB 19Jun2013 MGEX data does not include GLONASS SLOT
            // and GLONASS COD/PHS/BIS records
            // marker type is only required if the type is not GEODETIC or NON_GEODETIC
            allValid301            = 0x0012058B, ///< RINEX 3.01
            allValid302            = 0x0012058B, ///< RINEX 3.02
            allValid303            = 0x0012058B,  ///< RINEX 3.03
            allValid304            = 0x0012058B,  ///< RINEX 3.04
            allValid305            = 0x0012058B  ///< RINEX 3.05
        };

#ifndef SWIG // nested structs/classes not supported by SWIG
         /** RINEX 3 DCBS/PCVS info (for differential code bias and
          * phase center variations corr.) */
        struct Rinex3CorrInfo
        {
            std::string satSys;  ///< 1-char SV system (G/R/E/S)
            std::string name;    ///< program name used to apply corrections
            std::string source;  ///< source of corrections (URL)
        };

         /// RINEX 2 extra "WAVELENGTH FACT" lines
        struct ExtraWaveFact
        {
            /// List of Sats with this wavelength factor
            std::vector<SatID> satList;
            /// vector of wavelength factor values
            short wavelengthFactor[2];
        };
#endif

         /// Commonly used vector of strings
        typedef std::vector<std::string> StringVec;
        /// Simple vector of ints
        typedef std::vector<int> IntVec;

        /// @todo document me
        typedef std::map<SatID, IntVec> PRNNumObsMap;

        /** Scale Factor corrections for observations
         * map <ObsType, ScaleFactor> */
        typedef std::map<TypeID, int> ScaleFacMap;

        /** Per-system scale factor corrections.
         * Satellite system map of scale factor maps
         * <(G/R/E/S), <Rinex3ObsType, scalefactor>> */
        typedef std::map<std::string, ScaleFacMap> SysScaleFacMap;


         /// @todo document me
        typedef std::map<std::string, TypeID> TypeIDMap;

         /// @todo document me
        typedef std::map<std::string, TypeIDMap> VersionObsMap;

         /// Map satellite ID to phase shift
        typedef std::map<SatID, double> SVPhsShftMap;

         /// Map observation type to sv-phase shift map
        typedef std::map<TypeID, SVPhsShftMap> ObsPhsShftMap;

         /// Map system to observation type phase shift map
        typedef std::map<std::string, ObsPhsShftMap> SysPhsShftMap;

         /// Map GLONASS SV ID to frequency number
        typedef std::map<SatID, int> GLOFreqNumMap;

         /// Map GLONASS SV observable to code phase bias
        typedef std::map<TypeID, double> GLOCodPhsBias;

         /// Map SysChar + R2 Obs Type to Obs ID
        typedef std::map<std::string,std::string> DisAmbMap;

#ifndef SWIG

         /// Corrections (e.g. code bias) vector
        typedef std::vector<Rinex3CorrInfo> CorrVec;
         /// Vector of wavelength factors
        typedef std::vector<ExtraWaveFact> FactorVector;

#endif

         /** Storage for R2 <-> R3 conversion of obstypes during
          * reallyGet/Put Vector of strings containing ver 2 obs types
          * (e.g. "C1" "L2") defined in reallyGet; also defined in
          * PrepareVer2Write() from R3 ObsIDs */
        StringVec R2ObsTypes;

         /** map between RINEX ver 3 ObsIDs and ver 2 obstypes for
          * each system: reallyPut */
        VersionObsMap mapSysR2toR3ObsID;

         /** map Sys + R2ot to their ObsID origins*/
        DisAmbMap R2DisambiguityMap;


        double version;                 ///< RINEX 3 version/type
        std::string fileType;           ///< RINEX 3 file type
         /// file sys char: SatID system OR Mixed
        std::string fileSys;
         /** If false, the file type and system will be re-generated
          * in the gnss preferred format when writing the header,
          * otherwise the strings fileType fileSys will be written
          * unaltered */
        bool preserveVerType;
        SatID fileSysSat;               ///< fileSys as a SatID
        std::string fileProgram;        ///< program used to generate file
        std::string fileAgency;         ///< who ran program
        std::string date;               ///< when program was run
         /** If false, the current system time will be used when
          * writing the header, otherwise the value in date will be
          * used. */
        bool preserveDate;
        StringVec commentList;          ///< comments in header
        std::string markerName;         ///< MARKER NAME
        std::string markerNumber;       ///< MARKER NUMBER
        std::string markerType;         ///< MARKER TYPE
        std::string observer;           ///< who collected the data
        std::string agency;             ///< observer's agency
        std::string recNo;              ///< receiver number
        std::string recType;            ///< receiver type
        std::string recVers;            ///< receiver version
        std::string antNo;              ///< antenna number
        std::string antType;            ///< antenna type
        Triple antennaPosition;         ///< APPROX POSITION XYZ
        Triple antennaDeltaHEN;         ///< ANTENNA: DELTA H/E/N
        Triple antennaDeltaXYZ;         ///< ANTENNA: DELTA X/Y/Z
        std::string antennaSatSys;      ///< ANTENNA P.CTR BLOCK: SAT SYS
        std::string antennaObsCode;     ///< ANTENNA P.CTR BLOCK: OBS CODE
        Triple antennaPhaseCtr;         ///< ANTENNA P.CTR BLOCK: PCTR POS
        Triple antennaBsightXYZ;        ///< ANTENNA B.SIGHT XYZ
        double antennaZeroDirAzi;       ///< ANTENNA ZERODIR AZI
        Triple antennaZeroDirXYZ;       ///< ANTENNA ZERODIR XYZ
        Triple centerOfMass;            ///< vehicle CENTER OF MASS: XYZ

        SysTypesMap mapObsTypes;        ///< SYS / # / OBS TYPES

        short wavelengthFactor[2];      ///< WAVELENGTH FACT (system-wide)
#ifndef SWIG // nested structs/classes not supported by SWIG
        FactorVector extraWaveFactList;   ///< WAVELENGTH FACT (per SV)
#endif
        std::string sigStrengthUnit;    ///< SIGNAL STRENGTH UNIT
        double interval;                ///< INTERVAL
        CivilTime firstObs;             ///< TIME OF FIRST OBS
        CivilTime lastObs;              ///< TIME OF LAST OBS
        int receiverOffset;             ///< RCV CLOCK OFFS APPL
#ifndef SWIG // nested structs/classes not supported by SWIG
        CorrVec infoDCBS;               ///< DCBS INFO
        CorrVec infoPCVS;               ///< PCVS INFO
#endif
        SysScaleFacMap sysSfacMap;      ///< SYS / SCALE FACTOR
        SysPhsShftMap sysPhaseShift;    ///< SYS / PHASE SHIFT
        GLOFreqNumMap glonassFreqNo;    ///< GLONASS SLOT / FRQ #
        GLOCodPhsBias glonassCodPhsBias;///< GLONASS COD/PHS/BIS
        int leapSeconds;                ///< LEAP SECONDS
        short numSVs;                   ///< # OF SATELLITES
        PRNNumObsMap numObsForSat;      ///< PRN / # OF OBS


        /// number & types of observations R2 only
        ///@bug  this is being used but is not actually being filled
        TypeIDVec obsTypeList;

        std::map<std::string, std::map<std::string, std::vector<std::string>>> 
            codeSysObsTypes;

        std::map<std::string, std::map<std::string, std::vector<std::string>>> 
            phaseSysObsTypes;


        /// bits set when header rec.s present & valid
        unsigned long valid;
        /// true if found END OF HEADER
        bool validEoH;

        /// Map P to Y code observations in RINEX 2 files
        bool PisY;

        /// Used to help debug this class
        static int debug;

        /// Destructor
        virtual ~Rx3ObsHeader()
        {}

         /// Rx3ObsHeader is a "header" so this function always
         /// returns true.
        virtual bool isHeader() const
        { return true; }

         /// This is a simple Debug output function.
         /// It simply outputs the version, name and antenna number of this
         /// RINEX header.
        virtual void dump(std::ostream& s) const;


         /** This method returns the numerical index of a given observation
          *
          * @param[in] type String representing the observation type.
          */
        virtual std::size_t getObsIndex(const std::string& type ) const
            noexcept(false);

         /** This method returns the numerical index of a given observation
          *
          * @param[in] sys   GNSS system character for the obs
          * @param[in] obsID TypeID of the observation
          */
        virtual std::size_t getObsIndex( const std::string& sys,
                                         const TypeID& obsID ) const
            noexcept(false);

         /** Parse a single header record, and modify valid
          * accordingly.  Used by reallyGetRecord for both
          * Rx3ObsHeader and Rinex3ObsData. */
        void parseHeaderRecord(std::string& line)
            noexcept(false);

         /** Compute number of valid header records that
          * writeHeaderRecords() will write */
        int numberHeaderRecordsToBeWritten(void) const throw();

         /** Write all valid header records to the given stream.  Used
          * by reallyPutRecord for both Rx3ObsHeader and
          * Rinex3ObsData. */
        void writeHeaderRecords(std::fstream& strm) const
            noexcept(false);

         /// Return boolean : is this a valid Rinex header?
        bool isValid() const
        {
            if(!validEoH) return false;
            unsigned long allValid;
            if(     version < 3.00) allValid = allValid2;
            else if(version < 3.01) allValid = allValid30;
            else if(version < 3.02) allValid = allValid301;
            else if(version < 3.03) allValid = allValid302;
            else                    allValid = allValid303;
            return ((valid & allValid) == allValid);
        }

//       /** Compute map of obs types for use in writing version 2
//        * header and data */
//      void prepareVer2Write(void);

         /** Compare this header with another.
          * @param[in] right the header to compare this with.
          * @param[out] diffs The header strings/identifiers that are
          *   different between this and right.
          * @param[in] inclExclList a list of header strings to be ignored
          *   or used when making the comparison (e.g. "RINEX VERSION / TYPE").
          * @param[in] incl When true, only header lines listed in
          *   inclExclList will be compared.  When false, differences in
          *   header lines in inclExclList will be ignored.
          * @return true if this and right match given the
          *   included/excluded headers. */
        bool compare( const Rx3ObsHeader& right,
                      StringVec& diffs,
                      const StringVec& inclExclList,
                      bool incl = false );


         /// Helper methods
         /// The conversion between RINEX v2.11 to RINEX v3 observation
         /// type is fraught with system-specific idiosyncracies.   These
         /// methods read the list of v2.11 obs types stored in R2ObsTypes
         /// and attempt to build a corresponding list of v3 observation
         /// types where appropriate.
        std::vector<TypeID> mapR2ObsToR3Obs_G() noexcept(false);
        std::vector<TypeID> mapR2ObsToR3Obs_R() noexcept(false);
        std::vector<TypeID> mapR2ObsToR3Obs_E() noexcept(false);
        std::vector<TypeID> mapR2ObsToR3Obs_S() noexcept(false);

        friend class Rinex3ObsData;

        // write data record 
        virtual void reallyPutRecord(std::fstream& strm) 
            noexcept(false);

        // read data record
        virtual void reallyGetRecord(std::fstream& strm)
            noexcept(false);


    private:

         /** Converts the daytime \a dt into a Rinex Obs time
          * string for the header */
        std::string writeTime(const CivilTime& civtime) const;

         /** This function sets the time for this header.
          * It looks at \a line to obtain the needed information. */
        CivilTime parseTime(const std::string& line) const;

         /// save ObsID for cont. "PHASE SHIFT" R3.01
        TypeID sysPhaseShiftObsID;
         /// save the syschar while reading ScaleFactor
        std::string satSysTemp;
         /// recall the prev sat. sys for continuation lines.
        std::string satSysPrev;
         /// save OBS # / TYPES and Sys / SCALE FACTOR for continuation lines.
        int numObs;
         /// recall the prev # obs for cont. lines
        int numObsPrev;
         /// save PRN while reading PRN/OBS cont. lines
        SatID lastPRN;
         /// Scale factor holding data for continuation lines.
        int factor, factorPrev;

    }; // end class Rx3ObsHeader


    // global re-define the operator >> for reading from file stream
    inline std::fstream& operator>>(std::fstream& strm, Rx3ObsHeader& hdr)
    {
        hdr.reallyGetRecord(strm);
        return strm;
    }

    // global re-define the operator << for reading from file stream
    inline std::fstream& operator<<(std::fstream& strm, Rx3ObsHeader& hdr)
    {
        hdr.reallyPutRecord(strm);
        return strm;
    }

      //@}

} // namespace

#endif // Rx3ObsHeader_HPP
