/**
 *  Function:
 *  test the getopt_long() function
 *  getopt函数用于解析短格式选项。短格式选项即一个字符的命令行选项，
 *  例如：ls -l
 *  而getopt_long函数既可以解析短格式选项，也可以解析长格式选项。
 *  长格式选项例如:ls --all
 *  通过使用这两个函数，就可以从字符串中识别出娜个是选项，哪个是该选项的参数，
 *  而选项输入的参数会被保存在optarg里,就可以进一步在代码中进行处理啦！
 *  可以利用这两个函数实现用户通过命令行对应用程序进行配置的功能
 */

//unistd.h为Linux/Unix系统中内置头文件，包含了许多系统服务的函数原型，例如read函数、write函数和getpid函数等。
//其作用相当于windows操作系统的"windows.h"，是操作系统为用户提供的统一API接口，方便调用系统提供的一些服务。
#include<unistd.h>  
#include<getopt.h>
#include<iostream>
#include<string>
#include<fstream>
#include<vector>

#define debug 1

using namespace std;

int main(int argc,char* argv[])
{
    char arg;
    char shortOptStr[]="hf:o:";

    vector<string> testFile;
    string outFile;

    struct option longopts[]={
            {"help",no_argument, NULL,'h'},
            {"file",required_argument,NULL,'f'},
            {"output",required_argument,NULL,'o'},
            {0,0,0,0},
    };

    string helpInfo="Usage:\n"
                    " \n"
                    "required options:\n"
                    "    --input <input_rinex_file_path>  input rinex file, repeatable\n"
                    "optional options:\n"
                    "    --output <output_rinex_file_name>  output rinex file\n"
                    "    --help                             Prints this help\n"
                    " \n"
                    "Examples:\n"
                    "getopt_test --file ./test.txt --output ./result.txt \n"
                    " \n"
                    "Author:\n"
                    "   written by zhang shoujian & wang yishi, 2022\n"
                    "   email: sjzhang@sgg.whu.edu.cn ";

    while((arg=getopt_long(argc,argv,shortOptStr,longopts,NULL))!=-1)
    {
        switch(arg)
        {
            case 'h':
                if(debug)
                cout<<helpInfo<<endl;
                break;
            case 'f':
                if(debug)
                cout<<"The file path is: "<<optarg<<endl;
                testFile.push_back(optarg);
                break;
            case 'o':
                if(debug)
                cout<<"The output file path is: "<<optarg<<endl;
                outFile = optarg;
                break;
            default:
                if(debug)
                cout<<"Invalid argument!"<<endl;
                return -1;
        }
    }

    vector<string> fileData;
    for(auto it =testFile.begin();it!=testFile.end();++it)
    {
        if(debug)
        cout<<(*it)<<endl;

        if(testFile.size()!=0)
        {
            fstream testFileStrm((*it).c_str(),ios::in);

            /*while(true)
            {
                string str="";
                getline(testFileStrm,str);
                if(testFileStrm.eof()==true) break;
                FileData.push_back(str);
            }*/

            while(testFileStrm.peek()!=EOF)
            {
                string str="";
                getline(testFileStrm,str);
                fileData.push_back(str);
            }
            testFileStrm.close();
        }
    }

    ///test whether the read function is succeed
    for(auto it = fileData.begin();it!=fileData.end();++it)
    {
        if(debug)
        cout<<(*it)<<endl;
    }


    if(outFile.size()!=0)
    {
        fstream outStrm(outFile.c_str(), ios::out);
        for(auto data:fileData)
        {
            outStrm<<data<<endl;
        }
        outStrm.close();
    }
    return 0;
}














