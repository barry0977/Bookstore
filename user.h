//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H
#include "blockchain.h"
class Visitor
{
public:
    void create(char name[],char password[])
    {

    }

    void logon(string name,string password)
    {

    }
};

class Customer:public Visitor
{
    friend class Visitor;
public:
    void revise(std::string oldpassword,string newpassword)
    {

    }

    
};

class Staff:public Customer
{

};

class Boss:public Staff
{
public:

};

struct user
{
    char UserID[35];
    char Password[35];
    char Username[35];
    int Privilege;
};


#endif //BOOKSTORE_USER_H
