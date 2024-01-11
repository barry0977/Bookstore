//
// Created by barry on 2023/12/21.
//

//2024.1.11 裂块那里好像有问题，空块记录不对  会把裂出来的新快写在原来的块的位置

#ifndef BOOKSTORE_BLOCKCHAIN_H
#define BOOKSTORE_BLOCKCHAIN_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

const int Max_size = 1050;//每个块中储存元素数量的最大值

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class S>
struct blockinf//准备把首个块和储存空块信息的数组合并成一个结构体
{
    S block1;
    int emptyinf[400]{ 0 };//记录空块地址
    int len = 0;//记录有多少个空块

    blockinf() = default;
};

template<class S, class T>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    blockinf<S> emptyblock;//记录删除的空块的位置
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "")
    {
        file_name = FN;
        file.open(file_name);
        if (!file.good())
        {
            file.open(file_name, std::ios::out | std::ios::binary);
            file.close();
            file.open(file_name);
            file.write(reinterpret_cast<char*>(&emptyblock), sizeof(blockinf<S>));
        }
        file.close();
    }

    //读出首个块
    void get_info(blockinf<S>& tmp) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(blockinf<S>));
        file.close();
    }

    //将新的索引写入首个块中
    void write_info(blockinf<S> tmp) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(blockinf<S>));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T& t) {
        blockinf<S> tmp;
        get_info(tmp);
        if (tmp.len > 0)//如果有空出来的区域，则写在那里
        {
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            int index = tmp.emptyinf[tmp.len - 1];
            tmp.len--;
            file.seekp(index, std::ios::beg);
            file.write(reinterpret_cast<char*>(&t), sizeofT);
            file.close();
            write_info(tmp);
            return index;
        }
        else
        {
            file.open(file_name, std::ios::app | std::ios::binary);
            int index = file.tellp();

//            std::cout<<"打开时地址："<<index<<"\n";

            file.write(reinterpret_cast<char*>(&t), sizeofT);
            file.close();
            return index;
        }
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T& t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekg(index);
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T& t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }
};

template<int len, class VALUE>//len表示index字符数组的长度，VALUE表示值
class blockchain
{
private:
    struct element
    {
        char index[len]{ 0 };//不能用string，否则会出现free(),invalid point
        VALUE value;

        friend bool operator<(const element& lhs, const element& rhs)
        {
            if (strcmp(lhs.index, rhs.index) != 0)
            {
                return strcmp(lhs.index, rhs.index) < 0;
            }
            else
            {
                return lhs.value < rhs.value;
            }
        }//原来的比较函数

//        friend bool operator<(const element& lhs, const element& rhs)
//        {
//            if (strlen(lhs.index)!=strlen( rhs.index))
//            {
//                return strlen(lhs.index)<strlen( rhs.index);
//            }
//            else if(strcmp(lhs.index, rhs.index) != 0)
//            {
//                return strcmp(lhs.index, rhs.index) < 0;
//            }
//            else
//            {
//                return lhs.value < rhs.value;
//            }
//        }

        friend bool operator>(const element& lhs, const element& rhs)
        {
            if (strcmp(lhs.index, rhs.index) != 0)
            {
                return strcmp(lhs.index, rhs.index) > 0;
            }
            else
            {
                return lhs.value > rhs.value;
            }
        }

        friend bool operator==(const element& lhs, const element& rhs)
        {
            if (strcmp(lhs.index, rhs.index) == 0 && lhs.value == rhs.value)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const element& obj)
        {
            os << obj.index << " " << obj.value << std::endl;
            return os;
        }

        element() = default;

        element(char ind[], VALUE num)
        {
            strcpy(index, ind);
            value = num;
        }
    };

    struct block//后面的块，里面是element的数组
    {
        int booknum = 0;
        element blocklist[400];

        block() = default;
    };

    struct firstblock//首个块，里面存储所有的索引
    {
        int number = 0;//记录块的数量
        element mininf[400];//每个块的首个元素（最小元素）
        int minindex[400]{ 0 };//每个块的首地址

