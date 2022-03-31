/**
 *  Function:
 *  Single point positioning
 *
 *  copyright
 *
 *  shoujian zhang, 2019-2022
 */

// System
#include <iostream>
#include <string>

// 命令行参数解析
#include "OptionUtil.hpp"

// File
#include "ConfigReader.hpp"
#include "Rx3NavStore.hpp"
#include "Rx3ObsData.hpp"
#include "ChooseOptimalTypes.hpp"
#include "KeepSystems.hpp"
#include "FilterCode.hpp"
#include "ConvertObs.hpp"
#include "Counter.hpp"
#include "ComputeSatPos.hpp"
#include "ComputeDerivative.hpp"
#include "ComputeTropModel.hpp"
#include "TropModel.hpp"
#include "DataStructures.hpp"
#include "Variable.hpp"
#include "PrintSols.hpp"
#include "DumpRinex.hpp"
#include "RequiredObs.hpp"
#include "LinearCombinations.hpp"
#include "ComputeCombination.hpp"
#include "ComputeElevWeights.hpp"
#include "DetectCSMW.hpp"
#include "MarkArc.hpp"
#include "LsqSPP.hpp"
#include "LsqRTK.hpp"
#include "DeltaOp.hpp"
#include "ComputePrefit.hpp"

#define debug 0

using namespace std;
using namespace gnssSpace;
using namespace utilSpace;

