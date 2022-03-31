/**
 *  Function
 *
 *  程序运行时一般都需要用户从命令行输入一些不同类型的参数，有些参数后面还需要传值，
 *  如输入和输出文件的路径名等
 *  每个命令行参数应具备以下3个属性：是否必需，是否需要传值，是否可重复
 *  为此我们设计OptionAttribute结构体，其中包括三个bool成员变量：
 *  requiredOpt，requiredArg，repeatableOpt分别对应参数的3个属性
 *  此外解析命令行参数还需要用到两个关键map型数据结构：
 *  map<string, OptionAttribute> optionAttMap 和 map<string, vector<string>> optionValueData
 *  optionAttMap中的第一项string即为当前程序的参数名称，第二项为该参数的属性
 *  optionValueData中的第一项string也是参数名称，第二项为该参数的传值
 *  因此只有当optionAttMap中某参数的属性requiredArg为真时，才会往optionValueData插入数据
 *
 *  Author
 *      zhang shoujian
 *
 *  Contact
 *      shjzhang@sgg.whu.edu.cn
 *
 */
#ifndef OptionAttribute_HPP
#define OptionAttribute_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace utilSpace
{
    struct OptionAttribute
    {
        OptionAttribute()
            : requiredArg(false), repeatableOpt(false)
        {};

        OptionAttribute(bool reqArg,bool repeatOpt)
        {
            requiredArg = reqArg;
            repeatableOpt = repeatOpt;
        };

        OptionAttribute(const OptionAttribute& _OptAtt)
        {
            requiredArg = _OptAtt.requiredArg;
            repeatableOpt = _OptAtt.repeatableOpt;
        };

        ~OptionAttribute() {};

        bool requiredArg;
        bool repeatableOpt;
    };

    typedef map<string, OptionAttribute> OptionAttMap;
    typedef map<string, vector<string>> OptionValueMap;

    void parseOption(int argc,
                     char* argv[],
                     OptionAttMap& optionAttData,
                     OptionValueMap& optionValueData, 
                     string& helpInfo)
    {
        if(argc==1)
        {
            cout << helpInfo << endl;
            exit(-1);
        }

        int iarg = 1;
        while(iarg < argc)
        {
            string optionStr = argv[iarg];

            if(optionStr == "--help")
            {
                cout << helpInfo << endl;
            }

            OptionAttribute currOptAttribute;
            if(optionAttData.find(optionStr)!=optionAttData.end())
            {
                currOptAttribute = optionAttData[optionStr];
            }
            else
            {
                cerr << "current optionStr: " 
                     << optionStr 
                     << " is not defined!" << endl;
                cout << "  use --help to get more information"<<endl;
                exit(-1);
            }

            string valueStr;
            if(currOptAttribute.requiredArg)
            {
                string valueStr = argv[++iarg];
                optionValueData[optionStr].push_back(valueStr);
            }
            else
            {
                optionValueData[optionStr].push_back("");
            }

            /// according input value, check the option/value pair for correct reading
            if(!currOptAttribute.repeatableOpt && 
                optionValueData[optionStr].size()>1)
            {
                cerr << "current option:" << optionStr 
                     << " Multiple parameter values are not supported!" << endl;
                cout << "  use --help to get more information"<<endl;
                exit(-1);
            }

            iarg++;
        }
    }
}

#endif
