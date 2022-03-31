///////////////////////////////////////////////////////////////////////////////////
//  Function:
//
//  test the get and parse option function
//  you can search the define of OptionAttribute/optionAttData/optionValueData
//  in optutility.hpp
//
//  Author
//  
//  shjzhang, shjzhang@sgg.whu.edu.cn, wuhan university, 2022.1.5
//
//
///////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "OptionUtil.hpp"

#define debug 1

using namespace std;
using namespace utilSpace;

int main(int argc,char* argv[])
{
    string helpInfo
        =
    "Usage:\n"
    " \n"
    "required options:\n"
    "    --input <input_file_path>    input file, repeatable\n"
    "    --output <output_file_name>  output file, unreaptable\n"
    "optional options:\n"
    "    --help                       Prints this help\n"
    " \n"
    "Examples:\n"
    "getopt_test --input ./test.txt --input ./test01.txt --output ./result.txt \n"
    " \n"
    "Author:\n"
    "   written by zhang shoujian & wang yishi, 2022\n"
    "   email: shjzhang@sgg.whu.edu.cn ";


    OptionAttMap optionAttData;

    ///initialization
    OptionAttribute inputAttribute(1, 1);
    OptionAttribute outputAttribute(1, 0);
    OptionAttribute helpAttribute(0, 0);

    OptionValueMap optionValueData;

    /// define and insert
    optionAttData["--input"] = inputAttribute;
    optionAttData["--output"] = outputAttribute;
    optionAttData["--help"] = helpAttribute;

    ///prase the options
    parseOption(argc,argv,optionAttData,optionValueData,helpInfo);

    vector<string> fileData;
    for(auto it :optionValueData["--input"])
    {
        if(it.size()!=0)
        {
            fstream inFileStrm(it.c_str(),ios::in);
            while(inFileStrm.peek()!=EOF)
            {
                string str="";
                getline(inFileStrm,str);
                fileData.push_back(str);
            }
            inFileStrm.close();
        }
    }

    ///test whether the read function is succeed
    if(debug)
    {
        for(auto it = fileData.begin();it!=fileData.end();++it)
        {
            if(debug)
                cout<< "file:" <<(*it)<<endl;
        }
    }

    for(auto it : optionValueData["--output"])
    {
        if(it.size()!=0)
        {
            fstream outFileStrm(it.c_str(), ios::out);
            for(auto data:fileData)
            {
                outFileStrm<<data<<endl;
            }
            outFileStrm.close();
        }
    }

    return 0;
}