int main(int argc,char* argv[]) 
{
    string helpInfo
       =
    "Usage: \n"
    "  spp for single station using multiple GNSS with uncombined model! \n"
    "\n"
    "required options:\n"
    "  --baseObsFile <fileName>      base station rinex file name \n"
    "  --roverObsFile <fileName>     rover staton rinex file name \n"
    "  --baseXYZ <coordinate>        reference coordinates for base station, seperated with whitespace \n"
    "  --navFile <nav_file>          input nav file list, this option can be repeated \n"
    "\n"
    "optional options:\n"
    "  --help                        Prints this help \n"
    "  --outputFile <out_file>       output file name \n"
    "\n"
    "Examples: "
    "   \n"
    "\n"
    "Warning: \n"
    "  rtk.conf MUST be given.\n"
    "\n"
    "Examples: \n"
    "  obsFile=./data/ABMF00GLP_R_20210010000_01D_30S_MO.rnx \n"
    "  rtk --baseObsFile $obsFile --roverObsFile $obsFile --baseXYZ \"2919785.7120 -5383745.0670  1774604.6920\" \n"
    "\n"
    "Copyright(c)\n"
    "  shoujian zhang, 2022, Wuhan University\n";

    // map for attribute/value data
    OptionAttMap optAttData;
    OptionValueMap optValData;

    // define option attribute for options
    OptionAttribute baseObsAttribute(1, 0);
    OptionAttribute roverObsAttribute(1, 0);
    OptionAttribute navAttribute(1, 1);
    OptionAttribute outAttribute(1, 0);
    OptionAttribute baseXYZAttribute(0, 0);
    OptionAttribute helpAttribute(0, 0);

    /// define and insert
    optAttData["--baseObsFile"] = baseObsAttribute;
    optAttData["--roverObsFile"] = roverObsAttribute;
    optAttData["--baseXYZ"] = baseXYZAttribute;
    optAttData["--navFile"] = navAttribute;
    optAttData["--outputFile"] = outAttribute;
    optAttData["--help"] = helpAttribute;

    ///prase the options
    parseOption(argc, argv, optAttData, optValData, helpInfo);

    // Parse required options
    string baseObsFile;
    string roverObsFile;
    std::vector<string> navFileVec;
    string outputFile;

    ///--baseObsFile
    if (optValData.find("--baseObsFile") != optValData.end())
    {
        baseObsFile = optValData["--baseObsFile"][0];
    }
    else
    {
        cerr << "--baseObsFile is required!" << endl;
        exit(-1);
    }

    ///--roverObsFile
    if (optValData.find("--roverObsFile") != optValData.end())
    {
        roverObsFile = optValData["--roverObsFile"][0];
    }
    else
    {
        cerr << "--roverObsFile is required!" << endl;
        exit(-1);
    }

    /// --navFile
    if (optValData.find("--navFile") != optValData.end())
    {
        navFileVec = optValData["--navFile"];
    } else
    {
        cerr << "--ephFile is required!" << endl;
        exit(-1);
    }

    ///--outputFile
    if (optValData.find("--outputFile") != optValData.end())
    {
        outputFile = optValData["--outputFile"][0];
    }
    else
    {
        cerr << "--outputFile is required!" << endl;
        exit(-1);
    }

    //===============================================================
    // now, Let's load configuration data from conf file
    //===============================================================
    ConfigReader confReader;
    try
    {
        confReader.open("./spp.conf");
    }
    catch (Exception &e)
    {
        cerr << "please put spp.conf in current directory!" << endl;
        exit(-1);
    }

    /// Tropospheric model
    NeillTropModel neillTM;

    string system;
    double elev;
    double begin_sod;
    double end_sod;
    int    rcvState;

    try
    {
        system = confReader.getValue("system");
        elev = confReader.getValueAsDouble("elevation");
        begin_sod = confReader.getValueAsDouble("begin_sod");
        end_sod = confReader.getValueAsDouble("end_sod");
    }
    catch (Exception &e)
    {
        cerr << e << endl;
        exit(-1);
    }

    if (debug)
    {
        cout << "systems:" << system << endl;
        cout << "elev:" << elev << endl;
        cout << "begin_sod" << begin_sod << endl;
        cout << "end_sod" << end_sod << endl;
    }

    ///>now, read nav files
    Rx3NavStore navStore;

    for (auto f: navFileVec)
    {
        if (debug)
        {
            cout << "nav file:" << f << endl;


        }

        try
        {

            navStore.loadFile(f);

        }
        catch (Exception &e)
        {
            cerr << e << endl;
            cerr << "unknow error in read nav data" << endl;
            exit(-1);
        }
    }
    cout<<"after nav load"<<endl;

    //*************************************************
    // read rover station header and stream for record data reading
    //*************************************************

    /// now, let's read data for current satation
    Rx3ObsHeader rxHeaderRover;
    Rx3ObsData rxDataRover;

    std::fstream rxStreamRover(roverObsFile.c_str(), ios::in);
    if (!rxStreamRover)
    {
        cerr << "can't open file:" << baseObsFile.c_str() << endl;
    }

    // first time
    CommonTime firstEpoch, lastEpoch;
    try
    {
        rxStreamRover >> rxHeaderRover;
    }
    catch (Exception &e)
    {
        cerr << e << endl;
        exit(-1);
    }

    ChooseOptimalTypes chooseOptimalTypes;
    SysTypesMap sysPrioriTypes = chooseOptimalTypes.get(rxHeaderRover.mapObsTypes);

    if (debug)
    {
        cout << "sysPrioriTypes" << endl;
        for (auto sT: sysPrioriTypes)
        {
            cout << sT.first << endl;
            for (int i = 0; i < sT.second.size(); i++)
            {
                cout << sT.second[i].asString() << endl;
            }
        }
    }

    Triple rcvPosRover = rxHeaderRover.antennaPosition;

    // set the header pointer to rxDataRover for data reading.
    rxDataRover.pHeader = &rxHeaderRover;

    firstEpoch = rxHeaderRover.firstObs.convertToCommonTime() + begin_sod;
    lastEpoch = rxHeaderRover.firstObs.convertToCommonTime() + end_sod;

    //*************************************************
    // read base station header and stream for record data reading
    //*************************************************

    /// now, let's read data for current satation
    Rx3ObsHeader rxHeaderBase;
    Rx3ObsData rxDataBase;

    std::fstream rxStreamBase(baseObsFile.c_str(), ios::in);
    if (!rxStreamBase)
    {
        cerr << "can't open file:" << baseObsFile.c_str() << endl;
    }

    try
    {
        rxStreamBase >> rxHeaderBase;
    }
    catch (Exception &e)
    {
        cerr << e << endl;
        exit(-1);
    }

    Triple rcvPosBase = rxHeaderBase.antennaPosition;
    rxDataBase.pHeader = &rxHeaderBase;


    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // the following classes are for rover and base station;
    // warning:
    // some classes will store internal state for different station, 
    // you must check and gurantee the correct state for different station.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // keep satellite system for positioning
    KeepSystems keepSystems(system);

    // filter out bad code/phase observables
    FilterCode filterCode;

    // convert 4-char TypeID to 2-char TypeID for easy use in following classes
    ConvertObs convertObs;
    convertObs.setSysPrioriTypes(sysPrioriTypes);

    RequiredObs reqObs;
    reqObs.addRequiredType(SatelliteSystem::GPS,     TypeID::C1G);
    reqObs.addRequiredType(SatelliteSystem::GPS,     TypeID::C2G);
    reqObs.addRequiredType(SatelliteSystem::GPS,     TypeID::L1G);
    reqObs.addRequiredType(SatelliteSystem::GPS,     TypeID::L2G);
//    reqObs.addRequiredType(SatelliteSystem::Galileo, TypeID::C1E);
//    reqObs.addRequiredType(SatelliteSystem::Galileo, TypeID::C5E);

    reqObs.addRequiredType(SatelliteSystem::BDS,     TypeID::C2C);
    reqObs.addRequiredType(SatelliteSystem::BDS,     TypeID::C6C);
    reqObs.addRequiredType(SatelliteSystem::BDS,     TypeID::L2C);
    reqObs.addRequiredType(SatelliteSystem::BDS,     TypeID::L6C);


//    reqObs.addRequiredType(SatelliteSystem::GLONASS, TypeID::C1R);
//    reqObs.addRequiredType(SatelliteSystem::GLONASS, TypeID::C2R);

    LinearCombinations linear;

    ComputeCombination computeIF;
    computeIF.addLinear(SatelliteSystem::GPS,     linear.pc12CombOfGPS);
    computeIF.addLinear(SatelliteSystem::GPS,     linear.lc12CombOfGPS);
    computeIF.addLinear(SatelliteSystem::BDS,     linear.pc26CombOfBDS);
    computeIF.addLinear(SatelliteSystem::BDS,     linear.lc26CombOfBDS);
//    computeIF.addLinear(SatelliteSystem::Galileo, linear.pc15CombOfGAL);
//    computeIF.addLinear(SatelliteSystem::Galileo, linear.lc15CombOfGAL);

    ComputeCombination computeMW;
    computeMW.addLinear(SatelliteSystem::GPS,     linear.mw21CombOfGPS);
    computeMW.addLinear(SatelliteSystem::BDS,     linear.mw62CombOfBDS);

    // 
    // 周跳标记和弧段标记会存储内部状态，不能混用不同测站
    //
    DetectCSMW detectCSMWRover;
    detectCSMWRover.addType(SatelliteSystem::GPS,     TypeID::MW21G);
    detectCSMWRover.addType(SatelliteSystem::BDS,     TypeID::MW62C);

    DetectCSMW detectCSMWBase;
    detectCSMWBase.addType(SatelliteSystem::GPS,     TypeID::MW21G);
    detectCSMWBase.addType(SatelliteSystem::BDS,     TypeID::MW62C);

    //
    MarkArc markArcRover;

    // compute satellite-positions according to nav file
    ComputeSatPos computeSatPos(navStore);

    ComputeDerivative computeDerivative;

    ComputeTropModel computeTrop;
    computeTrop.setTropModel(neillTM);

    ComputeElevWeights elevWeight;

    LsqSPP lsqSPP;
    lsqSPP.setSource(rxHeaderRover.markerName);

    // 构建历元间差分prefit
    ComputeCombination sppPrefit;

    ComputePrefit computePrefit;

    // code
    gnssLinearCombination c1PrefitOfGPS = linear.c1PrefitOfGPS;
    gnssLinearCombination c2PrefitOfGPS = linear.c2PrefitOfGPS;
//    gnssLinearCombination c1PrefitOfGAL = linear.c1PrefitOfGAL;
//    gnssLinearCombination c5PrefitOfGAL = linear.c5PrefitOfGAL;
    gnssLinearCombination c2PrefitOfBDS = linear.c2PrefitOfBDS;
    gnssLinearCombination c6PrefitOfBDS = linear.c6PrefitOfBDS;

    // phase
    gnssLinearCombination l1PrefitOfGPS = linear.l1PrefitOfGPS;
    gnssLinearCombination l2PrefitOfGPS = linear.l2PrefitOfGPS;
//    gnssLinearCombination l1PrefitOfGAL = linear.l1PrefitOfGAL;
//    gnssLinearCombination l5PrefitOfGAL = linear.l5PrefitOfGAL;
    gnssLinearCombination l2PrefitOfBDS = linear.l2PrefitOfBDS;
    gnssLinearCombination l6PrefitOfBDS = linear.l6PrefitOfBDS;

    // 没有gravDelay，观测值残差无法计算
    c1PrefitOfGPS.addOptionalType(TypeID::gravDelay) ;
    c2PrefitOfGPS.addOptionalType(TypeID::gravDelay) ;
//    c1PrefitOfGAL.addOptionalType(TypeID::gravDelay) ;
//    c5PrefitOfGAL.addOptionalType(TypeID::gravDelay) ;
    c2PrefitOfBDS.addOptionalType(TypeID::gravDelay) ;
    c6PrefitOfBDS.addOptionalType(TypeID::gravDelay) ;

    l1PrefitOfGPS.addOptionalType(TypeID::gravDelay) ;
    l2PrefitOfGPS.addOptionalType(TypeID::gravDelay) ;
//    l1PrefitOfGAL.addOptionalType(TypeID::gravDelay) ;
//    l5PrefitOfGAL.addOptionalType(TypeID::gravDelay) ;
    l2PrefitOfBDS.addOptionalType(TypeID::gravDelay) ;
    l6PrefitOfBDS.addOptionalType(TypeID::gravDelay) ;

    // windup 也必须给定
    l1PrefitOfGPS.addOptionalType(TypeID::windUpL1G) ;
    l2PrefitOfGPS.addOptionalType(TypeID::windUpL2G) ;
//    l1PrefitOfGAL.addOptionalType(TypeID::windUpL1E) ;
//    l5PrefitOfGAL.addOptionalType(TypeID::windUpL5E) ;
    l2PrefitOfBDS.addOptionalType(TypeID::windUpL2C) ;
    l6PrefitOfBDS.addOptionalType(TypeID::windUpL6C) ;

    // gps
    sppPrefit.addLinear(SatelliteSystem::GPS,     c1PrefitOfGPS);
    sppPrefit.addLinear(SatelliteSystem::GPS,     c2PrefitOfGPS);
    sppPrefit.addLinear(SatelliteSystem::GPS,     l1PrefitOfGPS);
    sppPrefit.addLinear(SatelliteSystem::GPS,     l2PrefitOfGPS);

//    // gal
//    sppPrefit.addLinear(SatelliteSystem::Galileo, c1PrefitOfGAL);
//    sppPrefit.addLinear(SatelliteSystem::Galileo, c5PrefitOfGAL);
//    sppPrefit.addLinear(SatelliteSystem::Galileo, l1PrefitOfGAL);
//    sppPrefit.addLinear(SatelliteSystem::Galileo, l5PrefitOfGAL);

    // bds
    sppPrefit.addLinear(SatelliteSystem::BDS,     c2PrefitOfBDS);
    sppPrefit.addLinear(SatelliteSystem::BDS,     c6PrefitOfBDS);
    sppPrefit.addLinear(SatelliteSystem::BDS,     l2PrefitOfBDS);
    sppPrefit.addLinear(SatelliteSystem::BDS,     l6PrefitOfBDS);


    ///////////////////////////////////////////////////
    // RTK classes 
    ///////////////////////////////////////////////////

    // compute between-station difference equation
    DeltaOp deltaOp;
    deltaOp.addDiffType(SatelliteSystem::GPS, TypeID::prefitC1G);
    deltaOp.addDiffType(SatelliteSystem::GPS, TypeID::prefitC2G);
    deltaOp.addDiffType(SatelliteSystem::GPS, TypeID::prefitL1G);
    deltaOp.addDiffType(SatelliteSystem::GPS, TypeID::prefitL2G);
    deltaOp.addDiffType(SatelliteSystem::BDS, TypeID::prefitC2C);
    deltaOp.addDiffType(SatelliteSystem::BDS, TypeID::prefitC6C);
    deltaOp.addDiffType(SatelliteSystem::BDS, TypeID::prefitL2C);
    deltaOp.addDiffType(SatelliteSystem::BDS, TypeID::prefitL6C);

    // LsqRTK: single-epoch RTK
    LsqRTK lsqRTK;
    lsqRTK.setSource(rxHeaderRover.markerName);

    /////////// print spp solutions //////////
    std::ofstream outStream(outputFile.c_str(), ios::out);
    if(!outStream.is_open())
    {
        cerr << "can't open sppOutFile!" << endl;
        exit(-1);
    }

    PrintSols printSols(outStream);

    // now, let's process gnss data for curret station
    while (true)
    {
        ///////////////////////////////////////
        // data processing for rover station
        ///////////////////////////////////////
        try
        {
            rxStreamRover >> rxDataRover;
        }
        catch (EndOfFile &e)
        {
            break;
        }

        /// write solution to files
        CommonTime currEpoch = rxDataRover.currEpoch;

//        cout << "processing data for rover obs file at epoch:" << YDSTime(currEpoch) << endl;
        if(debug)
            rxDataRover.dump(cout, 1);

        // keep only given system
        keepSystems.Process(rxDataRover);
        filterCode.Process(rxHeaderRover.mapObsTypes, rxDataRover);
        convertObs.Process(rxDataRover);
        reqObs.Process(rxDataRover);
        computeIF.Process(rxDataRover);

        rxDataRover.stvData.removeSatID(SatID(SatelliteSystem::BDS,6));


        if (rxDataRover.numSats() <= 6)
        {
            continue;
        }

        if(YDSTime(currEpoch).sod<= 26160.0){
            continue;
        }
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        // systematic and linearise for rover station
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        int iter(0);
        while (true)
        {
            iter++;

            // 卫星位置的计算与接收机初始坐标无关, 可以把地球自传改正独立出来
            computeSatPos.setRxPos(rcvPosRover);
            computeSatPos.Process(rxDataRover);

            computeDerivative.setCoordinates(rcvPosRover);
            computeDerivative.Process(rxDataRover);

            computeTrop.setAllParameters(currEpoch, rcvPosRover);
            computeTrop.Process(rxDataRover);

            sppPrefit.Process(rxDataRover);

            // compute spp soulution using LSQ
            lsqSPP.Process(rxDataRover);


            // get dx
            Triple dxTriple = lsqSPP.getDx();
            double dxMag = dxTriple.mag();



            rcvPosRover = rcvPosRover + dxTriple;

            // convergence threshold
            if(dxMag < 0.01)
            {
                break;
            }

            if(iter>5)
            {
                break;
            }
        }

        if(debug)
        {
            cout << "after spp for rover!" << endl;
            rxDataRover.dump(cout, 1);
        }

        computeMW.Process(rxDataRover);
        detectCSMWRover.Process(rxDataRover);

        ///////////////////////////////////////
        // data processing for base station
        ///////////////////////////////////////
        try
        {
            rxDataBase.readRecordByTime(rxStreamBase,currEpoch);
        }
        catch (EndOfFile &e)
        {
            cout << "end of file" << endl;
            break;
        }catch (bool &flag){
            cout<<"同步失败"<<endl;
            continue;
        }

        // keep only given system
        keepSystems.Process(rxDataBase);
        filterCode.Process(rxHeaderBase.mapObsTypes, rxDataBase);
        convertObs.Process(rxDataBase);
        reqObs.Process(rxDataBase);
        computeIF.Process(rxDataBase);
        if (rxDataBase.numSats() <= 6)
        {
            continue;
        }

        // 卫星位置的计算与接收机初始坐标无关, 可以把地球自传改正独立出来
        computeSatPos.setRxPos(rcvPosBase);
        computeSatPos.Process(rxDataBase);
        // partial
        computeDerivative.setCoordinates(rcvPosBase);
        computeDerivative.Process(rxDataBase);
        // trop 
        computeTrop.setAllParameters(currEpoch, rcvPosBase);
        computeTrop.Process(rxDataBase);
        // prefit 
//        sppPrefit.Process(rxDataBase);
//        computeMW.Process(rxDataBase);
//        detectCSMWBase.Process(rxDataBase);

        computePrefit.Process(rxDataBase.currEpoch,rxDataBase.stvData);

        ///////////////////////////////////////
        // Now, between-station observation equation preparation for rover and base.
        ///////////////////////////////////////
        deltaOp.Process(rxDataRover.stvData, rxDataBase.stvData);


        //printSols.printPrefitDiff(rxDataRover.stvData,currEpoch);


        // 对站间差分观测值标记弧段
        markArcRover.Process(rxDataRover);
        elevWeight.Process(rxDataRover);

        SatID satId(SatelliteSystem::BDS,6);

         rxDataRover.stvData.removeSatID(satId);

        if (rxDataRover.numSats() <= 6)
        {
            continue;
        }

        lsqRTK.Process(rxDataRover);

        Triple dxTriple = lsqRTK.getDx();
        Triple dxFixTriple = lsqRTK.getDxFixed();

        if(abs(dxTriple[0])<10){
            outStream<<
                     "OSS"<<" "<<
                     currEpoch<<" "<<
                     dxTriple[0]<<" "<<
                     dxTriple[1]<<" "<<
                     dxTriple[2]<<" "<<endl;

            dxTriple = dxTriple + rcvPosRover - rcvPosBase;

            outStream<<
                     "SSS"<<" "<<
                     currEpoch<<" "<<
                     dxTriple[0]<<" "<<
                     dxTriple[1]<<" "<<
                     dxTriple[2]<<" "<<endl;

            dxFixTriple = dxFixTriple + rcvPosRover - rcvPosBase;

            outStream<<
                     "ISS"<<" "<<
                     currEpoch<<" "<<
                     dxFixTriple[0]<<" "<<
                     dxFixTriple[1]<<" "<<
                     dxFixTriple[2]<<" "<<
                     lsqRTK.getIsFixed()<<" "<<endl;

        }else{
            outStream<<
                     "ESS"<<" "<<
                     currEpoch<<" "<<
                     dxTriple[0]<<" "<<
                     dxTriple[1]<<" "<<
                     dxTriple[2]<<" "<<endl;

        }




//        Triple dxFixed = lsqRTK.getDxFixed();
//        bool isFixed = lsqRTK.getIsFixed();

//        Triple rcvPosRoverFloat;
//        rcvPosRoverFloat = rcvPosRover + dxTriple;
//
//        Triple rcvPosRoverFixed;
//        rcvPosRoverFixed = rcvPosRover + dxFixed;
//
//        cout << "print solution for epoch:" << YDSTime(currEpoch) << endl;
//
//        // here is the spp solutions
//        printSols.printRTKSols(currEpoch, rxDataRover.numSats(), rcvPosRoverFloat, isFixed, rcvPosRoverFixed);
//
//        // at last, transfer the float rcvPosRoverFloat to the next epoch for initialization
//        rcvPosRover = rcvPosRoverFloat;

    }

    // close streams
    rxStreamRover.close();
    rxStreamBase.close();
    outStream.close();

    cout << "end of processing file:" << outputFile << endl;
    return 0;
}
