#pragma ident "$Id$"

/**
 * @file TypeID.hpp
 * gnssSpace::TypeID - This class was written taking as inspiration ObsID. The
 * objective of this class is to create an index able to represent any type
 * of observation, correction, model parameter or other data value of interest
 * for GNSS data processing. This class is extensible in run-time, so the
 * programmer may add indexes on-demand.
 */

#ifndef TYPEID_HPP
#define TYPEID_HPP

//============================================================================
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2007, 2008, 2009
// 
//  2020 shjzhang change 
//
//============================================================================


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>

#include "Exception.hpp"

using namespace std;
using namespace utilSpace;

namespace gnssSpace
{

    /** This class creates an index able to represent any type of observation,
     *  correction, model parameter or other data value of interest for GNSS
     *  data processing.
     *
     * From now on, you'll be able to use INS as TypeID when you need to
     * refer to inertial system data.
     *
     */


    class TypeID
    {
    public:

        /// The type of the data value.
        enum ValueType
        {

        Unknown=0,
        C1CG, C1SG, C1LG, C1XG, C1PG, C1WG, C1YG, C1MG,
        L1CG, L1SG, L1LG, L1XG, L1PG, L1WG, L1YG, L1MG, L1NG,
        D1CG, D1SG, D1LG, D1XG, D1PG, D1WG, D1YG, D1MG, D1NG,
        S1CG, S1SG, S1LG, S1XG, S1PG, S1WG, S1YG, S1MG, S1NG,

        // rinex 2.20 for LEO observation store
        LACG, SACG, 
       
        C2CG, C2DG, C2SG, C2LG, C2XG, C2PG, C2WG, C2YG, C2MG,
        L2CG, L2DG, L2SG, L2LG, L2XG, L2PG, L2WG, L2YG, L2MG, L2NG,
        D2CG, D2DG, D2SG, D2LG, D2XG, D2PG, D2WG, D2YG, D2MG, D2NG,
        S2CG, S2DG, S2SG, S2LG, S2XG, S2PG, S2WG, S2YG, S2MG, S2NG,

        C5IG, C5QG, C5XG,
        L5IG, L5QG, L5XG,
        D5IG, D5QG, D5XG,
        S5IG, S5QG, S5XG,

        C1CR, C1PR,  
        L1CR, L1PR,  
        D1CR, D1PR,  
        S1CR, S1PR,

        C2CR, C2PR,  
        L2CR, L2PR,  
        D2CR, D2PR,  
        S2CR, S2PR,

        C3IR, C3QR, C3XR,
        L3IR, L3QR, L3XR,
        D3IR, D3QR, D3XR,
        S3IR, S3QR, S3XR,

        C4AR, C4BR, C4XR,
        L4AR, L4BR, L4XR,
        D4AR, D4BR, D4XR,
        S4AR, S4BR, S4XR,
                        
        C6AR, C6BR, C6XR,
        L6AR, L6BR, L6XR,
        D6AR, D6BR, D6XR,
        S6AR, S6BR, S6XR,

        C1AE, C1BE, C1CE, C1XE, C1ZE,
        L1AE, L1BE, L1CE, L1XE, L1ZE,
        D1AE, D1BE, D1CE, D1XE, D1ZE,
        S1AE, S1BE, S1CE, S1XE, S1ZE,

        C5IE, C5QE, C5XE,
        L5IE, L5QE, L5XE,
        D5IE, D5QE, D5XE,
        S5IE, S5QE, S5XE,

        C7IE, C7QE, C7XE,
        L7IE, L7QE, L7XE,
        D7IE, D7QE, D7XE,
        S7IE, S7QE, S7XE,

        C8IE, C8QE, C8XE,
        L8IE, L8QE, L8XE,
        D8IE, D8QE, D8XE,
        S8IE, S8QE, S8XE,

        C6AE, C6BE, C6CE, C6XE, C6ZE,
        L6AE, L6BE, L6CE, L6XE, L6ZE,
        D6AE, D6BE, D6CE, D6XE, D6ZE,
        S6AE, S6BE, S6CE, S6XE, S6ZE,

