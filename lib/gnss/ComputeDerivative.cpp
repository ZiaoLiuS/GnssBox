/**
 * ComputeDerivative
 */

#include "ComputeDerivative.hpp"
#include "YDSTime.hpp"
#include "constants.hpp"
#include "DataStructures.hpp"

using namespace std;
using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
using namespace gnssSpace;

#define debug 0

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
namespace gnssSpace 
{

      // Return a string identifying this object.
    std::string ComputeDerivative::getClassName() const
    { return "ComputeDerivative"; }


      /* Explicit constructor, taking as input reference station
       * coordinates, ephemeris to be used, default observable
       * and whether TGD will be computed or not.
       *
       * @param RxCoordinates Reference station coordinates.
       * @param dEphemeris    EphemerisStore object to be used by default.
       * @param dObservable   Observable type to be used by default.
       * @param applyTGD      Whether or not C1 observable will be
       *                      corrected from TGD effect or not.
       *
       */
    ComputeDerivative::ComputeDerivative( const Position& StaPos)
    {
        minElev = 10.0;
        nominalPos = StaPos;
    }  // End of 'ComputeDerivative::ComputeDerivative()'


      /* Return a satTypeValueMap object, adding the new data generated when
       * calling a modeling object.
       *
       * @param time      Epoch.
       * @param gData     Data object holding the data.
       */
    satTypeValueMap& ComputeDerivative::Process( const CommonTime& time,
                                                 satTypeValueMap& gData )
            noexcept(false)
    {

        try
        {
            SatIDSet satRejectedSet;
            TypeID defaultObs;

            // Loop through all the satellites
            for(satTypeValueMap::iterator it = gData.begin();
                it != gData.end();
                ++it)
            {
                SatID sat = (*it).first;

                Triple svPos;
                svPos[0] = (*it).second[TypeID::satXECEF];
                svPos[1] = (*it).second[TypeID::satYECEF];
                svPos[2] = (*it).second[TypeID::satZECEF];

                if(debug)
                {
                    cout << getClassName() << " sat:" << sat << svPos << endl;
                }

                // rho
                double rho(0.0);
                rho = RSS(svPos[0] - nominalPos.X(), 
                          svPos[1] - nominalPos.Y(), 
                          svPos[2] - nominalPos.Z());

                // partials
                Triple cosines;
                cosines[0] = (nominalPos.X() - svPos[0]) / rho;
                cosines[1] = (nominalPos.Y() - svPos[1]) / rho;
                cosines[2] = (nominalPos.Z() - svPos[2]) / rho;

                // elevation, azimuth
                double elevation(0.0), azimuth(0.0);

                try
                {
                    elevation = nominalPos.elevationGeodetic(svPos);
                    azimuth = nominalPos.azimuth(svPos);
                }
                catch(Exception& e)
                {
                    satRejectedSet.insert((*it).first);
                    continue;
                }

                // Let's test if satellite has enough elevation over horizon
                if ( elevation < minElev )
                {
                    // Mark this satellite if it doesn't have enough elevation
                    satRejectedSet.insert( (*it).first );
                    continue;
                }

                // now, let's compute the prefitC for spp
                double relativity, cdtSat;

                try
                {
                    // extract values from gnssRinex using operator()
                    // if not found, remove this satellite. shjzhang
                    relativity = (*it).second(TypeID::relativity);
                    cdtSat     = (*it).second(TypeID::cdtSat);
                }
                catch(Exception& e)
                {
                    satRejectedSet.insert((*it).first);
                    continue;
                }

                // rho
                (*it).second[TypeID::rho] = rho;

                // Let's insert partials for station position at receive time
                (*it).second[TypeID::dX] = cosines[0];
                (*it).second[TypeID::dY] = cosines[1];
                (*it).second[TypeID::dZ] = cosines[2];
                (*it).second[TypeID::cdt] = 1.0;

                if(sat.system == SatelliteSystem::GPS)
                {
                    (*it).second[TypeID::dcdtGPS] = 1.0;
                }
                else if(sat.system == SatelliteSystem::Galileo)
                {
                    (*it).second[TypeID::dcdtGAL] = 1.0;
                }
                else if(sat.system == SatelliteSystem::BDS)
                {
                    (*it).second[TypeID::dcdtBDS] = 1.0;
                }
                else if(sat.system == SatelliteSystem::GLONASS)
                {
                    (*it).second[TypeID::dcdtGLO] = 1.0;
                }

                (*it).second[TypeID::elevation] = elevation;
                (*it).second[TypeID::azimuth] = azimuth;

            } // End of loop for(satTypeValueMap = gData.begin()...

            // Remove satellites with missing data
            gData.removeSatID(satRejectedSet);

            return gData;

        }   // End of try...
        catch(SVNumException& u)
        {
            THROW(u);
        }
        catch(Exception& u)
        {
            // Throw an exception if something unexpected happens
            ProcessingException e( getClassName() + ":" + u.what() );
            THROW(u);
        }

    }  // End of method 'ComputeDerivative::Process()'

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;
}  // End of namespace gnssSpace
