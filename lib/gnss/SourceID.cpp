#pragma ident "$Id$"

/**
 * @file SourceID.cpp
 * gnssSpace::SourceID - Simple index to represent the source of data.
 *
 * 2020/08/07
 *
 * add method getTypeFromString() 
 * shoujian zhang
 */


#include "SourceID.hpp"
#include "Exception.hpp"


using namespace utilSpace;

namespace gnssSpace
{


   // Assignment operator
   SourceID& SourceID::operator=(const SourceID& right)
   {

      if ( this == &right )
      {
         return (*this);
      }

      sourceName = right.sourceName;

      return *this;

   }  // End of 'SourceID::operator=()'



   // Convenience output method
   std::ostream& SourceID::dump(std::ostream& s) const
   {

      s << sourceName ;

      return s;

   }  // End of method 'SourceID::dump()'


   // Equality operator requires all fields to be the same.
   bool SourceID::operator==(const SourceID& right) const
   {

      return (sourceName==right.sourceName );

   }  // End of 'SourceID::operator==()'



   // Ordering is arbitrary but required to be able to use a SourceID
   // as an index to a std::map. If an application needs
   // some other ordering, inherit and override this function.
   bool SourceID::operator<(const SourceID& right) const
   {

       return sourceName < right.sourceName;

   }  // End of 'SourceID::operator<()'

      // Stream output for SourceID
   std::ostream& operator<<( std::ostream& s,
                             const SourceID& p )
   {

      p.dump(s);

      return s;

   }  // End of 'operator<<'

}  // End of namespace gnssSpace
