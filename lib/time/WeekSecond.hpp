/// @file WeekSecond.hpp  Implement full week, mod week and seconds-of-week time
/// representation.

#ifndef WEEKSECOND_HPP
#define WEEKSECOND_HPP

#include "Exception.hpp"
#include "Week.hpp"
#include "TimeConstants.hpp"
#include "TimeSystem.hpp"

using namespace utilSpace;

namespace timeSpace
{
   /// This class encapsulates the "Full Week and Seconds-of-week"
   /// time representation.
   class WeekSecond : public Week
   {
   public:

         /**
          * @defgroup wsbo WeekSecond Basic Operations
          * Default and Copy Constructors, Assignment Operator and Destructor.
          */
         //@{

         /**
          * Default Constructor.
          * All elements are initialized to zero.
          */
      WeekSecond(unsigned int w = 0,
                 double s = 0.,
                 TimeSystem ts = TimeSystem::Unknown)
            : Week(w), sow(s)
      { timeSystem = ts; }

         /**
          * Copy Constructor.
          * @param right a reference to the WeekSecond object to copy
          */
      WeekSecond( const WeekSecond& right )
            : Week( right ), sow( right.sow )
      { timeSystem = right.timeSystem; }


         /**
          * Alternate Copy Constructor.
          * Takes a const CommonTime reference and copies its contents via
          * the convertFromCommonTime method.
          * @param right a const reference to the CommonTime object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      WeekSecond( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /**
          * Assignment Operator.
          * @param right a const reference to the WeekSecond to copy
          * @return a reference to this WeekSecond
          */
      WeekSecond& operator=( const WeekSecond& right );

         /// Virtual Destructor.
      virtual ~WeekSecond()
      {}


      virtual CommonTime convertToCommonTime() const;

      virtual void convertFromCommonTime( const CommonTime& ct );

      virtual bool isValid() const;

      virtual void reset();

      inline virtual unsigned int getDayOfWeek() const
      {
         return static_cast<unsigned int>(sow) / SEC_PER_DAY;
      }

      inline double getSOW() const { return sow; }

      /// @defgroup wsco WeekSecond Comparison Operators
      /// All comparison operators have a parameter "right" which corresponds
      /// to the WeekSecond object to the right of the symbol.
      /// All comparison operators are const and return true on success
      /// and false on failure.
         //@{
      bool operator==( const WeekSecond& right ) const;
      bool operator!=( const WeekSecond& right ) const;
      bool operator<( const WeekSecond& right ) const;
      bool operator>( const WeekSecond& right ) const;
      bool operator<=( const WeekSecond& right ) const;
      bool operator>=( const WeekSecond& right ) const;
         //@}

      double sow;
   };

}

#endif // WEEKSECOND_HPP