        C1DC, C1PC, C1XC, C1AC,  
        L1DC, L1PC, L1XC, L1AC, L1NC, 
        D1DC, D1PC, D1XC, D1AC, D1NC, 
        S1DC, S1PC, S1XC, S1AC, S1NC, 
                        
        C2IC, C2QC, C2XC,
        L2IC, L2QC, L2XC,
        D2IC, D2QC, D2XC,
        S2IC, S2QC, S2XC,
                        
        C5DC, C5PC, C5XC, 
        L5DC, L5PC, L5XC, 
        D5DC, D5PC, D5XC, 
        S5DC, S5PC, S5XC, 
                        
        C7IC, C7QC, C7XC, 
        L7IC, L7QC, L7XC, 
        D7IC, D7QC, D7XC, 
        S7IC, S7QC, S7XC, 
                        
        C7DC, C7PC, C7ZC,
        L7DC, L7PC, L7ZC,
        D7DC, D7PC, D7ZC,
        S7DC, S7PC, S7ZC,
                        
        C8DC, C8PC, C8XC,
        L8DC, L8PC, L8XC,
        D8DC, D8PC, D8XC,
        S8DC, S8PC, S8XC,
                  
        C6IC, C6QC, CC6X, CC6A,
        L6IC, L6QC, CL6X, CL6A,
        D6IC, D6QC, CD6X, CD6A,
        S6IC, S6QC, CS6X, CS6A,

        C5AI, C5BI, C5CI, C5XI,
        L5AI, L5BI, L5CI, L5XI,
        D5AI, D5BI, D5CI, D5XI,
        S5AI, S5BI, S5CI, S5XI,
                              
        C9AI, C9BI, C9CI, C9XI,
        L9AI, L9BI, L9CI, L9XI,
        D9AI, D9BI, D9CI, D9XI,
        S9AI, S9BI, S9CI, S9XI,

        C1CJ, C1SJ, C1LJ, C1XJ, C1ZJ,
        L1CJ, L1SJ, L1LJ, L1XJ, L1ZJ,
        D1CJ, D1SJ, D1LJ, D1XJ, D1ZJ,
        S1CJ, S1SJ, S1LJ, S1XJ, S1ZJ,
                                    
        C2SJ, C2LJ, C2XJ,           
        L2SJ, L2LJ, L2XJ,           
        D2SJ, D2LJ, D2XJ,           
        S2SJ, S2LJ, S2XJ,           
                                    
        C5IJ, C5QJ, C5XJ, C5DJ, C5PJ, C5ZJ,
        L5IJ, L5QJ, L5XJ, L5DJ, L5PJ, L5ZJ,
        D5IJ, D5QJ, D5XJ, D5DJ, D5PJ, D5ZJ,
        S5IJ, S5QJ, S5XJ, S5DJ, S5PJ, S5ZJ,
                                    
        C6SJ, C6LJ, C6XJ, C6EJ, C6ZJ,
        L6SJ, L6LJ, L6XJ, L6EJ, L6ZJ,
        D6SJ, D6LJ, D6XJ, D6EJ, D6ZJ,
        S6SJ, S6LJ, S6XJ, S6EJ, S6ZJ,
                        
        C1CS,           
        L1CS,           
        D1CS,           
        S1CS,           
                        
        C5IS, C5QS, C5XS,
        L5IS, L5QS, L5XS,
        D5IS, D5QS, D5XS,
        S5IS, S5QS, S5XS,

        C1G, C2G, C5G,
        L1G, L2G, L5G,
        D1G, D2G, D5G,
        S1G, S2G, S5G,
                     
        C1E, C5E, C7E, C8E, C6E,
        L1E, L5E, L7E, L8E, L6E,
        D1E, D5E, D7E, D8E, D6E,
        S1E, S5E, S7E, S8E, S6E,
                               
