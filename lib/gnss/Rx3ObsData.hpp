
/**
 * @file Rx3ObsData.hpp
 * Encapsulate RINEX observation file data, including I/O
 *
 * 2020/05/01
 * Modified from gnssSpace::Rinex3ObsData, but remove complicated
 * inherited class FFTextStream:FFStream:fstream and FFData.
 *
 * this new class is easy to read and use.
 *
 * 2020/08/07
 * remove RinexDatum/RinexObsID 
 *
 * 2020/08/07
 * add dump for print out 
 *
 * 2020/09/02
 * add sourceRxData for multi-station gnss data processing
 *
 * Author:
 * Shoujian Zhang, 2020, Wuhan 
 */

#ifndef Rx3ObsData_HPP
#define Rx3ObsData_HPP

#include <vector>
#include <list>
#include <map>

#include "CommonTime.hpp"
#include "DataStructures.hpp"
#include "Rx3ObsHeader.hpp"

using namespace utilSpace;
using namespace timeSpace;

namespace gnssSpace
{

      /// @ingroup FileHandling
      //@{

      /// This class models a RINEX Observation Data Record.
      /// @sa gnssSpace::Rx3ObsHeader.
      /// @sa rx_obs_test.cpp for examples.
   class Rx3ObsData 
   {
   public:

      Rx3ObsData()
         : pHeader(NULL),
           currEpoch(CommonTime::BEGINNING_OF_TIME),
           epochFlag(-1),
           numSVs(-1),
           clockOffset(0.L),
           ffp(0)
      {}


         /// Destructor
      virtual ~Rx3ObsData() {}

      /// 是否已经读了头文件， 如果没有，将文件流重置, 读取头文件
/////////////////////
//    bool headerRead;
//    bool headerWrite;
/////////////////////

      /////////////////////////
      //
      // useful data for processing in other classes
      //
      /////////////////////////

      /// 在读取数据以前，先读取头文件，如果读取完成了，则直接读数据
      Rx3ObsHeader* pHeader;

      /// Time corresponding to the observations
      CommonTime currEpoch;

      /// Epoch flag has the following values
      /// 0 ok
      /// 1 power failure since previous epoch
      /// 2 start moving antenna
      /// 3 new site occupation (end moving antenna)
      ///   at least MARKER NAME header record follows
      /// 4 header records follow
      /// 5 external event
      /// 6 cycle slip record - same format as observation, but slips not data,
      ///   and LLI and SSI are blank
      short epochFlag;

      /// Number of satellites in this observation, except when
      /// epochFlag=2-5, then number of auxiliary header records to follow.
      short numSVs;

      /// receiver clock offset
      double clockOffset;        ///< optional clock offset in seconds

      int ffp;

      /// map to store the TypeIDs for each satellite
      std::map<SatID, TypeIDVec> satTypes;
      std::map<SatID, TypeIDVec> satShortTypes;

      /// observation data 
      satTypeValueMap stvData;

      /// lli data
      satTypeValueMap stvDataLLI;

      /// ssi data
      satTypeValueMap stvDataSSI;


      /// 读辅助行，比如接收机开始移动了等等（epochFlag 2-5）
      Rx3ObsHeader auxHeader; ///< auxiliary header records (epochFlag 2-5)

      /////////////////////////
      /////////////////////////
      /////////////////////////

      /// A Debug output function.
      /// Dumps the time of observations and the IDs of the Sats
      /// in the map.
      /// detail: 
      /// 0 --- only print the data 
      /// 1 --- print TypeID name and data
      /// type:
      /// 0 --- print observation data in the order defined by RinexHeader
      /// 1 --- print all the typeValueData in the order defined by TypeID
      void dumpRAW(std::ostream& s, int detail=0) const;


      std::string timeString() const
         noexcept(false);

      /// Writes a correctly formatted record from this data to stream \a s.
      /// When printing comment records, you'll need to format them
      /// correctly yourself. This means making sure that "COMMENT" is at
      /// the end of the line and that they're the correct length
      /// (<= 80 chrs).
      /// Also, make sure to correctly set the epochFlag to the correct
      /// number for the type of header data you want to write.
      virtual void writeRecord(std::fstream& strm) 
         noexcept(false);

      virtual void writeRecordVer2(std::fstream& strm) 
         noexcept(false);


      virtual void readRecord(std::fstream& strm)
         noexcept(false);

      virtual void readRecordByTime(std::fstream& strm,CommonTime current)
        noexcept(false);

      virtual void readRecordVer2(std::fstream& strm)
         noexcept(false);

