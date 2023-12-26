//
// Created by 24115 on 2023/12/26.
//

#ifndef CODE_STAFFLOG_H
#define CODE_STAFFLOG_H
#include <fstream>
#include <queue>
#include <iostream>
#include <iomanip>
#include <vector>
#include "blockchain.h"
#include "user.h"

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
struct loguser
{
    char id[35]{};
    char act[35]{};
    char obj[100]{};
    char obj2[100]{};
    char obj3[100]{};
};

class Userlog {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(loguser);
    long long number=0;//记录数量
public:
    Userlog()
    {
        initialise("stafflog");
    }

    void initialise(string FN = "")
    {
        file_name = FN;
        file.open( file_name);
        if(!file.good())
        {
            file.open( file_name,std::ios::out);
            file.close();
            file.open( file_name);
            file.write(reinterpret_cast<char*>(&number), sizeof(long long));
        }
        file.close();
    }

    //读出首个块
    void get_info(long long &tmp) {
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(long long));
        file.close();
    }

    //将新的索引写入首个块中
    void write_info(long long tmp) {
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(long long));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    void write(loguser &t) {
        file.open(file_name, std::ios::app|std::ios::binary);
        int index = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
//        std::cout<<"写入位置为："<<index<<'\n';
        long long tmp;
        get_info(tmp);
        tmp++;
        write_info(tmp);
//        info xx;
//        get_info(xx);
//        std::cout<<"读出位置为："<<xx.last<<'\n';
    }

    void show()
    {
        long long tmp;
        get_info(tmp);
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(sizeof(long long ),std::ios::beg);
        for(long long i=0;i<tmp;i++)
        {
            loguser t;
            file.read(reinterpret_cast<char *>(&t),sizeofT);
            std::cout<<t.id<<" "<<t.act<<" "<<t.obj<<" "<<t.obj2<<" "<<t.obj3<<"\n";
        }
        file.close();
    }
};


#endif //CODE_STAFFLOG_H
