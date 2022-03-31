#pragma ident "$Id$"

/**
 * @file RequiredObs.cpp
 * This class filters out satellites with observations grossly out of bounds.
 */

#include "RequiredObs.hpp"

namespace gnssSpace
{

    // Return a string identifying this object.
    std::string RequiredObs::getClassName() const
    { return "RequiredObs"; }


      /* Method to add a set of TypeID's to be required.
       *
       * @param typeSet    Set of TypeID's to be required.
       */
    RequiredObs& RequiredObs::addRequiredType( const SatelliteSystem& sys,
                                               const TypeIDSet& typeSet )
    {
        if(sys == SatelliteSystem::GPS)
        {
            requiredTypeSetOfGPS.insert( typeSet.begin(), typeSet.end() );
        }
        else if(sys == SatelliteSystem::GLONASS)
        {
            requiredTypeSetOfGLO.insert( typeSet.begin(), typeSet.end() );
        }
        else if(sys == SatelliteSystem::Galileo)
        {
            requiredTypeSetOfGAL.insert( typeSet.begin(), typeSet.end() );
        }
        else if(sys == SatelliteSystem::BDS)
        {
            requiredTypeSetOfBDS.insert( typeSet.begin(), typeSet.end() );
        }

        return (*this);

    }  // End of method 'RequiredObs::addRequiredTypeOfGPS()'


      // Return a satTypeValueMap object, filtering the target observables.
      //
      // @param gData     Data object holding the data.
      //
    satTypeValueMap& RequiredObs::Process(satTypeValueMap& gData)
        noexcept(false)
    {

        try
        {
            SatIDSet satRejectedSet;

            TypeIDSet requiredTypeSet;

            // Loop through all the satellites
            for(satTypeValueMap::iterator satIt = gData.begin();
                satIt != gData.end();
                ++satIt)
            {
                SatID sat( satIt->first );

                if(sat.system == SatelliteSystem::GPS)
                {
                    requiredTypeSet = requiredTypeSetOfGPS;
                }
                else if(sat.system == SatelliteSystem::GLONASS)
                {
                    requiredTypeSet = requiredTypeSetOfGLO;
                }
                else if(sat.system == SatelliteSystem::Galileo)
                {
                    requiredTypeSet = requiredTypeSetOfGAL;
                }
                else if(sat.system == SatelliteSystem::BDS)
                {
                    requiredTypeSet = requiredTypeSetOfBDS;
                }

                // Check all the indicated TypeID's
                if(requiredTypeSet.empty())
                {
                    satRejectedSet.insert( sat );
                    continue;
                }

                for(TypeIDSet::const_iterator typeIt = requiredTypeSet.begin();
                    typeIt != requiredTypeSet.end();
                    ++typeIt)
                {
                    // Try to find required type
                    typeValueMap::iterator it( satIt->second.find(*typeIt) );

                     // Now, check if this TypeID exists in this data structure
                    if(it == satIt->second.end())
                    {
                        // If we couldn't find type, then schedule this
                        // satellite for removal
                        satRejectedSet.insert( sat );

                        // It is not necessary to keep looking
                        //typeIt = requiredTypeSet.end();
                        //--typeIt;

                        break;
                    }
                }
            }

            // Let's remove satellites without all TypeID's
            gData.removeSatID(satRejectedSet);

            return gData;

        }
        catch(Exception& u)
        {
            // Throw an exception if something unexpected happens
            ProcessingException e( getClassName() + ":" + u.what() );
            THROW(e);
        }

    }  // End of 'RequiredObs::Process()'

} // End of namespace gnssSpace
