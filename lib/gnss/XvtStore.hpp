#pragma ident "$Id$"

/**
 * @file XvtStore.hpp
 * Abstract base class for storing and/or computing position, velocity, 
 * and clock data.
 */

#ifndef XVTSTORE_INCLUDE
#define XVTSTORE_INCLUDE

#include <iostream>

#include "Exception.hpp"
#include "CommonTime.hpp"
#include "Xvt.hpp"

using namespace utilSpace;
using namespace coordSpace;
using namespace timeSpace;
using namespace mathSpace;

namespace gnssSpace
{
   /** @addtogroup ephemstore */
   //@{

   /// Abstract base class for storing and accessing an object's position, 
   /// velocity, and clock data. Also defines a simple interface to remove
   /// data that had been added.
   template <class IndexType>
   class XvtStore
   {
   public:
      virtual ~XvtStore()
      {}

      /// Returns the position, velocity, and clock offset of the indicated
      /// object in ECEF coordinates (meters) at the indicated time.
      /// @param[in] id the object's identifier
      /// @param[in] t the time to look up
      /// @return the Xvt of the object at the indicated time
      /// @throw InvalidRequest If the request can not be completed for any
      ///    reason, this is thrown. The text may have additional
      ///    information as to why the request failed.
      virtual Xvt getXvt(const IndexType& id, const CommonTime& t) = 0;

      /// A debugging function that outputs in human readable form,
      /// all data stored in this object.
      /// @param[in] s the stream to receive the output; defaults to cout
      /// @param[in] detail the level of detail to provide
      virtual void dump(std::ostream& s = std::cout, short detail = 0) const = 0;

      /// Edit the dataset, removing data outside the indicated time interval
      /// @param[in] tmin defines the beginning of the time interval
      /// @param[in] tmax defines the end of the time interval
      virtual void edit(const CommonTime& tmin, 
                        const CommonTime& tmax = CommonTime::END_OF_TIME) = 0;

      /// Clear the dataset, meaning remove all data
      virtual void clear(void) = 0;

      /// Return the time system of the store
      virtual TimeSystem getTimeSystem(void) const = 0;

      /// Determine the earliest time for which this object can successfully 
      /// determine the Xvt for any object.
      /// @return The initial time
      /// @throw InvalidRequest This is thrown if the object has no data.
      virtual CommonTime getInitialTime(void) const = 0;

      /// Determine the latest time for which this object can successfully 
      /// determine the Xvt for any object.
      /// @return The final time
      /// @throw InvalidRequest This is thrown if the object has no data.
      virtual CommonTime getFinalTime(void) const = 0;

      /// Return true if velocity data is present in the store
      virtual bool hasVelocity(void) const = 0;

      /// Return true if the given IndexType is present in the store
      virtual bool isPresent(const IndexType& id) const = 0;

   }; // end class XvtStore

   //@}

} // namespace

#endif // XVTSTORE_INCLUDE
