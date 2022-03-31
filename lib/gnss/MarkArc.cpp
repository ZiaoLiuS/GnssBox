#pragma ident "$Id$"

/**
 * @file MarkArc.cpp
 * This class keeps track of satellite arcs caused by cycle slips.
 */

#include "MarkArc.hpp"

#define debug 0

namespace gnssSpace
{

      // Return a string identifying this object.
    std::string MarkArc::getClassName() const
    { return "MarkArc"; }


    void MarkArc::Init()
    {
       std::vector<TypeID> combTypeGPS;
       combTypeGPS.push_back(TypeID::LC12G);
       combTypeGPS.push_back(TypeID::WL21G);
       combTypeGPS.push_back(TypeID::WL25G);
       combTypeGPS.push_back(TypeID::IFCBLC1215G);

       std::vector<TypeID> combTypeGAL;
       combTypeGAL.push_back(TypeID::LC15E);
       combTypeGAL.push_back(TypeID::WL65E);
       combTypeGAL.push_back(TypeID::WL67E);
       combTypeGAL.push_back(TypeID::WL68E);
       combTypeGAL.push_back(TypeID::WL61E);

       combTypeGAL.push_back(TypeID::WL75E);
       combTypeGAL.push_back(TypeID::WL78E);
       combTypeGAL.push_back(TypeID::WL71E);

       std::vector<TypeID> combTypeBDS;
       combTypeBDS.push_back(TypeID::LC26C);
       combTypeBDS.push_back(TypeID::WL65C);
       combTypeBDS.push_back(TypeID::WL67C);
       combTypeBDS.push_back(TypeID::WL68C);
       combTypeBDS.push_back(TypeID::WL62C);
       combTypeBDS.push_back(TypeID::WL61C);
       combTypeBDS.push_back(TypeID::WL21C);

       satCombType[SatelliteSystem::GPS] = combTypeGPS ;
       satCombType[SatelliteSystem::Galileo] = combTypeGAL;
       satCombType[SatelliteSystem::BDS] = combTypeBDS;
    }

    /* Method to get the arc changed epoch.
     * @param sat              Interested SatID.
     */
    CommonTime MarkArc::getArcChangedEpoch(const SatID& sat)
    {
    }  // End of method 'MarkArc::getArcChangedEpoch()'


    /* Method to get the arc changed epoch.
     * @param source           Interested SourceID.
     * @param sat              Interested SatID.
     */
    CommonTime MarkArc::getArcChangedEpoch(const SourceID& source,
                                                const SatID& sat)
    {
    }  // End of method 'MarkArc::getArcChangedEpoch()'


      /* Return a satTypeValueMap object, adding the new data generated
       *  when calling this object.
       *
       * @param epoch     Time of observations.
       * @param gData     Data object holding the data.
       */
    satTypeValueMap& MarkArc::Process( const CommonTime& epoch,
                                              std::map<SatID, TypeIDVec>& satShortTypes,
                                              satTypeValueMap& gData )
        noexcept(false)
    {

        SatIDSet satRejectedSet;

        if(debug)
        {
            cout << "MarkArc" << endl;
            gData.dump(cout, 1);
        }

        // Loop through all the satellites
        for ( satTypeValueMap::iterator it = gData.begin();
              it != gData.end();
              ++it )
        {
            SatID sat = (*it).first;

            if(debug)
            {
                cout << "current"<< "sat:" << sat << endl;
            }

            TypeIDVec combTypeVec = satCombType[sat.system];

            if(debug)
            {
                cout << "combTypeVec" << endl;
            }

            ///////////////////
            // 获取载波相位类型
            ///////////////////
            TypeIDVec typeVec = satShortTypes[sat];
            TypeIDVec phaseTypeVec;
            for(int i=0; i<typeVec.size(); i++)
            {
                if(typeVec[i].asString()[0] == 'L')
                {
                    phaseTypeVec.push_back(typeVec[i]);
                }
            }

            if(debug)
            {
                cout << "phaseTypeVec before" << endl;
            }

            // add combType into phaseTypeVec
            for(int i=0; i<combTypeVec.size(); i++)
            {
                phaseTypeVec.push_back(combTypeVec[i]);
            }

            if(debug)
            {
                cout << "before satArc" << endl;
            }

            //////////////////
            // 为每一个类型累计satArc
            //////////////////

            // Check if satellite currently has entries
            satTypeArcDataMap::const_iterator itArc( satTypeArcData.find( sat ) );
            if( itArc == satTypeArcData.end() )
            {
                // If it doesn't have an entry, insert one
                // 如果当前卫星第一次出现，则为每一个类型设置初始化弧段时间
                for(int i=0; i<phaseTypeVec.size(); i++)
                {
                    TypeID phaseType = phaseTypeVec[i];
                    satTypeArcData[sat][phaseType].arcChangeTime = CommonTime::BEGINNING_OF_TIME;
                    satTypeArcData[sat][phaseType].arcNum = 0.0;
                }
            }


            for(int i=0; i<phaseTypeVec.size(); i++)
            {
                TypeID phaseType = phaseTypeVec[i];

                if(debug)
                {
                    cout << "phaseType:" << phaseType << endl;
                }

                TypeID csFlagType = TypeID("CSFlag" + phaseType.asString());

                if(debug)
                {
                    cout << "csFlagType:" << csFlagType << endl;
                }

                // Check if there was a cycle slip
                // compatible with different cycle-slip method
                if ( (*it).second.find(csFlagType) != (*it).second.end() )
                {
                    if(csDataStream!=NULL)
                    {
                        (*csDataStream) 
                            << sat  << " "
                            << YDSTime(epoch) << " "
                            << (*it).second[csFlagType]<< " "
                            << endl;
                    }

                    if(debug)
                    {
                        cout << "sat:" << sat << endl;
                        cout << "csFlagType:" << csFlagType << endl;
                        cout << "csFlagValue:" << (*it).second[csFlagType] << endl;
                    }
  
                    if((*it).second(csFlagType) > 0 )
                    {
                        // Increment the value of "TypeID::satArc"
                        satTypeArcData[sat][phaseType].arcNum = satTypeArcData[sat][phaseType].arcNum + 1.0;

                        // Update arc change epoch
                        satTypeArcData[sat][phaseType].arcChangeTime = epoch;
                    }
                    
                    if(debug)
                    {
                        cout << "satArcMarker:" << sat << ":" << satTypeArcData[sat][phaseType].arcNum << endl;
                    }

                    (*it).second[TypeID("satArc"+phaseType.asString())] = satTypeArcData[sat][phaseType].arcNum;
                }

            }
        }

        // Remove satellites with missing data
        gData.removeSatID(satRejectedSet);

        return gData;

    }  // End of method 'MarkArc::Process()'


}  // End of namespace gnssSpace
