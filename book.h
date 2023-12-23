//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H
#include "blockchain.h"
struct Book
{
    char ISBN[20];
    char BookName[60];
    char Author[60];
    char Keyword[60];
    float price;//单价
    int count;//库存数量
};


class bookinf
{
private:
    blockchain Books;
};
#endif //BOOKSTORE_BOOK_H
