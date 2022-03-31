#pragma ident "$Id$"

/**
 * @file LinearCombinations.hpp
 * This class defines handy linear combinations of GDS data.
 */

#ifndef LINEARCOMBINATIONS_HPP
#define LINEARCOMBINATIONS_HPP

//============================================================================
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2007, 2008, 2009
//
//============================================================================


#include "DataStructures.hpp"
#include "constants.hpp"

namespace gnssSpace
{

      /** @addtogroup DataStructures */
      //@{


      /** This class defines handy linear combinations of GDS data.
       *
       * This class is meant to be used with the GNSS data structures (GDS)
       * objects found in "DataStructures" class, and it is intended to be
       * coupled with class ComputeLinear.hpp.
       *
       * A typical way to use this class follows:
       *
       * @code
       *
       *      // Define a LinearCombinations object
       *   LinearCombinations comb;
       *
       *      // Object to compute linear combinations of data
       *      // Linear combinations will be computed in a FIFO basis
       *   ComputeLinear linear;
       *
       *      // Add a linear combination to compute PC combination using C1
       *   linear.addLinear(comb.pcCombWithC1);
       *
       *      // Add a linear combination to compute prefit residual using PC
       *   linear.addLinear(comb.pcPrefit);
       *
       *
       *      // Load observation data
       *   RinexObsStream rin("ebre0300.02o");
       *
       *      // Loads precise ephemeris object with file data
       *   SP3EphemerisStore SP3EphList;
       *   SP3EphList.loadFile("igs11513.sp3");
       *
       *      // Sets nominal position of receiver
       *   Position nominalPos(4833520.3800, 41536.8300, 4147461.2800);
       *
       *      // Declare a MOPSTropModel object, setting the defaults
       *   MOPSTropModel mopsTM( nominalPos.getAltitude(),
       *                         nominalPos.getGeodeticLatitude(), 30);
       *
       *      // Object to compute the tropospheric data
       *   ComputeTropModel computeTropo(mopsTM);
       *
       *      // Declare a basic modeler
       *   BasicModel basic(nominalPos, SP3EphList);
       *
       *   gnssRinex gRin;
       *
       *   while(rin >> gRin)
       *   {
       *
       *      gRin >> basic >> computeTropo >> linear;
       *
       *         // Dump results
       *      gRin.body.dump(cout,1);
       *
       *   }
       *
       * @endcode
       *
       * @sa ComputeLinear.hpp
       */
    class LinearCombinations
    {
    public:

        /// Default constructor
        LinearCombinations();

        /// Definition to compute PC combination of GPS, C1 and C2
        gnssLinearCombination pc12CombOfGPS;
        gnssLinearCombination lc12CombOfGPS;

        gnssLinearCombination pc15CombOfGPS;
        gnssLinearCombination lc15CombOfGPS;

        gnssLinearCombination ifcbPC1215G;
        gnssLinearCombination ifcbLC1215G;

        /// Definition to compute PC combination of Galileo, C1 and C5
        gnssLinearCombination pc15CombOfGAL;
        gnssLinearCombination lc15CombOfGAL;

        gnssLinearCombination pc16CombOfGAL;
        gnssLinearCombination lc16CombOfGAL;

        /// Definition to compute PC combination of BDS, C2 and C7
        gnssLinearCombination pc27CombOfBDS;
        gnssLinearCombination lc27CombOfBDS;

        gnssLinearCombination pc26CombOfBDS;
        gnssLinearCombination lc26CombOfBDS;

        /// Definition to compute the MW combination of GPS, C1+C2 and L1+L2
        gnssLinearCombination mw12CombOfGPS;
        gnssLinearCombination mw15CombOfGPS;

        gnssLinearCombination mw21CombOfGPS; // wavelength=5.8610451221896
        gnssLinearCombination mw25CombOfGPS; // wavelength=5.8610451221896

        /// Definition to compute the MW combination of GPS, C1+C5 and L1+L5

        /// Definition to compute the MW combination of Galileo, C1+C5 and L1+L5
        gnssLinearCombination mw58CombOfGAL;
        gnssLinearCombination mw57CombOfGAL;
        gnssLinearCombination mw56CombOfGAL;
        gnssLinearCombination mw51CombOfGAL;

        gnssLinearCombination mw15CombOfGAL;
  
        gnssLinearCombination mw65CombOfGAL;
        gnssLinearCombination mw68CombOfGAL; 
        gnssLinearCombination mw67CombOfGAL;
        gnssLinearCombination mw61CombOfGAL;

        gnssLinearCombination mw75CombOfGAL;
        gnssLinearCombination mw78CombOfGAL; 
        gnssLinearCombination mw71CombOfGAL;

        //...

        /// Definition to compute the MW combination of BDS, C2+C7 and L2+L7
        gnssLinearCombination mw25CombOfBDS;
        gnssLinearCombination mw27CombOfBDS;
        gnssLinearCombination mw28CombOfBDS;
        gnssLinearCombination mw26CombOfBDS;
        gnssLinearCombination mw21CombOfBDS;
        gnssLinearCombination mw12CombOfBDS;
  
        gnssLinearCombination mw65CombOfBDS; // 
        gnssLinearCombination mw67CombOfBDS; //
        gnssLinearCombination mw68CombOfBDS; //
        gnssLinearCombination mw62CombOfBDS; //
        gnssLinearCombination mw61CombOfBDS; //

        gnssLinearCombination mw21CombOfQZS;
        gnssLinearCombination mw25CombOfQZS;

