//
// Created by zero on 3/7/22.
//

#ifndef GNSSBOX_COMPUTEPREFIT_HPP
#define GNSSBOX_COMPUTEPREFIT_HPP

#include "Rx3ObsData.hpp"
#include "Rx3NavStore.hpp"

namespace gnssSpace{
    class ComputePrefit{
    public:
        ComputePrefit(){};

        virtual satTypeValueMap Process(const CommonTime& time,
                                   satTypeValueMap& gData);


        ~ComputePrefit(){};
    };
}


#endif //GNSSBOX_COMPUTEPREFIT_HPP
