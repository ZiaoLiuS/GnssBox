/**
 * @file SP3EphHeader.hpp
 * Encapsulate header of SP3 file data, including I/O
 * modify from SP3EphHeader
 *
 * author 
 * 
 * shoujian zhang
 */

#ifndef SP3EphHeader_HPP
#define SP3EphHeader_HPP

#include <string>
#include <map>
#include <vector>
#include "SatID.hpp"
#include "CommonTime.hpp"
#include "TimeSystem.hpp"

using namespace utilSpace;
using namespace timeSpace;
using namespace gnssSpace;

namespace gnssSpace
{

      /// This class models the header for a SP3 file.
      ///
      /// @note A valid header MUST be read before data can be read from an SP3 file
      /// because the header contains the file version or format. The version in
      /// this Header is used by SP3Stream to determine the format of output SP3Data.
      ///
    class SP3EphHeader 
    {
    public:

         /// Supported SP3 versions (file formats) : 'a' 'b' or 'c'
         /// See the SP3 format definition documents.
        enum Version
        {
            undefined,        ///< Unknown or undefined SP3 file format
            SP3a,             ///< SP3 version a
            SP3b,             ///< SP3 version b (very similar to SP3a)
            SP3c,             ///< SP3 version c (contains a/b as a subset)
            SP3d              ///< SP3 version d
        };

         /// constructor
        SP3EphHeader()
            : version(undefined), numberOfEpochs(0),
              system(1, SatelliteSystem::GPS), timeSystem(TimeSystem::Any),
              basePV(0.0), baseClk(0.0)
        {}

         /// destructor
        virtual ~SP3EphHeader() {}

         /// access the version or file format
         /// @return the current Version
        Version getVersion(void) const throw() { return version; }

         /// access the version or file format as a character
         /// @return a character version of the current Version
        char versionChar(void) const throw()
        {
            char ch;
            switch(version)
            {
                case SP3a:
                    ch = 'a'; break;
                case SP3b:
                    ch = 'b'; break;
                case SP3c:
                    ch = 'c'; break;
                case SP3d:
                    ch = 'd'; break;
                case undefined: default:
                    ch = 'U'; break;
            };
            return ch;
        }

         /// access the version or file format as a string
         /// @return a string version of the current Version
        std::string versionString(void) const throw()
        {
            std::string str;
            switch(version)
            {
                case SP3a:
                    str = std::string("SP3a"); break;
                case SP3b:
                    str = std::string("SP3b"); break;
                case SP3c:
                    str = std::string("SP3c"); break;
                case SP3d:
                    str = std::string("SP3d"); break;
                case undefined: default:
                    str = std::string("Undefined"); break;
            };
            return str;
        }

         /// set the version or file format. Note that reading an SP3 file
         /// automatically sets the version in the SP3EphHeader object that is read.
         /// @param ver the Version to be assigned to this header
         /// @return the current Version
        Version setVersion(const Version ver) throw()
        {
            Version oldFormat = version;
            version = ver;
            return oldFormat;
        }

         /// return a string with time system name
        std::string timeSystemString() const throw()
        { return timeSystem.asString(); }

         // The next four lines is our common interface
         /// SP3EphHeader is a "header" so this function always returns true.
        virtual bool isHeader() const { return true; }

         /// Dump contents to an ostream
        virtual void dump(std::ostream& s=std::cout) const throw();


         ///@name data members
         //@{

         /// The SP3 version (file format) is initially undefined, but it will be
         /// assigned by reallyGetRecord() while reading, and may be reassigned
         /// by the user before writing.
        Version version;           ///< SP3 Version or file format
        bool containsVelocity;     ///< If true, file contains velocities
        CommonTime time;           ///< Time of first Epoch in file
        double epochInterval;      ///< Duration of Epoch in seconds
        int numberOfEpochs;        ///< Number of epochs in this file
        std::string dataUsed;      ///< Types of data input into the positions
        std::string coordSystem;   ///< Coordinate System of the data
        std::string orbitType;     ///< Type of Orbit Estimate
        std::string agency;        ///< Agency generating the Orbit

        // the following are specific to version 'c'
        SatID system;        ///< system of satellites in file, e.g. G for GPS
        TimeSystem timeSystem;  ///< Time system used
        double basePV;          ///< Base used in Pos or Vel (mm or 10**-4mm/sec)
        double baseClk;         ///< Base used in Clk or rate (psec or 10**-4psec/sec)
        /// Map<SatID,accuracy flag> (all SVs in file)
        std::map<SatID, short> satList;
        /// vector of 4 comment lines
        std::vector<std::string> comments;

         //@}
        friend class SP3Data;

        virtual void reallyPutRecord(std::fstream& strm) 
            noexcept(false);

        virtual void reallyGetRecord(std::fstream& strm)
            noexcept(false);

    }; // end class SP3EphHeader

        // global re-define the operator >> for reading from file stream
    inline std::fstream& operator>>(std::fstream& strm, SP3EphHeader& hdr)
    {
        hdr.reallyGetRecord(strm);
        return strm;
    }

    // global re-define the operator << for reading from file stream
    inline std::fstream& operator<<(std::fstream& strm, SP3EphHeader& hdr)
    {
        hdr.reallyPutRecord(strm);
        return strm;
    }


}  // namespace

#endif
