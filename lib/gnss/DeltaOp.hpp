//
// Created by zero on 3/8/22.
//

#ifndef GNSSBOX_DELTAOP_HPP
#define GNSSBOX_DELTAOP_HPP

#include "SatelliteSystem.hpp"
#include "TypeID.hpp"
#include "Rx3ObsData.hpp"

namespace gnssSpace{
    class DeltaOp {

    public:
        virtual void addDiffType(SatelliteSystem::Systems systems, TypeID::ValueType type);

        virtual void Process(satTypeValueMap &rxDataRover, satTypeValueMap &rxDataBase);

        /// Clear all linear combinations.
        virtual DeltaOp& clearAll(void)
        {
            systemDiffs.clear();
            return (*this);
        };
    private:
        /// List of linear combinations to compute
        std::map<SatelliteSystem, std::list<TypeID>> systemDiffs;



    };
}


#endif //GNSSBOX_DELTAOP_HPP
