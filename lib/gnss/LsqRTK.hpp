//
// Created by zero on 3/2/22.
//

#ifndef GNSSBOX_LSQRTK_HPP
#define GNSSBOX_LSQRTK_HPP

#include "Rx3ObsData.hpp"
#include "StochasticModel.hpp"
#include "EquSysForPoint.hpp"
#include <Eigen/Eigen>

using namespace utilSpace;
using namespace Eigen;

namespace gnssSpace {
    class LsqRTK {
    public:


        /** Explicit constructor. Sets the default equation definition
         *  to be used when fed with GNSS data structures.
         */

        /// Returns an index identifying this object.
        virtual void defineEquations();

        LsqRTK()
                : firstTime(true), systemStr("G"), pOutStream(NULL) {
            defineEquations();
            // outType vector list
            typeVec.clear();
            typeVec.push_back("X");
            typeVec.push_back("Y");
            typeVec.push_back("Z");
        };

        void setSource(string markerName) {
            source = markerName;
        };

        /// Return a reference to a Rx3ObsData object after solving
        /// the previously defined equation system.
        ///
        ///@param gData    Data object holding the data.
        ///
        virtual Rx3ObsData &Process(Rx3ObsData &rxDataRover) noexcept(false);

        double getSolution(const TypeID &type,
                           const VectorXd &stateVec) const
        noexcept(false);

        Triple getDx() const
        noexcept(false) {
            return delta;
        };

        Triple getDxFixed() const
        noexcept(false) {
            return deltaFixed;
        };

        bool getIsFixed() const
        noexcept(false) {
            return isFixed;
        };

        // print comment for solution
        void printComment() const;

        // print solutions
        void printSolution(CommonTime &time, Triple &rcvPos) const;

        /// Return a string identifying this object.
        virtual std::string getClassName(void) const;

        virtual VectorXd fixAmbiguity(Rx3ObsData &rxDataRover, SatelliteSystem sys) noexcept(false);

        /// Destructor.
        virtual ~LsqRTK() {};

    private:

        bool firstTime;

        SourceID source;

        string systemStr;

        RandomWalkModel ionoStoModel;

        TypeID defaultObsType;
        SatelliteSystem defaultSys;

        EquSysForPoint equSys;

        VariableSet currentUnkSet;

        vector<string> typeVec;

        VectorXd solution;
        MatrixXd covMatrix;

        Triple delta;
        Triple deltaFixed;

        bool isFixed;

        std::fstream *pOutStream;
    };

}
#endif //GNSSBOX_LSQRTK_HPP