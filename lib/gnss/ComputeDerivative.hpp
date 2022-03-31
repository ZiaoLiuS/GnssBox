
/**
 * given the initial position, compute the dx/dy/dz/elevation/azimuth, etc.
 * 
 * shjzhang, wuhan
 */

#ifndef ComputeDerivative_HPP
#define ComputeDerivative_HPP


#include "XvtStore.hpp"
#include "Position.hpp"
#include "DataStructures.hpp"
#include "Rx3ObsData.hpp"

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
using namespace gnssSpace;

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
namespace gnssSpace
{

    class ComputeDerivative 
    {
    public:

        // given initial nominal position, compute the partial ComputeDerivatives for
        // observables, including dx/dy/dz/dcdt, and the elevation, azimuth,
        // etc.
        ComputeDerivative()
           : minElev(10.0)
        {
            nominalPos = Position(0.0,0.0,0.0,Position::Cartesian,NULL);
        };

         /// Explicit constructor, taking as input a Position object
         /// containing reference station coordinates.
        ComputeDerivative(const Position& staCoordinates);


        satTypeValueMap& Process( const CommonTime& time,
                                  satTypeValueMap& gData )
            noexcept(false);

        /** Return a satTypeValueMap object, adding the new data generated
         *  when calling a modeling object.
         */
        virtual Rx3ObsData& Process(Rx3ObsData& rRin)
            noexcept(false)
        {
            Process(rRin.currEpoch, rRin.stvData);
            return rRin;
        };


        /// Method to set satellite elevation cut-off angle. By default, it
        /// is set to 10 degrees.
        virtual ComputeDerivative& setCoordinates(Triple& coord)
        { 
            nominalPos = coord; 
            return (*this); 
        };


        /// Method to set satellite elevation cut-off angle. By default, it
        /// is set to 10 degrees.
        virtual ComputeDerivative& setMinElev(double newElevation)
        { 
            minElev = newElevation; 
            return (*this); 
        };


        /// Return a string identifying this object.
        virtual std::string getClassName(void) const;


        /// Destructor.
        virtual ~ComputeDerivative() {};


    protected:

         /// The elevation cut-off angle for accepted satellites.
         /// By default it is set to 10 degrees.
        double minElev;

         /// Station position
        Position nominalPos;

    }; // End of class 'ComputeDerivative'

      //@}

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
}  // End of namespace gnssSpace

#endif   // ComputeDerivative_HPP
