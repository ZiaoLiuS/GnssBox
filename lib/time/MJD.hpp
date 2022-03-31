#ifndef MJD_HPP
#define MJD_HPP

#include <iostream>
#include <sstream>
#include "TimeSystem.hpp"
#include "CommonTime.hpp"
#include "StringUtils.hpp"

using namespace std;
using namespace utilSpace;

namespace timeSpace
{
      /**
       * This class encapsulates the "Modified Julian Date" time representation.
       */
   class MJD                 
   {
   public:
         /**
          * @defgroup mjdbo MJD Basic Operations
          * Default and Copy Constructors, Assignment Operator and Destructor.
          */
         //@{
         /**
          * Default Constructor.
          * All elements are initialized to zero.
          */
      MJD( long double m = 0.,
           TimeSystem ts = TimeSystem::Unknown )
            : mjd( m )
      { timeSystem = ts; }

         /**
          * Copy Constructor.
          * @param right a reference to the MJD object to copy
          */
      MJD( const MJD& right )
            : mjd( right.mjd )
      { timeSystem = right.timeSystem; }

         /**
          * Alternate Copy Constructor.
          * Takes a const CommonTime reference and copies its contents via
          * the convertFromCommonTime method.
          * @param right a const reference to the CommonTime object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      MJD( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /**
          * Assignment Operator.
          * @param right a const reference to the MJD to copy
          * @return a reference to this MJD
          */
      MJD& operator=( const MJD& right );

         /// Virtual Destructor.
      virtual ~MJD()
      {}
         //@}

         // The following functions are required by TimeTag.
      virtual CommonTime convertToCommonTime() const;

      virtual void convertFromCommonTime( const CommonTime& ct );

      virtual bool isValid() const;

      virtual void reset();

      std::string asString()
      {
        ostringstream oss;
        oss << setfill('0')
            << setw(7) << mjd << " "
            << " " << timeSystem.asString() ;
        return oss.str();
      }


      TimeSystem getTimeSystem()
      {
          return timeSystem;
      }
         /**
          * @defgroup mjdco MJD Comparison Operators
          * All comparison operators have a parameter "right" which corresponds
          *  to the MJD object to the right of the symbol.
          * All comparison operators are const and return true on success
          *  and false on failure.
          */
         //@{
      bool operator==( const MJD& right ) const;
      bool operator!=( const MJD& right ) const;
      bool operator<( const MJD& right ) const;
      bool operator>( const MJD& right ) const;
      bool operator<=( const MJD& right ) const;
      bool operator>=( const MJD& right ) const;
         //@}

      long double mjd;
      TimeSystem timeSystem;
   };

} // namespace

#endif // MJD_HPP
