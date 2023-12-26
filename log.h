//
// Created by 24115 on 2023/12/26.
//

#ifndef CODE_LOG_H
#define CODE_LOG_H
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
struct loginf
{
    int mode=0;//1为人员操作，2为交易记录
    char id[35]{};
    char act[35]{};
    char obj[100]{};
    char obj2[100]{};
    char obj3[100]{};

    char isbn[25]{};
    int number=0;
    double money=0;
};


class Log {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(loginf);
    long long number=0;//记录数量
public:
    Log()
    {
        initialise("log");
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
    void write(loginf &t) {
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
//        std::cout<<"oldplace："<<xx.last<<'\n';
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
//        std::cout<<"place: "<<tmp.last<<'\n';
        file.seekg(sizeof(long long ),std::ios::beg);
        for(long long i=0;i<tmp;i++)
        {
            loginf t;
            file.read(reinterpret_cast<char *>(&t),sizeofT);
            if(t.mode == 1)
            {
                std::cout << t.id << " " << t.act << " " << t.obj << " " << t.obj2 << " " << t.obj3 << "\n";
            }
            if(t.mode == 2)
            {
                std::cout<<"book: "<<t.isbn;
                if(t.money>0)
                {
                    std::cout<<" -"<<t.number<<" +"<<t.money<<"dollar\n";
                }
                else
                {
                    std::cout<<" +"<<t.number<<" -"<<-t.money<<"dollar\n";
                }
            }
        }
        file.close();
    }
};
#endif //CODE_LOG_H
