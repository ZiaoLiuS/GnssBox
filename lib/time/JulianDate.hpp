#ifndef JULIANDATE_HPP
#define JULIANDATE_HPP

#include "CommonTime.hpp"

using namespace utilSpace;
namespace timeSpace
{
      /**
       * This class encapsulates the "Julian Date" time representation.
       */
   class JulianDate                 
   {
   public:
         /**
          * @defgroup jdbo JulianDate Basic Operations
          * Default and Copy Constructors, Assignment Operator and Destructor.
          */
         //@{
         /**
          * Default Constructor.
          * All elements are initialized to zero.
          */
      JulianDate( long double j = 0., TimeSystem ts = TimeSystem::Unknown )
            : jd( j )
      { timeSystem = ts; }

         /**
          * Copy Constructor.
          * @param right a reference to the JulianDate object to copy
          */
      JulianDate( const JulianDate& right )
            : jd( right.jd )
      { timeSystem = right.timeSystem; }

         /**
          * Alternate Copy Constructor.
          * Takes a const CommonTime reference and copies its contents via
          * the convertFromCommonTime method.
          * @param right a const reference to the CommonTime object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      JulianDate( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /**
          * Assignment Operator.
          * @param right a const reference to the JulianDate to copy
          * @return a reference to this JulianDate
          */
      JulianDate& operator=( const JulianDate& right );

         /// Virtual Destructor.
      virtual ~JulianDate()
      {}
         //@}

         // The following functions are required by TimeTag.
      virtual CommonTime convertToCommonTime() const;

      virtual void convertFromCommonTime( const CommonTime& ct );

      virtual bool isValid() const;

      virtual void reset();

      TimeSystem getTimeSystem()
      {
          return timeSystem;
      }

         /**
          * @defgroup jdco JulianDate Comparison Operators
          * All comparison operators have a parameter "right" which corresponds
          *  to the JulianDate object to the right of the symbol.
          * All comparison operators are const and return true on success
          *  and false on failure.
          */
         //@{
      bool operator==( const JulianDate& right ) const;
      bool operator!=( const JulianDate& right ) const;
      bool operator<( const JulianDate& right ) const;
      bool operator>( const JulianDate& right ) const;
      bool operator<=( const JulianDate& right ) const;
      bool operator>=( const JulianDate& right ) const;
         //@}

      long double jd;

      TimeSystem timeSystem;
   };

} // namespace

#endif // JULIANDATE_HPP
