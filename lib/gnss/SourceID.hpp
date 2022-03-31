#pragma ident "$Id$"

/**
 * @file SourceID.hpp
 * gnssSpace::SourceID - Simple index to represent the source of data.
 */

#ifndef SOURCEID_HPP
#define SOURCEID_HPP

//============================================================================
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2006, 2007, 2008
//
//  shoujian zhang, 2022
//
//============================================================================


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>

using namespace std;

namespace gnssSpace
{

      /** @addtogroup DataStructures */

   class SourceID
   {
   public:

         /// empty constructor, creates an unknown source data object
      SourceID()
         : sourceName("")
      {};

         /// Explicit constructor
      SourceID( std::string name )
      {
          sourceName = name.substr(0,4);
      };

         /// Equality operator requires all fields to be the same.
      virtual bool operator==(const SourceID& right) const;


         /// Ordering is arbitrary but required to be able to use a SourceID
         /// as an index to a std::map. If an application needs
         /// some other ordering, inherit and override this function.
      virtual bool operator<(const SourceID& right) const;


         /// Inequality operator
      bool operator!=(const SourceID& right) const
      { return !(operator==(right)); }


         /// 'Greater than' operator
      bool operator>(const SourceID& right) const
      {  return (!operator<(right) && !operator==(right)); }


         /// 'Less or equal than' operator
      bool operator<=(const SourceID& right) const
      { return (operator<(right) || operator==(right)); }


         /// 'Greater or equal than' operator
      bool operator>=(const SourceID& right) const
      { return !(operator<(right)); }


         /// Assignment operator
      SourceID& operator=(const SourceID& right);

         /// Convenience output method
      virtual std::ostream& dump(std::ostream& s) const;


         /// Destructor
      virtual ~SourceID() {};


      /// Name of the data source
      std::string sourceName;

   }; // End of class 'SourceID'

   /// Stream output for SourceID
   std::ostream& operator<<( std::ostream& s,
                             const SourceID& p );

}  // End of namespace gnssSpace

#endif   // SOURCEID_HPP
