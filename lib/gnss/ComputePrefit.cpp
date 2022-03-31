//
// Created by zero on 3/7/22.
//

#include "ComputePrefit.hpp"

satTypeValueMap ComputePrefit::Process(const CommonTime& time,
                                  satTypeValueMap& gData) {
    try{

        for(satTypeValueMap::iterator it = gData.begin();
            it != gData.end();
            ++it)
        {
//            if((*it).second.find(TypeID::rho)==(*it).second.end()||
//               (*it).second.find(TypeID::cdtSat)==(*it).second.end()||
//               (*it).second.find(TypeID::tropoSlant)==(*it).second.end()||
//               (*it).second.find(TypeID::relativity)==(*it).second.end()||
//               (*it).second.find(TypeID::gravDelay)==(*it).second.end()){
//                continue;
//            }


            SatID sat = (*it).first;
            double remainder = -(*it).second[TypeID::rho]+(*it).second[TypeID::cdtSat]-(*it).second[TypeID::tropoSlant]-(*it).second[TypeID::relativity]-(*it).second[TypeID::gravDelay];


            if(sat.system == SatelliteSystem::GPS){
                (*it).second[TypeID::prefitC1G]=(*it).second[TypeID::C1G]+remainder;
                (*it).second[TypeID::prefitC2G]=(*it).second[TypeID::C2G]+remainder;
                (*it).second[TypeID::prefitL1G]=(*it).second[TypeID::L1G]+remainder;
                (*it).second[TypeID::prefitL2G]=(*it).second[TypeID::L2G]+remainder;



            } else if(sat.system == SatelliteSystem::BDS){
                (*it).second[TypeID::prefitC2C]=(*it).second[TypeID::C2C]+remainder;
                (*it).second[TypeID::prefitC6C]=(*it).second[TypeID::C6C]+remainder;
                (*it).second[TypeID::prefitL2C]=(*it).second[TypeID::L2C]+remainder;
                (*it).second[TypeID::prefitL6C]=(*it).second[TypeID::L6C]+remainder;
            }


        }
    }catch (SVNumException& u){

    }

    return gData;
}
