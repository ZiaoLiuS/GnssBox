#pragma ident "$Id$"

/**
 * @file DataStructures.cpp
 * Set of several data structures to be used by other GPSTk classes.
 */

//============================================================================
//
//  Revision
//
//  2012/06/19   Add 'getGnssRinex(SatID& sat)' method for class sourceRinex.
//               The new method will return the first gnssRinex data which
//               contain the given satellite. ( 2012/06/19 )
//
//  2014/03/16   add the sourceNumber for 'source' in header.
//               f.header.source.sourceNumber= roh.markerNumber;
//
//  2014/07/15   Define a temporary 'source' to initialize the value of
//               the source in header; (shjzhang)
//
//============================================================================


#include "DataStructures.hpp"

using namespace utilSpace;
using namespace mathSpace;
using namespace std;

namespace gnssSpace
{

      ////// typeValueMap //////

      // Return a TypeIDSet with all the data types present in
      // this object.
    TypeIDSet typeValueMap::getTypeID() const
    {

        TypeIDSet typeSet;

        for( typeValueMap::const_iterator pos = (*this).begin();
             pos != (*this).end();
             ++pos )
        {
            typeSet.insert( (*pos).first );
        }

        return typeSet;

    }  // End of method 'typeValueMap::getTypeID()'



      // Return a typeValueMap with only this type of data.
      // @param type Type of value to be extracted.
    typeValueMap typeValueMap::extractTypeID(const TypeID& type) const
    {

        TypeIDSet typeSet;
        typeSet.insert(type);

        return extractTypeID(typeSet);

    }  // End of method 'typeValueMap::extractTypeID()'



      // Return a typeValueMap with only these types of data.
      // @param typeSet Set (TypeIDSet) containing the types of data to
      //                be extracted.
    typeValueMap typeValueMap::extractTypeID(const TypeIDSet& typeSet) const
    {

        typeValueMap tvMap;

        for( TypeIDSet::const_iterator pos = typeSet.begin();
             pos != typeSet.end();
             ++pos )
        {

            typeValueMap::const_iterator itObs( (*this).find(*pos) );
            if( itObs != (*this).end() )
            {
                tvMap[ (*itObs).first ] = (*itObs).second;
            }
        }

        return tvMap;

    }  // End of method 'typeValueMap::extractTypeID()'


      // Modifies this object, keeping only this type of data.
      // @param type Type of value to be kept.
    typeValueMap& typeValueMap::keepOnlyTypeID(const TypeID& type)
    {

        TypeIDSet typeSet;
        typeSet.insert(type);

        return (keepOnlyTypeID(typeSet));

    }  // End of method 'typeValueMap::keepOnlyTypeID()'



    // Modifies this object, keeping only these types of data.
    // @param typeSet Set (TypeIDSet) containing the types of data
    //                to be kept.
    typeValueMap& typeValueMap::keepOnlyTypeID(const TypeIDSet& typeSet)
    {

        typeValueMap tvMap( (*this).extractTypeID(typeSet) );
        (*this) = tvMap;

        return (*this);

    }  // End of method 'typeValueMap::keepOnlyTypeID()'



      // Modifies this object, removing these types of data.
      // @param typeSet Set (TypeIDSet) containing the types of data
      //                to be kept.
    typeValueMap& typeValueMap::removeTypeID(const TypeIDSet& typeSet)
    {

        for( TypeIDSet::const_iterator pos = typeSet.begin();
             pos != typeSet.end();
             ++pos )
        {
            (*this).erase(*pos);
        }

        return (*this);

    }  // End of method 'typeValueMap::removeTypeID()'



      /* Return the data value (double) corresponding to provided type.
       *
       * @param type       Type of value to be looked for.
       */
    double typeValueMap::getValue(const TypeID& type) const
        noexcept(false)
    {

        typeValueMap::const_iterator itObs( (*this).find(type) );
        if ( itObs != (*this).end() )
        {
            return (*itObs).second;
        }
        else
        {
            string typeStr = asString(type);
            THROW(TypeIDNotFound(typeStr+"TypeID not found in map"));
        }

    }  // End of method 'typeValueMap::getValue()'



      // Return a reference to the data value (double) with
      // corresponding type.
      // @param type Type of value to be looked for.
    double& typeValueMap::operator()(const TypeID& type)
        noexcept(false)
    {

        typeValueMap::iterator itObs ( (*this).find(type) );

        if ( itObs != (*this).end() )
        {
            return (*itObs).second;
        }
        else
        {
            THROW(TypeIDNotFound("TypeID not found in map"));
        }

    }  // End of method 'typeValueMap::operator()'


