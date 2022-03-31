#pragma ident "$Id$"



/**
 * @file Xv.hpp
 * Position and velocity, representation as Triples
 */

#ifndef XV_HPP
#define XV_HPP


#include <iostream>
#include "Triple.hpp"

using namespace mathSpace;

namespace coordSpace
{
    /** @addtogroup geodeticgroup */
    //@{

      /// An Earth-Centered, Earth-Fixed position/velocity/clock representation
   class Xv
   {
   public:
         /// Default constructor
      Xv() { }

      Triple x;         ///< SV position (x,y,z). Earth-fixed. meters
      Triple v;       ///< SV velocity. Earth-fixed, including rotation. meters/sec

   }; 

   //@}

}

/**
 * Output operator for Xv
 * @param s output stream to which xv is sent
 * @param xv Xv that is sent to s
 */
inline std::ostream& operator<<( std::ostream& s, 
                          const coordSpace::Xv& xv )
{
   s << "x:" << xv.x
     << ", v:" << xv.v;
   return s;
}

#endif
