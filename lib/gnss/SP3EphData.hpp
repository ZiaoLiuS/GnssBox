#pragma ident "$Id$"

/**
 * @file SP3EphData.hpp
 * Encapsulate SP3 file data, versions a,b,c, including I/O
 */

#ifndef SP3EphData_HPP
#define SP3EphData_HPP


#include "SatID.hpp"
#include "Exception.hpp"
#include "CommonTime.hpp"
#include <iomanip>

using namespace utilSpace;
using namespace timeSpace;
using namespace gnssSpace;

namespace gnssSpace
{
      /**
       * This class encapsulates data for satellite orbits and clocks, including
       * positions, velocities and other orbit and estimation information read
       * as found in I/O of SP3 format (versions a, b, or c) files.
       *
       * This class is used in conjuction with class SP3Stream, which handles the I/O,
       * and SP3Header, which holds information from the SP3 file header.
       * Note that the version of SP3 is stored ONLY in the SP3Header object.
       * This version is set when an SP3 header is read into SP3Header, and it may
       * be set by the user using SP3Header::setVersion().
       * On output, SP3Stream uses the version stored in the SP3Header to determine
       * how SP3EphData (this object) is output.
       *
       * @code
       * std::fstream ss("igr14080.sp3");
       * SP3EphData sd;
       *
       * while (ss >> sd)
       * {
       *    // Interesting stuff...
       * }    
       *
       * std::fstream ssout("myfile.sp3", ios::out);
       * sh.setVersion(SP3Header::SP3c);
       * ssout << sh;
       * for(...) {
       *    // perhaps modify sd
       *    ssout << sd
       * }
       * @endcode
       *
       * @sa gnssSpace::SP3Header and gnssSpace::SP3Stream for more information.
       * @sa petest.cpp for an example.
       */
   class SP3EphData                 
   {
   public:
         /// Constructor.
      SP3EphData() 
          : RecType(' '), time(CommonTime::BEGINNING_OF_TIME),
            clockEventFlag(false), clockPredFlag(false),
            orbitManeuverFlag(false), orbitPredFlag(false),
            correlationFlag(false), 
            headerRead(false), 
            headerWrite(false),
            reachEOF(false)
         {}
     
         /// Destructor
      virtual ~SP3EphData() {}
     
         // The next four lines is our common interface
         /// SP3EphData is "data" so this function always returns true.
      virtual bool isData() const {return true;}

         /// Debug output function.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
      virtual void dump(std::ostream& s=std::cout, bool includeC=true) const throw();
#pragma clang diagnostic pop

      // header-related variables
      SP3EphHeader header;
      bool         headerRead;
      bool         headerWrite;

      CommonTime currentEpoch;
      std::string lastLine;
      std::vector<std::string> warnings;

      // memebers
      char RecType;    ///< Data type indicator. P position, V velocity, * epoch
      SatID sat;       ///< Satellite ID
      CommonTime time; ///< Time of epoch for this record
      double x[3];     ///< The three-vector for position | velocity (m | dm/s).
      double clk;      ///< The clock bias or drift for P|V (microsec|1).

      /// the rest of the member are for version c only
      int sig[4];      ///< Four-vector of integer exponents for estimated sigma 
                       ///< of position,clock or velocity,clock rate; sigma = base**n
                       ///< units are mm,psec or 10^-4 mm/sec,psec/sec); base in head.
                       ///< n is >= 0, and n = -1 means unknown (blank in file)
      bool clockEventFlag;    ///< clock event flag, 'E' in file
      bool clockPredFlag;     ///< clock prediction flag, 'P' in file
      bool orbitManeuverFlag; ///< orbit maneuver flag, 'M' in file
      bool orbitPredFlag;     ///< orbit prediction flag, 'P' in file

      bool reachEOF;
      int  numCol;

      /// data for optional P|V Correlation record
      bool correlationFlag;   ///< If true, on input: a correlation record was read;
                              ///< on output: stream should output correlation.
      unsigned sdev[4];  ///< std dev of 3 positions (XYZ,mm) and clock (psec)
                         ///< or velocities(10^-4 mm/sec) and clock rate (10^-4 ps/s)
      int correlation[6];///< elements of correlation matrix: xy,xz,xc,yz,yc,zc
      

         /// Writes the formatted record to the FFStream \a s.
         /// @warning This function is currently unimplemented
      virtual void reallyPutRecord(std::fstream& strm) 
         noexcept(false);

         /**
          * This function reads a record from the given FFStream.
          * If an error is encountered in retrieving the record, the 
          * stream is reset to its original position and its fail-bit is set.
          * @throws StringException when a StringUtils function fails
          * @throws FFStreamError when exceptions(failbit) is set and
          *  a read or formatting error occurs.  This also resets the
          *  stream to its pre-read position.
          */
      virtual void reallyGetRecord(std::fstream& strm) 
         noexcept(false);

   };

           // global re-define the operator >> for reading from file stream
    inline std::fstream& operator>>(std::fstream& strm, SP3EphData& data)
    {
        data.reallyGetRecord(strm);
        return strm;
    }

    // global re-define the operator << for reading from file stream
    inline std::fstream& operator<<(std::fstream& strm, SP3EphData& data)
    {
        data.reallyPutRecord(strm);
        return strm;
    }

}  // namespace

#endif
