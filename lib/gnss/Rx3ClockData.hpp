
/**
 * @file Rx3ClockData.hpp
 * Encapsulate Rinex3Clock file data, including I/O
 *
 * modified from Rx3ClockData, but remove the fftextstream and
 * ffdata classes for easy using.
 *
 * shoujian zhang.
 */

#ifndef Rx3ClockData_HPP
#define Rx3ClockData_HPP

#include <iomanip>
#include "CommonTime.hpp"
#include "Rx3ClockHeader.hpp"

using namespace utilSpace;
using namespace timeSpace;

namespace gnssSpace
{
    /**
        This class encapsulates data for satellite clocks as found in
        RINEX Clock format files, and is used in conjuction with
        class Rinex3ClockStream, which handles the I/O, and
        Rx3ClockHeader, which holds information from the
        Rinex3Clock file header.
        
        @code
        std::fstream ss("igs14080.clk_30s");
        std::fstream ssout("myfile.clk_30s", ios::out);

        Rx3ClockData sd;
        
        while (true) 
        {
           // Interesting stuff...
          try
          {
            ss >> sd
            ssout << sd
          }
          catch(FFStreamError& e)
          {
            continue;
          }
          catch(EndOfFile& e)
          {
            break;
          };
        }
        @endcode

        see rx_clock_test.cpp for usage.
        
    */
   class Rx3ClockData                         
   {
   public:

         /// Constructor.
      Rx3ClockData()
          : headerRead(false)
      { clear(); }

         /// Destructor
      virtual ~Rx3ClockData() {}

         // The next four lines is our common interface
         /// Rx3ClockData is "data" so this function always returns true.
      virtual bool isData() const {return true;}

         /// Debug output function.
      virtual void dump(std::ostream& s=std::cout) const throw();

      bool headerRead;
      bool headerWrite;
      Rx3ClockHeader header;

      std::string datatype;   ///< Data type : AR, AS, etc
      SatID sat;         ///< Satellite ID        (if AS)
      std::string site;       ///< Site label (4-char) (if AR)
      CommonTime time;        ///< Time of epoch for this record
      double bias;            ///< Clock bias in seconds
      double sig_bias;        ///< Clock bias sigma in seconds
      double drift;           ///< Clock drift in sec/sec
      double sig_drift;       ///< Clock drift sigma in sec/sec
      double accel;           ///< Clock acceleration in 1/sec
      double sig_accel;       ///< Clock acceleration sigma in 1/sec

      void clear(void) throw()
      {
         datatype = std::string();
         sat = SatID(-1,SatelliteSystem::GPS);
         time = CommonTime::BEGINNING_OF_TIME;
         bias = sig_bias = drift = sig_drift = accel = sig_accel = 0.0;
      }

      virtual void reallyPutRecord(std::fstream& strm) 
         noexcept(false);

      virtual void reallyGetRecord(std::fstream& strm)
         noexcept(false);
   };


   // global re-define the operator >> for reading from file stream
   inline std::fstream& operator>>(std::fstream& strm, Rx3ClockData& data)
   {
       data.reallyGetRecord(strm);
       return strm;
   }

   // global re-define the operator << for reading from file stream
   inline std::fstream& operator<<(std::fstream& strm, Rx3ClockData& data)
   {
       data.reallyPutRecord(strm);
       return strm;
   }

}  // namespace

#endif // Rx3ClockData_HPP
