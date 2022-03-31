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
#include "LsqSPP.hpp"

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
    "  --obsFile <fileName>          rinex file name \n"
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
    "  spp.conf MUST be given.\n"
    "\n"
    "Copyright(c)\n"
    "  shoujian zhang, 2022, Wuhan University\n";

    // map for attribute/value data
    OptionAttMap optAttData;
    OptionValueMap optValData;

    // define option attribute for options
    OptionAttribute obsAttribute(1, 0);
    OptionAttribute navAttribute(1, 1);
    OptionAttribute outAttribute(1, 0);
    OptionAttribute helpAttribute(0, 0);

    /// define and insert
    optAttData["--obsFile"] = obsAttribute;
    optAttData["--navFile"] = navAttribute;
    optAttData["--outputFile"] = outAttribute;
    optAttData["--help"] = helpAttribute;

    ///prase the options
    parseOption(argc, argv, optAttData, optValData, helpInfo);

    // Parse required options
    string obsFile;
    std::vector<string> navFileVec;
    string outputFile;

    ///--obsFile
    if (optValData.find("--obsFile") != optValData.end())
    {
        obsFile = optValData["--obsFile"][0];
    }
    else
    {
        cerr << "--obsFile is required!" << endl;
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
        rcvState    = confReader.getValueAsInt("rcvState");
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

    /// now, let's read data for current satation
    Rx3ObsHeader rxHeader;
    Rx3ObsData rxData;

    std::fstream rxStream(obsFile.c_str(), ios::in);
    if (!rxStream)
    {
        cerr << "can't open file:" << obsFile.c_str() << endl;
    }

    // first time
    CommonTime firstEpoch, lastEpoch;

    try
    {
        rxStream >> rxHeader;
    }
    catch (Exception &e)
    {
        cerr << e << endl;
        exit(-1);
    }

    if (debug)
    {
        cout << "after read rxHeader" << endl;
    }

    ChooseOptimalTypes chooseOptimalTypes;
    SysTypesMap sysPrioriTypes = chooseOptimalTypes.get(rxHeader.mapObsTypes);

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

    double interval = rxHeader.interval;
    if (interval == 0.0)
    {
        interval = 30.0;
    }

    firstEpoch = rxHeader.firstObs.convertToCommonTime() + int(begin_sod / interval) * interval;
    lastEpoch = rxHeader.firstObs.convertToCommonTime() + int(end_sod / interval) * interval;

    if (debug)
    {
        cout << "firstEpoch:" << firstEpoch << endl;/**/
        cout << "lastEpoch:" << lastEpoch << endl;
    }

    // set the header pointer to rxData for data reading.
    rxData.pHeader = &rxHeader;

    // keep satellite system for positioning
    KeepSystems keepSystems(system);

    if (debug)
    {
        cout << "KeepSystems" << endl;
    }

    // filter out bad code/phase observables
    FilterCode filterCode;

    if (debug)
    {
        cout << "FilterCode" << endl;
    }

    // convert 4-char TypeID to 2-char TypeID for easy use in following classes
    ConvertObs convertObs;
    convertObs.setSysPrioriTypes(sysPrioriTypes);

    RequiredObs reqObs;
    reqObs.addRequiredType(SatelliteSystem::GPS,     TypeID::C1G);
    reqObs.addRequiredType(SatelliteSystem::GPS,     TypeID::C2G);
    reqObs.addRequiredType(SatelliteSystem::Galileo, TypeID::C1E);
    reqObs.addRequiredType(SatelliteSystem::Galileo, TypeID::C5E);

    reqObs.addRequiredType(SatelliteSystem::BDS,     TypeID::C2C);
    reqObs.addRequiredType(SatelliteSystem::BDS,     TypeID::C6C);
    reqObs.addRequiredType(SatelliteSystem::GLONASS, TypeID::C1R);
    reqObs.addRequiredType(SatelliteSystem::GLONASS, TypeID::C2R);

    LinearCombinations linear;

    ComputeCombination computeIF;
    computeIF.addLinear(SatelliteSystem::GPS,     linear.pc12CombOfGPS);
    computeIF.addLinear(SatelliteSystem::GPS,     linear.lc12CombOfGPS);
    computeIF.addLinear(SatelliteSystem::BDS,     linear.pc26CombOfBDS);
    computeIF.addLinear(SatelliteSystem::BDS,     linear.lc26CombOfBDS);
    computeIF.addLinear(SatelliteSystem::Galileo, linear.pc15CombOfGAL);
    computeIF.addLinear(SatelliteSystem::Galileo, linear.lc15CombOfGAL);

    // 构建历元间差分prefit
    ComputeCombination sppPrefit;

    // gps
    gnssLinearCombination c1PrefitOfGPS = linear.c1PrefitOfGPS;
    gnssLinearCombination c2PrefitOfGPS = linear.c2PrefitOfGPS;
    gnssLinearCombination c1PrefitOfGAL = linear.c1PrefitOfGAL;
    gnssLinearCombination c5PrefitOfGAL = linear.c5PrefitOfGAL;
    gnssLinearCombination c2PrefitOfBDS = linear.c2PrefitOfBDS;
    gnssLinearCombination c6PrefitOfBDS = linear.c6PrefitOfBDS;

    c1PrefitOfGPS.addOptionalType(TypeID::gravDelay) ;
    c2PrefitOfGPS.addOptionalType(TypeID::gravDelay) ;
    c1PrefitOfGAL.addOptionalType(TypeID::gravDelay) ;
    c5PrefitOfGAL.addOptionalType(TypeID::gravDelay) ;
    c2PrefitOfBDS.addOptionalType(TypeID::gravDelay) ;
    c6PrefitOfBDS.addOptionalType(TypeID::gravDelay) ;

    // gps
    sppPrefit.addLinear(SatelliteSystem::GPS,     c1PrefitOfGPS);
    sppPrefit.addLinear(SatelliteSystem::GPS,     c2PrefitOfGPS);

    // gal
    sppPrefit.addLinear(SatelliteSystem::Galileo, c1PrefitOfGAL);
    sppPrefit.addLinear(SatelliteSystem::Galileo, c5PrefitOfGAL);

    // bds
    sppPrefit.addLinear(SatelliteSystem::BDS,     c2PrefitOfBDS);
    sppPrefit.addLinear(SatelliteSystem::BDS,     c6PrefitOfBDS);

    std::string sppFile;
    sppFile = outputFile + ".spp." + system;

    std::ofstream sppStream(sppFile.c_str(), ios::out);
    if(!sppStream.is_open())
    {
        cerr << "can't open sppFile!" << endl;
        exit(-1);
    }

    Triple rcvPosRef;
    rcvPosRef = rxHeader.antennaPosition;

    Triple rcvPos = rcvPosRef;

    // compute satellite-positions according to nav file
    ComputeSatPos computeSatPos(navStore);

    ComputeDerivative computeDerivative;

    if (debug)
    {
        cout << "after ComputeDerivative" << endl;
    }

    ComputeTropModel computeTrop;
    computeTrop.setTropModel(neillTM);

    if (debug)
    {
        cout << "after define:ComputeTropModel" << endl;
    }

    LsqSPP lsqSPP;
    lsqSPP.setSource(rxHeader.markerName);

    /////////// print spp solutions //////////
    std::string sppOutFile;
    sppOutFile = outputFile + ".filter.spp.out";

    cout << "sppOutFile:" << sppOutFile << endl;

    std::ofstream sppOutStream(sppOutFile.c_str(), ios::out);
    if(!sppOutStream.is_open())
    {
        cerr << "can't open sppOutFile!" << endl;
        exit(-1);
    }

    PrintSols printSppSols(sppOutStream);
    printSppSols.printHeader();

    // now, let's process gnss data for curret station
    bool firstTime(true);
    while (true)
    {
        try
        {
            double clock1(Counter::now());
            // read data
            try
            {
                rxStream >> rxData;
            }
            catch (EndOfFile &e)
            {
                cout << "end of file" << endl;
                break;
            }

            if(debug)
            {
                cout << "after rxStream" << endl;
                rxData.dump(cout, 1);
            }

            /// write solution to files
            CommonTime currEpoch = rxData.currEpoch;

            /// skip record with flag >1
            /// see details in Rx3ObsData or rinex3.04.pdf
            if (rxData.epochFlag > 1)
            {
                continue;
            }

            if (debug)
            {
                cout << "after obsStrm" << endl;
                rxData.dump(cout, 1);
            }

            // keep only given system
            keepSystems.Process(rxData);
            if (debug)
            {
                cout << "after keepSystems" << endl;
                rxData.dump(cout, 1);
            }

            // filter out outliers
            filterCode.Process(rxHeader.mapObsTypes, rxData);
            if (debug)
            {
                cout << "after filterCode" << endl;
                rxData.dump(cout, 1);
            }

            convertObs.Process(rxData);
            if (debug)
            {
                cout << "after convertObs" << endl;
                rxData.dump(cout, 1);
            }

            // required obs
            reqObs.Process(rxData);

            // now, compute if combinations
            computeIF.Process(rxData);

            if (rxData.numSats() <= 6)
            {
                continue;
            }

            //////////////////////////////////////////
            ///  计算接收机位置初始位置并改正各类系统误差
            //////////////////////////////////////////

            int iter(0);
            while (true)
            {
                iter++;

                if (debug)
                {
                    cout << "rcvPos" << endl;
                    cout << rcvPos << endl;
                }

                // 卫星位置的计算与接收机初始坐标无关,
                // 可以把地球自传改正独立出来
                computeSatPos.setRxPos(rcvPos);
                computeSatPos.Process(rxData);

                if (debug)
                {
                    cout << "after computeSatPos" << endl;
                    rxData.dump(cout, 1);
                    computeSatPos.printTimeUsed(cout);
                }

                computeDerivative.setCoordinates(rcvPos);
                computeDerivative.Process(rxData);

                if (debug)
                {
                    cout << "after computeDerivative" << endl;
                    rxData.dump(cout, 1);
                }

                if (debug)
                    cout << "firstEpoch:" << firstEpoch << endl;

                computeTrop.setAllParameters(firstEpoch, rcvPos);
                computeTrop.Process(rxData);
                if (debug)
                {
                    cout << "after computeTrop" << endl;
                    rxData.dump(cout, 1);
                }

                // 计算第一次prefit
                sppPrefit.Process(rxData);

                // compute spp soulution using LSQ
                lsqSPP.Process(rxData);

                // get dx
                Triple dxTriple = lsqSPP.getDx();
                double dxMag = dxTriple.mag();

                // update the receiver solution
                if(debug)
                {
                    cout << "iter:" << iter << endl;
                    cout << YDSTime(currEpoch) << " dxTriple:" << dxTriple << endl;
                    cout << "dxMag:" << dxMag << endl;
                }

                rcvPos = rcvPos + dxTriple;

                if(debug)
                {
                    cout << "update rcvPos:" << rcvPos << endl;
                }

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


            // here is the spp solutions
            printSppSols.printRecord(currEpoch, rxData.numSats(), rcvPos);

            // 必须放在最后，如果有任何异常，比如卫星号小于４颗，
            // 则不能到这里，那么还需要保留firstTime
            // reset firstTime
            if(firstTime)
            {
                firstTime = false;
            }
        }
        catch (Exception &e)
        {
            cerr << e << endl;
            exit(-1);
        }
    }

    // close streams
    rxStream.close();

    cout << "end of processing file:" << outputFile << endl;
    return 0;
}
