//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H
#include <iomanip>
#include "blockchain.h"
#include "error.h"
#include "sentenceslice.h"
#include "financelog.h"
#include "user.h"

class Bookinf;

extern std::vector<User>stack;
extern Bookinf bookinf;
extern Userinf userinf;
extern Financelog<double> financeinf;
extern Financeinf financeinf1;
extern Log log;

struct Book
{
    char ISBN[25]{};
    char BookName[65]{};
    char Author[65]{};
    char Keyword[65]{};
    double Price = 0;//单价
    long long Count = 0;//库存数量

    friend bool operator<(const Book& a, const Book& b)
    {
        if(strlen(a.ISBN)!=strlen(b.ISBN))
        {
            return strlen(a.ISBN)<strlen(b.ISBN);
        }
        else
        {
            return strcmp(a.ISBN, b.ISBN) < 0;
        }
    }

    friend bool operator==(const Book& a, const Book& b)
    {
        return strcmp(a.ISBN, b.ISBN) == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Book& obj)
    {
        os << obj.ISBN << '\t' << obj.BookName << '\t' << obj.Author << '\t' << obj.Keyword << '\t' << std::fixed << std::setprecision(2) << obj.Price << '\t' << obj.Count << '\n';
        return os;
    }
};


class Bookinf
{
private:
    blockchain<25, Book>booklist;//按照ISBN号排序，对应的值是图书的所有信息
    blockchain<65, mystr<25>>booknamelist;//按照书名排序,对应的值是ISBN
    blockchain<65, mystr<25>>authorlist;//按照作者排序，对应的值是ISBN
    blockchain<65, mystr<25>>keywordlist;//按照关键字排序，对应的值是ISBN
public:
    friend User;

    Bookinf()
    {
        booklist.initial("Bookinfo");
        booknamelist.initial("Booknamelist");
        authorlist.initial("Authorlist");
        keywordlist.initial("Keywordlist");
    }

    void bookinsert(Book& obj)
    {
        mystr<25> isbn(obj.ISBN);
        booklist.Insert(obj.ISBN, obj);
        if (strlen(obj.BookName) != 0)
        {
            booknamelist.Insert(obj.BookName, isbn);
        }
        if (strlen(obj.Author) != 0)
        {
            authorlist.Insert(obj.Author, isbn);
        }
        if (strlen(obj.Keyword) != 0)
        {
            std::vector<string>keys = readkey(obj.Keyword);
            for (int i = 0; i < keys.size(); i++)
            {
                char tmp[60]{};
                strcpy(tmp, keys[i].c_str());
                keywordlist.Insert(tmp, isbn);
            }
        }
    }

    void Delete(Book& obj)
    {
        mystr<25> isbn(obj.ISBN);
        booklist.Delete(obj.ISBN, obj);
        booknamelist.Delete(obj.BookName, isbn);
        authorlist.Delete(obj.Author, isbn);
        std::vector<string>keys = readkey(obj.Keyword);
        for (int i = 0; i < keys.size(); i++)
        {
            char tmp[60]{};
            strcpy(tmp, keys[i].c_str());
            keywordlist.Delete(tmp, isbn);
        }
    }

    void show()//无附加参数，输出所有图书
    {
        if (stack.empty())
        {
            throw Error();
        }
        booklist.display();
    }

