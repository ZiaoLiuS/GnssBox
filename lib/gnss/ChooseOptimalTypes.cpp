
/**
 *@file ChooseOptimalTypes.cpp
 *
 * this class choose the optimal observation for current carrier-band according
 * to the defined priori order.
 *
 *author
 *
 *shoujian zhang, 2020
 **/

#include <string>
#include <iostream>


#include "Exception.hpp"
#include "ChooseOptimalTypes.hpp"

using namespace std;
using namespace utilSpace;

#define debug 0

namespace gnssSpace
{

   // this defines the priority order of all observed types
   // characters are ORDERED from best to worst

   // see references:
   // PRANGE L,ORLIAC E,DACH R,et al.
   // CODE's five-system orbit and clock solution:
   // The challenges of multi-GNSS data analysis[J].Journal of Geodesy,2017,91(4):345-360.

   // Notes:
   // according to PRANGE(2017), the priority order of the observed signals with a high 
   // availability are preferred (except GPS C1C)

   stringCharStringMap ChooseOptimalTypes::priorityCodes = ChooseOptimalTypes::init();
   
   // initialize
   stringCharStringMap ChooseOptimalTypes::init()
   {
       stringCharStringMap codes;

       // GPS
       codes["G"]['1'] = "WCSLXPYMN";   // except no C1N
       codes["G"]['2'] = "WCDSLXPYMN";  // except no C2N
       codes["G"]['5'] = "XQI";

       // BDS
       codes["C"]['1'] = "DPXAN"; // no C1N
       codes["C"]['2'] = "IXQ";
       codes["C"]['5'] = "DPX";
       codes["C"]['6'] = "IXQA";
       codes["C"]['8'] = "DPX";
       // BDS-2(IQX) and BDS-3(DPZ) B2b band
       codes["C"]['7'] = "DIXQPZ";

       // Glonass
       codes["R"]['1'] = "CP";
       codes["R"]['2'] = "PC";
       codes["R"]['3'] = "IQX";
       codes["R"]['4'] = "ABX";
       codes["R"]['6'] = "ABX";

       // Galileo
       codes["E"]['1'] = "XCABZ";
       codes["E"]['5'] = "XQI";
       codes["E"]['6'] = "XCABZ";
       codes["E"]['7'] = "XQI";
       codes["E"]['8'] = "XQI";

       // SBAS
       codes["S"]['1'] = "C";
       codes["S"]['5'] = "IQX";

       // QZSS
       codes["J"]['1'] = "XCSLZ";
       codes["J"]['2'] = "XLS";
       codes["J"]['5'] = "XQIDPZ";
       codes["J"]['6'] = "XSLEZ";

       // IRNSS
       codes["I"]['5'] = "ABCX";
       codes["I"]['9'] = "ABCX";

       return codes;
   };


   // Returns a string identifying this object.
   std::string ChooseOptimalTypes::getClassName() const
   { return "ChooseOptimalTypes"; }


   // Returns a Rx3ObsData object, filtering the target observables.
   //
   // @param gData     Data object holding the data.
   //
   SysTypesMap ChooseOptimalTypes::get( SysTypesMap mapObsTypes )
      noexcept(false)
   {
       try
       {

           SysTypesMap sysPrioriTypes;

           // only choose the optimal CODE(C) and PHASE(L) observables.
           string otStr = "CL";
           for(SysTypesMap::iterator sysTypeIter = mapObsTypes.begin();
                   sysTypeIter!=mapObsTypes.end();
                   sysTypeIter++)
           {

                std::vector<TypeID> prioriTypes{};
                std::vector<TypeID> obsTypes = (*sysTypeIter).second;

                string sysStr = (*sysTypeIter).first;

                std::map<char, std::string> bandCodes = priorityCodes[sysStr];
                for(auto bc: bandCodes)
                {
                   char band = bc.first;
                   string codes = bc.second;
     
                   // construct 4-char TypeID using obsType/band/trackingCode/sys
                   // e.g. C1CG, C1WG 
                   for(int i=0; i<otStr.size(); i++)
                   {
                       for(int j=0; j<codes.size(); j++)
                       {
                           char typeStr[4] ={0};  
                           typeStr[0] = otStr[i];
                           typeStr[1] = band;
                           typeStr[2] = codes[j];
                           typeStr[3] = sysStr[0];
   
                           // Rinex 3.04 don't have C1NG/C2NG/C1NC
                           if( string(typeStr) == "C1NG" || 
                               string(typeStr) == "C2NG" ||
                               string(typeStr) == "C1NC" ) 
                           {
                               continue;
                           }
   
                           TypeID type(typeStr);

                           if(debug)
                           {
                               cout << getClassName() << "type:" << type.asString() << endl;
                           }
   
                           // whether is stored in Rinex header
                           // vector/list don't have find method.
                           std::vector<TypeID>::iterator 
                               it = find(obsTypes.begin(), obsTypes.end(), type);
                           if( it != obsTypes.end())
                           {
                              prioriTypes.push_back(type);
                              break;
                           }
                       }
                   }

                }

                // replaece observed types with prioriTypes 
                sysPrioriTypes[(*sysTypeIter).first] = prioriTypes;
           }

           return sysPrioriTypes;

       }
       catch(Exception& u)
       {
           // Throw an exception if something unexpected happens
           ProcessingException e( getClassName() + ":"
                   + u.what() );

           THROW(e);
       }

   }  // End of 'ChooseOptimalTypes::Process()'



} // End of namespace gnssSpace
