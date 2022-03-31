
/**
 * @file LinearCombinations.cpp
 * This class defines handy linear combinations of GDS data.
 */

//============================================================================
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2007, 2008, 2009
//
//============================================================================


#include "LinearCombinations.hpp"

#define debug 0

namespace gnssSpace
{

    LinearCombinations::LinearCombinations()
    {

        // Coefficients of GPS PC/LC combinations, L1 and L2
        const double ag21( 1.0            /(  1.0 - GAMMA_GPS_L1L2 ) );
        const double bg21( GAMMA_GPS_L1L2 /(  1.0 - GAMMA_GPS_L1L2 ) );

        // ag12 = (f1^2)/(f1^2-f2^2) = GAMMA_GPS_L1L2/(GAMMA_GPS_L1L2 - 1.0 )
        // bg12 = (f2^2)/(f1^2-f2^2) = 1.0/(GAMMA_GPS_L1L2 - 1.0)
        const double ag12( GAMMA_GPS_L1L2 /(  GAMMA_GPS_L1L2 - 1.0 ) );
        const double bg12( 1.0            /(  GAMMA_GPS_L1L2 - 1.0 ) );

        // ag15 = (f1^2)/(f1^2-f5^2) = GAMMA_GPS_L1L5/(GAMMA_GPS_L1L5 - 1.0 )
        // bg15 = (f5^2)/(f1^2-f5^2) = 1.0/(GAMMA_GPS_L1L5 - 1.0)
        const double ag15( GAMMA_GPS_L1L5 /(  GAMMA_GPS_L1L5 - 1.0 ) );
        const double bg15( 1.0            /(  GAMMA_GPS_L1L5 - 1.0 ) );

        // Galileo
        // Coefficients of Galileo PC/LC combinations, L1 and L5
        const double ae51( 1.0            /( 1.0  - GAMMA_GAL_L1L5 ) );
        const double be51( GAMMA_GAL_L1L5 /( 1.0  - GAMMA_GAL_L1L5 ) );

        const double ae15( GAMMA_GAL_L1L5 /( GAMMA_GAL_L1L5 - 1.0) );
        const double be15( 1.0            /( GAMMA_GAL_L1L5 - 1.0) );

        const double ae16( GAMMA_GAL_L1L6 /( GAMMA_GAL_L1L6 - 1.0) );
        const double be16( 1.0            /( GAMMA_GAL_L1L6 - 1.0) );

        // BDS
        // Coefficients of BDS PC/LC combinations, L2 and L7
        // ac72 = (f7^2*L7 - f2^2*L2)/(f7^2 - f2^2)
        //      = (L7 - GAMMA_BDS_L2L7 * L2)/(1 - GAMMA_BDS_L2L7)
        const double ac72( 1.0            /( 1.0 -  GAMMA_BDS_L2L7 ) );
        const double bc72( GAMMA_BDS_L2L7 /( 1.0 -  GAMMA_BDS_L2L7 ) );

        // Coefficients of BDS PC/LC combinations, L2 and L7
        // ac27 = (f2^2*L2 - f7^2*L7)/(f2^2 - f7^2)
        //      = (GAMMA_BDS_L2L7 * L2 - L7)/(GAMMA_BDS_L2L7 - 1)
        const double ac27( GAMMA_BDS_L2L7 /( GAMMA_BDS_L2L7 - 1.0 ) );
        const double bc27( 1.0            /( GAMMA_BDS_L2L7 - 1.0 ) );


        pc12CombOfGPS.header                =   TypeID::PC12G;
        pc12CombOfGPS.body[TypeID::C1G]     =   +ag12;
        pc12CombOfGPS.body[TypeID::C2G]     =   -bg12;

        // Definition to compute LC combination of GPS, L1 and L2
        lc12CombOfGPS.header                =   TypeID::LC12G;
        lc12CombOfGPS.body[TypeID::L1G]     =   +ag12;
        lc12CombOfGPS.body[TypeID::L2G]     =   -bg12;

        pc15CombOfGPS.header                =   TypeID::PC15G;
        pc15CombOfGPS.body[TypeID::C1G]     =   +ag15;
        pc15CombOfGPS.body[TypeID::C5G]     =   -bg15;

        // Definition to compute LC combination of GPS, L1 and L5
        lc15CombOfGPS.header                =   TypeID::LC15G;
        lc15CombOfGPS.body[TypeID::L1G]     =   +ag15;
        lc15CombOfGPS.body[TypeID::L5G]     =   -bg15;

        ifcbPC1215G.header                    =   TypeID::IFCBPC1215G;
        ifcbPC1215G.body[TypeID::PC12G]       =   +1.0;
        ifcbPC1215G.body[TypeID::PC15G]       =   -1.0;

        ifcbLC1215G.header                    =   TypeID::IFCBLC1215G;
        ifcbLC1215G.body[TypeID::LC12G]       =   +1.0;
        ifcbLC1215G.body[TypeID::LC15G]       =   -1.0;

        //=====================

        // Definition to compute PC combination of Galileo, C1 and C5
        pc15CombOfGAL.header                =   TypeID::PC15E;
        pc15CombOfGAL.body[TypeID::C1E]     =   +ae15;
        pc15CombOfGAL.body[TypeID::C5E]     =   -be15;

        // Definition to compute LC combination of Galileo, L1 and L5
        lc15CombOfGAL.header                =   TypeID::LC15E;
        lc15CombOfGAL.body[TypeID::L1E]     =   +ae15;
        lc15CombOfGAL.body[TypeID::L5E]     =   -be15;

        // Definition to compute PC combination of Galileo, C1 and C6
        pc16CombOfGAL.header                =   TypeID::PC16E;
        pc16CombOfGAL.body[TypeID::C1E]     =   +ae16;
        pc16CombOfGAL.body[TypeID::C6E]     =   -be16;

        // Definition to compute LC combination of Galileo, L1 and L6
        lc16CombOfGAL.header                =   TypeID::LC16E;
        lc16CombOfGAL.body[TypeID::L1E]     =   +ae16;
        lc16CombOfGAL.body[TypeID::L6E]     =   -be16;

        //=====================

        // Coefficients of BDS PC/LC combinations, L2 and L7
        // ac26 = (f2^2*L2 - f6^2*L6)/(f2^2 - f6^2)
        //      = (GAMMA_BDS_L2L6 * L2 - L6)/(GAMMA_BDS_L2L6 - 1)
        const double ac26( GAMMA_BDS_L2L6 /( GAMMA_BDS_L2L6 - 1.0 ) );
        const double bc26( 1.0            /( GAMMA_BDS_L2L6 - 1.0 ) );

         // Definition to compute PC combination of BDS, C2 and C6
        pc26CombOfBDS.header                =   TypeID::PC26C;
        pc26CombOfBDS.body[TypeID::C2C]     =   +ac26;
        pc26CombOfBDS.body[TypeID::C6C]     =   -bc26;

        lc26CombOfBDS.header                =   TypeID::LC26C;
        lc26CombOfBDS.body[TypeID::L2C]     =   +ac26;
        lc26CombOfBDS.body[TypeID::L6C]     =   -bc26;

        double cg12( L1_FREQ_GPS/(L1_FREQ_GPS - L2_FREQ_GPS) );
        double dg12( L2_FREQ_GPS/(L1_FREQ_GPS - L2_FREQ_GPS) );
        double eg12( L1_FREQ_GPS/(L1_FREQ_GPS + L2_FREQ_GPS) );
        double fg12( L2_FREQ_GPS/(L1_FREQ_GPS + L2_FREQ_GPS) );

        double cg15( L1_FREQ_GPS/(L1_FREQ_GPS - L5_FREQ_GPS) );
        double dg15( L5_FREQ_GPS/(L1_FREQ_GPS - L5_FREQ_GPS) );
        double eg15( L1_FREQ_GPS/(L1_FREQ_GPS + L5_FREQ_GPS) );
        double fg15( L5_FREQ_GPS/(L1_FREQ_GPS + L5_FREQ_GPS) );

        // Coefficients of GPS PW/WL/PN/LN combinations, L2 and L5
        double cg21( L2_FREQ_GPS/(L2_FREQ_GPS - L1_FREQ_GPS) );
        double dg21( L1_FREQ_GPS/(L2_FREQ_GPS - L1_FREQ_GPS) );
        double eg21( L2_FREQ_GPS/(L2_FREQ_GPS + L1_FREQ_GPS) );
        double fg21( L1_FREQ_GPS/(L2_FREQ_GPS + L1_FREQ_GPS) );

        // Coefficients of GPS PW/WL/PN/LN combinations, L2 and L5
        double cg25( L2_FREQ_GPS/(L2_FREQ_GPS - L5_FREQ_GPS) );
        double dg25( L5_FREQ_GPS/(L2_FREQ_GPS - L5_FREQ_GPS) );
        double eg25( L2_FREQ_GPS/(L2_FREQ_GPS + L5_FREQ_GPS) );
        double fg25( L5_FREQ_GPS/(L2_FREQ_GPS + L5_FREQ_GPS) );

        //>>>>>>>>>>>>>>>>>>>>>>>>>>

        // Coefficients of Galileo MW combinations, L1 and L5
        double ce15( L1_FREQ_GAL/(L1_FREQ_GAL - L5_FREQ_GAL) );
        double de15( L5_FREQ_GAL/(L1_FREQ_GAL - L5_FREQ_GAL) );
        double ee15( L1_FREQ_GAL/(L1_FREQ_GAL + L5_FREQ_GAL) );
        double fe15( L5_FREQ_GAL/(L1_FREQ_GAL + L5_FREQ_GAL) );

        // Coefficients of Galileo PW/WL/PN/LN combinations, L5 and L7
        double ce57( L5_FREQ_GAL/(L5_FREQ_GAL - L7_FREQ_GAL) );
        double de57( L7_FREQ_GAL/(L5_FREQ_GAL - L7_FREQ_GAL) );
        double ee57( L5_FREQ_GAL/(L5_FREQ_GAL + L7_FREQ_GAL) );
        double fe57( L7_FREQ_GAL/(L5_FREQ_GAL + L7_FREQ_GAL) );

        // Coefficients of Galileo PW/WL/PN/LN combinations, L5 and L8
        double ce58( L5_FREQ_GAL/(L5_FREQ_GAL - L8_FREQ_GAL) );
        double de58( L8_FREQ_GAL/(L5_FREQ_GAL - L8_FREQ_GAL) );
        double ee58( L5_FREQ_GAL/(L5_FREQ_GAL + L8_FREQ_GAL) );
        double fe58( L8_FREQ_GAL/(L5_FREQ_GAL + L8_FREQ_GAL) );

         // Coefficients of Galileo PW/WL/PN/LN combinations, L5 and L6
        double ce56( L5_FREQ_GAL/(L5_FREQ_GAL - L6_FREQ_GAL) );
        double de56( L6_FREQ_GAL/(L5_FREQ_GAL - L6_FREQ_GAL) );
        double ee56( L5_FREQ_GAL/(L5_FREQ_GAL + L6_FREQ_GAL) );
        double fe56( L6_FREQ_GAL/(L5_FREQ_GAL + L6_FREQ_GAL) );

        double ce51( L5_FREQ_GAL/(L5_FREQ_GAL - L1_FREQ_GAL) );
        double de51( L1_FREQ_GAL/(L5_FREQ_GAL - L1_FREQ_GAL) );
        double ee51( L5_FREQ_GAL/(L5_FREQ_GAL + L1_FREQ_GAL) );
        double fe51( L1_FREQ_GAL/(L5_FREQ_GAL + L1_FREQ_GAL) );

        //***********

        // Coefficients of Galileo MW combinations, L1 and L5
        double ce61( L6_FREQ_GAL/(L6_FREQ_GAL - L1_FREQ_GAL) );
        double de61( L1_FREQ_GAL/(L6_FREQ_GAL - L1_FREQ_GAL) );
        double ee61( L6_FREQ_GAL/(L6_FREQ_GAL + L1_FREQ_GAL) );
        double fe61( L1_FREQ_GAL/(L6_FREQ_GAL + L1_FREQ_GAL) );

        // Coefficients of Galileo PW/WL/PN/LN combinations, L6 and L7
        double ce67( L6_FREQ_GAL/(L6_FREQ_GAL - L7_FREQ_GAL) );
        double de67( L7_FREQ_GAL/(L6_FREQ_GAL - L7_FREQ_GAL) );
        double ee67( L6_FREQ_GAL/(L6_FREQ_GAL + L7_FREQ_GAL) );
        double fe67( L7_FREQ_GAL/(L6_FREQ_GAL + L7_FREQ_GAL) );

        // Coefficients of Galileo PW/WL/PN/LN combinations, L6 and L8
        double ce68( L6_FREQ_GAL/(L6_FREQ_GAL - L8_FREQ_GAL) );
        double de68( L8_FREQ_GAL/(L6_FREQ_GAL - L8_FREQ_GAL) );
        double ee68( L6_FREQ_GAL/(L6_FREQ_GAL + L8_FREQ_GAL) );
        double fe68( L8_FREQ_GAL/(L6_FREQ_GAL + L8_FREQ_GAL) );

         // Coefficients of Galileo PW/WL/PN/LN combinations, L7 and L6
        double ce65( L6_FREQ_GAL/(L6_FREQ_GAL - L5_FREQ_GAL) );
        double de65( L5_FREQ_GAL/(L6_FREQ_GAL - L5_FREQ_GAL) );
        double ee65( L6_FREQ_GAL/(L6_FREQ_GAL + L5_FREQ_GAL) );
        double fe65( L5_FREQ_GAL/(L6_FREQ_GAL + L5_FREQ_GAL) );

        //>>>>>>>

        // Coefficients of Galileo MW combinations, L1 and L5
        double ce71( L7_FREQ_GAL/(L7_FREQ_GAL - L1_FREQ_GAL) );
        double de71( L1_FREQ_GAL/(L7_FREQ_GAL - L1_FREQ_GAL) );
        double ee71( L7_FREQ_GAL/(L7_FREQ_GAL + L1_FREQ_GAL) );
        double fe71( L1_FREQ_GAL/(L7_FREQ_GAL + L1_FREQ_GAL) );

        // Coefficients of Galileo PW/WL/PN/LN combinations, L7 and L8
        double ce78( L7_FREQ_GAL/(L7_FREQ_GAL - L8_FREQ_GAL) );
        double de78( L8_FREQ_GAL/(L7_FREQ_GAL - L8_FREQ_GAL) );
        double ee78( L7_FREQ_GAL/(L7_FREQ_GAL + L8_FREQ_GAL) );
        double fe78( L8_FREQ_GAL/(L7_FREQ_GAL + L8_FREQ_GAL) );

         // Coefficients of Galileo PW/WL/PN/LN combinations, L7 and L7
        double ce75( L7_FREQ_GAL/(L7_FREQ_GAL - L5_FREQ_GAL) );
        double de75( L5_FREQ_GAL/(L7_FREQ_GAL - L5_FREQ_GAL) );
        double ee75( L7_FREQ_GAL/(L7_FREQ_GAL + L5_FREQ_GAL) );
        double fe75( L5_FREQ_GAL/(L7_FREQ_GAL + L5_FREQ_GAL) );

        //>>>>>>>>>>>>>>>>>>>>>>>>>>

        // Coefficients of BDS PW/WL/PN/LN combinations, L2 and L7
        double cc27( L2_FREQ_BDS/(L2_FREQ_BDS - L7_FREQ_BDS) );
        double dc27( L7_FREQ_BDS/(L2_FREQ_BDS - L7_FREQ_BDS) );
        double ec27( L2_FREQ_BDS/(L2_FREQ_BDS + L7_FREQ_BDS) );
        double fc27( L7_FREQ_BDS/(L2_FREQ_BDS + L7_FREQ_BDS) );

        double cc26( L2_FREQ_BDS/(L2_FREQ_BDS - L6_FREQ_BDS) );
        double dc26( L6_FREQ_BDS/(L2_FREQ_BDS - L6_FREQ_BDS) );
        double ec26( L2_FREQ_BDS/(L2_FREQ_BDS + L6_FREQ_BDS) );
        double fc26( L6_FREQ_BDS/(L2_FREQ_BDS + L6_FREQ_BDS) );

        double cc28( L2_FREQ_BDS/(L2_FREQ_BDS - L8_FREQ_BDS) );
        double dc28( L8_FREQ_BDS/(L2_FREQ_BDS - L8_FREQ_BDS) );
        double ec28( L2_FREQ_BDS/(L2_FREQ_BDS + L8_FREQ_BDS) );
        double fc28( L8_FREQ_BDS/(L2_FREQ_BDS + L8_FREQ_BDS) );

        double cc25( L2_FREQ_BDS/(L2_FREQ_BDS - L5_FREQ_BDS) );
        double dc25( L5_FREQ_BDS/(L2_FREQ_BDS - L5_FREQ_BDS) );
        double ec25( L2_FREQ_BDS/(L2_FREQ_BDS + L5_FREQ_BDS) );
        double fc25( L5_FREQ_BDS/(L2_FREQ_BDS + L5_FREQ_BDS) );

        double cc21( L2_FREQ_BDS/(L2_FREQ_BDS - L1_FREQ_BDS) );
        double dc21( L1_FREQ_BDS/(L2_FREQ_BDS - L1_FREQ_BDS) );
        double ec21( L2_FREQ_BDS/(L2_FREQ_BDS + L1_FREQ_BDS) );
        double fc21( L1_FREQ_BDS/(L2_FREQ_BDS + L1_FREQ_BDS) );

        double cc12( L1_FREQ_BDS/(L1_FREQ_BDS - L2_FREQ_BDS) );
        double dc12( L2_FREQ_BDS/(L1_FREQ_BDS - L2_FREQ_BDS) );
        double ec12( L1_FREQ_BDS/(L1_FREQ_BDS + L2_FREQ_BDS) );
        double fc12( L2_FREQ_BDS/(L1_FREQ_BDS + L2_FREQ_BDS) );

        //$$$$

        // Coefficients of BDS PW/WL/PN/LN combinations, L2 and L7

        double cc65( L6_FREQ_BDS/(L6_FREQ_BDS - L5_FREQ_BDS) );
        double dc65( L5_FREQ_BDS/(L6_FREQ_BDS - L5_FREQ_BDS) );
        double ec65( L6_FREQ_BDS/(L6_FREQ_BDS + L5_FREQ_BDS) );
        double fc65( L5_FREQ_BDS/(L6_FREQ_BDS + L5_FREQ_BDS) );

        double cc68( L6_FREQ_BDS/(L6_FREQ_BDS - L8_FREQ_BDS) );
        double dc68( L8_FREQ_BDS/(L6_FREQ_BDS - L8_FREQ_BDS) );
        double ec68( L6_FREQ_BDS/(L6_FREQ_BDS + L8_FREQ_BDS) );
        double fc68( L8_FREQ_BDS/(L6_FREQ_BDS + L8_FREQ_BDS) );

        double cc67( L6_FREQ_BDS/(L6_FREQ_BDS - L7_FREQ_BDS) );
        double dc67( L7_FREQ_BDS/(L6_FREQ_BDS - L7_FREQ_BDS) );
        double ec67( L6_FREQ_BDS/(L6_FREQ_BDS + L7_FREQ_BDS) );
        double fc67( L7_FREQ_BDS/(L6_FREQ_BDS + L7_FREQ_BDS) );

        double cc62( L6_FREQ_BDS/(L6_FREQ_BDS - L2_FREQ_BDS) );
        double dc62( L2_FREQ_BDS/(L6_FREQ_BDS - L2_FREQ_BDS) );
        double ec62( L6_FREQ_BDS/(L6_FREQ_BDS + L2_FREQ_BDS) );
        double fc62( L2_FREQ_BDS/(L6_FREQ_BDS + L2_FREQ_BDS) );

        double cc61( L6_FREQ_BDS/(L6_FREQ_BDS - L1_FREQ_BDS) );
        double dc61( L1_FREQ_BDS/(L6_FREQ_BDS - L1_FREQ_BDS) );
        double ec61( L6_FREQ_BDS/(L6_FREQ_BDS + L1_FREQ_BDS) );
        double fc61( L1_FREQ_BDS/(L6_FREQ_BDS + L1_FREQ_BDS) );

        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // Coefficients of QZSS PW/WL/PN/LN combinations, L1 and L2
        double cj21( L2_FREQ_QZS/(L2_FREQ_QZS - L1_FREQ_QZS) );
        double dj21( L1_FREQ_QZS/(L2_FREQ_QZS - L1_FREQ_QZS) );
        double ej21( L2_FREQ_QZS/(L2_FREQ_QZS + L1_FREQ_QZS) );
        double fj21( L1_FREQ_QZS/(L2_FREQ_QZS + L1_FREQ_QZS) );

        double cj12( L1_FREQ_QZS/(L1_FREQ_QZS - L2_FREQ_QZS) );
        double dj12( L2_FREQ_QZS/(L1_FREQ_QZS - L2_FREQ_QZS) );
        double ej12( L1_FREQ_QZS/(L1_FREQ_QZS + L2_FREQ_QZS) );
        double fj12( L2_FREQ_QZS/(L1_FREQ_QZS + L2_FREQ_QZS) );

        // Coefficients of QZSS PW/WL/PN/LN combinations, L2 and L5
        double cj25( L2_FREQ_QZS/(L2_FREQ_QZS - L5_FREQ_QZS) );
        double dj25( L5_FREQ_QZS/(L2_FREQ_QZS - L5_FREQ_QZS) );
        double ej25( L2_FREQ_QZS/(L2_FREQ_QZS + L5_FREQ_QZS) );
        double fj25( L5_FREQ_QZS/(L2_FREQ_QZS + L5_FREQ_QZS) );

        // coefficients for BL1/BL2 combination using P1/P2/L1/L2
        // lambdaN1*N1 = L1 - [ (alfa+1)*P1 - 2*P2 ]/(alfa-1)
        // lambdaN2*N2 = L2 - [ (2*alfa)*P1 - (alfa+1)*P2]/(alfa-1)
        double agBL1 = 1.0;  
        double bgBL1 = (GAMMA_GPS_L1L2 + 1.0)/(GAMMA_GPS_L1L2 - 1.0);  
        double cgBL1 = -2.0/(GAMMA_GPS_L1L2 - 1.0);  

        double agBL2 = 1.0;  
        double bgBL2 = 2.0*GAMMA_GPS_L1L2/(GAMMA_GPS_L1L2 - 1.0);  
        double cgBL2 = -(GAMMA_GPS_L1L2 + 1.0)/(GAMMA_GPS_L1L2 - 1.0);  


        mw12CombOfGPS.header                =   TypeID::MW12G;
        mw12CombOfGPS.body[TypeID::L1G]      =   +cg12;
        mw12CombOfGPS.body[TypeID::L2G]      =   -dg12;
        mw12CombOfGPS.body[TypeID::C1G]      =   -eg12;
        mw12CombOfGPS.body[TypeID::C2G]      =   -fg12;

        mw15CombOfGPS.header                =   TypeID::MW15G;
        mw15CombOfGPS.body[TypeID::L1G]      =   +cg15;
        mw15CombOfGPS.body[TypeID::L5G]      =   -dg15;
        mw15CombOfGPS.body[TypeID::C1G]      =   -eg15;
        mw15CombOfGPS.body[TypeID::C5G]      =   -fg15;

        mw21CombOfGPS.header                =   TypeID::MW21G;
        mw21CombOfGPS.body[TypeID::L2G]      =   +cg21;
        mw21CombOfGPS.body[TypeID::L1G]      =   -dg21;
        mw21CombOfGPS.body[TypeID::C2G]      =   -eg21;
        mw21CombOfGPS.body[TypeID::C1G]      =   -fg21;

        // Definition to compute the MW combination of GPS, L2 and L5
        mw25CombOfGPS.header                =   TypeID::MW25G;
        mw25CombOfGPS.body[TypeID::L2G]      =   +cg25;
        mw25CombOfGPS.body[TypeID::L5G]      =   -dg25;
        mw25CombOfGPS.body[TypeID::C2G]      =   -eg25;
        mw25CombOfGPS.body[TypeID::C5G]      =   -fg25;

        // Definition to compute the MW combination of Galileo, L1 and L5
        mw58CombOfGAL.header                =   TypeID::MW58E;
        mw58CombOfGAL.body[TypeID::L5E]      =   +ce58;
        mw58CombOfGAL.body[TypeID::L8E]      =   -de58;
        mw58CombOfGAL.body[TypeID::C5E]      =   -ee58;
        mw58CombOfGAL.body[TypeID::C8E]      =   -fe58;

        mw57CombOfGAL.header                =   TypeID::MW57E;
        mw57CombOfGAL.body[TypeID::L5E]      =   +ce57;
        mw57CombOfGAL.body[TypeID::L7E]      =   -de57;
        mw57CombOfGAL.body[TypeID::C5E]      =   -ee57;
        mw57CombOfGAL.body[TypeID::C7E]      =   -fe57;

        mw56CombOfGAL.header                =   TypeID::MW56E;
        mw56CombOfGAL.body[TypeID::L5E]      =   +ce56;
        mw56CombOfGAL.body[TypeID::L6E]      =   -de56;
        mw56CombOfGAL.body[TypeID::C5E]      =   -ee56;
        mw56CombOfGAL.body[TypeID::C6E]      =   -fe56;

        mw51CombOfGAL.header                =   TypeID::MW51E;
        mw51CombOfGAL.body[TypeID::L5E]      =   +ce51;
        mw51CombOfGAL.body[TypeID::L1E]      =   -de51;
        mw51CombOfGAL.body[TypeID::C5E]      =   -ee51;
        mw51CombOfGAL.body[TypeID::C1E]      =   -fe51;

        mw15CombOfGAL.header                =   TypeID::MW15E;
        mw15CombOfGAL.body[TypeID::L1E]      =   +ce15;
        mw15CombOfGAL.body[TypeID::L5E]      =   -de15;
        mw15CombOfGAL.body[TypeID::C1E]      =   -ee15;
        mw15CombOfGAL.body[TypeID::C5E]      =   -fe15;

        // definition to compute the MW combination of galileo
        //===================================================
        mw65CombOfGAL.header                =   TypeID::MW65E;
        mw65CombOfGAL.body[TypeID::L6E]      =   +ce65;
        mw65CombOfGAL.body[TypeID::L5E]      =   -de65;
        mw65CombOfGAL.body[TypeID::C6E]      =   -ee65;
        mw65CombOfGAL.body[TypeID::C5E]      =   -fe65;

        mw68CombOfGAL.header                =   TypeID::MW68E;
        mw68CombOfGAL.body[TypeID::L6E]      =   +ce68;
        mw68CombOfGAL.body[TypeID::L8E]      =   -de68;
        mw68CombOfGAL.body[TypeID::C6E]      =   -ee68;
        mw68CombOfGAL.body[TypeID::C8E]      =   -fe68;

        mw67CombOfGAL.header                =   TypeID::MW67E;
        mw67CombOfGAL.body[TypeID::L6E]      =   +ce67;
        mw67CombOfGAL.body[TypeID::L7E]      =   -de67;
        mw67CombOfGAL.body[TypeID::C6E]      =   -ee67;
        mw67CombOfGAL.body[TypeID::C7E]      =   -fe67;

        mw61CombOfGAL.header                =   TypeID::MW61E;
        mw61CombOfGAL.body[TypeID::L6E]      =   +ce61;
        mw61CombOfGAL.body[TypeID::L1E]      =   -de61;
        mw61CombOfGAL.body[TypeID::C6E]      =   -ee61;
        mw61CombOfGAL.body[TypeID::C1E]      =   -fe61;

        // definition to compute the MW combination of galileo
        //===================================================
        mw75CombOfGAL.header                =   TypeID::MW75E;
        mw75CombOfGAL.body[TypeID::L7E]      =   +ce75;
        mw75CombOfGAL.body[TypeID::L5E]      =   -de75;
        mw75CombOfGAL.body[TypeID::C7E]      =   -ee75;
        mw75CombOfGAL.body[TypeID::C5E]      =   -fe75;

        mw78CombOfGAL.header                =   TypeID::MW78E;
        mw78CombOfGAL.body[TypeID::L7E]      =   +ce78;
        mw78CombOfGAL.body[TypeID::L8E]      =   -de78;
        mw78CombOfGAL.body[TypeID::C7E]      =   -ee78;
        mw78CombOfGAL.body[TypeID::C8E]      =   -fe78;

        mw71CombOfGAL.header                =   TypeID::MW71E;
        mw71CombOfGAL.body[TypeID::L7E]      =   +ce71;
        mw71CombOfGAL.body[TypeID::L1E]      =   -de71;
        mw71CombOfGAL.body[TypeID::C7E]      =   -ee71;
        mw71CombOfGAL.body[TypeID::C1E]      =   -fe71;
        //===================================================

        //===================================================

        // Definition to compute the MW combination of BDS, L2 and L7
        mw28CombOfBDS.header                =   TypeID::MW28C;
        mw28CombOfBDS.body[TypeID::L2C]      =   +cc28;
        mw28CombOfBDS.body[TypeID::L8C]      =   -dc28;
        mw28CombOfBDS.body[TypeID::C2C]      =   -ec28;
        mw28CombOfBDS.body[TypeID::C8C]      =   -fc28;

        mw27CombOfBDS.header                =   TypeID::MW27C;
        mw27CombOfBDS.body[TypeID::L2C]      =   +cc27;
        mw27CombOfBDS.body[TypeID::L7C]      =   -dc27;
        mw27CombOfBDS.body[TypeID::C2C]      =   -ec27;
        mw27CombOfBDS.body[TypeID::C7C]      =   -fc27;

        // Definition to compute the MW combination of BDS, L2 and L6
        mw26CombOfBDS.header                =   TypeID::MW26C;
        mw26CombOfBDS.body[TypeID::L2C]      =   +cc26;
        mw26CombOfBDS.body[TypeID::L6C]      =   -dc26;
        mw26CombOfBDS.body[TypeID::C2C]      =   -ec26;
        mw26CombOfBDS.body[TypeID::C6C]      =   -fc26;

        // Definition to compute the MW combination of BDS, L2 and L5
        mw25CombOfBDS.header                =   TypeID::MW25C;
        mw25CombOfBDS.body[TypeID::L2C]      =   +cc25;
        mw25CombOfBDS.body[TypeID::L5C]      =   -dc25;
        mw25CombOfBDS.body[TypeID::C2C]      =   -ec25;
        mw25CombOfBDS.body[TypeID::C5C]      =   -fc25;

        // Definition to compute the MW combination of BDS, L2 and L1
        mw21CombOfBDS.header               =   TypeID::MW21C;
        mw21CombOfBDS.body[TypeID::L2C]    =   +cc21;
        mw21CombOfBDS.body[TypeID::L1C]    =   -dc21;
        mw21CombOfBDS.body[TypeID::C2C]    =   -ec21;
        mw21CombOfBDS.body[TypeID::C1C]    =   -fc21;

        //$$$$$
        // Definition to compute the MW combination of BDS, L2 and L5

        mw65CombOfBDS.header                =   TypeID::MW65C;
        mw65CombOfBDS.body[TypeID::L6C]      =   +cc65;
        mw65CombOfBDS.body[TypeID::L5C]      =   -dc65;
        mw65CombOfBDS.body[TypeID::C6C]      =   -ec65;
        mw65CombOfBDS.body[TypeID::C5C]      =   -fc65;

        // Definition to compute the MW combination of BDS, L6 and L7
        mw68CombOfBDS.header                =   TypeID::MW68C;
        mw68CombOfBDS.body[TypeID::L6C]      =   +cc68;
        mw68CombOfBDS.body[TypeID::L8C]      =   -dc68;
        mw68CombOfBDS.body[TypeID::C6C]      =   -ec68;
        mw68CombOfBDS.body[TypeID::C8C]      =   -fc68;

        mw67CombOfBDS.header                =   TypeID::MW67C;
        mw67CombOfBDS.body[TypeID::L6C]      =   +cc67;
        mw67CombOfBDS.body[TypeID::L7C]      =   -dc67;
        mw67CombOfBDS.body[TypeID::C6C]      =   -ec67;
        mw67CombOfBDS.body[TypeID::C7C]      =   -fc67;

        // Definition to compute the MW combination of BDS, L3 and L6
        mw62CombOfBDS.header                =   TypeID::MW62C;
        mw62CombOfBDS.body[TypeID::L6C]      =   +cc62;
        mw62CombOfBDS.body[TypeID::L2C]      =   -dc62;
        mw62CombOfBDS.body[TypeID::C6C]      =   -ec62;
        mw62CombOfBDS.body[TypeID::C2C]      =   -fc62;

        // Definition to compute the MW combination of BDS, L6 and L1
        mw61CombOfBDS.header               =   TypeID::MW61C;
        mw61CombOfBDS.body[TypeID::L6C]    =   +cc61;
        mw61CombOfBDS.body[TypeID::L1C]    =   -dc61;
        mw61CombOfBDS.body[TypeID::C6C]    =   -ec61;
        mw61CombOfBDS.body[TypeID::C1C]    =   -fc61;

        //=================================================

        mw21CombOfQZS.header               =   TypeID::MW21J;
        mw21CombOfQZS.body[TypeID::L2J]    =   +cj21;
        mw21CombOfQZS.body[TypeID::L1J]    =   -dj21;
        mw21CombOfQZS.body[TypeID::C2J]    =   -ej21;
        mw21CombOfQZS.body[TypeID::C1J]    =   -fj21;

        // Definition to compute the MW combination of QZS, L2 and L5
        mw25CombOfQZS.header               =   TypeID::MW25J;
        mw25CombOfQZS.body[TypeID::L2J]    =   +cj25;
        mw25CombOfQZS.body[TypeID::L5J]    =   -dj25;
        mw25CombOfQZS.body[TypeID::C2J]    =   -ej25;
        mw25CombOfQZS.body[TypeID::C5J]    =   -fj25;

        li12CombOfGPS.header               =   TypeID::LI12G;
        li12CombOfGPS.body[TypeID::L1G]    =   1.0;
        li12CombOfGPS.body[TypeID::L2G]    =  -1.0;

        li15CombOfGAL.header               =   TypeID::LI15E;
        li15CombOfGAL.body[TypeID::L1E]    =   1.0;
        li15CombOfGAL.body[TypeID::L5E]    =  -1.0;

        li27CombOfBDS.header               =   TypeID::LI27C;
        li27CombOfBDS.body[TypeID::L2C]    =   1.0;
        li27CombOfBDS.body[TypeID::L7C]    =  -1.0;

        BL1CombOfGPS.header                = TypeID::BL1G;
        BL1CombOfGPS.body[TypeID::L1G]     =   agBL1;
        BL1CombOfGPS.body[TypeID::C1G]     =  -bgBL1;
        BL1CombOfGPS.body[TypeID::C2G]     =  -cgBL1;

        BL2CombOfGPS.header                =   TypeID::BL2G;
        BL2CombOfGPS.body[TypeID::L2G]     =   agBL2;
        BL2CombOfGPS.body[TypeID::C1G]     =  -bgBL2;
        BL2CombOfGPS.body[TypeID::C2G]     =  -cgBL2;

        //////////////////////////////////////
        // coefficient of ionoOfPC12GPS 
        // C1 = rou + IonoC1;
        // C2 = rou + f1^2/f2^2*IonoC1;
        // C2 - C1 = (f1^2/f2^2 - 1) * IonoC1;
        // C2 - C1 = (f1^2-f2^2)/f2^2 * IonoC1;
        // IonoC1 = f2^2/(f1^2-f2^2)*(C2-C1)
        //        = 1.0/(GAMMA_GPS_C1L2 -1) (C2 - C1);
        //////////////////////////////////////
        double c1IonoC1G = -1.0/(GAMMA_GPS_L1L2 - 1.0);
        double c2IonoC1G =  1.0/(GAMMA_GPS_L1L2 - 1.0);

        ionoC1G.header = TypeID::ionoC1G;
        ionoC1G.body[TypeID::C1G] = c1IonoC1G; 
        ionoC1G.body[TypeID::C2G] = c2IonoC1G; 

        //////////////////////////////////////
        // coefficient of ionoOfPC12GPS 
        // C1 = rou + IonoC1;
        // C5 = rou + f1^2/f5^2*IonoC1;
        // C5 - C1 = (f1^2/f5^2 - 1) * IonoC1;
        // IonoC1 = 1.0/(GAMMA_GAL_L1L5 - 1.0) (C5 - C1);
        //////////////////////////////////////
        double c1IonoC1E = -1.0/(GAMMA_GAL_L1L5 - 1.0);
        double c5IonoC1E =  1.0/(GAMMA_GAL_L1L5 - 1.0);

        ionoC1E.header = TypeID::ionoC1E;
        ionoC1E.body[TypeID::C1E] = c1IonoC1E;
        ionoC1E.body[TypeID::C5E] = c5IonoC1E;

        //////////////////////////////////////
        // coefficient of ionoOfPC12GPS 
        // P2 = rou + IonoL2;
        // P6 = rou + f2^2/f6^2*IonoL2;
        // P6 - P2 = (f2^2/f6^2 - 1) * IonoL2;
        // IonoL2 = 1.0/(GAMMA_BDS_L2L6 - 1.0) (P6 - P2);
        //////////////////////////////////////
        double c2IonoC2C = -1.0/(GAMMA_BDS_L2L6 - 1.0);
        double c6IonoC2C =  1.0/(GAMMA_BDS_L2L6 - 1.0);

        ionoC2C.header = TypeID::ionoC2C;
        ionoC2C.body[TypeID::C2C] = c2IonoC2C;
        ionoC2C.body[TypeID::C6C] = c6IonoC2C;

        //////////////////////////////////////
        // coefficient of ionoOfPC12GPS 
        // P1 = rou + IonoP1;
        // P5 = rou + f1^2/f5^2*IonoP1;
        // P5 - P1 = (f1^2/f5^2 - 1) * IonoP1;
        // IonoP1 = 1.0/(GAMMA_GAL_L1L5 - 1.0) (P5 - P1);
        //////////////////////////////////////

        // Definition to compute prefit residual of GPS C1
        c1PrefitOfGPS.header                     =   TypeID::prefitC1G;
        c1PrefitOfGPS.body[TypeID::C1G]          =   +1.0;
        c1PrefitOfGPS.body[TypeID::rho]          =   -1.0;
        c1PrefitOfGPS.body[TypeID::cdtSat]       =   +1.0;
        c1PrefitOfGPS.body[TypeID::relativity]   =   -1.0;
        c1PrefitOfGPS.body[TypeID::gravDelay]    =   -1.0;
        c1PrefitOfGPS.body[TypeID::tropoSlant]   =   -1.0;

         // Definition to compute prefit residual of GPS C2
        c2PrefitOfGPS.header                     =   TypeID::prefitC2G;
        c2PrefitOfGPS.body[TypeID::C2G]          =   +1.0;
        c2PrefitOfGPS.body[TypeID::rho]          =   -1.0;
        c2PrefitOfGPS.body[TypeID::cdtSat]       =   +1.0;
        c2PrefitOfGPS.body[TypeID::relativity]   =   -1.0;
        c2PrefitOfGPS.body[TypeID::gravDelay]    =   -1.0;
        c2PrefitOfGPS.body[TypeID::tropoSlant]   =   -1.0;


        // Definition to compute prefit residual of GPS C5
        c5PrefitOfGPS.header                     =   TypeID::prefitC5G;
        c5PrefitOfGPS.body[TypeID::C5G]          =   +1.0;
        c5PrefitOfGPS.body[TypeID::rho]          =   -1.0;
        c5PrefitOfGPS.body[TypeID::cdtSat]       =   +1.0;
        c5PrefitOfGPS.body[TypeID::relativity]   =   -1.0;
        c5PrefitOfGPS.body[TypeID::gravDelay]    =   -1.0;
        c5PrefitOfGPS.body[TypeID::tropoSlant]   =   -1.0;

        // GAL

        // Definition to compute prefit residual of Galileo C1E
        c1PrefitOfGAL.header                     =   TypeID::prefitC1E;
        c1PrefitOfGAL.body[TypeID::C1E]          =   +1.0;
        c1PrefitOfGAL.body[TypeID::rho]          =   -1.0;
        c1PrefitOfGAL.body[TypeID::cdtSat]       =   +1.0;
        c1PrefitOfGAL.body[TypeID::relativity]   =   -1.0;
        c1PrefitOfGAL.body[TypeID::gravDelay]    =   -1.0;
        c1PrefitOfGAL.body[TypeID::tropoSlant]   =   -1.0;

        // Definition to compute prefit residual of Galileo C5E
        c5PrefitOfGAL.header                     =   TypeID::prefitC5E;
        c5PrefitOfGAL.body[TypeID::C5E]          =   +1.0;
        c5PrefitOfGAL.body[TypeID::rho]          =   -1.0;
        c5PrefitOfGAL.body[TypeID::cdtSat]       =   +1.0;
        c5PrefitOfGAL.body[TypeID::relativity]   =   -1.0;
        c5PrefitOfGAL.body[TypeID::gravDelay]    =   -1.0;
        c5PrefitOfGAL.body[TypeID::tropoSlant]   =   -1.0;

        // Definition to compute prefit residual of Galileo C6E
        c6PrefitOfGAL.header                     =   TypeID::prefitC6E;
        c6PrefitOfGAL.body[TypeID::C6E]          =   +1.0;
        c6PrefitOfGAL.body[TypeID::rho]          =   -1.0;
        c6PrefitOfGAL.body[TypeID::cdtSat]       =   +1.0;
        c6PrefitOfGAL.body[TypeID::relativity]   =   -1.0;
        c6PrefitOfGAL.body[TypeID::gravDelay]    =   -1.0;
        c6PrefitOfGAL.body[TypeID::tropoSlant]   =   -1.0;

        // Definition to compute prefit residual of Galileo C7E
        c7PrefitOfGAL.header                     =   TypeID::prefitC7E;
        c7PrefitOfGAL.body[TypeID::C7E]          =   +1.0;
        c7PrefitOfGAL.body[TypeID::rho]          =   -1.0;
        c7PrefitOfGAL.body[TypeID::cdtSat]       =   +1.0;
        c7PrefitOfGAL.body[TypeID::relativity]   =   -1.0;
        c7PrefitOfGAL.body[TypeID::gravDelay]    =   -1.0;
        c7PrefitOfGAL.body[TypeID::tropoSlant]   =   -1.0;

        // Definition to compute prefit residual of Galileo C6
        c8PrefitOfGAL.header                      =   TypeID::prefitC8E;
        c8PrefitOfGAL.body[TypeID::C8E]           =   +1.0;
        c8PrefitOfGAL.body[TypeID::rho]           =   -1.0;
        c8PrefitOfGAL.body[TypeID::cdtSat]        =   +1.0;
        c8PrefitOfGAL.body[TypeID::relativity]    =   -1.0;
        c8PrefitOfGAL.body[TypeID::gravDelay]     =   -1.0;
        c8PrefitOfGAL.body[TypeID::tropoSlant]    =   -1.0;

        // BDS

        // Definition to compute prefit residual of BDS C1C
        c1PrefitOfBDS.header                    =   TypeID::prefitC1C;
        c1PrefitOfBDS.body[TypeID::C1C]         =   +1.0;
        c1PrefitOfBDS.body[TypeID::rho]         =   -1.0;
        c1PrefitOfBDS.body[TypeID::cdtSat]      =   +1.0;
        c1PrefitOfBDS.body[TypeID::relativity]  =   -1.0;
        c1PrefitOfBDS.body[TypeID::gravDelay]   =   -1.0;
        c1PrefitOfBDS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of BDS C2C
        c2PrefitOfBDS.header                    =   TypeID::prefitC2C;
        c2PrefitOfBDS.body[TypeID::C2C]         =   +1.0;
        c2PrefitOfBDS.body[TypeID::rho]         =   -1.0;
        c2PrefitOfBDS.body[TypeID::cdtSat]      =   +1.0;
        c2PrefitOfBDS.body[TypeID::relativity]  =   -1.0;
        c2PrefitOfBDS.body[TypeID::gravDelay]   =   -1.0;
        c2PrefitOfBDS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of BDS C5C
        c5PrefitOfBDS.header                    =   TypeID::prefitC5C;
        c5PrefitOfBDS.body[TypeID::C5C]         =   +1.0;
        c5PrefitOfBDS.body[TypeID::rho]         =   -1.0;
        c5PrefitOfBDS.body[TypeID::cdtSat]      =   +1.0;
        c5PrefitOfBDS.body[TypeID::relativity]  =   -1.0;
        c5PrefitOfBDS.body[TypeID::gravDelay]   =   -1.0;
        c5PrefitOfBDS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of BDS C7C
        c7PrefitOfBDS.header                    =   TypeID::prefitC7C;
        c7PrefitOfBDS.body[TypeID::C7C]          =   +1.0;
        c7PrefitOfBDS.body[TypeID::rho]         =   -1.0;
        c7PrefitOfBDS.body[TypeID::cdtSat]      =   +1.0;
        c7PrefitOfBDS.body[TypeID::relativity]  =   -1.0;
        c7PrefitOfBDS.body[TypeID::gravDelay]   =   -1.0;
        c7PrefitOfBDS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of BDS C8C
        c8PrefitOfBDS.header                    =   TypeID::prefitC8C;
        c8PrefitOfBDS.body[TypeID::C8C]         =   +1.0;
        c8PrefitOfBDS.body[TypeID::rho]         =   -1.0;
        c8PrefitOfBDS.body[TypeID::cdtSat]      =   +1.0;
        c8PrefitOfBDS.body[TypeID::relativity]  =   -1.0;
        c8PrefitOfBDS.body[TypeID::gravDelay]   =   -1.0;
        c8PrefitOfBDS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of BDS C6C
        c6PrefitOfBDS.header                    =   TypeID::prefitC6C;
        c6PrefitOfBDS.body[TypeID::C6C]         =   +1.0;
        c6PrefitOfBDS.body[TypeID::rho]         =   -1.0;
        c6PrefitOfBDS.body[TypeID::cdtSat]      =   +1.0;
        c6PrefitOfBDS.body[TypeID::relativity]  =   -1.0;
        c6PrefitOfBDS.body[TypeID::gravDelay]   =   -1.0;
        c6PrefitOfBDS.body[TypeID::tropoSlant]  =   -1.0;

        // GLO-code

        // Definition to compute prefit residual of GLO C1R
        c1PrefitOfGLO.header                    =   TypeID::prefitC1R;
        c1PrefitOfGLO.body[TypeID::C1R]         =   +1.0;
        c1PrefitOfGLO.body[TypeID::rho]         =   -1.0;
        c1PrefitOfGLO.body[TypeID::cdtSat]      =   +1.0;
        c1PrefitOfGLO.body[TypeID::relativity]  =   -1.0;
        c1PrefitOfGLO.body[TypeID::gravDelay]   =   -1.0;
        c1PrefitOfGLO.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of GLO C2R
        c2PrefitOfGLO.header                    =   TypeID::prefitC2R;
        c2PrefitOfGLO.body[TypeID::C2R]         =   +1.0;
        c2PrefitOfGLO.body[TypeID::rho]         =   -1.0;
        c2PrefitOfGLO.body[TypeID::cdtSat]      =   +1.0;
        c2PrefitOfGLO.body[TypeID::relativity]  =   -1.0;
        c2PrefitOfGLO.body[TypeID::gravDelay]   =   -1.0;
        c2PrefitOfGLO.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of GLO C3
        c3PrefitOfGLO.header                    =   TypeID::prefitC3R;
        c3PrefitOfGLO.body[TypeID::C3R]         =   +1.0;
        c3PrefitOfGLO.body[TypeID::rho]         =   -1.0;
        c3PrefitOfGLO.body[TypeID::cdtSat]      =   +1.0;
        c3PrefitOfGLO.body[TypeID::relativity]  =   -1.0;
        c3PrefitOfGLO.body[TypeID::gravDelay]   =   -1.0;
        c3PrefitOfGLO.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of GLO C4R
        c4PrefitOfGLO.header                    =   TypeID::prefitC4R;
        c4PrefitOfGLO.body[TypeID::C4R]         =   +1.0;
        c4PrefitOfGLO.body[TypeID::rho]         =   -1.0;
        c4PrefitOfGLO.body[TypeID::cdtSat]      =   +1.0;
        c4PrefitOfGLO.body[TypeID::relativity]  =   -1.0;
        c4PrefitOfGLO.body[TypeID::gravDelay]   =   -1.0;
        c4PrefitOfGLO.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of GLO C6R
        c6PrefitOfGLO.header                    =   TypeID::prefitC6R;
        c6PrefitOfGLO.body[TypeID::C6R]         =   +1.0;
        c6PrefitOfGLO.body[TypeID::rho]         =   -1.0;
        c6PrefitOfGLO.body[TypeID::cdtSat]      =   +1.0;
        c6PrefitOfGLO.body[TypeID::relativity]  =   -1.0;
        c6PrefitOfGLO.body[TypeID::gravDelay]   =   -1.0;
        c6PrefitOfGLO.body[TypeID::tropoSlant]  =   -1.0;

        // QZS-code

        // Definition to compute prefit residual of QZS C1J
        c1PrefitOfQZS.header                    =   TypeID::prefitC1J;
        c1PrefitOfQZS.body[TypeID::C1J]          =   +1.0;
        c1PrefitOfQZS.body[TypeID::rho]         =   -1.0;
        c1PrefitOfQZS.body[TypeID::cdtSat]      =   +1.0;
        c1PrefitOfQZS.body[TypeID::relativity]  =   -1.0;
        c1PrefitOfQZS.body[TypeID::gravDelay]   =   -1.0;
        c1PrefitOfQZS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of QZS C2J
        c2PrefitOfQZS.header                    =   TypeID::prefitC2J;
        c2PrefitOfQZS.body[TypeID::C2J]         =   +1.0;
        c2PrefitOfQZS.body[TypeID::rho]         =   -1.0;
        c2PrefitOfQZS.body[TypeID::cdtSat]      =   +1.0;
        c2PrefitOfQZS.body[TypeID::relativity]  =   -1.0;
        c2PrefitOfQZS.body[TypeID::gravDelay]   =   -1.0;
        c2PrefitOfQZS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of QZS C5J
        c5PrefitOfQZS.header                    =   TypeID::prefitC5J;
        c5PrefitOfQZS.body[TypeID::C5J]         =   +1.0;
        c5PrefitOfQZS.body[TypeID::rho]         =   -1.0;
        c5PrefitOfQZS.body[TypeID::cdtSat]      =   +1.0;
        c5PrefitOfQZS.body[TypeID::relativity]  =   -1.0;
        c5PrefitOfQZS.body[TypeID::gravDelay]   =   -1.0;
        c5PrefitOfQZS.body[TypeID::tropoSlant]  =   -1.0;

        // Definition to compute prefit residual of QZS C6J
        c6PrefitOfQZS.header                    =   TypeID::prefitC6J;
        c6PrefitOfQZS.body[TypeID::C6J]         =   +1.0;
        c6PrefitOfQZS.body[TypeID::rho]         =   -1.0;
        c6PrefitOfQZS.body[TypeID::cdtSat]      =   +1.0;
        c6PrefitOfQZS.body[TypeID::relativity]  =   -1.0;
        c6PrefitOfQZS.body[TypeID::gravDelay]   =   -1.0;
        c6PrefitOfQZS.body[TypeID::tropoSlant]  =   -1.0;

        // GPS-phase

        // Definition to compute prefit residual of GPS L1G
        l1PrefitOfGPS.header                    =   TypeID::prefitL1G;
        l1PrefitOfGPS.body[TypeID::L1G]         =   +1.0;
        l1PrefitOfGPS.body[TypeID::rho]         =   -1.0;
        l1PrefitOfGPS.body[TypeID::cdtSat]      =   +1.0;
        l1PrefitOfGPS.body[TypeID::relativity]  =   -1.0;
        l1PrefitOfGPS.body[TypeID::gravDelay]   =   -1.0;
        l1PrefitOfGPS.body[TypeID::tropoSlant]  =   -1.0;
        l1PrefitOfGPS.body[TypeID::windUpL1G]   =   -1.0;

        // Definition to compute prefit residual of GPS L2G
        l2PrefitOfGPS.header                    =   TypeID::prefitL2G;
        l2PrefitOfGPS.body[TypeID::L2G]         =   +1.0;
        l2PrefitOfGPS.body[TypeID::rho]         =   -1.0;
        l2PrefitOfGPS.body[TypeID::cdtSat]      =   +1.0;
        l2PrefitOfGPS.body[TypeID::relativity]  =   -1.0;
        l2PrefitOfGPS.body[TypeID::gravDelay]   =   -1.0;
        l2PrefitOfGPS.body[TypeID::tropoSlant]  =   -1.0;
        l2PrefitOfGPS.body[TypeID::windUpL2G]   =   -1.0;

        // Definition to compute prefit residual of GPS L5G
        l5PrefitOfGPS.header                    =   TypeID::prefitL5G;
        l5PrefitOfGPS.body[TypeID::L5G]         =   +1.0;
        l5PrefitOfGPS.body[TypeID::rho]         =   -1.0;
        l5PrefitOfGPS.body[TypeID::cdtSat]      =   +1.0;
        l5PrefitOfGPS.body[TypeID::relativity]  =   -1.0;
        l5PrefitOfGPS.body[TypeID::gravDelay]   =   -1.0;
        l5PrefitOfGPS.body[TypeID::tropoSlant]  =   -1.0;
        l5PrefitOfGPS.body[TypeID::windUpL5G]   =   -1.0;

        // GAL

        // Definition to compute prefit residual of Galileo L1E
        l1PrefitOfGAL.header                       =   TypeID::prefitL1E;
        l1PrefitOfGAL.body[TypeID::L1E]            =   +1.0;
        l1PrefitOfGAL.body[TypeID::rho]            =   -1.0;
        l1PrefitOfGAL.body[TypeID::cdtSat]         =   +1.0;
        l1PrefitOfGAL.body[TypeID::relativity]     =   -1.0;
        l1PrefitOfGAL.body[TypeID::gravDelay]      =   -1.0;
        l1PrefitOfGAL.body[TypeID::tropoSlant]     =   -1.0;
        l1PrefitOfGAL.body[TypeID::windUpL1E]      =   -1.0;

        // Definition to compute prefit residual of Galileo L5E
        l5PrefitOfGAL.header                       =   TypeID::prefitL5E;
        l5PrefitOfGAL.body[TypeID::L5E]            =   +1.0;
        l5PrefitOfGAL.body[TypeID::rho]            =   -1.0;
        l5PrefitOfGAL.body[TypeID::cdtSat]         =   +1.0;
        l5PrefitOfGAL.body[TypeID::relativity]     =   -1.0;
        l5PrefitOfGAL.body[TypeID::gravDelay]      =   -1.0;
        l5PrefitOfGAL.body[TypeID::tropoSlant]     =   -1.0;
        l5PrefitOfGAL.body[TypeID::windUpL5E]      =   -1.0;

        // Definition to compute prefit residual of Galileo L6
        l6PrefitOfGAL.header                       =   TypeID::prefitL6E;
        l6PrefitOfGAL.body[TypeID::L6E]            =   +1.0;
        l6PrefitOfGAL.body[TypeID::rho]            =   -1.0;
        l6PrefitOfGAL.body[TypeID::cdtSat]         =   +1.0;
        l6PrefitOfGAL.body[TypeID::relativity]     =   -1.0;
        l6PrefitOfGAL.body[TypeID::gravDelay]      =   -1.0;
        l6PrefitOfGAL.body[TypeID::tropoSlant]     =   -1.0;
        l6PrefitOfGAL.body[TypeID::windUpL6E]      =   -1.0;

        // Definition to compute prefit residual of Galileo L7
        l7PrefitOfGAL.header                       =   TypeID::prefitL7E;
        l7PrefitOfGAL.body[TypeID::L7E]            =   +1.0;
        l7PrefitOfGAL.body[TypeID::rho]            =   -1.0;
        l7PrefitOfGAL.body[TypeID::cdtSat]         =   +1.0;
        l7PrefitOfGAL.body[TypeID::relativity]     =   -1.0;
        l7PrefitOfGAL.body[TypeID::gravDelay]      =   -1.0;
        l7PrefitOfGAL.body[TypeID::tropoSlant]     =   -1.0;
        l7PrefitOfGAL.body[TypeID::windUpL7E]      =   -1.0;

        // Definition to compute prefit residual of Galileo L8
        l8PrefitOfGAL.header                      =   TypeID::prefitL8E;
        l8PrefitOfGAL.body[TypeID::L8E]           =   +1.0;
        l8PrefitOfGAL.body[TypeID::rho]           =   -1.0;
        l8PrefitOfGAL.body[TypeID::cdtSat]        =   +1.0;
        l8PrefitOfGAL.body[TypeID::relativity]    =   -1.0;
        l8PrefitOfGAL.body[TypeID::gravDelay]     =   -1.0;
        l8PrefitOfGAL.body[TypeID::tropoSlant]    =   -1.0;
        l8PrefitOfGAL.body[TypeID::windUpL8E]     =   -1.0;

        // BDS

        // Definition to compute prefit residual of BDS L1
        l1PrefitOfBDS.header                      =   TypeID::prefitL1C;
        l1PrefitOfBDS.body[TypeID::L1C]            =   +1.0;
        l1PrefitOfBDS.body[TypeID::rho]           =   -1.0;
        l1PrefitOfBDS.body[TypeID::cdtSat]        =   +1.0;
        l1PrefitOfBDS.body[TypeID::relativity]    =   -1.0;
        l1PrefitOfBDS.body[TypeID::gravDelay]     =   -1.0;
        l1PrefitOfBDS.body[TypeID::tropoSlant]    =   -1.0;
        l1PrefitOfBDS.body[TypeID::windUpL1C]      =   -1.0;

        // Definition to compute prefit residual of BDS L2
        l2PrefitOfBDS.header                      =   TypeID::prefitL2C;
        l2PrefitOfBDS.body[TypeID::L2C]           =   +1.0;
        l2PrefitOfBDS.body[TypeID::rho]           =   -1.0;
        l2PrefitOfBDS.body[TypeID::cdtSat]        =   +1.0;
        l2PrefitOfBDS.body[TypeID::relativity]    =   -1.0;
        l2PrefitOfBDS.body[TypeID::gravDelay]     =   -1.0;
        l2PrefitOfBDS.body[TypeID::tropoSlant]    =   -1.0;
        l2PrefitOfBDS.body[TypeID::windUpL2C]     =   -1.0;

        // Definition to compute prefit residual of BDS L5
        l5PrefitOfBDS.header                      =   TypeID::prefitL5C;
        l5PrefitOfBDS.body[TypeID::L5C]           =   +1.0;
        l5PrefitOfBDS.body[TypeID::rho]           =   -1.0;
        l5PrefitOfBDS.body[TypeID::cdtSat]        =   +1.0;
        l5PrefitOfBDS.body[TypeID::relativity]    =   -1.0;
        l5PrefitOfBDS.body[TypeID::gravDelay]     =   -1.0;
        l5PrefitOfBDS.body[TypeID::tropoSlant]    =   -1.0;
        l5PrefitOfBDS.body[TypeID::windUpL5C]     =   -1.0;

        // Definition to compute prefit residual of BDS L7
        l7PrefitOfBDS.header                      =   TypeID::prefitL7C;
        l7PrefitOfBDS.body[TypeID::L7C]           =   +1.0;
        l7PrefitOfBDS.body[TypeID::rho]           =   -1.0;
        l7PrefitOfBDS.body[TypeID::cdtSat]        =   +1.0;
        l7PrefitOfBDS.body[TypeID::relativity]    =   -1.0;
        l7PrefitOfBDS.body[TypeID::gravDelay]     =   -1.0;
        l7PrefitOfBDS.body[TypeID::tropoSlant]    =   -1.0;
        l7PrefitOfBDS.body[TypeID::windUpL7C]     =   -1.0;

        // Definition to compute prefit residual of BDS L8
        l8PrefitOfBDS.header                      =   TypeID::prefitL8C;
        l8PrefitOfBDS.body[TypeID::L8C]           =   +1.0;
        l8PrefitOfBDS.body[TypeID::rho]           =   -1.0;
        l8PrefitOfBDS.body[TypeID::cdtSat]        =   +1.0;
        l8PrefitOfBDS.body[TypeID::relativity]    =   -1.0;
        l8PrefitOfBDS.body[TypeID::gravDelay]     =   -1.0;
        l8PrefitOfBDS.body[TypeID::tropoSlant]    =   -1.0;
        l8PrefitOfBDS.body[TypeID::windUpL8C]     =   -1.0;

        // Definition to compute prefit residual of BDS L6
        l6PrefitOfBDS.header                      =   TypeID::prefitL6C;
        l6PrefitOfBDS.body[TypeID::L6C]           =   +1.0;
        l6PrefitOfBDS.body[TypeID::rho]           =   -1.0;
        l6PrefitOfBDS.body[TypeID::cdtSat]        =   +1.0;
        l6PrefitOfBDS.body[TypeID::relativity]    =   -1.0;
        l6PrefitOfBDS.body[TypeID::gravDelay]     =   -1.0;
        l6PrefitOfBDS.body[TypeID::tropoSlant]    =   -1.0;
        l6PrefitOfBDS.body[TypeID::windUpL6C]     =   -1.0;

        // GLO-phase
        // To be done: wavelength for windup should be modified. 

        // Definition to compute prefit residual of BDS L1
        l1PrefitOfGLO.header                      =   TypeID::prefitL1R;
        l1PrefitOfGLO.body[TypeID::L1R]            =   +1.0;
        l1PrefitOfGLO.body[TypeID::rho]           =   -1.0;
        l1PrefitOfGLO.body[TypeID::cdtSat]        =   +1.0;
        l1PrefitOfGLO.body[TypeID::relativity]    =   -1.0;
        l1PrefitOfGLO.body[TypeID::gravDelay]     =   -1.0;
        l1PrefitOfGLO.body[TypeID::tropoSlant]    =   -1.0;
        l1PrefitOfGLO.body[TypeID::windUpL1R]     =    -1.0;

        // Definition to compute prefit residual of GLO L2
        l2PrefitOfGLO.header                      =   TypeID::prefitL2R;
        l2PrefitOfGLO.body[TypeID::L2R]            =   +1.0;
        l2PrefitOfGLO.body[TypeID::rho]           =   -1.0;
        l2PrefitOfGLO.body[TypeID::cdtSat]        =   +1.0;
        l2PrefitOfGLO.body[TypeID::relativity]    =   -1.0;
        l2PrefitOfGLO.body[TypeID::gravDelay]     =   -1.0;
        l2PrefitOfGLO.body[TypeID::tropoSlant]    =   -1.0;
        l2PrefitOfGLO.body[TypeID::windUpL2R]      =   -1.0;

        // Definition to compute prefit residual of GLO L3
        l3PrefitOfGLO.header                      =   TypeID::prefitL3R;
        l3PrefitOfGLO.body[TypeID::L3R]            =   +1.0;
        l3PrefitOfGLO.body[TypeID::rho]           =   -1.0;
        l3PrefitOfGLO.body[TypeID::cdtSat]        =   +1.0;
        l3PrefitOfGLO.body[TypeID::relativity]    =   -1.0;
        l3PrefitOfGLO.body[TypeID::gravDelay]     =   -1.0;
        l3PrefitOfGLO.body[TypeID::tropoSlant]    =   -1.0;
        l3PrefitOfGLO.body[TypeID::windUpL3R]     =    -1.0;

        // Definition to compute prefit residual of GLO L4
        l4PrefitOfGLO.header                      =   TypeID::prefitL4R;
        l4PrefitOfGLO.body[TypeID::L4R]            =   +1.0;
        l4PrefitOfGLO.body[TypeID::rho]           =   -1.0;
        l4PrefitOfGLO.body[TypeID::cdtSat]        =   +1.0;
        l4PrefitOfGLO.body[TypeID::relativity]    =   -1.0;
        l4PrefitOfGLO.body[TypeID::gravDelay]     =   -1.0;
        l4PrefitOfGLO.body[TypeID::tropoSlant]    =   -1.0;
        l4PrefitOfGLO.body[TypeID::windUpL4R]      =   -1.0;

        // Definition to compute prefit residual of GLO L6
        l6PrefitOfGLO.header                      =   TypeID::prefitL6R;
        l6PrefitOfGLO.body[TypeID::L6R]            =   +1.0;
        l6PrefitOfGLO.body[TypeID::rho]           =   -1.0;
        l6PrefitOfGLO.body[TypeID::cdtSat]        =   +1.0;
        l6PrefitOfGLO.body[TypeID::relativity]    =   -1.0;
        l6PrefitOfGLO.body[TypeID::gravDelay]     =   -1.0;
        l6PrefitOfGLO.body[TypeID::tropoSlant]    =   -1.0;
        l6PrefitOfGLO.body[TypeID::windUpL6R]      =   -1.0;

        // QZSS

        // Definition to compute prefit residual of QZS L1
        l1PrefitOfQZS.header                      =   TypeID::prefitL1J;
        l1PrefitOfQZS.body[TypeID::L1J]            =   +1.0;
        l1PrefitOfQZS.body[TypeID::rho]           =   -1.0;
        l1PrefitOfQZS.body[TypeID::cdtSat]        =   +1.0;
        l1PrefitOfQZS.body[TypeID::relativity]    =   -1.0;
        l1PrefitOfQZS.body[TypeID::gravDelay]     =   -1.0;
        l1PrefitOfQZS.body[TypeID::tropoSlant]    =   -1.0;
        l1PrefitOfQZS.body[TypeID::windUpL1J]      =   -1.0;

        // Definition to compute prefit residual of QZS L2
        l2PrefitOfQZS.header                      =   TypeID::prefitL2J;
        l2PrefitOfQZS.body[TypeID::L2J]            =   +1.0;
        l2PrefitOfQZS.body[TypeID::rho]           =   -1.0;
        l2PrefitOfQZS.body[TypeID::cdtSat]        =   +1.0;
        l2PrefitOfQZS.body[TypeID::relativity]    =   -1.0;
        l2PrefitOfQZS.body[TypeID::gravDelay]     =   -1.0;
        l2PrefitOfQZS.body[TypeID::tropoSlant]    =   -1.0;
        l2PrefitOfQZS.body[TypeID::windUpL2J]      =   -1.0;

        // Definition to compute prefit residual of QZS L5
        l5PrefitOfQZS.header                      =   TypeID::prefitL5J;
        l5PrefitOfQZS.body[TypeID::L5J]            =   +1.0;
        l5PrefitOfQZS.body[TypeID::rho]           =   -1.0;
        l5PrefitOfQZS.body[TypeID::cdtSat]        =   +1.0;
        l5PrefitOfQZS.body[TypeID::relativity]    =   -1.0;
        l5PrefitOfQZS.body[TypeID::gravDelay]     =   -1.0;
        l5PrefitOfQZS.body[TypeID::tropoSlant]    =   -1.0;
        l5PrefitOfQZS.body[TypeID::windUpL5J]     =   -1.0;


        // Definition to compute prefit residual of QZS L6
        l6PrefitOfQZS.header                      =   TypeID::prefitL6J;
        l6PrefitOfQZS.body[TypeID::L6J]            =   +1.0;
        l6PrefitOfQZS.body[TypeID::rho]           =   -1.0;
        l6PrefitOfQZS.body[TypeID::cdtSat]        =   +1.0;
        l6PrefitOfQZS.body[TypeID::relativity]    =   -1.0;
        l6PrefitOfQZS.body[TypeID::gravDelay]     =   -1.0;
        l6PrefitOfQZS.body[TypeID::tropoSlant]    =   -1.0;
        l6PrefitOfQZS.body[TypeID::windUpL6J]     =   -1.0;


         // Definition to compute prefit residual of GPS PC, L1 + L2
        pc21PrefitOfGPS.header                    =   TypeID::prefitPC21G;
        pc21PrefitOfGPS.body[TypeID::prefitC2G]   =   +ag21;
        pc21PrefitOfGPS.body[TypeID::prefitC1G]   =   -bg21;

         // Definition to compute prefitG residual of GPS LC, L1 + L2
        lc21PrefitOfGPS.header                    =   TypeID::prefitLC21G;
        lc21PrefitOfGPS.body[TypeID::prefitL2G]   =   +ag21;
        lc21PrefitOfGPS.body[TypeID::prefitL1G]   =   -bg21;

        // Definition to compute prefit residual of GPS PC, L1 + L2
        pc12PrefitOfGPS.header                    =   TypeID::prefitPC12G;
        pc12PrefitOfGPS.body[TypeID::prefitC1G]   =   +ag12;
        pc12PrefitOfGPS.body[TypeID::prefitC2G]   =   -bg12;

        // Definition to compute prefitG residual of GPS LC, L1 + L2
        lc12PrefitOfGPS.header                    =   TypeID::prefitLC12G;
        lc12PrefitOfGPS.body[TypeID::prefitL1G]   =   +ag12;
        lc12PrefitOfGPS.body[TypeID::prefitL2G]   =   -bg12;

        // Galiloe-related

        // Definition to compute prefit residual of GAL PC, L1 + L5
        pc15PrefitOfGAL.header                    =   TypeID::prefitPC15E;
        pc15PrefitOfGAL.body[TypeID::prefitC1E]   =   +ae15;
        pc15PrefitOfGAL.body[TypeID::prefitC5E]   =   -be15;

        // Definition to compute prefit residual of Galileo LC, L1 + L5
        lc15PrefitOfGAL.header                    =   TypeID::prefitLC15E;
        lc15PrefitOfGAL.body[TypeID::prefitL1E]   =   +ae15;
        lc15PrefitOfGAL.body[TypeID::prefitL5E]   =   -be15;

        // BDS-related combination

        pc26PrefitOfBDS.header                    =   TypeID::prefitPC26C;
        pc26PrefitOfBDS.body[TypeID::prefitC2C]   =   +ac26;
        pc26PrefitOfBDS.body[TypeID::prefitC6C]   =   -bc26;

         // Definition to compute prefitC residual of BDS LC, L6 + L2
        lc26PrefitOfBDS.header                    =   TypeID::prefitLC26C;
        lc26PrefitOfBDS.body[TypeID::prefitL2C]   =   +ac26;
        lc26PrefitOfBDS.body[TypeID::prefitL6C]   =   -bc26;

    }  // End of constructor 'LinearCombinations::LinearCombinations()'


