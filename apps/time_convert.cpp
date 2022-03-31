/**
 *  Function:
 *  test the timecvt function with optUtility.hpp
 */

#include <string>
#include <iostream>
#include <sstream>

#include "OptionUtil.hpp"
#include "ConvertTime.hpp"
#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"
#include "JulianDate.hpp"
#include "MJD.hpp"
#include "TimeSystem.hpp"

#include "YDSTime.hpp"
#include "SystemTime.hpp"
#include "StringUtils.hpp"

using namespace std;
using namespace timeSpace;
using namespace utilSpace;
using namespace gnssSpace;

#define debug 0

int main(int argc, char *argv[])
{
    string helpInfo=
        "usage: \n"
        "\n"
        "Converts from a given input time specification to other time formats.\n"
        "Include the quotation marks.\n"
        "All year values are four digit years.\n"
        "we only support mjd|jd|civil|ws|YDS.\n"
        "\n"
        "options:\n"
        " --mjd         <ModifiedJulianDate>                               \n"
        " --jd          <JulianDate>                                       \n"
        " --civil       <Year Month(numeric) DayOfMonth Hour:Minute:Second>\n"
        " --ws          <FullWeek SecondOfWeek>                         \n"
        " --YDS         <Year DayOfYear SecondsOfDay>                      \n"
        "\n"
        "optional options:\n"
        " --sys         <Time Systems>\n"
        " --toSys       <GPS|BDT|GAL|GLO|UTC|TAI|TT|TDB>\n"
        " --toCalendar  <mjd|jd|civil|ws|YDS>\n"
        " --help        print this help\n"
        "\n"
        "examples:\n"
        "\n"
        "   time_convert --jd \"2459592.5\" --toCalendar civil --sys GPS --toSys BDT\n"
        "   time_convert --YDS \"2022 1 43200.13456\" --sys GPS --toSys BDT \n"
        "   time_convert --ws \"1356 14\" --sys GPS --toSys BDT \n"
        "\n"
        "Author:\n"
        "\n"
        "   written by zhang shoujian & wang yishi, 2022\n"
        "   email: shjzhang@sgg.whu.edu.cn\n";

    ///initialization
    OptionAttribute mjdAttribute(1, 0);
    OptionAttribute jdAttribute(1, 0);
    OptionAttribute civilAttribute(1, 0);
    OptionAttribute wsAttribute(1, 0);
    OptionAttribute YDSAttribute(1, 0);
    OptionAttribute sysAttribute(1, 0);
    OptionAttribute toFormatAttribute(1, 0);
    OptionAttribute toCalendarAttribute(1, 0);
    OptionAttribute toSysAttribute(1, 0);
    OptionAttribute helpAttribute(0, 0);

    OptionAttMap optAttData;
    OptionValueMap optValData;

    /// define and insert
    optAttData["--mjd"]        = mjdAttribute;
    optAttData["--jd"]         = jdAttribute;
    optAttData["--civil"]      = civilAttribute;
    optAttData["--ws"]         = wsAttribute;
    optAttData["--YDS"]        = YDSAttribute;
    optAttData["--sys"]        = sysAttribute;
    optAttData["--toSys"]      = toSysAttribute;
    optAttData["--toCalendar"] = toCalendarAttribute;
    optAttData["--toFormat"]   = toFormatAttribute;
    optAttData["--help"]       = helpAttribute;

    ///prase the options
    parseOption(argc,argv,optAttData,optValData,helpInfo);

    CommonTime ct;
    string inCalendar;
    ///====================================>   --mJulian
    if(optValData.find("--mjd")!=optValData.end())
    {
        ///initial
        inCalendar = "mjd";
        string mJulian_day = optValData["--mjd"][0];
        long double mjd = stold(mJulian_day);
        MJD tt(mjd);
        ct = tt.convertToCommonTime();
    }

    ///====================================>   --julian
    if(optValData.find("--jd")!=optValData.end())
    {
        ///initial
        inCalendar = "jd";
        std::string julian_day = optValData["--jd"][0];
        long double jd = stold(julian_day);
        JulianDate tt(jd);
        ct = tt.convertToCommonTime();
    }

    ///====================================>   --YDS
    if(optValData.find("--YDS")!=optValData.end())
    {
        ///initial
        inCalendar = "YDS";
        std::string YDS_day = optValData["--YDS"][0];
        long year, doy;
        double sod;
        stringstream sstr(YDS_day);
        sstr>>year>>doy>>sod;
        YDSTime tt(year,doy,sod);
        ct = tt.convertToCommonTime();
    }

    ///====================================>   --civil
    if(optValData.find("--civil")!=optValData.end())
    {
        ///initial
        inCalendar = "civil";
        std::string civil_day = optValData["--civil"][0];
        int year, month, day, hour, min;
        double sec;
        std::vector<string> strVec = split(civil_day, " :");
        if(debug)
        {
            for(int i=0; i< strVec.size();i++)
            {
                cout << strVec[i] << " " ;
            }
        }

        if(strVec.size() !=6)
        {
            cerr << "input civil time should have 6 characters, e.g:"<< endl;
            cerr << "2021 10 8 00:00:00"<< endl;
            exit(-1);
        }

        year  = stoi(strVec[0]);
        month = stoi(strVec[1]);
        day   = stoi(strVec[2]);
        hour  = stoi(strVec[3]);
        min   = stoi(strVec[4]);
        sec   = stod(strVec[5]);

        CivilTime tt(year,month,day,hour,min,sec);
        ct = tt.convertToCommonTime();
    }

    ///--ews/ --ws is the same strategy in TimeString::scanTime function
    ///====================================>   --ws
    if(optValData.find("--ws")!=optValData.end())
    {
        ///initial
        inCalendar = "ws";
        std::string ws_day = optValData["--ws"][0];
        if(debug)
        {
            cout << "ws_day:" << ws_day << endl;
        }

        double week;
        double sow;
        stringstream sstr(ws_day);
        if(debug)
        {
            cout << sstr.str() << endl;
        }

        sstr>>week;
        sstr>>sow;
        if(debug)
        {
            cout << "week:" << week << endl;
            cout << "sow:" << sow << endl;
        }

        GPSWeekSecond tt(week,sow);
        ct = tt.convertToCommonTime();
        if(debug)
        {
            cout << "YDS:" << YDSTime(ct) << endl;
        }
    }

    ///set time system from the input parameter
    TimeSystem inSys;
    if(optValData.find("--sys")!=optValData.end())
    {
        string inSystem = optValData.at("--sys")[0];
        inSys.fromString(inSystem);
    }
    else
    {
        inSys = TimeSystem::GPS;
    }

    ct.setTimeSystem(inSys);

    ///before convert calendar and time system according to the parameters
    ///we should get toSys and toCalendar at first
    ///if there is no toSys or toCalendar parameters,
    ///then we keep them as the input
    CommonTime outCt = ct;

    TimeSystem outSys;
    if(optValData.find("--toSys")!=optValData.end())
    {
        string toSys = optValData.at("--toSys")[0];
        outSys.fromString(toSys);
        outCt = convertTimeSystem(ct,outSys);
    }

    bool hasOutCalender(false);
    string outCalendar;
    if(optValData.find("--toCalendar")!=optValData.end())
    {
        outCalendar = optValData.at("--toCalendar")[0];
        hasOutCalender = true;
    }


    if( !hasOutCalender )
    {
        CivilTime cvt(outCt);
        TimeSystem sys1 = cvt.getTimeSystem();
        cout<< setiosflags(ios::fixed);
        cout<<"CivilTime: "<<cvt.year<<" "<<cvt.month<<" "
            <<cvt.day<<" "<<cvt.hour<<":"<<cvt.minute<<":"
            << setprecision(6)
            <<cvt.second<<"  ("<<sys1.asString()<<")"<<endl;

        GPSWeekSecond wst(outCt);
        TimeSystem sys2 = wst.getTimeSystem();
        cout<< setiosflags(ios::fixed);
        cout<<"weekSecond: "<<wst.week<<" "
            << setprecision(6)
            << wst.sow
            <<"  ("<<sys2.asString()<<")"<<endl;

        YDSTime yds(outCt);
        TimeSystem sys3 = yds.getTimeSystem();
        cout<< setiosflags(ios::fixed);
        cout<<"YDSTime: "<<yds.year<<" "<<yds.doy<<" "
            << setprecision(6)
            <<yds.sod<<"  ("<<sys3.asString()<<")"<<endl;

        MJD mjd(outCt);
        TimeSystem sys4 = mjd.getTimeSystem();
        cout<< setiosflags(ios::fixed);
        cout<<"Modified Julian Date: "<<mjd.mjd
            <<"  ("<<sys4.asString()<<")"<<endl;

    }

    if( hasOutCalender )
    {
        ///convert calendar
        if(outCalendar == "mjd")
        {
            MJD tt(outCt);
            TimeSystem sys = tt.getTimeSystem();
            cout<< setiosflags(ios::fixed);
            cout<<"Modified Julian Date: "<<tt.mjd
                <<"  ("<<sys.asString()<<")"<<endl;
        }
        else if(outCalendar == "jd")
        {
            JulianDate tt(outCt);
            TimeSystem sys = tt.getTimeSystem();
            cout<< setiosflags(ios::fixed);
            cout<<"Julian Date: "<<tt.jd
                <<"  ("<<sys.asString()<<")"<<endl;
        }
        else if(outCalendar == "civil")
        {
            CivilTime tt(outCt);
            TimeSystem sys = tt.getTimeSystem();
            cout<< setiosflags(ios::fixed);
            cout<<"CivilTime: "<<tt.year<<" "<<tt.month<<" "
                <<tt.day<<" "<<tt.hour<<":"<<tt.minute<<":"
                << setprecision(6)
                <<tt.second<<"  ("<<sys.asString()<<")"<<endl;
        }
        else if(outCalendar == "ws")
        {
            GPSWeekSecond tt(outCt);
            TimeSystem sys = tt.getTimeSystem();
            cout<< setiosflags(ios::fixed);
            cout<<"weekSecond: "<<tt.week<<" "
                << setprecision(6)
                << tt.sow
                <<"  ("<<sys.asString()<<")"<<endl;
        }
        else if(outCalendar == "YDS")
        {
            YDSTime tt(outCt);
            TimeSystem sys = tt.getTimeSystem();
            cout<< setiosflags(ios::fixed);
            cout<<"YDSTime: "<<tt.year<<" "<<tt.doy<<" "
                << setprecision(6)
                <<tt.sod<<"  ("<<sys.asString()<<")"<<endl;
        }
    }

    return 0;
}
