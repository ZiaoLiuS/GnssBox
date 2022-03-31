#pragma ident "$Id: $"

/**
 * @file ComputeElevWeights.hpp
 * This class computes satellites weights simply based on the elevation.
 */

#ifndef ComputeElevWeights_HPP
#define ComputeElevWeights_HPP

//============================================================================

//  Copyright
//  ---------
//
//  Shoujian Zhang, Wuhan University, 2012
//
//============================================================================


#include "DataStructures.hpp"
#include "Rx3ObsData.hpp"

namespace gnssSpace
{
      /** This class computes simple satellites weights only based on
       * the satellite elevation.
       *
       * It is meant to be used with the GNSS data structures objects
       * found in "DataStructures" class.
       *
       * A typical way to use this class follows:
       *
       * @code
       *   RinexObsStream rin("ebre0300.02o");
       *
       *      // Declare a GDS object
       *   Rx3ObsData gRin;
       *
       *      // Create a 'ComputeElevWeights' object
       *   ComputeElevWeights elevWeights;
       *
       *   while(rin >> gRin)
       *   {
       *      gRin >> elevWeights;
       *   }
       * @endcode
       *
       * The "ComputeElevWeights" object will visit every satellite in the
       * GNSS data structure that is "gRin" and will try to compute its weight
       * based on a simple algorithm.
       *
       * When used with the ">>" operator, this class returns the same incoming
       * data structure with the weights inserted along their corresponding
       * satellites. Be warned that if it is not possible to compute the
       * weight for a given satellite, it will be summarily deleted from
       * the data structure.
       *
       * @sa ComputeIURAWeights.hpp, ComputeMOPSWeights.hpp.
       */
    class ComputeElevWeights                        
    {
    public:

        /// Default constructor.
        ComputeElevWeights( double exp=2.0,
                            double gps=2.0,
                            double glo=1.5,
                            double gal=1.0,
                            double bds=1.0 )
            : expotential(exp),
              weightOfGPS(gps),
              weightOfGLO(glo),
              weightOfGAL(gal),
              weightOfBDS(bds)
        {};


        /** Return a satTypeValueMap object, adding the new data
         *  generated when calling this object.
         *
         * @param time      Epoch corresponding to the data.
         * @param gData     Data object holding the data.
         */
        virtual satTypeValueMap& Process( const CommonTime& time,
                                          satTypeValueMap& gData )
            noexcept(false);


        virtual void Process(Rx3ObsData& rData);


        /// Return a string identifying this object.
        virtual std::string getClassName(void) const;


        /// Destructor
        virtual ~ComputeElevWeights() {};


    private:

        /// expotential for weighting function
        double expotential;

        /// weight of GPS, default is 2.0
        double weightOfGPS;

        /// weight of GLONASS, default is 1.5
        double weightOfGLO;

        /// weight of Galileo, default is 2.0
        double weightOfGAL;

        /// weight of BDS, default is 1.0
        double weightOfBDS;

    }; // End of class 'ComputeElevWeights'

}  // End of namespace gnssSpace

#endif   // COMPUTEELEVATIONWEIGHTS_HPP
