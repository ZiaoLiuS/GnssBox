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

#define debug 1

using namespace std;
using namespace gnssSpace;
using namespace utilSpace;

int main(int argc,char* argv[]) 
{
    string helpInfo
       =
    "Usage: \n"
    "  example showing the usage of gnss obs data structures! \n"
    "\n"
    "required options:\n"
    "  --obsFile <fileName>          rinex file name \n"
    "\n"
    "optional options:\n"
    "  --help                        Prints this help \n"
    "\n"
    "Examples: "
    "   \n"
    "\n"
    "\n"
    "Copyright(c)\n"
    "  shoujian zhang, 2022, Wuhan University\n";

    // map for attribute/value data
    OptionAttMap optAttData;
    OptionValueMap optValData;

    // define option attribute for options
    OptionAttribute obsAttribute(1, 0);
    OptionAttribute helpAttribute(0, 0);

    /// define and insert
    optAttData["--obsFile"] = obsAttribute;
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

    // set the header pointer to rxData for data reading.
    rxData.pHeader = &rxHeader;

    // now, let's process gnss data for curret station
    while (true)
    {
        double clock1(Counter::now());

        // read data
        try
        {
            rxData.readRecord(rxStream);
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
    }

    // close streams
    rxStream.close();

    cout << "end of processing file:" << outputFile << endl;

    return 0;
}
