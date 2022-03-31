#pragma ident "$ID$"

/**
 * this class is used to convet 4-char TypeID to 3-char TypeID,
 * which is convenient for the systematic bias computation, 
 * such as PCO/PCV/Windup correction.
 */

#include "Rx3ObsData.hpp"

#ifndef ConvertObs_HPP
#define ConvertObs_HPP

namespace gnssSpace
{

    class ConvertObs                         
    {
    public :

        // Default constructr
        ConvertObs() {};


        /** Return a satTypeValueMap object, adding the new data generated
         *  when calling this object.
         *
         * @param  time     Epoch corresponding to the data.
         * @param  gData    Data object holding the data.
         */
        virtual satTypeValueMap& Process( std::map<SatID, TypeIDVec>& satTypes,
                                          std::map<SatID, TypeIDVec>& satShortTypes,
                                          satTypeValueMap& gData )
            noexcept(false);


        /** Return a Rx3ObsData object, adding the new data generated
         *  when calling this object.
         *
         * @param gData     Data object holding the data.
         */
        virtual void Process(Rx3ObsData& rxData)
            noexcept(false)
        { 
            Process(rxData.satTypes, 
                    rxData.satShortTypes, 
                    rxData.stvData); 
        };


        // Return a string identifying this object.
        virtual std::string getClassName () const;

        virtual void setSysPrioriTypes(SysTypesMap& sysTypes)
        {
           sysPrioriTypes = sysTypes;
        };



        // Default deconstructr
        ~ConvertObs() {};


    private:

         SysTypesMap sysPrioriTypes;

        SatID::SatelliteSystem satSys;

        TypeID oldType;

        TypeID newType;

    }; // End of class 'ConvertObs'

}; // end of namespace gnssSpace

#endif   // ConvertObs_HPP