      // Convenience output method for structure satTypeValueMap
    std::ostream& typeValueMap::dump( std::ostream& s, int mode ) const
    {

        for(typeValueMap::const_iterator itObs = (*this).begin();
            itObs != (*this).end();
            ++itObs)
        {

            if (mode==1)
            {
                s << (*itObs).first << " ";
            }

            s << setprecision(12) ;
            s << (*itObs).second << " ";

        }  // End of 'for( typeValueMap::const_iterator itObs = ...'

         // Let's return the 'std::ostream'
        return s;

    }  // End of method 'satTypeValueMap::dump()'

      ////// satValueMap //////

      // Return a SatIDSet with all the satellites present in this object.
    SatIDSet satValueMap::getSatID() const
    {

        SatIDSet satSet;

        for( satValueMap::const_iterator pos = (*this).begin();
             pos != (*this).end();
             ++pos )
        {
            satSet.insert( (*pos).first );
        }

        return satSet;

    }  // End of method 'satValueMap::getSatID()'



      // Return a satValueMap with only this satellite.
      // @param satellite Satellite to be extracted.
    satValueMap satValueMap::extractSatID(const SatID& satellite) const
    {

        SatIDSet satSet;
        satSet.insert(satellite);

        return extractSatID(satSet);

    }  // End of method 'satValueMap::extractSatID()'


          // Return a satValueMap with only these satellites.
      // @param satSet Set (SatIDSet) containing the satellites to
      //               be extracted.
    satValueMap satValueMap::extractSatID(const SatIDSet& satSet) const
    {

        satValueMap svMap;

        for( SatIDSet::const_iterator pos = satSet.begin();
             pos != satSet.end();
             ++pos )
        {
            satValueMap::const_iterator itObs( (*this).find(*pos) );

            if( itObs != (*this).end() )
            {
                svMap[ (*itObs).first ] = (*itObs).second;
            }
        }

        return svMap;

    }  // End of method 'satValueMap::extractSatID()'



      // Modifies this object, keeping only this satellite.
      // @param satellite Satellite to be kept.
    satValueMap& satValueMap::keepOnlySatID(const SatID& satellite)
    {

        SatIDSet satSet;
        satSet.insert(satellite);

        return keepOnlySatID(satSet);

    }  // End of method 'satValueMap::keepOnlySatID()'


    // Modifies this object, keeping only these satellites.
    // @param satSet Set (SatIDSet) containing the satellites to be kept.
    satValueMap& satValueMap::keepOnlySatID(const SatIDSet& satSet)
    {

        satValueMap svMap = extractSatID(satSet);
        (*this) = svMap;

        return (*this);

    }  // End of method 'satValueMap::keepOnlySatID()'


    // Modifies this object, removing these satellites.
    // @param satSet Set (SatIDSet) containing the satellites to
    //               be removed.
    satValueMap& satValueMap::removeSatID(const SatIDSet& satSet)
    {

        for( SatIDSet::const_iterator pos = satSet.begin();
             pos != satSet.end();
             ++pos )
        {
            (*this).erase(*pos);
        }

        return (*this);

    }  // End of method 'satValueMap::removeSatID()'


      /* Return the data value (double) corresponding to provided SatID.
       *
       * @param satellite     Satellite to be looked for.
       */
    double satValueMap::getValue(const SatID& satellite) const
        noexcept(false)
    {

        satValueMap::const_iterator itObs( (*this).find(satellite) );
        if ( itObs != (*this).end() )
        {
            return (*itObs).second;
        }
        else
        {
            THROW(SatIDNotFound("SatID not found in map"));
        }

    }  // End of method 'satValueMap::getValue()'




      // Return a reference to the data value (double) with
      // corresponding SatID.
      // @param satellite Satellite to be looked for.
    double& satValueMap::operator()(const SatID& satellite)
        noexcept(false)
    {

        satValueMap::iterator itObs( (*this).find(satellite) );

        if ( itObs != (*this).end() )
        {
            return (*itObs).second;
        }
        else
        {
            THROW(SatIDNotFound("SatID not found in map"));
        }

    }  // End of method 'satValueMap::operator()'