        C1R, C2R, C3R, C4R, C6R,
        L1R, L2R, L3R, L4R, L6R,
        D1R, D2R, D3R, D4R, D6R,
        S1R, S2R, S3R, S4R, S6R,
                               
        C1C, C2C, C5C, C7C, C8C, C6C,
        L1C, L2C, L5C, L7C, L8C, L6C,
        D1C, D2C, D5C, D7C, D8C, D6C,
        S1C, S2C, S5C, S7C, S8C, S6C,
                          
        C5I, C9I,        
        L5I, L9I,        
        D5I, D9I,        
        S5I, S9I,        
                          
        C1J, C2J, C5J, C6J,
        L1J, L2J, L5J, L6J,
        D1J, D2J, D5J, D6J,
        S1J, S2J, S5J, S6J,
                
        C1S, C5S,        
        L1S, L5S,        
        D1S, D5S,        
        S1S, S5S,        

        diffC1G, diffC2G, diffC5G,
        diffL1G, diffL2G, diffL5G,
        diffC1E, diffC5E, diffC7E, diffC8E, diffC6E,
        diffL1E, diffL5E, diffL7E, diffL8E, diffL6E,
        diffC1R, diffC2R, diffC3R, diffC4R, diffC6R,
        diffL1R, diffL2R, diffL3R, diffL4R, diffL6R,

        diffC1C, diffC2C, diffC5C, diffC7C, diffC8C, diffC6C,
        diffL1C, diffL2C, diffL5C, diffL7C, diffL8C, diffL6C,

        MW12G, MW15G,
        MW58E, MW57E, MW56E, MW51E, 
        MW15E,
        MW27C, MW26C, MW28C, MW25C, MW21C, 
        MW12C,

        MW21G, MW25G, 
        MW65E, MW68E, MW67E, MW61E, 
        MW75E, MW78E, MW71E, 
        MW65C, MW68C, MW67C, MW62C, MW61C, 

        MW21J, MW25J,

        LI12G, LI15E, LI27C, LI26C,

        updMW12G, 

        // Cycle Slip
        CSFlagL1G, CSFlagL2G, CSFlagL5G,
        CSFlagL1E, CSFlagL5E, CSFlagL7E, CSFlagL8E, CSFlagL6E,               
        CSFlagL1R, CSFlagL2R, CSFlagL3R, CSFlagL4R, CSFlagL6R,               
        CSFlagL1C, CSFlagL2C, CSFlagL5C, CSFlagL7C, CSFlagL8C, CSFlagL6C,             
        CSFlagL1J, CSFlagL2J, CSFlagL5J, CSFlagL6J,
        CSFlagL1S, CSFlagL5S,            

        CSFlagLC12G, CSFlagLC15E, CSFlagLC26C,
        CSFlagWL12G, CSFlagWL15E, CSFlagWL26C,

        CSFlagWL21G, CSFlagWL25G, 
        CSFlagWL56E, CSFlagWL58E, CSFlagWL57E, CSFlagWL51E, 
        CSFlagWL65E, CSFlagWL68E, CSFlagWL67E, CSFlagWL61E, 
        CSFlagWL75E, CSFlagWL78E, CSFlagWL71E, 
        CSFlagWL65C, CSFlagWL68C, CSFlagWL67C, CSFlagWL62C, CSFlagWL61C, CSFlagWL21C,

        // cs flag for IFCBLC1215G
        CSFlagIFCBLC1215G,

        CSFlag,

        // Prefit Residual
        prefitC1G, prefitC2G, prefitC5G,
        prefitL1G, prefitL2G, prefitL5G,
        prefitC1E, prefitC5E, prefitC7E, prefitC8E, prefitC6E,               
        prefitL1E, prefitL5E, prefitL7E, prefitL8E, prefitL6E,               
        prefitC1R, prefitC2R, prefitC3R, prefitC4R, prefitC6R,               
        prefitL1R, prefitL2R, prefitL3R, prefitL4R, prefitL6R,               
                                                             
