#ifndef SATID_HPP
#define SATID_HPP

#include <set>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Exception.hpp"
#include "constants.hpp"
#include "SatelliteSystem.hpp"

/**
 * @file SatID.hpp
 * gnssSpace::SatID - navigation system-independent representation of a satellite.
 */

using namespace std;

namespace gnssSpace
{

    /// Satellite identifier consisting of a satellite number (PRN, etc.)
    /// and a satellite system
    class SatID : public SatelliteSystem
    {
    public:

        /// empty constructor, creates an invalid object
        SatID() 
        { id=-1; }

        /// constructor, creates an invalid object
        SatID(int prn, SatelliteSystem::Systems sys) 
            : id(prn), SatelliteSystem(sys)
        {}

        /// constructor, creates an invalid object
        SatID(SatelliteSystem::Systems sys, int prn ) 
            : id(prn), SatelliteSystem(sys)
        {}

        /// exlicit constructor
        SatID(const std::string& str) 
            noexcept(false)
        {
            try { fromString(str); }
            catch(Exception& e) { RETHROW(e); }
        }

        void fromString(const std::string s)
            noexcept(false)
        {
            char c;
            std::istringstream iss(s);

            id = -1; system = SatelliteSystem::GPS;    // default

            // all whitespace yields the default
            if(s.find_first_not_of(std::string(" \t\n"), 0) == std::string::npos)
                return;                     

            // read one character (non-whitespace)
            iss >> c;                       

            // no leading system character
            if(isdigit(c)!=0)
            {
               system = SatelliteSystem::GPS;
               iss.putback(c);
            }
            else
            {
               SatelliteSystem::fromChar(c);
            }

            iss >> id;
            if(id <= 0) id = -1;
        }

        std::string toString() const 
            noexcept(false)
        {
            std::ostringstream oss;
            oss.fill(fillchar);
            oss << SatelliteSystem::toChar() << std::setw(2) << id;
            return oss.str();
        }

        /// operator == for SatID
        bool operator==(const SatID& right) const
        { return ((system == right.system) && (id == right.id)); }

        /// operator != for SatID
        bool operator!=(const SatID& right) const
        { return !(operator==(right)); }

        /// operator < for SatID : order by system, then number
        bool operator<(const SatID& right) const
        {
            if (system==right.system)
                return (id<right.id);
            return (system<right.system);
        }

        /// operator > for SatID
        bool operator>(const SatID& right) const
        {  return (!operator<(right) && !operator==(right)); }

        /// operator <= for SatID
        bool operator<=(const SatID& right) const
        { return (operator<(right) || operator==(right)); }

        /// operator >= for SatID
        bool operator>=(const SatID& right) const
        { return !(operator<(right)); }

        bool isValid() const
        {
            switch(system)
            {
                case GPS:     return (id > 0 && id <= MAX_PRN_GPS);
                case GLONASS: return (id > 0 && id <= MAX_PRN_GLO);
                case Galileo: return (id > 0 && id <= MAX_PRN_GAL);
                case BDS:     return (id > 0 && id <= MAX_PRN_BDS);
                case Unknown:     return (id > 200);
                default: return (id > 0 && id < 1000);
            }
        }



        int id;                   ///< satellite identifier, e.g. PRN
        static char fillchar;  ///< fill character used during stream output

    }; // class SatID

    /// stream output for SatID
    inline std::ostream& operator<<(std::ostream& s, const SatID& p)
    {
        s << p.toString();
        return s;
    }

} // namespace gnssSpace

#endif