        firstblock() = default;
    };

    MemoryRiver<firstblock, block> memoryriver;

public:
    blockchain() = default;

    blockchain(string filename)
    {
        memoryriver.initialise(filename);
    }

    bool isfirst()//判断是否是第一次操作文件
    {
        bool is;
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);
        if (tmp.block1.number == 0)
        {
            is = true;
        }
        else
        {
            is = false;
        }
        return is;
    }

    void initial(string filename)
    {
        memoryriver.initialise(filename);
    }

    //裂块操作
    void Divide(int place)//place表示第几个块
    {
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);
        if (place == tmp.block1.number - 1)//如果是最后一个块
        {
            tmp.block1.number++;
            block btmp;
            memoryriver.read(btmp, tmp.block1.minindex[place]);

//            std::cout<<"裂块前:";
//            for(int i=0;i<btmp.booknum;i++)
//            {
//                std::cout<<btmp.blocklist[i]<<" ";
//            }
//            std::cout<<std::endl;

            tmp.block1.mininf[place + 1] = btmp.blocklist[(0 + btmp.booknum) / 2];
            block obj1, obj2;
            for (int i = 0; i < (0 + btmp.booknum) / 2; i++)
            {
                obj1.blocklist[i] = btmp.blocklist[i];
                obj1.booknum++;
            }
            for (int j = (0 + btmp.booknum) / 2; j < btmp.booknum; j++)
            {
                obj2.blocklist[j - (0 + btmp.booknum) / 2] = btmp.blocklist[j];
                obj2.booknum++;
            }
            int newindex = memoryriver.write(obj2);

//            std::cout<<"前："<<tmp.block1.minindex[place]<<" 后："<<newindex<<"\n";

            tmp.block1.minindex[place + 1] = newindex;
            memoryriver.update(obj1, tmp.block1.minindex[place]);
            memoryriver.write_info(tmp);

//            block x1,x2;
//            memoryriver.read(x1,tmp.block1.minindex[place]);
//            memoryriver.read(x2,tmp.block1.minindex[place+1]);
//            std::cout<<"裂块后：";
//            for(int i=0;i<x1.booknum;i++)
//            {
//                std::cout<<x1.blocklist[i]<<" ";
//            }
//            std::cout<<std::endl;
//            for(int i=0;i<x2.booknum;i++)
//            {
//                std::cout<<x2.blocklist[i]<<" ";
//            }
//            std::cout<<std::endl;

        }
        else
        {
            tmp.block1.number++;
            element teminf1 = tmp.block1.mininf[place + 1];
            int temindex1 = tmp.block1.minindex[place + 1];
            for (int i = place + 1; i < tmp.block1.number; i++)//要裂的块后所有索引往后移一格
            {
                element teminf2 = tmp.block1.mininf[i + 1];
                long temindex2 = tmp.block1.minindex[i + 1];
                tmp.block1.mininf[i + 1] = teminf1;
                tmp.block1.minindex[i + 1] = temindex1;
                teminf1 = teminf2;
                temindex1 = temindex2;
            }
            block btmp;
            memoryriver.read(btmp, tmp.block1.minindex[place]);
            tmp.block1.mininf[place + 1] = btmp.blocklist[(0 + btmp.booknum) / 2];
            block obj1, obj2;
            for (int i = 0; i < (0 + btmp.booknum) / 2; i++)
            {
                obj1.blocklist[i] = btmp.blocklist[i];
                obj1.booknum++;
            }
            for (int j = (0 + btmp.booknum) / 2; j < btmp.booknum; j++)
            {
                obj2.blocklist[j - (0 + btmp.booknum) / 2] = btmp.blocklist[j];
                obj2.booknum++;
            }
            int newindex = memoryriver.write(obj2);
            tmp.block1.minindex[place + 1] = newindex;
            memoryriver.update(obj1, tmp.block1.minindex[place]);
            memoryriver.write_info(tmp);
        }



    }

    //删块操作
    void DeleteBlock(int place)//place表示第几个块
    {
        blockinf<firstblock>tmp;
        memoryriver.get_info(tmp);
        tmp.len++;//空块的数量增加
        tmp.emptyinf[tmp.len - 1] = tmp.block1.minindex[place];
        if (place == tmp.block1.number - 1)//如果是最后一个块
        {
            tmp.block1.number--;
            tmp.block1.minindex[place] = 0;
            tmp.block1.mininf[place] = {};
        }
        else
        {
            for (int i = place; i < tmp.block1.number - 1; i++)
            {
                tmp.block1.minindex[i] = tmp.block1.minindex[i + 1];
                tmp.block1.mininf[i] = tmp.block1.mininf[i + 1];
            }
            tmp.block1.number--;
        }
        memoryriver.write_info(tmp);
    }

    void Insert(char name[], VALUE number)
    {
//        std::cout<<"进入insert\n";

        element obj(name, number);
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);

