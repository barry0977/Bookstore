//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H
#include <iomanip>
#include "blockchain.h"
#include "error.h"
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
    char selectedbook[25]{};//当前选中书的ISBN号

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
        keywordlist.Insert(obj.Keyword,isbn);
    }

    void show()//无附加参数，输出所有图书
    {
        booklist.display();
    }

    void show(string &s)//有附加参数
    {
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
                    std::string substring=s.substr(7,s.size()-8);//获取ISBN
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
                    std::string substring=s.substr(9,s.size()-10);//获取ISBN
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
                    std::string substring=s.substr(10,s.size()-10);//获取ISBN
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
        else
        {
            memset(selectedbook,0,25);
            strcpy(selectedbook,isbn);
        }
    }

    void modify()
};

#endif //BOOKSTORE_BOOK_H
