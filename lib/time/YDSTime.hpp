#ifndef YDSTIME_HPP
#define YDSTIME_HPP

#include "Exception.hpp"
#include "TimeConstants.hpp"
#include "TimeSystem.hpp"
#include "CommonTime.hpp"

using namespace utilSpace;

namespace timeSpace
{
      /**
       * This class encapsulates the "year, day-of-year, and seconds-of-day"
       * time format.
       */
   class YDSTime                 
   {
   public:

         /**
          * @defgroup ydstc YDSTime Time Constants
          * YDSTime-structured Constants
          */
         //@{
         /**
          * YDSTime constant corresponding to CommonTime::BEGINNING_OF_TIME
          */

      static const YDSTime BEGIN_TIME;

         //@}
         /**
          * @defgroup ydstbo YDSTime Basic Operations
          * Default and Copy Constructors, Assignment Operator and Destructor.
          */
         //@{
         /**
          * Default Constructor.
          * All elements are set to zero by default.
          */
      YDSTime( long y = 0,
               long d = 0,
               double s = 0.,
               TimeSystem ts = TimeSystem::GPS)
            : year(y), doy(d), sod(s)
      { timeSystem = ts; }

         /** Copy Constructor.
          * @param right a const reference to the YDSTime object to copy
          */
      YDSTime( const YDSTime& right )
            : year( right.year ), doy( right.doy ), sod( right.sod )
      { timeSystem = right.timeSystem; }

         /**
          * Alternate Copy Constructor.
          * Takes a const CommonTime reference and copies its contents via
          * the convertFromCommonTime method.
          * @param right a const reference to the CommonTime object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      YDSTime( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /**
          * Assignment Operator.
          * @param right a const reference to the YDSTime object to copy
          * @return a reference to this YDSTime
          */
      YDSTime& operator=( const YDSTime& right );

         /// Virtual Destructor.
      virtual ~YDSTime()
      {}
         //@}

      virtual CommonTime convertToCommonTime() const;

      virtual void convertFromCommonTime( const CommonTime& ct );

      virtual bool isValid() const;

      virtual void reset();


      TimeSystem getTimeSystem()
      {
          return timeSystem;
      }

         /**
          * @defgroup ydstco YDSTime Comparison Operators
          * All comparison operators have a parameter "right" which corresponds
          *  to the YDSTime object to the right of the symbol.
          * All comparison operators are const and return true on success
          *  and false on failure.
          */
         //@{
      bool operator==( const YDSTime& right ) const;
      bool operator!=( const YDSTime& right ) const;
      bool operator<( const YDSTime& right ) const;
      bool operator>( const YDSTime& right ) const;
      bool operator<=( const YDSTime& right ) const;
      bool operator>=( const YDSTime& right ) const;
         //@}

      int year;
      int doy;
      double sod;
      TimeSystem timeSystem;
   };

      // -----------YDSTime operator<< -----------
      //
      /**
       * Stream output for YDSTime objects.  Typically used for debugging.
       * @param s stream to append formatted YDSTime to.
       * @param yt YDSTime to append to stream \c s.
       * @return reference to \c s.
       */
   std::ostream& operator<<( std::ostream& s,
                             const timeSpace::YDSTime& yt );

} // namespace

#endif // YDSTIME_HPP
