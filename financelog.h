//
// Created by 24115 on 2023/12/24.
//

#ifndef CODE_FINANCELOG_H
#define CODE_FINANCELOG_H
#include <fstream>
#include <queue>
#include <iostream>
#include <iomanip>
#include "user.h"
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

struct info
{
    long long count=0;
    long long last=0;
};

template<class T>
class Financelog {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    Financelog()
    {
        initialise("financelog");
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
            info tmp;
            file.write(reinterpret_cast<char*>(&tmp), sizeof(info));
        }
        file.close();
    }

    //读出首个块
    void get_info(info &tmp) {
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(info));
        file.close();
    }

    //将新的索引写入首个块中
    void write_info(info tmp) {
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(info));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    void write(T &t) {
//        std::cout<<"数目为："<<t<<'\n';

        file.open(file_name, std::ios::app|std::ios::binary);
        int index = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
//        std::cout<<"写入位置为："<<index<<'\n';
        info tmp;
        get_info(tmp);
        tmp.count++;
        tmp.last=index;
        write_info(tmp);
//        info xx;
//        get_info(xx);
//        std::cout<<"读出位置为："<<xx.last<<'\n';
    }

    void show()
    {
        info tmp;
        get_info(tmp);
//        std::cout<<"oldplace："<<xx.last<<'\n';

        if(stack.empty())
        {
            throw Error();
        }
        User nowuser=stack.back();
        if(nowuser.Privilege<7)
        {
            throw Error();
        }
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        T add=0;
        T minus=0;
//        std::cout<<"place: "<<tmp.last<<'\n';
        file.seekg(tmp.last,std::ios::beg);
        for(long long i=tmp.count;i>0;i--)
        {
            T t;
            file.read(reinterpret_cast<char *>(&t),sizeofT);
            if(t>0)
            {
                add+=t;
            }
            else
            {
                minus-=t;
            }
            file.seekg(-2*sizeofT,std::ios::cur);//因为已经读了一次，要退回两格
//            std::cout<<"计算得到："<<t<<'\n';
        }
        std::cout<<"+ "<<std::fixed<<std::setprecision(2)<<add<<" - "<<minus<<"\n";
        file.close();
    }

    void show(long long num)
    {
        info tmp;
        get_info(tmp);
        if(num>tmp.count)
        {
            throw Error();
        }
//        std::cout<<"oldplace："<<xx.last<<'\n';

        if(stack.empty())
        {
            throw Error();
        }
        User nowuser=stack.back();
        if(nowuser.Privilege<7)
        {
            throw Error();
        }
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        T add=0;
        T minus=0;
        file.seekg(tmp.last,std::ios::beg);
        for(long long i=0;i<num;i++)
        {
            T t;
            file.read(reinterpret_cast<char *>(&t),sizeofT);
            if(t>0)
            {
                add+=t;
            }
            else
            {
                minus-=t;
            }
            file.seekg(-2*sizeofT,std::ios::cur);
        }
        std::cout<<"+ "<<std::fixed<<std::setprecision(2)<<add<<" - "<<minus<<"\n";
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    T read(const int index) {
        T t;
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(index,std::ios::beg);
        file.read(reinterpret_cast<char *>(&t),sizeofT);
        file.close();
        return t;
    }
};

struct logbook
{
    char isbn[25]{};
    int number=0;
    double money=0;
};

class Financeinf {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(logbook);
public:
    Financeinf()
    {
        initialise("financeinf");
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
            info tmp;
            file.write(reinterpret_cast<char*>(&tmp), sizeof(info));
        }
        file.close();
    }

    //读出首个块
    void get_info(info &tmp) {
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(info));
        file.close();
    }

    //将新的索引写入首个块中
    void write_info(info tmp) {
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(info));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    void write(logbook &t) {
//        std::cout<<"数目为："<<t<<'\n';

        file.open(file_name, std::ios::app|std::ios::binary);
        int index = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
//        std::cout<<"写入位置为："<<index<<'\n';
        info tmp;
        get_info(tmp);
        tmp.count++;
        tmp.last=index;
        write_info(tmp);
//        info xx;
//        get_info(xx);
//        std::cout<<"读出位置为："<<xx.last<<'\n';
    }

    void show()
    {
        info tmp;
        get_info(tmp);
//        std::cout<<"oldplace："<<xx.last<<'\n';

        if(stack.empty())
        {
            throw Error();
        }
        User nowuser=stack.back();
        if(nowuser.Privilege<7)
        {
            throw Error();
        }
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(sizeof(info),std::ios::beg);
        for(long long i=0;i<tmp.count;i++)
        {
            logbook t;
            file.read(reinterpret_cast<char *>(&t),sizeofT);
            std::cout<<"book: "<<t.isbn;
            if(t.money>0)
            {
                std::cout<<"-"<<t.number<<" +"<<t.money<<"dollar\n";
            }
            else
            {
                std::cout<<"+"<<t.number<<" -"<<t.money<<"dollar\n";
            }
//            file.seekg(-2*sizeofT,std::ios::cur);//因为已经读了一次，要退回两格
//            std::cout<<"计算得到："<<t<<'\n';
        }
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    logbook read(const int index) {
        logbook t;
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekg(index,std::ios::beg);
        file.read(reinterpret_cast<char *>(&t),sizeofT);
        file.close();
        return t;
    }
};
#endif //CODE_FINANCELOG_H
