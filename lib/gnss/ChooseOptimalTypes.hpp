
/**
 * @file ChooseOptimalTypes.hpp
 * filter out satellite with observations out of bound.
 *
 * by shoujian zhang
 */

#ifndef ChooseOptimalTypes_HPP
#define ChooseOptimalTypes_HPP

#include "Rx3ObsData.hpp"

namespace gnssSpace
{


      /** This class filters out satellites with observations grossly out of
       *  bounds.
       *
       * This class is meant to be used with the GNSS data structures objects
       * found in "DataStructures" class.
       *
       * A typical way to use this class follows:
       *
       * @code
       *
       *   std::fstream rin("ebre0300.02o");
       *
       *   ChooseOptimalTypes myFilter;
       *  
       *   gnssRinex rxData
       *
       *   while(true)
       *   {
       *      rin >> rxData;
       *      myFilter.Process(rxData);
       *   }
       *
       * @endcode
       *
       */
   class ChooseOptimalTypes                         
   {
   public:

	   // Default constructor.
	   ChooseOptimalTypes() 
	   {};


      /** Returns a gnssRinex object, filtering the target observables.
       *
       * @param gData    Data object holding the data.
       */
      virtual SysTypesMap get( SysTypesMap mapObsTypes) 
          noexcept(false);


         /// Returns a string identifying this object.
      virtual std::string getClassName(void) const;


         /// Destructor
      virtual ~ChooseOptimalTypes() {};


      /// This mapgives priority tracking codes for RINEX
      /// observations given the system and frequency;
      /// eg. priorityCodes["G"]['1']="WCSLXPYMN"
      /// The only exception is there is no pseudorange (C) on GPS L1/L2 N (codeless)
      /// These tracking code characters are ORDERED, basically 'best' to 'worst'
      static stringCharStringMap priorityCodes;

      static stringCharStringMap init();



   }; // End of class 'ChooseOptimalTypes'

}  // End of namespace gnssSpace

#endif   // ChooseOptimalTypes_HPP
