//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H
#include "blockchain.h"
#include "error.h"
class Visitor
{
protected:
    int privilege=0;
public:
    void create(char name[],char password[])
    {

    }

    void logon(char name,string password)
    {

    }
};

class Customer:public Visitor
{
protected:
    privilege=1;
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

struct User
{
    char UserID[35]{};
    char Password[35]{};
    char Username[35]{};
    int Privilege=0;

    bool operator<(const User& other)const
    {
        return strcmp(UserID,other.UserID)<0;
    }
};

class userinf
{
private:
    blockchain<35,User>userlist;
public:
    void insert(User& obj)
    {
        userlist.Insert(obj.UserID,obj);
    }

    void revise()
};

#endif //BOOKSTORE_USER_H