//        for(int i=0;i<tmp.block1.number;i++)
//        {
//            block fuck;
//            memoryriver.read(fuck,tmp.block1.minindex[i]);
//            std::cout<<"第"<<i<<"块中有"<<fuck.booknum<<"个\n";
//        }

        if (tmp.block1.number == 0)//如果原本没有块
        {
            tmp.block1.number++;
            tmp.block1.mininf[0] = obj;
            block x;
            x.blocklist[0] = obj;
            x.booknum++;
            int index = memoryriver.write(x);
            tmp.block1.minindex[0] = index;
            memoryriver.write_info(tmp);
        }
        else//如果原本有块
        {
            if (obj < tmp.block1.mininf[0])//如果obj是最小的，插入第一个块
            {
                block btmp;
                memoryriver.read(btmp, tmp.block1.minindex[0]);
                tmp.block1.mininf[0] = obj;
                element tem1 = btmp.blocklist[0];
                for (int i = 0; i < btmp.booknum; i++)
                {
                    element tem2 = btmp.blocklist[i + 1];
                    btmp.blocklist[i + 1] = tem1;
                    tem1 = tem2;
                }
                btmp.blocklist[0] = obj;
                btmp.booknum++;

                //                std::cout<<"操作后1 "<<btmp.booknum<<'\n';

                memoryriver.update(btmp, tmp.block1.minindex[0]);
                memoryriver.write_info(tmp);
                if (btmp.booknum > Max_size)
                {
                    Divide(0);
                }
            }
            else
            {
                int min = 0, max = tmp.block1.number - 1, place;//place为第一个首个元素比obj小的块的位置
                while (min <= max)//二分查找应该插入的块,保存在place中
                {
                    int mid = (min + max) / 2;
                    if (obj < tmp.block1.mininf[mid])
                    {
                        max = mid - 1;
                    }
                    else
                    {
                        min = mid + 1;
                        place = mid;
                    }
                }
                block btmp;
                memoryriver.read(btmp, tmp.block1.minindex[place]);//读出该块的内容
                int mini = 0, maxi = btmp.booknum - 1, order = -1;//order为要插入元素前面的一个
                while (mini <= maxi)//二分查找
                {
                    int middle = (mini + maxi) / 2;
                    if (btmp.blocklist[middle] < obj)
                    {
                        order = middle;
                        mini = middle + 1;
                    }
                    else
                    {
                        maxi = middle - 1;
                    }
                }

                if (order == btmp.booknum - 1)//如果要插入元素为块中最大
                {
                    btmp.blocklist[order + 1] = obj;
                }
                else
                {
                    element tem1 = btmp.blocklist[order + 1];
                    btmp.blocklist[order + 1] = obj;
                    for (int m = order + 1; m < btmp.booknum; m++)//把后面的书整体往后移
                    {
                        element tem2 = btmp.blocklist[m + 1];
                        btmp.blocklist[m + 1] = tem1;
                        tem1 = tem2;
                    }
                }

                btmp.booknum++;
                if (order == -1)
                {
                    tmp.block1.mininf[place] = obj;
                }
                memoryriver.update(btmp, tmp.block1.minindex[place]);
                memoryriver.write_info(tmp);
                if (btmp.booknum > Max_size)//如果长度过长，则需要裂块
                {
                    Divide(place);
                }
            }
        }
    }

    void Find(char obj[])
    {
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);
        if (tmp.block1.number == 0)//如果没有块，直接返回null
        {
            std::cout << "null\n";
            return;
        }
        else
        {
            int min = 0, max = tmp.block1.number - 1, place = -1;//找到最后一个小于目标的块,从这个块开始可能有目标
            while (min <= max)
            {
                int mid = (min + max) / 2;
                if (strcmp(tmp.block1.mininf[mid].index, obj) < 0)
                {
                    place = mid;
                    min = mid + 1;
                }
                else
                {
                    max = mid - 1;
                }
            }
            if (place < 0)//所有块都大于等于目标
            {
                if (strcmp(tmp.block1.mininf[0].index, obj) > 0)//如果第一个块的index比目标还要大，直接返回null
                {
                    std::cout << "null\n";
                    return;
                }
                else//第一个块的首个index等于目标
                {
                    for (int i = 0; i < tmp.block1.number; i++)
                    {
                        block btmp;
                        bool isbreak = false;
                        memoryriver.read(btmp, tmp.block1.minindex[i]);
                        for (int j = 0; j < btmp.booknum; j++)
                        {
                            if (strcmp(btmp.blocklist[j].index, obj) != 0)
                            {
                                isbreak = true;
                                break;
                            }
                            std::cout << btmp.blocklist[j].value << " ";
                        }
                        if (isbreak)
                        {
                            break;
                        }
                    }
                    std::cout << "\n";
                    return;
                }
            }
            else
            {
                bool isfind = false, isbreak = false;
                for (int i = place; i < tmp.block1.number; i++)
                {
                    block btmp;
                    memoryriver.read(btmp, tmp.block1.minindex[i]);
                    for (int j = 0; j < btmp.booknum; j++)
                    {
                        if (strcmp(btmp.blocklist[j].index, obj) == 0)
                        {
                            std::cout << btmp.blocklist[j].value << " ";
                            isfind = true;
                        }
                        if (strcmp(btmp.blocklist[j].index, obj) > 0)
                        {
                            isbreak = true;
                            break;
                        }
                    }
                    if (isbreak)
                    {
                        break;
                    }
                }
                if (!isfind)
                {
                    std::cout << "null";
                }
                std::cout << "\n";
                return;
            }
        }
    }

    std::vector<VALUE> findval(char obj[])
    {
        std::vector<VALUE>list;
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);
        if (tmp.block1.number > 0)
        {
            int min = 0, max = tmp.block1.number - 1, place = -1;//找到最后一个小于目标的块,从这个块开始可能有目标
            while (min <= max)
            {
                int mid = (min + max) / 2;
                if (strcmp(tmp.block1.mininf[mid].index, obj) < 0)
                {
                    place = mid;
                    min = mid + 1;
                }
                else
                {
                    max = mid - 1;
                }
            }
            if (place < 0)//所有块都大于等于目标
            {
                if (strcmp(tmp.block1.mininf[0].index, obj) <= 0)
                {
                    for (int i = 0; i < tmp.block1.number; i++)
                    {
                        block btmp;
                        bool isbreak = false;
                        memoryriver.read(btmp, tmp.block1.minindex[i]);
                        for (int j = 0; j < btmp.booknum; j++)
                        {
                            if (strcmp(btmp.blocklist[j].index, obj) != 0)
                            {
                                isbreak = true;
                                break;
                            }
                            list.push_back(btmp.blocklist[j].value);
                        }
                        if (isbreak)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                bool isbreak = false;
                for (int i = place; i < tmp.block1.number; i++)
                {
                    block btmp;
                    memoryriver.read(btmp, tmp.block1.minindex[i]);
                    for (int j = 0; j < btmp.booknum; j++)
                    {
                        if (strcmp(btmp.blocklist[j].index, obj) == 0)
                        {
                            list.push_back(btmp.blocklist[j].value);
                        }
                        if (strcmp(btmp.blocklist[j].index, obj) > 0)
                        {
                            isbreak = true;
                            break;
                        }
                    }
                    if (isbreak)
                    {
                        break;
                    }
                }
            }
        }
        return list;
    }

    void Delete(char name[], VALUE number)
    {
        element obj(name, number);
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);
        if (tmp.block1.number > 0)
        {
            int min = 0, max = tmp.block1.number - 1, place;
            while (min <= max)//二分查找所在块,保存在place中
            {
                int mid = (min + max) / 2;
                if (obj < tmp.block1.mininf[mid])
                {
                    max = mid - 1;
                }
                else
                {
                    min = mid + 1;
                    place = mid;
                }
            }
            block btmp;
            memoryriver.read(btmp, tmp.block1.minindex[place]);
            int mini = 0, maxi = btmp.booknum - 1, order = -1;//order表示目标位置
            while (mini <= maxi)
            {
                int middle = (mini + maxi) / 2;
                if (strcmp(btmp.blocklist[middle].index, name) == 0 && btmp.blocklist[middle].value == number)
                {
                    order = middle;
                    break;
                }
                else if (obj < btmp.blocklist[middle])
                {
                    maxi = middle - 1;
                }
                else
                {
                    mini = middle + 1;
                }
            }
            if (order >= 0)//如果在块中找到了目标
            {
                if (order == btmp.booknum - 1)//如果是该块最后一个
                {
                    btmp.booknum--;
                    if(btmp.booknum==0)
                    {
                        DeleteBlock(place);
                    }
                    else
                    {
                        memoryriver.update(btmp, tmp.block1.minindex[place]);
                    }
                }
                else
                {
                    for (int k = order + 1; k < btmp.booknum; k++)
                    {
                        btmp.blocklist[k - 1] = btmp.blocklist[k];
                    }
                    btmp.booknum--;
                    tmp.block1.mininf[place] = btmp.blocklist[0];
                    memoryriver.write_info(tmp);
                    memoryriver.update(btmp, tmp.block1.minindex[place]);
                }

//                if (btmp.booknum == 0)//如果块里面没有元素，就删块
//                {
//                    memoryriver.write_info(tmp);
//                    DeleteBlock(place);
//                }
//                else
//                {
//                    memoryriver.update(btmp, tmp.block1.minindex[place]);
//                    tmp.block1.mininf[place] = btmp.blocklist[0];
//                    memoryriver.write_info(tmp);
//                }
            }
        }
    }

    void display()
    {
        blockinf<firstblock> tmp;
        memoryriver.get_info(tmp);
        for (int i = 0; i < tmp.block1.number; i++)
        {
            block btmp;
            memoryriver.read(btmp, tmp.block1.minindex[i]);
            for (int j = 0; j < btmp.booknum; j++)
            {
                std::cout << btmp.blocklist[j].value;
            }
        }
    }
};

template<int len>
struct mystr//用于将字符数组封装成结构体
{
    char value[len]{};

    explicit mystr()
    {
        memset(value, 0, len);
    }

    explicit mystr(char a[])
    {
        strcpy(value, a);
    }

    explicit mystr(string& a)
    {
        strcpy(value, a.c_str());
    }

    friend bool operator<(const mystr& a, const mystr& b)
    {
        return strcmp(a.value, b.value) < 0;
    }

    friend bool operator>(const mystr& a, const mystr& b)
    {
        return strcmp(a.value, b.value) > 0;
    }

    friend bool operator==(const mystr& a, const mystr& b)
    {
        return strcmp(a.value, b.value) == 0;
    }

    mystr<len>& operator=(const mystr& a)
    {
        if (this == &a) return *this;
        memset(value, 0, sizeof(value));
        std::strcpy(value, a.value);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const mystr<len>& obj)
    {
        os << obj.value;
        return os;
    }
};


#endif //BOOKSTORE_BLOCKCHAIN_H