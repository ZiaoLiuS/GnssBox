
#ifndef UNIXTIME_HPP
#define UNIXTIME_HPP

#include <sys/time.h>     // for struct timeval

#include "TimeSystem.hpp"
#include "CommonTime.hpp"

namespace timeSpace
{
      /**
       * This class encapsulates the "Unix Timeval" time representation.
       */
   class UnixTime 
   {
   public:
         /**
          * Default Constructor.
          * All elements are initialized to zero.
          */
      UnixTime( int sec = 0,
                int usec = 0,
                TimeSystem ts = TimeSystem::Unknown )
      { tv.tv_sec = sec;  tv.tv_usec = usec;  timeSystem = ts; }

         /** struct timeval Constructor.
          * Sets time according to the given struct timeval.
          */
      UnixTime( struct timeval t,
                TimeSystem ts = TimeSystem::Unknown )
      {
         tv.tv_sec = t.tv_sec;  tv.tv_usec = t.tv_usec;  timeSystem = ts;
      }

         /**
          * Copy Constructor.
          * @param right a reference to the UnixTime object to copy
          */
      UnixTime( const UnixTime& right )
            : tv( right.tv )
      { timeSystem = right.timeSystem; }

         /**
          * Alternate Copy Constructor.
          * Takes a const CommonTime reference and copies its contents via
          * the convertFromCommonTime method.
          * @param right a const reference to the CommonTime object to copy
          * @throw InvalidRequest on over-/under-flow
          */
      UnixTime( const CommonTime& right )
      {
         convertFromCommonTime( right );
      }

         /**
          * Assignment Operator.
          * @param right a const reference to the UnixTime to copy
          * @return a reference to this UnixTime
          */
      UnixTime& operator=( const UnixTime& right );

         /// Virtual Destructor.
      virtual ~UnixTime()
      {}

      virtual CommonTime convertToCommonTime() const;

      virtual void convertFromCommonTime( const CommonTime& ct );

      virtual bool isValid() const;

      virtual void reset();


      TimeSystem getTimeSystem()
      {
          return timeSystem;
      }

         /**
          * @defgroup utco UnixTime Comparison Operators
          * All comparison operators have a parameter "right" which corresponds
          *  to the UnixTime object to the right of the symbol.
          * All comparison operators are const and return true on success
          *  and false on failure.
          */
         //@{
      virtual bool operator==( const UnixTime& right ) const;
      virtual bool operator!=( const UnixTime& right ) const;
      virtual bool operator<( const UnixTime& right ) const;
      virtual bool operator>( const UnixTime& right ) const;
      virtual bool operator<=( const UnixTime& right ) const;
      virtual bool operator>=( const UnixTime& right ) const;
         //@}

      struct timeval tv;

      TimeSystem timeSystem;
   };

} // namespace

#endif // UNIXTIME_HPP
