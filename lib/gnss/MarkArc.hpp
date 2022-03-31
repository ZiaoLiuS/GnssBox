#pragma ident "$Id$"

/**
 * @file MarkArc.hpp
 * This class keeps track of satellite arcs caused by cycle slips.
 */

#ifndef MarkArc_HPP
#define MarkArc_HPP

//============================================================================
//
// mark arc for every independent type
//
//============================================================================

#include "CommonTime.hpp"
#include "Rx3ObsData.hpp"

using namespace utilSpace;
using namespace timeSpace;

namespace gnssSpace
{

    class MarkArc                         
    {
    public:

        /// Default constructor. It will only watch "TypeID::CSL1" flag.
        MarkArc()
            :csDataStream(NULL)
        {
            Init();
        };

        void Init();

        /** Method to get the arc changed epoch.
         * @param sat              Interested SatID.
         */
        virtual CommonTime getArcChangedEpoch(const SatID& sat);


        /** Method to get the arc changed epoch.
         * @param source           Interested SourceID.
         * @param sat              Interested SatID.
         */
        virtual CommonTime getArcChangedEpoch( const SourceID& source,
                                               const SatID& sat );


        /** Return a satTypeValueMap object, adding the new data generated
         *  when calling this object.
         *
         * @param epoch     Time of observations.
         * @param gData     Data object holding the data.
         */
        virtual satTypeValueMap& Process( const CommonTime& epoch,
                                          std::map<SatID, TypeIDVec>& satShortTypes,
                                          satTypeValueMap& gData )
            noexcept(false);


        virtual void Process(Rx3ObsData& rRin)
            noexcept(false)
        {
            Process(rRin.currEpoch, rRin.satShortTypes, rRin.stvData);
        }


        virtual void setCSStream(std::ofstream& stream)
        {
            csDataStream = &stream;
        };


        /// Return a string identifying this object.
        virtual std::string getClassName(void) const;


        /// Destructor
        virtual ~MarkArc() {};


    private:


        struct arcData
        {
            arcData()
                : arcNum(0),
                  arcChangeTime(CommonTime::BEGINNING_OF_TIME),
                  arcNew(false)
            {};

            double arcNum;
            CommonTime arcChangeTime;
            bool arcNew;
        };


        typedef std::map<SatID, std::map<TypeID, arcData>> satTypeArcDataMap;

        std::ofstream* csDataStream;

        /// Map holding information regarding every satellite
        satTypeArcDataMap satTypeArcData;

        std::map<SatelliteSystem, std::vector<TypeID>> satCombType;

    }; // End of class 'MarkArc'

}  // End of namespace gnssSpace

#endif   // MarkArc_HPP
