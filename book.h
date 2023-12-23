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
};


class bookinf
{
private:
    blockchain<25, Book>booklist;//按照ISBN号排序，对应的键值是图书的所有信息
    blockchain<65,mystr<25>>booknamelist;//按照书名排序
    blockchain<65,mystr<25>>authorlist;//按照作者排序
    blockchain<65,mystr<25>>keywordlist;//按照关键字排序
public:
    void insert()
};

#endif //BOOKSTORE_BOOK_H
