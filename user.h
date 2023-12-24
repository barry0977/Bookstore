//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H
#include "blockchain.h"
#include "error.h"
#include "book.h"
#include <algorithm>

struct User
{
    char UserID[35]{};
    char Password[35]{};
    char Username[35]{};
    int Privilege=0;
    bool isselect=false;
    char selectiisdn[25]{};

    friend bool operator<(const User& lhs,const User& rhs)
    {
        return strcmp(lhs.UserID,rhs.UserID)<0;
    }

    friend bool operator==(const User& lhs,const User& rhs)
    {
        return strcmp(lhs.UserID,rhs.UserID)==0;
    }
};

extern std::vector<User>stack;//登录栈，记录所有登录人员,当前操作人员为登录栈的最后一个

class Userinf
{
private:
    blockchain<35,User>userlist;//按照id排序，储存所有信息
public:
    Userinf()
    {
        userlist.initial("userinfo");
        if(userlist.isfirst())//程序首次运行时自主执行所需的初始化操作；创建帐户名为 root，密码为 sjtu，权限为 {7} 的超级管理员帐户。
        {
            User boss;
            strcpy(boss.UserID,"root");
            boss.Privilege=7;
            strcpy(boss.Password,"sjtu");
            userlist.Insert(boss.UserID,boss);
        }
    }

    void regist(char id[],char passwd[],char name[])
    {
        if(userlist.findval(id).empty())//用户库中没有同样的id
        {
            User tmp;
            strcpy(tmp.UserID,id);
            strcpy(tmp.Password,passwd);
            strcpy(tmp.Username,name);
            tmp.Privilege=1;
            userlist.Insert(tmp.UserID,tmp);
        }
        else
        {
            throw Error();
        }
    }

    void Delete(char id[])//删除账户
    {
        User nowuser=stack.back();
        if(nowuser.Privilege<7) {throw Error();}//如果不是店主，操作失败
        if(userlist.findval(id).empty())//如果待删除帐户不存在则操作失败
        {
            throw Error();
        }
        else
        {
            User obj=userlist.findval(id)[0];
            if(std::find(stack.begin(), stack.end(),obj)!=stack.end())//如果在登录栈中找到，操作失败
            {
                throw Error();
            }
            else
            {
                userlist.Delete(id,obj);
            }
        }
    }

    void su(char id[])//登录账户
    {
        std::vector<User>user=userlist.findval(id);
        if(user.empty())//如果该帐户不存在则操作失败
        {
            throw Error();
        }
        if(stack.empty())//如果登录栈为空，则必须输入密码
        {
            throw Error();
        }
        if(stack.back().Privilege<=user[0].Privilege)
        {
            throw Error();
        }
        stack.push_back(user[0]);
    }

    void su(char id[],char password[])//登录账户
    {
        std::vector<User>user=userlist.findval(id);
        if(user.empty())//如果该帐户不存在则操作失败
        {
            throw Error();
        }
        if(stack.empty())//如果登录栈为空
        {
            if(strcmp(user[0].Password,password)==0)
            {
                stack.push_back(user[0]);
            }
            else//如果密码错误则操作失败
            {
                throw Error();
            }
        }
        else
        {
            User nowuser=stack.back();
            if(user[0].Privilege<nowuser.Privilege)//如果当前帐户权限等级高于登录帐户则可以省略密码
            {
                stack.push_back(user[0]);
            }
            else
            {
                if(strcmp(user[0].Password,nowuser.Password)==0)
                {
                    stack.push_back(user[0]);
                }
                else//如果密码错误则操作失败
                {
                    throw Error();
                }
            }
        }
    }

    void logout()//注销账户
    {
        if(stack.empty())
        {
            throw Error();
        }
        else
        {
            User obj=stack.back();
            User copy=obj;
            obj.isselect=false;
            userlist.Delete(copy.UserID,copy);
            userlist.Insert(obj.UserID,obj);
            stack.pop_back();
        }
    }

    void passwd(char id[],char newpasswd[])//修改密码
    {
        if(userlist.findval(id).empty())//如果该帐户不存在则操作失败
        {
            throw Error();
        }
        if(stack.empty())
        {
            throw Error();
        }
        else
        {
            User obj=userlist.findval(id)[0];
            User copy=obj;
            User nowuser=stack.back();
            if(nowuser.Privilege==7)//如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]
            {
                strcpy(obj.Password,newpasswd);
                userlist.Delete(id,copy);
                userlist.Insert(id,obj);
            }
            else
            {
                throw Error();
            }
        }
    }

    void passwd(char id[],char newpasswd[],char current[])//修改密码
    {
        if(userlist.findval(id).empty())//如果该帐户不存在则操作失败
        {
            throw Error();
        }
        if(stack.empty())
        {
            throw Error();
        }
        else
        {
            User obj=userlist.findval(id)[0];
            User copy=obj;
            User nowuser=stack.back();
            if(nowuser.Privilege==7)//如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]
            {
                strcpy(obj.Password,newpasswd);
                userlist.Delete(id,copy);
                userlist.Insert(id,obj);
            }
            else
            {
                if(strcmp(obj.Password,current)!=0)//如果密码错误则操作失败
                {
                    throw Error();
                }
                else
                {
                    strcpy(obj.Password,newpasswd);
                    userlist.Delete(id,copy);
                    userlist.Insert(id,obj);
                }
            }
        }
    }

    void useradd(char id[],char passwd[],int level,char name[])
    {
        User nowuser=stack.back();
        if(level>=nowuser.Privilege)//如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败
        {
            throw Error();
        }
        if(nowuser.Privilege<3)
        {
            throw Error();
        }
        else
        {
            if(!userlist.findval(id).empty())//如果 [UserID] 与已注册帐户重复则操作失败
            {
                throw Error();
            }
            else
            {
                User obj;
                obj.Privilege=level;
                strcpy(obj.UserID,id);
                strcpy(obj.Password,passwd);
                strcpy(obj.Username,name);
                userlist.Insert(id,obj);
            }
        }
    }

    void userselect(char isbn[])//选择图书
    {
        User nowuser=stack.back();
        if(nowuser.Privilege<3)
        {
            throw Error();
        }
        nowuser.isselect=true;
        strcpy(nowuser.selectiisdn,isbn);
    }

};

#endif //BOOKSTORE_USER_H