      // Convenience output method for structure satValueMap
    std::ostream& satValueMap::dump( std::ostream& s, int mode ) const
    {

        for(satValueMap::const_iterator it = (*this).begin();
            it != (*this).end();
            ++it)
        {

            // First, print satellite (system and PRN)
            s << (*it).first << " " << (*it).second;
            s << endl;

        }  // End of 'for( satValueMap::const_iterator it = ...'

         // Let's return the 'std::ostream'
        return s;

    }  // End of method 'satValueMap::dump()'


      ////// satTypeValueMap //////


      /* Return the total number of data elements in the map.
       * This method DOES NOT suppose that all the satellites have
       * the same number of type values.
       */
    size_t satTypeValueMap::numElements() const
    {

        size_t numEle(0);

        for( satTypeValueMap::const_iterator it = (*this).begin();
             it != (*this).end();
             ++it )
        {
            numEle = numEle + (*it).second.size();
        }

        return numEle;

    }  // End of method 'satTypeValueMap::numElements()'



      // Return a SatIDSet with all the satellites present in this object.
    SatIDSet satTypeValueMap::getSatID() const
    {

        SatIDSet satSet;

        for( satTypeValueMap::const_iterator pos = (*this).begin();
             pos != (*this).end();
             ++pos )
        {
            satSet.insert( (*pos).first );
        }

        return satSet;

    }  // End of method 'satTypeValueMap::getSatID()'


      // Return a TypeIDSet with all the data types present in
      // this object. This does not imply that all satellites have these types.
    TypeIDSet satTypeValueMap::getTypeID() const
    {

        TypeIDSet typeSet;

        for( satTypeValueMap::const_iterator pos = (*this).begin();
             pos != (*this).end();
             ++pos )
        {

            for( typeValueMap::const_iterator it = (*pos).second.begin();
                 it != (*pos).second.end();
                 ++it )
            {
                typeSet.insert( (*it).first );
            }

        }

        return typeSet;

    }  // End of method 'satTypeValueMap::getTypeID()'



      // Return a satTypeValueMap with only this satellite.
      // @param satellite Satellite to be extracted.
    satTypeValueMap satTypeValueMap::extractSatID(const SatID& satellite) const
    {

        SatIDSet satSet;
        satSet.insert(satellite);

        return extractSatID(satSet);

    }  // End of method 'satTypeValueMap::extractSatID()'


    satTypeValueMap satTypeValueMap::extractSatID(const SatIDSet& satSet) const
    {

        satTypeValueMap stvMap;

        for( SatIDSet::const_iterator pos = satSet.begin();
             pos != satSet.end();
             ++pos )
        {
            satTypeValueMap::const_iterator itObs( (*this).find(*pos) );
            if( itObs != (*this).end() )
            {
                stvMap[ (*itObs).first ] = (*itObs).second;
            }
        }

        return stvMap;

    }  // End of method 'satTypeValueMap::extractSatID()'



      // Modifies this object, keeping only this satellite.
      // @param satellite Satellite to be kept.
    satTypeValueMap& satTypeValueMap::keepOnlySatID(const SatID& satellite)
    {

        SatIDSet satSet;
        satSet.insert(satellite);

        return keepOnlySatID(satSet);

    }  // End of method 'satTypeValueMap::keepOnlySatID()'

      // Modifies this object, keeping only these satellites.
      // @param satSet Set (SatIDSet) containing the satellites to be kept.
    satTypeValueMap& satTypeValueMap::keepOnlySatID(const SatIDSet& satSet)
    {

        satTypeValueMap stvMap( extractSatID(satSet) );
        (*this) = stvMap;

        return (*this);

    }  // End of method 'satTypeValueMap::keepOnlySatID()'



      // Return a satTypeValueMap with only this type of value.
      // @param type Type of value to be extracted.
    satTypeValueMap satTypeValueMap::extractTypeID(const TypeID& type) const
    {

        TypeIDSet typeSet;
        typeSet.insert(type);

        return extractTypeID(typeSet);

    }  // End of method 'satTypeValueMap::extractTypeID()'

          // Return a satTypeValueMap with only these types of data.
      // @param typeSet Set (TypeIDSet) containing the types of data
      //                to be extracted.
    satTypeValueMap satTypeValueMap::extractTypeID(const TypeIDSet& typeSet) const
    {

        satTypeValueMap theMap;

        for( satTypeValueMap::const_iterator it = (*this).begin();
             it != (*this).end();
             ++it )
        {

            typeValueMap tvMap( (*it).second.extractTypeID(typeSet) );
            if( tvMap.size() > 0 )
            {
                theMap[(*it).first] = tvMap;
            }

        }

        return theMap;

    }  // End of method 'satTypeValueMap::extractTypeID()'


