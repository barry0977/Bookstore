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

    void insert(Book& obj)
    {
        mystr<25> isbn(obj.ISBN);
        booklist.Insert(obj.ISBN,obj);
        booknamelist.Insert(obj.BookName,isbn);
        authorlist.Insert(obj.Author,isbn);
        std::vector<string>keys= readkey(obj.Keyword);
        for(int i=0;i<keys.size();i++)
        {
            char tmp[60];
            strcpy(tmp,keys[i].c_str());
            keywordlist.Insert(tmp,isbn);
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
            char tmp[60];
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
        std::vector<Book>list;
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
                    char ISBN[25];
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
                    char ISBN[25];
                    char BookName[65];
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
                    char Author[65];
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
                    std::string substring=s.substr(10,s.size()-10);//获取keyword
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
                    char Keyword[65];
                    strcpy(Keyword,substring.c_str());
                    isbn = authorlist.findval(Keyword);
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
    }

    void select(char isbn[])
    {
        std::vector<Book>tmp=booklist.findval(isbn);
        if(tmp.empty())//如果没有符合条件的图书，则创建仅拥有ISBN信息的新图书
        {
            Book obj;
            strcpy(obj.ISBN,isbn);
            insert(obj);
        }
    }

    void modify(string &s)
    {
        if(stack.empty())
        {
            throw Error();
        }
        User nowuser=stack.back();
        if(nowuser.Privilege<3||!nowuser.isselect)
        {
            throw Error();
        }
        Book book=booklist.findval(nowuser.selectiisdn)[0];
        Book copy=book;
        switch(s[1])
        {
            case 'I':
            {
                if(s.size()==6) { throw Error(); }
                else
                {
                    std::string substring=s.substr(6);//获取ISBN
                    if(substring.size()>20|| !is2(substring))
                    {
                        throw Error();
                    }
                    char revise[25];
                    strcpy(revise,substring.c_str());
                    if(strcmp(book.ISBN,revise)==0)
                    {
                        throw Error();
                    }
                    strcpy(book.ISBN,substring.c_str());
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
                    strcpy(book.BookName,substring.c_str());
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
                    strcpy(book.Author,substring.c_str());
                }
                break;
            }
            case 'k':
            {
                if(s.size()==8) { throw Error(); }
                else
                {
                    std::string substring = s.substr(10, s.size() - 10);//获取keyword
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
                    char keyword[65];
                    strcpy(keyword,substring.c_str());
                    strcpy(book.Keyword,keyword);
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
                    book.Price=price;
                }
            }
        }
        Delete(copy);
        insert(book);
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

            Book book=booklist.findval(nowuser.selectiisdn)[0];
            Book copy=book;
            book.Count-=quantity;
            financeinf.write(totalcost);
            Delete(copy);
            insert(book);
        }
    }
};
#endif //BOOKSTORE_BOOK_H