        prefitC1C, prefitC2C, prefitC5C, prefitC7C, prefitC8C, prefitC6C,             
        prefitL1C, prefitL2C, prefitL5C, prefitL7C, prefitL8C, prefitL6C,             
                                                  
        prefitC1J, prefitC2J, prefitC5J, prefitC6J,             
        prefitL1J, prefitL2J, prefitL5J, prefitL6J,             

        PC12G, LC12G,
        PC15G, LC15G,
        PC21G, LC21G,
        PC51E, LC51E,
        PC15E, LC15E,
        PC16E, LC16E,
        PC26C, LC26C,

        LC1215G,

        IFCBL5G,
        IFCBLC1215G,
        diffIFCBLC1215G,
        IFCBPC1215G,
        diffIFCBPC1215G,

        WL12G,
        WL15E,
        WL26C,

        WL56E, WL58E, WL57E, WL51E, 

        WL21G, WL25G, 
        WL65E, WL68E, WL67E, WL61E, 
        WL75E, WL78E, WL71E, 
        WL65C, WL68C, WL67C, WL62C, WL61C, WL21C,

        prefitPC12G, prefitLC12G,
        prefitPC21G, prefitLC21G,
        prefitPC51E, prefitLC51E,
        prefitPC15E, prefitLC15E,
        prefitPC26C, prefitLC26C,

        prefitC1GDiff, prefitC2GDiff, prefitC5GDiff,
        prefitL1GDiff, prefitL2GDiff, prefitL5GDiff,
        prefitC1EDiff, prefitC5EDiff, prefitC7EDiff, prefitC8EDiff, prefitC6EDiff,
        prefitL1EDiff, prefitL5EDiff, prefitL7EDiff, prefitL8EDiff, prefitL6EDiff,
        prefitC1RDiff, prefitC2RDiff, prefitC3RDiff, prefitC4RDiff, prefitC6RDiff,
        prefitL1RDiff, prefitL2RDiff, prefitL3RDiff, prefitL4RDiff, prefitL6RDiff,

        prefitC1CDiff, prefitC2CDiff, prefitC5CDiff, prefitC7CDiff, prefitC8CDiff, prefitC6CDiff,
        prefitL1CDiff, prefitL2CDiff, prefitL5CDiff, prefitL7CDiff, prefitL8CDiff, prefitL6CDiff,

        diffPrefitLC12G, 

        // Phase Ambiguity
        BL1G, BL2G, BL5G,
        BL1E, BL5E, BL7E, BL8E, BL6E, 
        BL1C, BL2C, BL5C, BL7C, BL8C, BL6C, 

        // LC ambiguity
        BLC12G, 
        BLC21G, 
        BLC15E,
        BLC51E,
        BLC26C,

        // phase ambiguity
        BWL52G, BWL51G,
        BWL21G, BWL25G, 
        BWL12G, 

        // 5/8/7/6/1
        BWL58E, BWL57E, BWL56E, BWL51E, 
        BWL65E, BWL68E, BWL67E, BWL61E, 
        BWL75E, BWL78E, BWL71E, 
        BWL15E,

        // 5/8/7/6/2/1
        BWL65C, BWL68C, BWL67C, BWL62C, BWL61C,
        BWL27C, BWL26C, BWL28C, BWL25C, BWL21C,

        BWL12J, BWL25J,

        updBL1G, updBL2G, updBL5G,
        updBL1E, updBL5E, updBL7E, updBL8E, updBL6E,
        updBL1C, updBL2C, updBL5C, updBL7C, updBL8C, updBL6C,
        updBL1R, updBL2R, updBL3R, updBL4R, updBL6R,
        updBL1J, updBL2J, updBL5J, updBL6J,

        updBL1GSat, updBL2GSat, updBL5GSat,
        updBL1ESat, updBL5ESat, updBL7ESat, updBL8ESat, updBL6ESat,
        updBL1CSat, updBL2CSat, updBL5CSat, updBL7CSat, updBL8CSat, updBL6CSat,
        updBL1RSat, updBL2RSat, updBL3RSat, updBL4RSat, updBL6RSat,
        updBL1JSat, updBL2JSat, updBL5JSat, updBL6JSat,

