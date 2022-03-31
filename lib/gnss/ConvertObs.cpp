#pragma ident "$Id$"

/**
* @file ConvertObs.cpp
* Class to convert observables, from 4-char to 3-char.
*
* author: kaifa kuang
* time  : 2019/01/10
*/

#include "Exception.hpp"
#include "ConvertObs.hpp"

#define debug 0

using namespace std;
using namespace utilSpace;

namespace gnssSpace
{

      // Return a string identifying this object.
    string ConvertObs::getClassName() const
    { return "ConvertObs"; }


      /* Return a satTypeValueMap object, adding the new data generated
       *  when calling this object.
       *
       * @param time      Epoch corresponding to the data.
       * @param gData     Data object holding the data.
       */
    satTypeValueMap& ConvertObs::Process(std::map<SatID, TypeIDVec>& satTypes,
                                         std::map<SatID, TypeIDVec>& satShortTypes,
                                         satTypeValueMap& gData )
        noexcept(false)
    {
        try
        {
            SatIDSet satRejectedSet;

            // Loop through all the satellites
            for(auto it = gData.begin(); it != gData.end(); ++it)
            {

                // get the satellite  identify
                SatelliteSystem system = (*it).first.system;
                char sysChar = system.toChar();

                vector<TypeID> prioriTypes = sysPrioriTypes[string(1, sysChar)];

                if(debug)
                {
                    cout << "num of prioriTypes" <<  prioriTypes.size() << endl;
                }

                vector<TypeID> shortTypes = satShortTypes[(*it).first];

                for(int i=0; i<prioriTypes.size(); i++)
                {
                    string longTypeStr = prioriTypes[i].asString();

                    if(debug)
                    {
                        cout << "longTypeStr:" << longTypeStr << endl;
                    }

                    // L1CG=> L1G; C1WG=>C1G
                    TypeID shortType = TypeID(longTypeStr.substr(0,2) + longTypeStr.substr(3,1));

                    if( (*it).second.find(shortType) == (*it).second.end() &&
                        (*it).second.find( TypeID(longTypeStr) ) != (*it).second.end() )
                    {
                        (*it).second[shortType] = (*it).second[TypeID(longTypeStr)];

                        shortTypes.push_back(shortType);
                    }

                }

                if(debug)
                {
                    for(int i=0; i<shortTypes.size(); i++)
                    {
                        cout << getClassName() << shortTypes[i] << " ";
                    }
                    cout << endl;
                }

                // insert shortTypes into map
                satShortTypes[(*it).first] = shortTypes;
            }

            return gData;
        }
        catch(Exception& u)
        {
            // Throw an exception if something unexpected happens
            ProcessingException e( getClassName() + ":" + u.what() );
            THROW(e);
        }

    }  // End of method 'ConvertObs::Process()'

}  // End of namespace gnssSpace
