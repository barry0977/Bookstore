//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H
#include "blockchain.h"
struct Book
{
    char ISBN[25]{};
    char BookName[65]{};
    char Author[65]{};
    char Keyword[65]{};
    double price=0;//单价
    int count=0;//库存数量

    bool operator<(const Book& other)const
    {
        return strcmp(ISBN,other.ISBN);
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

    void modify()
};

#endif //BOOKSTORE_BOOK_H