        updBWL21G, updBWL25G, 
        updBWL56E, updBWL58E, updBWL57E, updBWL51E, 
        updBWL65E, updBWL68E, updBWL67E, updBWL61E, 
        updBWL65C, updBWL68C, updBWL67C, updBWL62C, updBWL61C,

        updBWL21GSat, updBWL25GSat, 
        updBWL56ESat, updBWL58ESat, updBWL57ESat, updBWL51ESat, 
        updBWL65ESat, updBWL68ESat, updBWL67ESat, updBWL61ESat, 
        updBWL65CSat, updBWL68CSat, updBWL67CSat, updBWL62CSat, updBWL61CSat,

        ionoMapC1G, ionoMapC2G, ionoMapC5G,
        ionoMapC1E, ionoMapC5E, ionoMapC7E, ionoMapC8E, ionoMapC6E,
        ionoMapC1C, ionoMapC2C, ionoMapC5C, ionoMapC7C, ionoMapC8C, ionoMapC6C,
        ionoMapC1R, ionoMapC2R, ionoMapC3R, ionoMapC4R, ionoMapC6R,
        ionoMapC1J, ionoMapC2J, ionoMapC5J, ionoMapC6J,

        ionoMapL1G, ionoMapL2G, ionoMapL5G,
        ionoMapL1E, ionoMapL5E, ionoMapL7E, ionoMapL8E, ionoMapL6E,
        ionoMapL1C, ionoMapL2C, ionoMapL5C, ionoMapL7C, ionoMapL8C, ionoMapL6C,
        ionoMapL1R, ionoMapL2R, ionoMapL3R, ionoMapL4R, ionoMapL6R,
        ionoMapL1J, ionoMapL2J, ionoMapL5J, ionoMapL6J,

        ionoC1G, ionoC2G, ionoC5G,
        ionoC1E, ionoC5E, ionoC7E, ionoC8E, ionoC6E,
        ionoC1C, ionoC2C, ionoC5C, ionoC7C, ionoC8C, ionoC6C,
        ionoC1R, ionoC2R, ionoC3R, ionoC4R, ionoC6R,
        ionoC1J, ionoC2J, ionoC5J, ionoC6J,

        ionoL1G, ionoL2G, ionoL5G,
        ionoL1E, ionoL5E, ionoL7E, ionoL8E, ionoL6E,
        ionoL1C, ionoL2C, ionoL5C, ionoL7C, ionoL8C, ionoL6C,
        ionoL1R, ionoL2R, ionoL3R, ionoL4R, ionoL6R,
        ionoL1J, ionoL2J, ionoL5J, ionoL6J,

        windUpL1G, windUpL2G, windUpL5G,
        windUpL1E, windUpL5E, windUpL7E, windUpL8E, windUpL6E,
        windUpL1C, windUpL2C, windUpL5C, windUpL7C, windUpL8C, windUpL6C,
        windUpL1R, windUpL2R, windUpL3R, windUpL4R, windUpL6R,
        windUpL1J, windUpL2J, windUpL5J, windUpL6J,

        rcvCorrC1G, rcvCorrC2G, rcvCorrC5G,
        rcvCorrC1E, rcvCorrC5E, rcvCorrC7E, rcvCorrC8E, rcvCorrC6E,
        rcvCorrC1C, rcvCorrC2C, rcvCorrC5C, rcvCorrC7C, rcvCorrC8C, rcvCorrC6C,
        rcvCorrC1R, rcvCorrC2R, rcvCorrC3R, rcvCorrC4R, rcvCorrC6R,           
        rcvCorrC1J, rcvCorrC2J, rcvCorrC5J, rcvCorrC6J,                       
                                                                              
