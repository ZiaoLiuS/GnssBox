#ifndef CIVILTIME_HPP
#define CIVILTIME_HPP

#include "TimeSystem.hpp"
#include "CommonTime.hpp"

namespace timeSpace
{
      /**
       * This class encapsulates the representation of time consisting of
       * year, month, day (of month), hour (of day), minute (of hour), and
       * second (of minute).
       */
   class CivilTime                 
   {
   public:

         /**
          * @defgroup caltbo CivilTime Basic Operations
          * Default and Copy Constructors, Assignment Operator and Destructor.
          */
         //@{
         /**
          * Default Constructor.
          * All elements default to zero.
          */
      CivilTime( int yr = 0,
                 int mo = 0,
                 int dy = 0,
                 int hr = 0,
                 int mn = 0,
                 double s = 0.0,
                 TimeSystem ts = TimeSystem::Unknown )
        : year(yr), month(mo), day(dy), hour(hr), minute(mn), second(s)
      { timeSystem = ts; }

         /**
          * Copy Constructor.
          * @param right a const reference to the CivilTime object to copy
          */
      CivilTime( const CivilTime& right )
            : year( right.year ), month( right.month )  , day( right.day ),
              hour( right.hour ), minute( right.minute ), second( right.second )
      { timeSystem = right.timeSystem; }


         /**
          * Alternate Copy Constructor.
          * Takes a const CommonTime reference and copies its contents via
          * the convertFromCommonTime method.
          * @param right a const reference to the CommonTime object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      CivilTime( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /**
          * Assignment Operator.
          * @param right a const reference to the CivilTime object to copy
          * @return a reference to this CivilTime object
          */
      CivilTime& operator=( const CivilTime& right );

         /// Virtual Destructor.
      virtual ~CivilTime()
      {}
         //@}

         /// Long month names for converstion from numbers to strings
      static const char *MonthNames[];

         /// Short month names for converstion from numbers to strings
      static const char *MonthAbbrevNames[];

      virtual CommonTime convertToCommonTime() const;

      virtual void convertFromCommonTime( const CommonTime& ct );

      virtual bool isValid() const;

      virtual void reset();

      std::string asString() const;

      TimeSystem getTimeSystem()
      {
          return timeSystem;
      }

         /**
          * @defgroup ctco CivilTime Comparison Operators
          * All comparison operators have a parameter "right" which corresponds
          *  to the CivilTime object to the right of the symbol.
          * All comparison operators are const and return true on success
          *  and false on failure.
          */
         //@{
      bool operator==( const CivilTime& right ) const;
      bool operator!=( const CivilTime& right ) const;
      bool operator<( const CivilTime& right ) const;
      bool operator>( const CivilTime& right ) const;
      bool operator<=( const CivilTime& right ) const;
      bool operator>=( const CivilTime& right ) const;
         //@}

      int year;
      int month;
      int day;
      int hour;
      int minute;
      double second;

      TimeSystem timeSystem;

   };

      // -----------CivilTime operator<< -----------
      //
      /**
       * Stream output for CivilTime objects.  Typically used for debugging.
       * @param s stream to append formatted CivilTime to.
       * @param cit CivilTime to append to stream \c s.
       * @return reference to \c s.
       */
   std::ostream& operator<<( std::ostream& s,
                             const timeSpace::CivilTime& cit );

} // namespace

#endif // CIVILTIME_HPP
