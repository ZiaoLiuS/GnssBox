
/**
 * function
 *
 * compute satellite position, velocity and relativity corrections.
 *
 * modified from BasicModel.hpp
 *
 * copyright(c)
 *
 * Shoujian zhang, 2020, Wuhan
 */


#ifndef ComputeSatPos_HPP
#define ComputeSatPos_HPP

#include "XvtStore.hpp"
#include "Position.hpp"
#include "Rx3ObsData.hpp"
#include "Counter.hpp"

using namespace utilSpace;
using namespace coordSpace;
using namespace gnssSpace;

namespace gnssSpace
{

      /** This class is used to compute gnss satellite's relativity correction, 
       *  satellite position and velocity at transmission time.
       *
       * A typical way to use this class follows:
       *
       * @code
       * 
       * @endcode
       *
       *
       */
    class ComputeSatPos                         
    {
    public:

         /// Default constructor. Observable C1 will be used for computations
         /// and satellites with elevation less than 10 degrees will be
         /// deleted.
        ComputeSatPos()
            : pEphStore(NULL) 
        {
            beginTime=Counter::now();
        };


         /** Explicit constructor, ephemeris to be used and whether TGD will
          *  be computed or not.
          *
          * @param dEphemeris    EphemerisStore object to be used by default.
          * @param dObservable   Observable type to be used by default.
          * @param applyTGD      Whether or not C1 observable will be
          *                      corrected from TGD effect.
          *
          */
        ComputeSatPos( XvtStore<SatID>& ephStore)
        {
            pEphStore = &ephStore;
        };


         /** Return a satTypeValueMap object, adding the new data generated
          *  when calling a modeling object.
          *
          * @param time      Epoch.
          * @param gData     Data object holding the data.
          */
        virtual satTypeValueMap& Process(const CommonTime& time,
                                         std::map<SatID, TypeIDVec>& satShortTypes,
                                         satTypeValueMap& gData)
            noexcept(false);


        virtual Rx3ObsData& Process(Rx3ObsData& rRin)
            noexcept(false)
        {
            Process(rRin.currEpoch, rRin.satShortTypes, rRin.stvData);
        };


        virtual Xvt ComputeAtTransmitTime(const CommonTime& tr,
                                          const double& pr,
                                          const SatID& sat)
            noexcept(false);

        virtual void setRxPos(const Position& pos)
        {
            rxPos = pos;
        };

        virtual void rotateEarth(Xvt& svPosVel);


        void printTimeUsed(std::ostream& os)
        {
            os << timeUsed << endl;
        };


         /// Return a string identifying this object.
        virtual std::string getClassName(void) const;


         /// Destructor.
        virtual ~ComputeSatPos() {};


    protected:


        double beginTime;
        double endTime;
        double timeUsed;

        Position    rxPos;


        /// Pointer to XvtStore<SatID> object
        XvtStore<SatID>* pEphStore;

    }; // End of class 'ComputeSatPos'

}  // End of namespace gnssSpace

#endif   // ComputeSatPos_HPP