        rcvCorrL1G, rcvCorrL2G, rcvCorrL5G,                                   
        rcvCorrL1E, rcvCorrL5E, rcvCorrL7E, rcvCorrL8E, rcvCorrL6E,           
        rcvCorrL1C, rcvCorrL2C, rcvCorrL5C, rcvCorrL7C, rcvCorrL8C, rcvCorrL6C,
        rcvCorrL1R, rcvCorrL2R, rcvCorrL3R, rcvCorrL4R, rcvCorrL6R,
        rcvCorrL1J, rcvCorrL2J, rcvCorrL5J, rcvCorrL6J,

        dcdtL1G, dcdtL2G, dcdtL5G, 
        dcdtL1E, dcdtL5E, dcdtL7E, dcdtL8E, dcdtL6E,
        dcdtL1C, dcdtL2C, dcdtL5C, dcdtL7C, dcdtL8C, dcdtL6C,
        dcdtL1R, dcdtL2R, dcdtL3R, dcdtL4R, dcdtL6R,
        dcdtL1J, dcdtL2J, dcdtL5J, dcdtL6J,

        // Geometric Distance
        rho,
        relativity,
        gravDelay,

        satPCOC1G, satPCOC2G, satPCOC5G,
        satPCOC1E, satPCOC5E, satPCOC7E, satPCOC8E, satPCOC6E,
        satPCOC1C, satPCOC2C, satPCOC5C, satPCOC7C, satPCOC8C, satPCOC6C,
        satPCOC1R, satPCOC2R, satPCOC3R, satPCOC4R, satPCOC6R,           
        satPCOC1J, satPCOC2J, satPCOC5J, satPCOC6J,                       

        satPCOL1G, satPCOL2G, satPCOL5G,
        satPCOL1E, satPCOL5E, satPCOL7E, satPCOL8E, satPCOL6E,
        satPCOL1C, satPCOL2C, satPCOL5C, satPCOL7C, satPCOL8C, satPCOL6C,
        satPCOL1R, satPCOL2R, satPCOL3R, satPCOL4R, satPCOL6R,           
        satPCOL1J, satPCOL2J, satPCOL5J, satPCOL6J,                       

        tropo, 
        tropoWeight, 
        dryTropo, 
        dryMap, 
        wetTropo,
        wetMap,
        tropoSlant, 
        gradNMap,
        gradEMap,

        iono, 
        ionoTEC, 
        ionoMap, 

        elevation, azimuth, 

        satArcL1G, satArcL2G, satArcL5G,
        satArcL1E, satArcL5E, satArcL7E, satArcL8E, satArcL6E,
        satArcL1C, satArcL2C, satArcL5C, satArcL7C, satArcL8C, satArcL6C,
        satArcL1R, satArcL2R, satArcL3R, satArcL4R, satArcL6R,           
        satArcL1J, satArcL2J, satArcL5J, satArcL6J,                       

        satArcLC12G,
        satArcLC15E,
        satArcLC26C,

        satArcIFCBLC1215G,

        satArcWL12G,
        satArcWL15E,
        satArcWL26C,

        satArcWL21G, satArcWL25G, 
        satArcWL56E, satArcWL58E, satArcWL57E, satArcWL51E, 
        satArcWL65E, satArcWL68E, satArcWL67E, satArcWL61E, 
        satArcWL75E, satArcWL78E, satArcWL71E, 
        satArcWL65C, satArcWL68C, satArcWL67C, satArcWL62C, satArcWL61C, satArcWL21C,

        satArc,
        weight, 

        XECEF, YECEF, ZECEF,
        satXECEF, satYECEF, satZECEF, satVXECEF, satVYECEF, satVZECEF,

        cdt, cdtDot, cdtSat, cdtSatDot,

        C5GBias, 

        C7EBias,
        C8EBias,
        C6EBias,

        C1CBias,
        C5CBias,
        C7CBias,
        C8CBias,
        C6CBias,

        upd, updSat,

        dcdtGPS, dcdtGAL, dcdtBDS, dcdtGLO, dcdtQZS,
        isbBDS, isbGAL, 

        dcdtLC12G,

        ifbC5G, 
        ifbC8E, ifbC7E, ifbC6E,
        ifbC5C, ifbC8C, ifbC7C, ifbC1C,