      // Return the frequency of the combination in cycles: i * L1 + j * L2
    double LinearCombinations::freqOfLC(int i, int j, double f1 , double f2 )
    {
        return ( double(i)*f1+double(j)*f2 );
    }

    /// Return the wavelength of the combination in cycles: i * L1 + j * L2
    double LinearCombinations::wavelengthOfLC(int i,int j,double f1,double f2)
    {
        return C_MPS / freqOfLC(i,j,f1,f2);
    }

    /// Return the f1 factor of the combination in cycles: i * L1 + j * L2
    double LinearCombinations::firstFactorOfLC(int i,int j,double f1,double f2)
    {
        return double(i)*f1/freqOfLC(i,j,f1,f2);
    }

    /// Return the f2 factor of the combination in cycles: i * L1 + j * L2
    double LinearCombinations::secondFactorOfLC(int i,int j,double f1,double f2 )
    {
        return double(j)*f2/freqOfLC(i,j,f1,f2);
    }

    /// Warning:global function
    double wavelengthOfMW(const SatelliteSystem& sys, const TypeID& type)
    {
        string typeStr = type.asString();

        int band1 = asInt(typeStr.substr(2,1));
        int band2 = asInt(typeStr.substr(3,1));

        double f1 = getFreq(sys, band1);
        double f2 = getFreq(sys, band2);

        double wavelength = C_MPS/(f1 - f2);

        if(debug)
        {
            cout << "f1:" << f1 << "f2:" << f2 << endl;
            cout << "wavelength:" << wavelength << endl;
        }

        return wavelength; 

    };

    /// Warning:global function
    double varOfMW(const SatelliteSystem& sys, const TypeID& type)
    {
        string typeStr = type.asString();

        int band1 = asInt(typeStr.substr(2,1));
        int band2 = asInt(typeStr.substr(3,1));

        double f1 = getFreq(sys, band1);
        double f2 = getFreq(sys, band2);

        double varPhase = 0.003*0.003;
        double varCode = 0.3*0.3;

        double var = (f1*f1 + f2*f2)/((f1-f2)*(f1-f2)) * varPhase  + 
                     (f1*f1 + f2*f2)/((f1+f2)*(f1+f2)) * varCode;

        if(debug)
        {
            cout << "f1:" << f1 << "f2:" << f2 << endl;
            cout << "var:" << var << endl;
        }

        return var; 

    };

} // End of namespace gnssSpace