      virtual void setCycleSlipLLI(satValueMap& satCycleSlipData)
      {
         for(auto& stv: stvData)
         {
             SatID sat = stv.first;

             int cycleSlipFlag = satCycleSlipData[sat]; 

             for(auto& tv: stv.second)
             {
                 if(tv.first.asString()[0] == 'L')
                 {
                     cout << "print out type " << endl;
                     cout << tv.first.asString() << endl;
                     
                     // copy the cycle slip flags into stvDataLLI
                     stvDataLLI[sat][tv.first] = cycleSlipFlag;
                 }
             }
         }
      };

      //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      // interface for easy reading and writing data into stvData
      //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

      // dump daata to ostream
      inline std::ostream& dump(std::ostream& s, int detail=1) const
      {
          s << YDSTime(currEpoch) << " ";
          s << epochFlag << " ";
          s << endl;
          if(detail)
          {
              s << "mapObsTypes" << endl;
              for(auto sys=(*pHeader).mapObsTypes.begin(); 
                       sys!=(*pHeader).mapObsTypes.end(); sys++)
              {
                  s << (*sys).first << " ";
                  for(auto type=(*sys).second.begin();
                           type!=(*sys).second.end();
                           type++)
                  {
                      s << (*type) << " ";
                  }
                  s << endl;
              }

              s << "satTypes:" << endl;
              for(auto satIt=satTypes.begin();
                       satIt!=satTypes.end(); 
                       satIt++)
              {
                  s << (satIt)->first << " ";
                  for(auto type=(*satIt).second.begin();
                           type!=(*satIt).second.end();
                           type++)
                  {
                      s << (*type) << " ";
                  }
                  s << endl;
              }
          }

          stvData.dump(s, detail);
          return s;
      };

      /// Return the number of satellites available.
      size_t numSats() 
      { return stvData.size(); }

      size_t numSats(const SatelliteSystem::Systems sys) 
      { return stvData.numSats(sys); }

      SatIDSet getSatID() const
      {
          SatIDSet satSet;
          satSet = stvData.getSatID();
          return satSet;
      };


      void keepOnlyTypeID(const TypeID& type)
      {
          stvData.keepOnlyTypeID(type);
      };

      void keepOnlyTypeID(const TypeIDSet& typeSet)
      {
          stvData.keepOnlyTypeID(typeSet);
      };

      // Return a satTypeValueMap with only these types of data.
      // @param typeSet Set (TypeIDSet) containing the types of data
      //                to be extracted.
      inline satValueMap extractTypeID(const TypeID& type) const
      {
          satValueMap theMap;
          for( auto stv: stvData )
          {

              typeValueMap::const_iterator itObs( stv.second.find(type) );
              if( itObs != stv.second.end() )
              {
                  theMap[stv.first] = (*itObs).second;
              }
          }

          return theMap;

      }  // End of method 'satTypeValueMap::extractTypeID()'


      /** Return the data value (double) corresponding to provided SatID
        *  and TypeID.
        *
        * @param satellite     Satellite to be looked for.
        * @param type          Type to be looked for.
        */
      double getValue( const SatID& satellite,
                       const TypeID& type ) const
          noexcept(false)
      { return stvData.getValue( satellite, type ); }


   private:


         /// Writes the CommonTime into RINEX 3 format.
         /// If it's a bad time, it will return blanks.
      std::string writeTime(const CommonTime& dt) const
         noexcept(false);


         /** This function constructs a CommonTime object from the given
          *  parameters.
          *
          * @param line The encoded time string found in the RINEX record.
          * @param hdr  The RINEX Observation Header object for the current
          *             RINEX file.
          */
      CommonTime parseTime( const std::string& line,
                            const Rx3ObsHeader& hdr,
                            const TimeSystem& ts) const
         noexcept(false);




   }; // End of class 'Rx3ObsData'

   // global re-define the operator >> for reading from file stream
   inline std::fstream& operator>>(std::fstream& strm, Rx3ObsData& data)
   {
       try
       {
            data.readRecord(strm);
            return strm;
       }
       catch(EndOfFile& e)
       {
           RETHROW(e);
       }
   }

   // global re-define the operator << for reading from file stream
   inline std::fstream& operator<<(std::fstream& strm, Rx3ObsData& data)
   {
       try
       {
          data.writeRecord(strm);
          return strm;
       }
       catch(EndOfFile& e)
       {
           RETHROW(e);
       }
   }

   typedef std::map<CommonTime, Rx3ObsData> epochRxDataMap;
   typedef std::map<SourceID, Rx3ObsData> sourceRxDataMap;

} // End of namespace gnssSpace

#endif   // Rx3ObsData_HPP