      // Modifies this object, keeping only this type of data.
      // @param type Type of value to be kept.
    satTypeValueMap& satTypeValueMap::keepOnlyTypeID(const TypeID& type)
    {

        TypeIDSet typeSet;
        typeSet.insert(type);

        return keepOnlyTypeID(typeSet);

    }  // End of method 'satTypeValueMap::keepOnlyTypeID()'

          // Modifies this object, keeping only these types of data.
      // @param typeSet Set (TypeIDSet) containing the types of data
      //                to be kept.
    satTypeValueMap& satTypeValueMap::keepOnlyTypeID(const TypeIDSet& typeSet)
    {

        satTypeValueMap stvMap( extractTypeID(typeSet) );
        (*this) = stvMap;

        return (*this);

    }  // End of method 'satTypeValueMap::keepOnlyTypeID()'



      // Modifies this object, removing this type of data.
      // @param type Type of value to be removed.
    satTypeValueMap& satTypeValueMap::removeTypeID(const TypeID& type)
    {

        for( satTypeValueMap::iterator it = (*this).begin();
             it != (*this).end();
             ++it )
        {
            (*it).second.removeTypeID(type);
        }

        return (*this);

    }  // End of method 'satTypeValueMap::removeTypeID()'




    // Modifies this object, removing these satellites.
    // @param satSet Set (SatIDSet) containing the satellites
    //               to be removed.
    satTypeValueMap& satTypeValueMap::removeSatID(const SatIDSet& satSet)
    {

        for( SatIDSet::const_iterator pos = satSet.begin();
             pos != satSet.end();
             ++pos )
        {
            (*this).erase(*pos);
        }

        return (*this);

    }  // End of method 'satTypeValueMap::removeSatID()'




      /* Return the data value (double) corresponding to provided SatID
       * and TypeID.
       *
       * @param satellite     Satellite to be looked for.
       * @param type          Type to be looked for.
       */
    double satTypeValueMap::getValue( const SatID& satellite,
                                      const TypeID& type ) const
        noexcept(false)
    {

        satTypeValueMap::const_iterator itObs( (*this).find(satellite) );
        if( itObs != (*this).end() )
        {
            return (*itObs).second.getValue( type );
        }
        else
        {
            THROW(SatIDNotFound("SatID not found in map"));
        }

    }  // End of method 'satTypeValueMap::getValue()'



      // Return a reference to the typeValueMap with corresponding SatID.
      // @param type Type of value to be looked for.
    typeValueMap& satTypeValueMap::operator()(const SatID& satellite) 
    {

        satTypeValueMap::iterator itObs( (*this).find(satellite) );
        if( itObs != (*this).end() )
        {
            return (*itObs).second;
        }
        else
        {
            cerr << (SatIDNotFound("SatID not found in map"));
        }

    }  // End of method 'satTypeValueMap::operator()'


      ////// Other stuff //////

      // Convenience output method for structure satTypeValueMap
    std::ostream& satTypeValueMap::dump( std::ostream& s, int mode ) const
    {

        for(satTypeValueMap::const_iterator it = (*this).begin();
            it != (*this).end();
            ++it)
        {

            // First, print satellite (system and PRN)
            s << (*it).first << " ";

            for(typeValueMap::const_iterator itObs = (*it).second.begin();
                itObs != (*it).second.end();
                ++itObs)
            {

                if (mode==1)
                {
                    s << (*itObs).first << " ";
                }

                s << setprecision(6) ;
                s << (*itObs).second << " ";

            }  // End of 'for( typeValueMap::const_iterator itObs = ...'

            s << endl;

        }  // End of 'for( satTypeValueMap::const_iterator it = ...'

         // Let's return the 'std::ostream'
        return s;

    }  // End of method 'satTypeValueMap::dump()'


      // stream output for satTypeValueMap
    std::ostream& operator<<( std::ostream& s, const typeValueMap& tvMap )
    {

        tvMap.dump(s,1);
        return s;

    }  // End of 'operator<<'

      // stream output for satTypeValueMap
    std::ostream& operator<<( std::ostream& s, const satValueMap& svMap )
    {

        svMap.dump(s,1);
        return s;

    }  // End of 'operator<<'


      // stream output for satTypeValueMap
    std::ostream& operator<<( std::ostream& s, const satTypeValueMap& stvMap )
    {

        stvMap.dump(s,1);
        return s;

    }  // End of 'operator<<'

}  // End of namespace gnssSpace
