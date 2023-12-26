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

struct Book
{
    char ISBN[25]{};
    char BookName[65]{};
    char Author[65]{};
    char Keyword[65]{};
    double Price=0;//单价
    long long Count=0;//库存数量

    friend bool operator<(const Book& a,const Book& b)
    {
        return strcmp(a.ISBN,b.ISBN)<0;
    }

//    friend bool operator==(const Book& a,const Book& b)
//    {
//        if(strcmp(a.ISBN,b.ISBN)==0&& strcmp(a.BookName,b.BookName)&&strcmp(a.Author,b.Author)&&strcmp(a.Keyword,b.Keyword)&&a.Price==b.Price&&a.Count==b.Count)
//        {
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
    friend bool operator==(const Book& a,const Book& b)
    {
        return strcmp(a.ISBN,b.ISBN)==0;
    }

    friend std::ostream &operator<<(std::ostream &os,const Book& obj)
    {
        os<<obj.ISBN<<'\t'<<obj.BookName<<'\t'<<obj.Author<<'\t'<<obj.Keyword<<'\t'<<std::fixed<<std::setprecision(2)<<obj.Price<<'\t'<<obj.Count<<'\n';
        return os;
    }
};


class Bookinf
{
private:
    blockchain<25, Book>booklist;//按照ISBN号排序，对应的值是图书的所有信息
    blockchain<65,mystr<25>>booknamelist;//按照书名排序,对应的值是ISBN
    blockchain<65,mystr<25>>authorlist;//按照作者排序，对应的值是ISBN
    blockchain<65,mystr<25>>keywordlist;//按照关键字排序，对应的值是ISBN
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
        booklist.Insert(obj.ISBN,obj);
        if(strlen(obj.BookName)!=0)
        {
            booknamelist.Insert(obj.BookName,isbn);
        }
        if(strlen(obj.Author)!=0)
        {
            authorlist.Insert(obj.Author,isbn);
        }
        if(strlen(obj.Keyword)!=0)
        {
            std::vector<string>keys= readkey(obj.Keyword);
            for(int i=0;i<keys.size();i++)
            {
                char tmp[60]{};
                strcpy(tmp,keys[i].c_str());
                keywordlist.Insert(tmp,isbn);
            }
        }
    }

    void Delete(Book& obj)
    {
        mystr<25> isbn(obj.ISBN);
        booklist.Delete(obj.ISBN,obj);
        booknamelist.Delete(obj.BookName,isbn);
        authorlist.Delete(obj.Author,isbn);
        std::vector<string>keys= readkey(obj.Keyword);
        for(int i=0;i<keys.size();i++)
        {
            char tmp[60]{};
            strcpy(tmp,keys[i].c_str());
            keywordlist.Delete(tmp,isbn);
        }
    }

    void show()//无附加参数，输出所有图书
    {
        if(stack.empty())
        {
            throw Error();
        }
        booklist.display();
    }

    void show(string &s)//有附加参数
    {
        if(stack.empty())
        {
            throw Error();
        }
        switch(s[1])
        {
            case 'I':
            {
                if(s.size()==6)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<Book>list;
                    std::string substring=s.substr(6);//获取ISBN
                    if(substring.size()>20|| !is2(substring))
                    {
                        throw Error();
                    }
                    char ISBN[25]{};
                    strcpy(ISBN,substring.c_str());
                    list = booklist.findval(ISBN);
                    if(list.empty())
                    {
                        std::cout<<"\n";
                    }
                    else
                    {
                        std::cout<<list[0];
                    }
                }
                break;
            }
            case 'n':
            {
                if(s.size()==6)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<mystr<25>>isbn;
                    std::string substring=s.substr(7,s.size()-8);//获取bookname
                    if(substring.size()>60|| !is3(substring))
                    {
                        throw Error();
                    }
                    char ISBN[25]{};
                    char BookName[65]{};
                    strcpy(BookName,substring.c_str());
                    isbn = booknamelist.findval(BookName);
                    if(isbn.empty())
                    {
                        std::cout<<"\n";
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
                if(s.size()==8)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<mystr<25>>isbn;
                    std::string substring=s.substr(9,s.size()-10);//获取author
                    if(substring.size()>60|| !is3(substring))
                    {
                        throw Error();
                    }
                    char Author[65]{};
                    strcpy(Author,substring.c_str());
                    isbn = authorlist.findval(Author);
                    if(isbn.empty())
                    {
                        std::cout<<"\n";
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
                if(s.size()==8)//附加参数内容为空
                {
                    throw Error();
                }
                else
                {
                    std::vector<mystr<25>>isbn;
                    std::string substring=s.substr(10,s.size()-11);//获取keyword
                    if(substring.size()>60|| !is3(substring))
                    {
                        throw Error();
                    }
                    for(int i = 0;i<substring.size();i++)
                    {
                        if(substring[i]=='|')//如果出现多个关键词，则操作失败
                        {
                            throw Error();
                        }
                    }
                    char Keyword[65]{};
                    strcpy(Keyword,substring.c_str());
                    isbn = keywordlist.findval(Keyword);
                    if(isbn.empty())
                    {
                        std::cout<<"\n";
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

    void buy(char isbn[],long long num)
    {
        if(num<=0)
        {
            throw Error();
        }
        std::vector<Book> tmp=booklist.findval(isbn);
        if(tmp.empty())
        {
            throw Error();
        }
        tmp[0].Count-=num;
        std::cout<<num*tmp[0].Price<<"\n";
        double profit=num*tmp[0].Price;
        financeinf.write(profit);
    }

    void select(char isbn[])
    {
        if(stack.empty())
        {
            throw Error();
        }
        User nowuser=stack.back();
        stack.pop_back();
        if(nowuser.Privilege<3)
        {
            throw Error();
        }
        nowuser.isselect=true;
        strcpy(nowuser.selectisdn,isbn);
        stack.push_back(nowuser);
        //std::cout<<nowuser.selectisdn<<'\n';
        std::vector<Book>tmp=booklist.findval(isbn);
        if(tmp.empty())//如果没有符合条件的图书，则创建仅拥有ISBN信息的新图书
        {
            Book obj;
            strcpy(obj.ISBN,isbn);
            bookinsert(obj);
        }
    }

    void modify(string &s)
    {
        if(stack.empty())//如果登录栈为空
        {
            throw Error();
        }
        User nowuser=stack.back();
        if(nowuser.Privilege<3||!nowuser.isselect)//如果权限不够或者没有选择
        {
            throw Error();
        }
        Book book=booklist.findval(nowuser.selectisdn)[0];//出问题
        Book copy=book;
        switch(s[1])
        {
            case 'I':
            {
                if(s.size()==6) { throw Error(); }
                else
                {
                    std::string substring=s.substr(6);//获取ISBN
                    if(substring.size()>20|| !is2(substring))//如果输入不合法
                    {
                        throw Error();
                    }
                    char revise[25]{};
                    strcpy(revise,substring.c_str());
                    if(trim(book.ISBN)== trim(revise))
                    {
                        throw Error();
                    }
                    strcpy(book.ISBN,substring.c_str());
                    stack.pop_back();
                    strcpy(nowuser.selectisdn,book.ISBN);
                    stack.push_back(nowuser);
                }
                break;
            }
            case 'n':
            {
                if(s.size()==6) { throw Error(); }
                else
                {
                    std::string substring=s.substr(7,s.size()-8);//获取书名
                    if(substring.size()>60|| !is3(substring))
                    {
                        throw Error();
                    }
                    if(trim(book.BookName)!=trim(substring))
                    {
                        strcpy(book.BookName,substring.c_str());
                    }
                }
                break;
            }
            case 'a':
            {
                if(s.size()==8) { throw Error(); }
                else
                {
                    std::string substring=s.substr(9,s.size()-10);//获取author
                    if(substring.size()>60|| !is3(substring))
                    {
                        throw Error();
                    }
                    if(trim(book.Author)!=trim(substring))
                    {
                        strcpy(book.Author,substring.c_str());
                    }
                }
                break;
            }
            case 'k':
            {
                if(s.size()==8) { throw Error(); }
                else
                {
                    std::string substring = s.substr(10, s.size() - 11);//获取keyword
                    if(substring.size()>60||!is3(substring))
                    {
                        throw Error();
                    }
                    std::vector<string>key= readkey(substring);
                    for(int i = 0;i<key.size();i++)
                    {
                        for(int j =0;j<key.size();j++)
                        {
                            if(i!=j&&key[i]==key[j])//如果包含重复信息段
                            {
                                throw Error();
                            }
                        }
                    }
                    if(trim(book.Keyword)!=trim(substring)) {
                        strcpy(book.Keyword, substring.c_str());
                    }
                }
                break;
            }
            case 'p':
            {
                if(s.size()==7) { throw Error(); }
                else
                {
                    std::string substring=s.substr(7);//获取price
                    double price= stringToReal(substring);
                    if(book.Price!=price)
                    {
                        book.Price = price;
                        bookinsert(book);//!!!这两个顺序弄反会出大问题，调了一整天！！！
                        Delete(copy);
                    }
                }
            }
        }
        Delete(copy);
        bookinsert(book);//!!!这两个顺序弄反会出大问题，调了一整天！！！
    }

    void import(long long quantity, double totalcost)
    {
        if(quantity<=0||totalcost<=0)
        {
            throw Error();
        }
        if(stack.empty())
        {
            throw Error();
        }
        User nowuser=stack.back();
        if(nowuser.Privilege<3)
        {
            throw Error();
        }
        else if(!nowuser.isselect)
        {
            throw Error();
        }
        else
        {
            Book book=booklist.findval(nowuser.selectisdn)[0];
            Book copy=book;
            book.Count+=quantity;
            financeinf.write(totalcost);
            Delete(copy);
            bookinsert(book);
        }
    }
};
#endif //BOOKSTORE_BOOK_H