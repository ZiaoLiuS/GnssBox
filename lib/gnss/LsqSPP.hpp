/**
 * @file LsqSPP.hpp
 * Class to compute the Least Mean Squares Solution
 *
 * author
 *
 * shoujian zhang
 */

#ifndef LsqSPP_HPP
#define LsqSPP_HPP

#include "Rx3ObsData.hpp"
#include "EquSysForPoint.hpp"
#include <Eigen/Eigen>

using namespace utilSpace;
using namespace Eigen;

namespace gnssSpace
{


   /* Class to compute the standard point positioning solution. By default,
    * this class only use GPS observables, if you want to use multi-gnss
    * observables, you should set them by using the method 'setSystems()'.
    * 
    * for ppp, only gps system is used to accelerate the computation speed.
    * but if want to estimate spp solution with different systems, you MUST
    * set the systems by yourself.
    *
    */

   class LsqSPP                                           
   {
   public:


         /** Explicit constructor. Sets the default equation definition
          *  to be used when fed with GNSS data structures.
          */
        LsqSPP()
            : firstTime(true), systemStr("G"), pOutStream(NULL)
        {
            defineEquations();
            // outType vector list
            typeVec.clear();
            typeVec.push_back("X");
            typeVec.push_back("Y");
            typeVec.push_back("Z");
        }; 

        /// Returns an index identifying this object.
        virtual void defineEquations() ;

        void setSource(string markerName)
        {
            source = markerName;
        };

        /// Return a reference to a Rx3ObsData object after solving
        /// the previously defined equation system.
        ///
        ///@param gData    Data object holding the data.
        ///
        virtual Rx3ObsData& Process(Rx3ObsData& rxData) noexcept(false);

        double getSolution( const TypeID& type, 
                            const VectorXd& stateVec ) const
          noexcept(false);

        Triple getDx() const
          noexcept(false)
        {
            return delta;
        };

        // print comment for solution
        void printComment() const;

        // print solutions
        void printSolution(CommonTime& time, Triple& rcvPos) const;

        /// Return a string identifying this object.
        virtual std::string getClassName(void) const;

        /// Destructor.
        virtual ~LsqSPP() {};

   private:

        bool firstTime;

        SourceID source;

        string systemStr;

        RandomWalkModel ionoStoModel;

        TypeID defaultObsType;
        SatelliteSystem defaultSys;
        
        EquSysForPoint equSys;

        VariableSet currentUnkSet;

        IonoRandomWalkModel ionoC1GStoModel;
        IonoRandomWalkModel ionoC1EStoModel;
        IonoRandomWalkModel ionoC2CStoModel;

        vector<string> typeVec;

        VectorXd solution;
        MatrixXd covMatrix;

        Triple delta;

        std::fstream* pOutStream;

   }; // End of class 'LsqSPP'

}  // End of namespace gnssSpace

#endif   // LsqSPP_HPP