    void show(string& s)//有附加参数
    {
        if (stack.empty())
        {
            throw Error();
        }
        switch (s[1])
        {
            case 'I':
            {
                if (s.size() == 6)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<Book>list;
                    std::string substring = s.substr(6);//获取ISBN
                    if (substring.size() > 20 || !is2(substring))
                    {
                        throw Error();
                    }
                    char ISBN[25]{};
                    strcpy(ISBN, substring.c_str());
                    list = booklist.findval(ISBN);
                    if (list.empty())
                    {
                        std::cout << "\n";
                    }
                    else
                    {
                        std::cout << list[0];
                    }
                }
                break;
            }
            case 'n':
            {
                if (s.size() == 6)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<mystr<25>>isbn;
                    std::string substring = s.substr(7, s.size() - 8);//获取bookname
                    if (substring.size() > 60 || !is3(substring))
                    {
                        throw Error();
                    }
                    char ISBN[25]{};
                    char BookName[65]{};
                    strcpy(BookName, substring.c_str());
                    isbn = booknamelist.findval(BookName);
                    if (isbn.empty())
                    {
                        std::cout << "\n";
                    }
                    else
                    {
                        for (int i = 0; i < isbn.size(); i++)
                        {
                            std::cout << booklist.findval(isbn[i].value)[0];
                        }
                    }
                }
                break;
            }
            case 'a':
            {
                if (s.size() == 8)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<mystr<25>>isbn;
                    std::string substring = s.substr(9, s.size() - 10);//获取author
                    if (substring.size() > 60 || !is3(substring))
                    {
                        throw Error();
                    }
                    char Author[65]{};
                    strcpy(Author, substring.c_str());
                    isbn = authorlist.findval(Author);
                    if (isbn.empty())
                    {
                        std::cout << "\n";
                    }
                    else
                    {
                        for (int i = 0; i < isbn.size(); i++)
                        {
                            std::cout << booklist.findval(isbn[i].value)[0];
                        }
                    }
                }
                break;
            }
            case 'k':
            {
                if (s.size() == 8)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<mystr<25>>isbn;
                    std::string substring = s.substr(10, s.size() - 11);//获取keyword
                    if (substring.size() > 60 || !is3(substring))
                    {
                        throw Error();
                    }
                    for (int i = 0; i < substring.size(); i++)
                    {
                        if (substring[i] == '|')//如果出现多个关键词，则操作失败
                        {
                            throw Error();
                        }
                    }
                    char Keyword[65]{};
                    strcpy(Keyword, substring.c_str());
                    isbn = keywordlist.findval(Keyword);
                    if (isbn.empty())
                    {
                        std::cout << "\n";
                    }
                    else
                    {
                        for (int i = 0; i < isbn.size(); i++)
                        {
                            std::cout << booklist.findval(isbn[i].value)[0];
                        }
                    }
                }
                break;
            }
        }
    }

    void buy(char isbn[], long long num)
    {
        if (num <= 0)
        {
            throw Error();
        }
        if(stack.empty())
        {
            throw Error();
        }
        std::vector<Book> tmp = booklist.findval(isbn);
        if (tmp.empty())
        {
            throw Error();
        }
        if(num>tmp[0].Count)
        {
            throw Error();
        }
        std::cout << std::fixed << std::setprecision(2) << num * tmp[0].Price << "\n";
        double profit = num * tmp[0].Price;

        Book book = tmp[0];
        Book copy = book;
        book.Count -= num;
        Delete(copy);
        bookinsert(book);

        financeinf.write(profit);

        logbook t;
        strcpy(t.isbn, isbn);
        t.money = profit;
        t.number = num;
        financeinf1.write(t);

        loginf x;
        x.mode=2;
        strcpy(x.isbn, isbn);
        x.money = profit;
        x.number = num;
        log.write(x);
    }

    void select(char isbn[])
    {
        if (stack.empty())
        {
            throw Error();
        }
        User nowuser = stack.back();
        if (nowuser.Privilege < 3)
        {
            throw Error();
        }
        stack.pop_back();
        nowuser.isselect = true;
        strcpy(nowuser.selectisdn, isbn);
        stack.push_back(nowuser);
        std::vector<Book>tmp = booklist.findval(isbn);
        if (tmp.empty())//如果没有符合条件的图书，则创建仅拥有ISBN信息的新图书
        {
            Book obj;
            strcpy(obj.ISBN, isbn);
            bookinsert(obj);
        }
    }


    void Modify(std::vector<string> s)
    {
        if (stack.empty())//如果登录栈为空
        {
            throw Error();
        }
        User nowuser = stack.back();
        if (nowuser.Privilege < 3 || !nowuser.isselect)//如果权限不够或者没有选择
        {
            throw Error();
        }
        if(s.size()==1)
        {
            throw Error();
        }
        Book book = booklist.findval(nowuser.selectisdn)[0];//出问题
        Book copy = book;
        bool is[5]={false};//记录是否有重复附加参数
        for(int i = 1;i < s.size();i++)
        {
            if(s[i][1]=='I')
            {
                if(is[0])
                {
                    throw Error();
                }
                is[0]=true;
                if(s[i].size()==6)
                {
                    throw Error();
                }
                std::string substring = s[i].substr(6);//获取ISBN
                if (substring.size() > 20 || !is2(substring))//如果输入不合法
                {
                    throw Error();
                }
                char revise[25]{};
                strcpy(revise, substring.c_str());
                if (trim(book.ISBN) == trim(revise))//不允许将 ISBN 改为原有的 ISBN
                {
                    throw Error();
                }
                if(!booklist.findval(revise).empty())//如果要修改的isbn已经存在
                {
                    throw Error();
                }

                char origin[25]{};
                strcpy(origin,book.ISBN);

                strcpy(book.ISBN, revise);
                stack.pop_back();
                strcpy(nowuser.selectisdn, revise);
                stack.push_back(nowuser);

                for(int i =0;i<stack.size()-1;i++)
                {
                    if(strcmp(stack[i].selectisdn,origin)==0)
                    {
                        strcpy(stack[i].selectisdn,revise);
                    }
                }
            }
            if(s[i][1]=='n')
            {
                if(is[1])
                {
                    throw Error();
                }
                is[1]=true;
                if(s[i].size()==6)
                {
                    throw Error();
                }
                string newname=s[i].substr(7, s[i].size() - 8);
                if (newname.size() > 60 || !is3(newname))
                {
                    throw Error();
                }
                if (trim(book.BookName) != trim(newname))
                {
                    strcpy(book.BookName, newname.c_str());
                }
            }
            if(s[i][1]=='a')
            {
                if(is[2])
                {
                    throw Error();
                }
                is[2]=true;
                if (s[i].size() == 8)
                {
                    throw Error();
                }
                std::string substring = s[i].substr(9, s[i].size() - 10);//获取author
                if (substring.size() > 60 || !is3(substring))
                {
                    throw Error();
                }
                if (trim(book.Author) != trim(substring))
                {
                    strcpy(book.Author, substring.c_str());
                }
            }
            if(s[i][1]=='k')
            {
                if(is[3])
                {
                    throw Error();
                }
                is[3]=true;
                if(s[i].size()==9)
                {
                    throw Error();
                }
                std::string substring = s[i].substr(10, s[i].size() - 11);//获取keyword
                if (substring.size() > 60 || !is3(substring))
                {
                    throw Error();
                }
                std::vector<string>key = readkey(substring);
                for (int i = 0; i < key.size(); i++)
                {
                    for (int j = 0; j < key.size(); j++)
                    {
                        if (i != j && key[i] == key[j])//如果包含重复信息段
                        {
                            throw Error();
                        }
                    }
                }
                if (trim(book.Keyword) != trim(substring))
                {
                    strcpy(book.Keyword, substring.c_str());
                }
            }
            if(s[i][1]=='p')
            {
                if(is[4])
                {
                    throw Error();
                }
                is[4]=true;
                if(s[i].size()==6)
                {
                    throw Error();
                }
                std::string substring = s[i].substr(7);//获取price
                double price = stringToReal(substring);
                book.Price=price;
            }
        }
        Delete(copy);
        bookinsert(book);
    }
    void modify(string& s)
    {
        if (stack.empty())//如果登录栈为空
        {
            throw Error();
        }
        User nowuser = stack.back();
        if (nowuser.Privilege < 3 || !nowuser.isselect)//如果权限不够或者没有选择
        {
            throw Error();
        }
        Book book = booklist.findval(nowuser.selectisdn)[0];//出问题
        Book copy = book;
        switch (s[1])
        {
            case 'I':
            {
                if (s.size() == 6) { throw Error(); }
                else
                {
                    std::string substring = s.substr(6);//获取ISBN
                    if (substring.size() > 20 || !is2(substring))//如果输入不合法
                    {
                        throw Error();
                    }
                    char revise[25]{};
                    strcpy(revise, substring.c_str());
                    if (trim(book.ISBN) == trim(revise))
                    {
                        throw Error();
                    }
                    if(!booklist.findval(revise).empty())
                    {
                        throw Error();
                    }
                    strcpy(book.ISBN, substring.c_str());
                    stack.pop_back();
                    strcpy(nowuser.selectisdn, book.ISBN);
                    stack.push_back(nowuser);
                }
                break;
            }
            case 'n':
            {
                if (s.size() == 6) { throw Error(); }
                else
                {
                    std::string substring = s.substr(7, s.size() - 8);//获取书名
                    if (substring.size() > 60 || !is3(substring))
                    {
                        throw Error();
                    }
                    if (trim(book.BookName) != trim(substring))
                    {
                        strcpy(book.BookName, substring.c_str());
                    }
                }
                break;
            }
            case 'a':
            {
                if (s.size() == 8) { throw Error(); }
                else
                {
                    std::string substring = s.substr(9, s.size() - 10);//获取author
                    if (substring.size() > 60 || !is3(substring))
                    {
                        throw Error();
                    }
                    if (trim(book.Author) != trim(substring))
                    {
                        strcpy(book.Author, substring.c_str());
                    }
                }
                break;
            }
            case 'k':
            {
                if (s.size() == 9) { throw Error(); }
                else
                {
                    std::string substring = s.substr(10, s.size() - 11);//获取keyword
                    if (substring.size() > 60 || !is3(substring))
                    {
                        throw Error();
                    }
                    std::vector<string>key = readkey(substring);
                    for (int i = 0; i < key.size(); i++)
                    {
                        for (int j = 0; j < key.size(); j++)
                        {
                            if (i != j && key[i] == key[j])//如果包含重复信息段
                            {
                                throw Error();
                            }
                        }
                    }
                    if (trim(book.Keyword) != trim(substring)) {
                        strcpy(book.Keyword, substring.c_str());
                    }
                }
                break;
            }
            case 'p':
            {
                if (s.size() == 7) { throw Error(); }
                else
                {
                    std::string substring = s.substr(7);//获取price
                    double price = stringToReal(substring);
                    if (book.Price != price)
                    {
                        book.Price = price;
                        bookinsert(book);
                        Delete(copy);
                    }
                }
            }
        }
        Delete(copy);
        bookinsert(book);
        if (nowuser.Privilege == 3)
        {
            loguser t;
            strcpy(t.id,nowuser.UserID);
            strcpy(t.act,"modify");
            strcpy(t.obj,nowuser.selectisdn);
            strcpy(t.obj2,s.c_str());
            userlog.write(t);
        }
        if(!stack.empty())
        {
            User sub = stack.back();
            loginf infor;
            infor.mode = 1;
            strcpy(infor.id,sub.UserID);
            strcpy(infor.act,"modify");
            strcpy(infor.obj,nowuser.selectisdn);
            strcpy(infor.obj2,s.c_str());
            log.write(infor);
        }
    }

    void import(long long quantity, double totalcost)
    {
        if (quantity <= 0 || totalcost <= 0)
        {
            throw Error();
        }
        if (stack.empty())
        {
            throw Error();
        }
        User nowuser = stack.back();
        if (nowuser.Privilege < 3)
        {
            throw Error();
        }
        else if (!nowuser.isselect)
        {
            throw Error();
        }
        else
        {
            Book book = booklist.findval(nowuser.selectisdn)[0];
            Book copy = book;
            book.Count += quantity;
            double cost = -totalcost;
            financeinf.write(cost);

            logbook t;
            strcpy(t.isbn, nowuser.selectisdn);
            t.money = cost;
            t.number = quantity;
            financeinf1.write(t);

            loginf x;
            strcpy(x.isbn, nowuser.selectisdn);
            x.money = cost;
            x.number = quantity;
            log.write(x);

            Delete(copy);
            bookinsert(book);

            if (nowuser.Privilege == 3)
            {
                loguser t;
                strcpy(t.id,nowuser.UserID);
                strcpy(t.act,"import");
                strcpy(t.obj,nowuser.selectisdn);
                string a = realToString(totalcost);
                string b = integerToString(quantity);
                strcpy(t.obj2,b.c_str());
                strcpy(t.obj3,a.c_str());
                userlog.write(t);
            }
        }
    }
};
#endif //BOOKSTORE_BOOK_H