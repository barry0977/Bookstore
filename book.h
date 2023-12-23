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


class bookinf
{
private:
    blockchain<25, Book>booklist;//按照ISBN号排序，对应的值是图书的所有信息
    blockchain<65,mystr<25>>booknamelist;//按照书名排序,对应的值是ISBN
    blockchain<65,mystr<25>>authorlist;//按照作者排序，对应的值是ISBN
    blockchain<65,mystr<25>>keywordlist;//按照关键字排序，对应的值是ISBN
public:
    void insert(Book& obj)
    {
        mystr<25> isbn(obj.ISBN);
        booklist.Insert(obj.ISBN,obj);
        booknamelist.Insert(obj.BookName,isbn);
        authorlist.Insert(obj.Author,isbn);
        keywordlist.Insert(obj.Keyword,isbn);
    }

    void show()
    {
        booklist.display();
    }

    void show(string &s)
    {
        std::vector<Book>list;
        switch(s[1])
        {
            case 'I':
            {
                list=booklist.findval()
            }
            case 'n':
            {

            }
            case 'a':
            {

            }
            case 'k':
            {

            }
        }
    }

    void modify(char isbn[],)
    {

    }

    void buy(char isbn[],long long num)
    {
        if(num<=0)
        {
            throw Error();
        }
        std::vector<Book> tmp=booklist.findval(isbn);
        if(tmp.size()==0)
        {
            throw Error();
        }
        tmp[0].Count-=num;
        std::cout<<num*tmp[0].Price<<"\n";
    }
};

#endif //BOOKSTORE_BOOK_H
