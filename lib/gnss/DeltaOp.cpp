//
// Created by zero on 3/8/22.
//

#include "DeltaOp.hpp"

namespace gnssSpace{

    void DeltaOp::addDiffType(gnssSpace::SatelliteSystem::Systems systems, gnssSpace::TypeID::ValueType type) {
        systemDiffs[systems].push_back(type);

    }

    void DeltaOp::Process(satTypeValueMap &rxDataRover, satTypeValueMap &rxDataBase) {
        SatIDSet satRejectedSet;
        list<TypeID> difflist;


        try {
            for (satTypeValueMap::iterator it = rxDataRover.begin();
                 it != rxDataRover.end();
                 ++it) {
                if(rxDataBase.find(it->first)==rxDataBase.end()){
                    satRejectedSet.insert(it->first);
                    continue;
                }

                difflist = systemDiffs[(*it).first.system];

                // Loop through all the defined linear combinations
                for (auto pos = difflist.begin(); pos != difflist.end(); ++pos) {

                    double result(0.0);

                    bool valid( true );

                    result = it->second.getValue(pos->type)-rxDataBase[it->first].getValue(pos->type);
                    string type = pos->asString()+"Diff";
                    it->second[type] = result;
                }

                it->second[TypeID::N1]=1;
                it->second[TypeID::N2]=1;
            }


        }catch (Exception e){
            cout<<0;
        }

        rxDataRover.removeSatID(satRejectedSet);


    }
}