        dX, dY, dZ,
        dN, dE, dU,
        dLat, dLon, dH,
        vX, vY, vZ,

        staPCON, staPCOE, staPCOU,

        xpole, ypole, xpoleRate, ypoleRate, UT1mUTC, LOD, 

        beta, mu, yawangle, phiangle, 

        wavelengthL1R,
        wavelengthL2R,

        varC1, varC2, varL1, varL2, varPC12, varLC12,

        postfitC1G, postfitC2G, postfitC5G,
        postfitL1G, postfitL2G, postfitL5G,
        postfitC1E, postfitC5E, postfitC7E, postfitC8E, postfitC6E,               
        postfitL1E, postfitL5E, postfitL7E, postfitL8E, postfitL6E,               
        postfitC1R, postfitC2R, postfitC3R, postfitC4R, postfitC6R,               
        postfitL1R, postfitL2R, postfitL3R, postfitL4R, postfitL6R,               
                                                             
        postfitC1C, postfitC2C, postfitC5C, postfitC7C, postfitC8C, postfitC6C,             
        postfitL1C, postfitL2C, postfitL5C, postfitL7C, postfitL8C, postfitL6C,             
                                                  
        postfitC1J, postfitC2J, postfitC5J, postfitC6J,             
        postfitL1J, postfitL2J, postfitL5J, postfitL6J,             

        postfitPC12G, postfitLC12G,

        // carrier-range of L1 and L2
        RL1, RL2, RLC,

        N1,N2,

        N1GPS,N2GPS,
        N1BDS,N2BDS,



        // count must be last
        count 

        };


        /// empty constructor, creates an invalid object
        TypeID()
            : type(Unknown)
        {};


        /** Explicit constructor
         *
         * @param vt   ValueType for the new TypeID. If you want to use the
         *             next available ValueType, generate it using the
         *             'newValueType()' method, as indicated in the example in
         *             the documentation.
         */
        TypeID(ValueType vt)
            : type(vt)
        {};


        /** Explicit constructor
         *
         * @param name string name for ValueType, first search tString and
         *             then search mapUserTypeID, if it is not found, create
         *             it.
         */
        TypeID(std::string name);


        /// Equality requires all fields to be the same
        virtual bool operator==(const TypeID& right) const
        { return type==right.type; };


        /// This ordering is somewhat arbitrary but is required to be able
        /// to use an TypeID as an index to a std::map. If an application
        /// needs some other ordering, inherit and override this function.
        virtual bool operator<(const TypeID& right) const
        { return type < right.type; };


        /// Inequality operator
        bool operator!=(const TypeID& right) const
        { return !(operator==(right)); };

        /// Greater than operator
        bool operator>(const TypeID& right) const
        {  return (!operator<(right) && !operator==(right)); };


        /// Less than or equal operator
        bool operator<=(const TypeID& right) const
        { return (operator<(right) || operator==(right)); };

        /// Greater than or equal operator
        bool operator>=(const TypeID& right) const
        { return !(operator<(right)); };

        /// Assignment operator
        virtual TypeID operator=(const TypeID& right);

        /// Convenience output method
        virtual std::ostream& dump(std::ostream& s) const;

        virtual std::string asString() const;

        /// Return true if this is a valid TypeID. Basically just
        /// checks that the enum is defined
        virtual bool isValid() const;

        /// Destructor
        virtual ~TypeID() {};

        /// Type of the value
        ValueType type;

        /// Map holding type descriptions
        /// must const
        static const std::string tStrings[];

    }; // End of class 'TypeID'


    /// stream output for TypeID
    std::ostream& operator<<(std::ostream& s, const TypeID& p);

    /// some useful TypeID data structures
    typedef std::set<TypeID> TypeIDSet;
    typedef std::vector<TypeID> TypeIDVec;
    typedef std::map<std::string, TypeIDVec> SysTypesMap;


}  // End of namespace gnssSpace

#endif   // TYPEID_HPP