        gnssLinearCombination li12CombOfGPS;
        gnssLinearCombination li15CombOfGAL;
        gnssLinearCombination li27CombOfBDS;

        /// Definition to compute prefit residual of GPS
        gnssLinearCombination c1PrefitOfGPS;
        gnssLinearCombination c2PrefitOfGPS;
        gnssLinearCombination c5PrefitOfGPS;

        gnssLinearCombination BL1CombOfGPS;
        gnssLinearCombination BL2CombOfGPS;

        /// Definition to compute prefit residual of Galileo
        gnssLinearCombination c1PrefitOfGAL;
        gnssLinearCombination c5PrefitOfGAL;
        gnssLinearCombination c6PrefitOfGAL;
        gnssLinearCombination c7PrefitOfGAL;
        gnssLinearCombination c8PrefitOfGAL;


        /// Definition to compute prefit residual of BDS
        gnssLinearCombination c1PrefitOfBDS;
        gnssLinearCombination c2PrefitOfBDS;
        gnssLinearCombination c5PrefitOfBDS;
        gnssLinearCombination c8PrefitOfBDS;
        gnssLinearCombination c7PrefitOfBDS;
        gnssLinearCombination c6PrefitOfBDS;

        // GLONASS
        gnssLinearCombination c1PrefitOfGLO;
        gnssLinearCombination c2PrefitOfGLO;
        gnssLinearCombination c3PrefitOfGLO;
        gnssLinearCombination c4PrefitOfGLO;
        gnssLinearCombination c6PrefitOfGLO;

        // QZSS
        gnssLinearCombination c1PrefitOfQZS;
        gnssLinearCombination c2PrefitOfQZS;
        gnssLinearCombination c5PrefitOfQZS;
        gnssLinearCombination c6PrefitOfQZS;

        /// Definition to compute prefit residual of GPS
        gnssLinearCombination l1PrefitOfGPS;
        gnssLinearCombination l2PrefitOfGPS;
        gnssLinearCombination l5PrefitOfGPS;

        /// Definition to compute prefit residual of Galileo
        gnssLinearCombination l1PrefitOfGAL;
        gnssLinearCombination l5PrefitOfGAL;
        gnssLinearCombination l6PrefitOfGAL;
        gnssLinearCombination l7PrefitOfGAL;
        gnssLinearCombination l8PrefitOfGAL;

        /// Definition to compute prefit residual of BDS
        gnssLinearCombination l1PrefitOfBDS;
        gnssLinearCombination l2PrefitOfBDS;
        gnssLinearCombination l5PrefitOfBDS;
        gnssLinearCombination l6PrefitOfBDS;
        gnssLinearCombination l7PrefitOfBDS;
        gnssLinearCombination l8PrefitOfBDS;

        // GLONASS
        gnssLinearCombination l1PrefitOfGLO;
        gnssLinearCombination l2PrefitOfGLO;
        gnssLinearCombination l3PrefitOfGLO;
        gnssLinearCombination l4PrefitOfGLO;
        gnssLinearCombination l6PrefitOfGLO;

        /// QZSS phase
        gnssLinearCombination l1PrefitOfQZS;
        gnssLinearCombination l2PrefitOfQZS;
        gnssLinearCombination l5PrefitOfQZS;
        gnssLinearCombination l6PrefitOfQZS;

        /// Definition to compute prefit residual of GPS
        gnssLinearCombination pc12PrefitOfGPS;
        gnssLinearCombination lc12PrefitOfGPS;

        gnssLinearCombination pc21PrefitOfGPS;
        gnssLinearCombination lc21PrefitOfGPS;

        gnssLinearCombination pc25PrefitOfGPS;
        gnssLinearCombination lc25PrefitOfGPS;

        /// Definition to compute prefit residual of Galileo
        gnssLinearCombination pc15PrefitOfGAL;
        gnssLinearCombination lc15PrefitOfGAL;

        /// Definition to compute prefit residual of BDS
        gnssLinearCombination pc27PrefitOfBDS;
        gnssLinearCombination lc27PrefitOfBDS;

        gnssLinearCombination pc26PrefitOfBDS;
        gnssLinearCombination lc26PrefitOfBDS;

        /// definitaion to compute the slant ionospheric delays
        gnssLinearCombination ionoC1G;
        gnssLinearCombination ionoC1E;
        gnssLinearCombination ionoC2C;

    public:

        /// Return the frequency of the combination in cycles: i * L1 + j * L2
        static double freqOfLC(int i, int j, double f1 = L1_FREQ_GPS, double f2 = L2_FREQ_GPS);

        /// Return the wavelength of the combination in cycles: i * L1 + j * L2
        static double wavelengthOfLC(int i, int j, double f1 = L1_FREQ_GPS, double f2 = L2_FREQ_GPS);
        /// Return the f1 factor of the combination in cycles: i * L1 + j * L2
        static double firstFactorOfLC(int i, int j, double f1 = L1_FREQ_GPS, double f2 = L2_FREQ_GPS);
        /// Return the f2 factor of the combination in cycles: i * L1 + j * L2
        static double secondFactorOfLC(int i, int j, double f1 = L1_FREQ_GPS, double f2 = L2_FREQ_GPS);

    }; // End of class 'LinearCombinations'


    double wavelengthOfMW(const SatelliteSystem& sys, const TypeID& type);
    double varOfMW(const SatelliteSystem& sys, const TypeID& type);

    //@}

}

#endif // LINEARCOMBINATIONS_HPP
