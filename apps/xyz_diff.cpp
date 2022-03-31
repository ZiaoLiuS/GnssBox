/**
 * compute the difference between user-xyz and reference xyz
 * 
 * Copyright(C)
 * shoujian zhang
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "YDSTime.hpp"
#include "StringUtils.hpp"
#include "Triple.hpp"

// option-handling
#include "OptionUtil.hpp"
#include "NEUUtil.hpp"
#include "StringUtils.hpp"

using namespace std;
using namespace utilSpace;
using namespace timeSpace;
using namespace mathSpace;
using namespace coordSpace;
using namespace gnssSpace;

#define debug 1

int main(int argc, char **argv)
{
    // Add help information

    // Usage information
    string helpInfo
        = 
    "Usage: \n"
    " \n"
    " compute the difference between xyz-data from xyzFile and the reference xyz\n"
    " data from command-line or refXYZFile. the reference xyz for IGS sites can be \n"
    " obtained from IGS SINEX file. \n"
    " \n"
    " xyz-file format is as follows:\n"
    " year doy sod x y z vx vy vz\n"
    " \n"
    "required options:\n"
    " --xyzFile <xyzFile_name>          file storing the xyz data\n"
    " --xCol    <xCol>                  column for x\n"
    " --yCol    <yCol>                  column for y\n"
    " --zCol    <zCol>                  column for z\n"
    " \n"
    "optional options:\n"
    " --refXYZ <refXYZ>                 static IGS xyz solution from sinex file\n"
    " --refXYZFile <reffile_name>       file storing reference xyz data for kinematic data\n"
    " --outputFile<outputFile_name>     output difference between xyz and refXYZ\n"
    " --help                            Prints this help \n"
    " \n"
    "wanring: \n"
    "   xyzFile and refXYZFile must have the same fromat! \n"
    " \n"
    "Copyright(c)\n"
    "   shoujian zhang, 2019-2022, School of Geodesy and Geomatics, Wuhan University\n";

    // map for attribute/value data
    OptionAttMap optAttData;
    OptionValueMap optValData;

    // define option attribute for options
    OptionAttribute xyzFileAttribute(1, 0);
    OptionAttribute xColAttribute(1, 0);
    OptionAttribute yColAttribute(1, 0);
    OptionAttribute zColAttribute(1, 0);
    OptionAttribute refXYZAttribute(1, 0);
    OptionAttribute refXYZFileAttribute(1, 0);
    OptionAttribute outputFileAttribute(1, 0);
    OptionAttribute helpAttribute(0, 0);

    /// define and insert
    optAttData["--xyzFile"]    = xyzFileAttribute;
    optAttData["--xCol"]       = xColAttribute;
    optAttData["--yCol"]       = yColAttribute;
    optAttData["--zCol"]       = zColAttribute;
    optAttData["--refXYZ"]     = refXYZAttribute;
    optAttData["--refXYZFile"] = refXYZFileAttribute;
    optAttData["--outputFile"] = outputFileAttribute;
    optAttData["--help"]       = helpAttribute;

    /// prase the options
    parseOption(argc,argv,optAttData,optValData,helpInfo);

    // Parse required options

    ///--xyzFile
    string xyzFile;
    if(optValData.find("--xyzFile")!=optValData.end())
    {
        xyzFile = optValData["--xyzFile"][0];
    }
    else
    {
        cerr << "--xyzFile is required!" << endl;
        exit(-1);
    }

    string xCol;
    if(optValData.find("--xCol")!=optValData.end())
    {
        xCol = optValData["--xCol"][0];
    }
    else
    {
        cerr << "--xCol is required!" << endl;
        exit(-1);
    }

    string yCol;
    if(optValData.find("--yCol")!=optValData.end())
    {
        yCol = optValData["--yCol"][0];
    }
    else
    {
        cerr << "--yCol is required!" << endl;
        exit(-1);
    }

    string zCol;
    if(optValData.find("--zCol")!=optValData.end())
    {
        zCol = optValData["--zCol"][0];
    }
    else
    {
        cerr << "--zCol is required!" << endl;
        exit(-1);
    }

    ///--refXYZFile
    bool hasRefXYZFile(false);
    string refXYZFile;
    if(optValData.find("--refXYZFile")!=optValData.end())
    {
        refXYZFile = optValData["--refXYZFile"][0];
        hasRefXYZFile = true;
    }

    // --erpFile
    bool hasRefXYZ(false);
    string refXYZ;

    if(optValData.find("--refXYZ")!=optValData.end())
    {
        refXYZ = optValData["--refXYZ"][0];
        hasRefXYZ = true;
    }

    ///--outputFile
    string outputFile;
    if(optValData.find("--outputFile")!=optValData.end())
    {
        outputFile = optValData["--outputFile"][0];
    }
    else
    {
        outputFile = xyzFile + ".diff";
    }

    if( (!hasRefXYZ) && (!hasRefXYZFile) )
    {
        cerr << "either refXYZ or refXYZFile MUST be given." << endl;
        exit(-1);
    }

    Triple refXYZVec;
    if(hasRefXYZ)
    {
        vector<string> splitVec;
        splitVec = split(refXYZ, " ");
        double xref = asDouble(splitVec[0]);
        double yref = asDouble(splitVec[1]);
        double zref = asDouble(splitVec[2]);

        refXYZVec = Triple(xref, yref, zref);
    }

    //
    // open input file
    //
    std::fstream xyzStream(xyzFile.c_str(), ios::in);
    if(!xyzStream)
    {
        cout << "error open xyzFile" << endl;
        exit(-1);
    }

    std::fstream outStream(outputFile.c_str(), ios::out);
    if(!outStream)
    {
        cout << "error open outputFile" << endl;
        exit(-1);
    }

    std::fstream refXYZStream;
    if(hasRefXYZFile)
    {
        refXYZStream.open(refXYZFile.c_str(), ios::in);
        if(!refXYZStream)
        {
            cout << "error open refXYZFile" << endl;
            exit(-1);
        }
    }

    ///////////// read refXYZFile/////////////////////

    NEUUtil neuConvert;
    std::map<YDSTime,Triple> epochRefXYZData;
    string line;

    // 如果给定了先验的参考坐标文件，读取
    if(hasRefXYZFile)
    {
        while(true)
        {
            getline(refXYZStream, line);
            if(refXYZStream.eof())
            {
                break;
            }

            stripTrailing(line);

            if(debug)
                cout << line << endl;

            // skip comments
            if(line[0] == '#') continue;

            // read XCol/YCol/ZCol
            if(numWords(line) < 6)
            {
                cerr << "xyzFile has at least 6 columns" << endl;
                exit(-1);
            };

            std::vector<string> record;
            record = split(line, " ");

            int year   = asInt(record[0]);
            int doy    = asInt(record[1]);
            double sod = asDouble(record[2]);

            YDSTime yds = YDSTime(year, doy, sod);

            double xref = asDouble(record[asInt(xCol) -1 ]);
            double yref = asDouble(record[asInt(yCol) -1 ]);
            double zref = asDouble(record[asInt(zCol) -1 ]);

            Triple xyzref(xref, yref, zref);

            epochRefXYZData[yds] = xyzref;

        }
    }

    //------------------------------------
    // read data line by line
    //------------------------------------

    int year, doy;
    double sod;
    double xecef, yecef, zecef;

    int numRec(0);
    while(true) 
    {
        getline(xyzStream, line);
        if(xyzStream.eof())
        {
            break;
        }

        stripTrailing(line);

        if(debug)
            cout << line << endl;

        // skip comments
        if(line[0] == '#') continue;

        // read XCol/YCol/ZCol
        if(numWords(line) < 6) 
        {
            cerr << "xyzFile has at least 6 columns" << endl;
            exit(-1);
        }; 

        vector<string> record;
        record = split(line, " ");

        year  = asInt(record[0]);
        doy   = asInt(record[1]);
        sod   = asInt(record[2]);

        YDSTime yds(year, doy, sod);

        xecef = asDouble(record[asInt(xCol) -1]);
        yecef = asDouble(record[asInt(yCol) -1]);
        zecef = asDouble(record[asInt(zCol) -1]);

        Triple xyzECEF(xecef, yecef, zecef);

        if(hasRefXYZFile)
        {
            if(epochRefXYZData.find(yds) != epochRefXYZData.end())
            {
                refXYZVec = epochRefXYZData[yds];

                if(debug)
                {
                    cout << "xyzECEF:" << xyzECEF << endl;
                    cout << "refXYZVec:" << refXYZVec << endl;
                }

                Triple dxyz = xyzECEF - refXYZVec;

                if(debug)
                {
                    cout << "dxyz:" << dxyz << endl;
                }


                neuConvert.setRefPosition(refXYZVec);
                Triple dneu = neuConvert.convertToNEU(dxyz);

                outStream 
                    << year << " " 
                    << doy  << " " 
                    << setw(14) 
                    << setiosflags(ios::fixed)
                    << setprecision(3)  
                    << sod  << " " 
                    << setw(10)
                    << setprecision(3)
                    << dxyz[0] << " " 
                    << setw(10)
                    << dxyz[1] << " " 
                    << setw(10)
                    << dxyz[2] << " " 
                    << setw(10)
                    << dneu[0] << " " 
                    << setw(10)
                    << dneu[1] << " " 
                    << setw(10)
                    << dneu[2] << " " 
                    << endl;
            }
        }
        else
        {
                Triple dxyz = xyzECEF - refXYZVec;

                if(debug)
                {
                    cout << "dxyz:" << dxyz << endl;
                }


                neuConvert.setRefPosition(refXYZVec);
                Triple dneu = neuConvert.convertToNEU(dxyz);

                outStream 
                    << year << " " 
                    << doy  << " " 
                    << setw(14) 
                    << setiosflags(ios::fixed)
                    << setprecision(3)  
                    << sod  << " " 
                    << setw(10)
                    << setprecision(3)
                    << dxyz[0] << " " 
                    << setw(10)
                    << dxyz[1] << " " 
                    << setw(10)
                    << dxyz[2] << " " 
                    << setw(10)
                    << dneu[0] << " " 
                    << setw(10)
                    << dneu[1] << " " 
                    << setw(10)
                    << dneu[2] << " " 
                    << endl;
        }


        numRec++;
    }

    if(numRec==0)
    {
        cerr << "Abort:no data.";
        cerr << endl;
        return -3;
    }

    xyzStream.close();
    outStream.close();
    refXYZStream.close();

    return 0;

}   // end main()
