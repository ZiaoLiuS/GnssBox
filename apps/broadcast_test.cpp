/**
 *  Function:
 *  test the Rx3NavStore class
 *
 */

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include "OptionUtil.hpp"
#include "SP3EphStore.hpp"
#include "Rx3NavStore.hpp"

#define debug 0

using namespace std;
using namespace gnssSpace;
using namespace utilSpace;

int main(int argc,char* argv[])
{
    string helpInfo
        =
    "Usage:\n"
    " \n"
    "required options:\n"
    "    --nav <input_nav_file_path>  input nav file, unrepeatable\n"
    "    --sp3 <input_sp3_file_path>  input sp3 file, unreaptable\n"
    "    --output <output_file_name>  output file, unreaptable\n"
    "optional options:\n"
     "    --help                       Prints this help\n"
    " \n"
    "Examples:\n"
    "broadcast_test  --nav ***.rnx  --sp3 ***.SP3  --output ./result.txt \n"
    " \n"
    "Author:\n"
    "   written by zhang shoujian & wang yishi, 2022\n"
    "   email: shjzhang@sgg.whu.edu.cn ";

    ///initialization
    OptionAttribute navAttribute(1, 0);
    OptionAttribute sp3Attribute(1, 0);
    OptionAttribute outputAttribute(1, 0);
    OptionAttribute helpAttribute(0, 0);

    OptionAttMap optAttData;
    OptionValueMap optValData;

    /// define and insert
    optAttData["--nav"] = navAttribute;
    optAttData["--sp3"] = sp3Attribute;
    optAttData["--output"] = outputAttribute;
    optAttData["--help"] = helpAttribute;

    ///prase the options
    parseOption(argc,argv,optAttData,optValData,helpInfo);

    Rx3NavStore navStore;
    navStore.loadFile(optValData["--nav"][0]);

    SP3EphStore sp3Store;
    sp3Store.loadSP3File(optValData["--sp3"][0]);

    fstream outFileStrm(optValData["--output"][0].c_str(), ios::out);

    // satList
    std::vector<SatID> satVec;
    satVec = sp3Store.getSatList();

    for(int i=0; i<satVec.size(); i++)
    {
        cout << satVec[i] << "";
    }
    cout << endl;

    CommonTime firstTime = sp3Store.getInitialTime();
    CommonTime finalTime = sp3Store.getFinalTime();

    double interval = 3000.0;

    CommonTime time = firstTime;

    while(time <= finalTime )
    {
        time += interval;
        try
        {
            for (auto it = satVec.begin(); it != satVec.end(); it++)
            {
                Xvt xvtNav = navStore.getXvt((*it), time);
                Xvt xvtSP3 = sp3Store.getXvt((*it), time);

                Triple xNav = xvtNav.getPos();
                Triple xSP3 = xvtSP3.getPos();

                Triple diffXYZ = xNav - xSP3;

                double clockBiasNav = xvtNav.getClockBias()*C_MPS;
                double clockBiasSP3 = xvtSP3.getClockBias()*C_MPS;

                double diffClock = clockBiasNav - clockBiasSP3;

                YDSTime yds(time);
                outFileStrm << yds << " "
                            << (*it) << " "
                            << diffXYZ << " "
                            << diffClock <<  " "
                            << xvtNav << " "
                            << xvtSP3 << " "
                            << endl;
            }
        }
        catch(InvalidRequest& e)
        {
            continue;
        }
    }
    outFileStrm.close();

    return 0;
}
