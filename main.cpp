//
// Created by 24115 on 2023/12/23.
//
#include "blockchain.h"
#include "book.h"
#include "user.h"
#include "sentenceslice.h"
#include "error.h"
#include "financelog.h"
#include "log.h"
#include "stafflog.h"
#include <limits>

std::vector<User>stack;
Bookinf bookinf;
Userinf userinf;
Financelog<double> financeinf;//财务记录
Financeinf financeinf1;//财务报表
Userlog userlog;//员工工作情况
Log log;//系统工作日志

int main()
{
//    freopen("in.txt","r",stdin);
//    freopen("out.txt","w",stdout);
    std::string input;
    while (std::getline(std::cin, input))
    {
        try
        {
            std::vector<std::string> token = readtokens(input);
            if(token.empty())
            {
                break;
            }
            int l = token.size();
            for(int i=0;i<l;i++)
            {
                token[i]=trim(token[i]);
            }
            if (token[0] == "exit" || token[0] == "quit")
            {
                if (l > 1)
                {
                    throw Error();
                }
                return 0;
            }
            else if (token[0] == "su")
            {
                if (l == 2)
                {
                    if (token[1].size() > 30 || !is1(token[1]))
                    {
                        throw Error();
                    }
                    char id[35];
                    strcpy(id, token[1].c_str());
                    userinf.su(id);
                }
                else if (l == 3)
                {
                    if (token[1].size() > 30 || token[2].size() > 30 || !is1(token[1]) || !is1(token[2]))
                    {
                        throw Error();
                    }
                    char id[35], pw[35];
                    strcpy(id, token[1].c_str());
                    strcpy(pw, token[2].c_str());
                    userinf.su(id, pw);
                }
                else
                {
                    throw Error();
                }
            }
            else if (token[0] == "logout")
            {
                if (l > 1)
                {
                    throw Error();
                }
                userinf.logout();
            }
            else if (token[0] == "register")
            {
                if (l != 4)
                {
                    throw Error();
                }
                if (token[1].size() > 30 || token[2].size() > 30 || token[3].size() > 30 || !is1(token[1]) || !is1(token[2]) || !is2(token[3]))
                {
                    throw Error();
                }
                char id[35], pw[35], name[35];
                strcpy(id, token[1].c_str());
                strcpy(pw, token[2].c_str());
                strcpy(name, token[3].c_str());
                userinf.regist(id, pw, name);
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"register");
                    strcpy(infor.obj,id);
                    strcpy(infor.obj2,pw);
                    strcpy(infor.obj3,name);
                    log.write(infor);
                }
            }
            else if (token[0] == "passwd")
            {
                if (l == 3)
                {
                    if (token[1].size() > 30 || token[2].size() > 30 || !is1(token[1]) || !is1(token[2]))
                    {
                        throw Error();
                    }
                    char id[35], npw[35];
                    strcpy(id, token[1].c_str());
                    strcpy(npw, token[2].c_str());
                    userinf.passwd(id, npw);
                    if(!stack.empty())
                    {
                        User sub = stack.back();
                        loginf infor;
                        infor.mode = 1;
                        strcpy(infor.id,sub.UserID);
                        strcpy(infor.act,"revise password");
                        strcpy(infor.obj,id);
                        strcpy(infor.obj2,npw);
                        log.write(infor);
                    }
                }
                else if (l == 4)
                {
                    if (token[1].size() > 30 || token[2].size() > 30 || token[3].size() > 30 || !is1(token[1]) || !is1(token[2]) || !is1(token[3]))
                    {
                        throw Error();
                    }
                    char id[35], npw[35], cpw[35];
                    strcpy(id, token[1].c_str());
                    strcpy(cpw, token[2].c_str());
                    strcpy(npw, token[3].c_str());
                    userinf.passwd(id, npw, cpw);
                    if(!stack.empty())
                    {
                        User sub = stack.back();
                        loginf infor;
                        infor.mode = 1;
                        strcpy(infor.id,sub.UserID);
                        strcpy(infor.act,"revise password");
                        strcpy(infor.obj,id);
                        strcpy(infor.obj2,npw);
                        log.write(infor);
                    }
                }
                else
                {
                    throw Error();
                }
            }
            else if (token[0] == "useradd")
            {
                if (l != 5)
                {
                    throw Error();
                }
                if (token[1].size() > 30 || token[2].size() > 30 || token[4].size() > 30 || !is1(token[1]) || !is1(token[2]) || !is2(token[4]))
                {
                    throw Error();
                }
                char id[35], pw[35], name[35];
                int pri = stringToInteger(token[3]);
                if (pri != 1 && pri != 3 && pri != 7)
                {
                    throw Error();
                }
                strcpy(id, token[1].c_str());
                strcpy(pw, token[2].c_str());
                strcpy(name, token[3].c_str());
                userinf.useradd(id, pw, pri, name);
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"add user");
                    strcpy(infor.obj,id);
                    strcpy(infor.obj2,pw);
                    strcpy(infor.obj3,name);
                    log.write(infor);
                }
            }
            else if (token[0] == "delete")
            {
                if (l != 2)
                {
                    throw Error();
                }
                if (token[1].size() > 30 || !is1(token[1]))
                {
                    throw Error();
                }
                char id[35];
                strcpy(id, token[1].c_str());
                userinf.Delete(id);
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"delete");
                    strcpy(infor.obj,id);
                    log.write(infor);
                }
            }
            else if (token[0] == "show")
            {
                if (l == 1)
                {
                    bookinf.show();
                    if(!stack.empty())
                    {
                        User sub = stack.back();
                        loginf infor;
                        infor.mode = 1;
                        strcpy(infor.id,sub.UserID);
                        strcpy(infor.act,"show");
                        log.write(infor);
                    }
                }
                else if (l == 2)
                {
                    if (token[1] == "finance")
                    {
                        financeinf.show();
                        if(!stack.empty())
                        {
                            User sub = stack.back();
                            loginf infor;
                            infor.mode = 1;
                            strcpy(infor.id,sub.UserID);
                            strcpy(infor.act,"show finance");
                            log.write(infor);
                        }
                    }
                    else
                    {
                        bookinf.show(token[1]);
                        if(!stack.empty())
                        {
                            User sub = stack.back();
                            loginf infor;
                            infor.mode = 1;
                            strcpy(infor.id,sub.UserID);
                            strcpy(infor.act,"show");
                            strcpy(infor.obj,token[1].c_str());
                            log.write(infor);
                        }
                    }
                }
                else if (l == 3 && token[1] == "finance")
                {
                    long long count = stringToInteger(token[2]);
                    financeinf.show(count);
                    if(!stack.empty())
                    {
                        User sub = stack.back();
                        loginf infor;
                        infor.mode = 1;
                        strcpy(infor.id,sub.UserID);
                        strcpy(infor.act,"show finance");
                        strcpy(infor.obj,token[2].c_str());
                        log.write(infor);
                    }
                }
                else
                {
                    throw Error();
                }
            }
            else if (token[0] == "buy")
            {
                if (l != 3)
                {
                    throw Error();
                }
                if (token[1].size() > 20 || !is2(token[1]))
                {
                    throw Error();
                }
                long long quantity = stringToInteger(token[2]);
                char isbn[25];
                strcpy(isbn, token[1].c_str());
                bookinf.buy(isbn, quantity);
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"buy");
                    strcpy(infor.obj,isbn);
                    strcpy(infor.obj2,token[2].c_str());
                    log.write(infor);
                }
            }
            else if (token[0] == "select")
            {
                if (l != 2)
                {
                    throw Error();
                }
                if (token[1].size() > 20 || !is2(token[1]))
                {
                    throw Error();
                }
                //std::cout<<"enter select\n";
                char isbn[25];
                strcpy(isbn, token[1].c_str());
                bookinf.select(isbn);
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"select");
                    strcpy(infor.obj,isbn);
                    log.write(infor);
                }
            }
            else if (token[0] == "modify")
            {
              bookinf.Modify(token);
            }
            else if (token[0] == "import")
            {
                if (l != 3)
                {
                    throw Error();
                }
                long long num = stringToInteger(token[1]);
                double cost = stringToReal(token[2]);
                bookinf.import(num, cost);
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"import");
                    strcpy(infor.obj,sub.selectisdn);
                    strcpy(infor.obj2,token[1].c_str());
                    strcpy(infor.obj3,token[2].c_str());
                    log.write(infor);
                }
            }
            else if (token[0] == "log")
            {
                if (l != 1)
                {
                    throw Error();
                }
                log.show();
                if(!stack.empty())
                {
                    User sub = stack.back();
                    loginf infor;
                    infor.mode = 1;
                    strcpy(infor.id,sub.UserID);
                    strcpy(infor.act,"log");
                    log.write(infor);
                }
            }
            else if (token[0] == "report")
            {
                if (l != 2)
                {
                    throw Error();
                }
                if (token[1] == "finance")
                {
                    financeinf1.show();
                    if(!stack.empty())
                    {
                        User sub = stack.back();
                        loginf infor;
                        infor.mode = 1;
                        strcpy(infor.id,sub.UserID);
                        strcpy(infor.act,"show finance");
                        log.write(infor);
                    }
                }
                else if (token[1] == "employee")
                {
                    if(stack.empty())
                    {
                        throw Error();
                    }
                    User nowuser=stack.back();
                    if(nowuser.Privilege<7)
                    {
                        throw Error();
                    }
                    userlog.show();
                    if(!stack.empty())
                    {
                        User sub = stack.back();
                        loginf infor;
                        infor.mode = 1;
                        strcpy(infor.id,sub.UserID);
                        strcpy(infor.act,"show employee");
                        log.write(infor);
                    }
                }
                else
                {
                    throw Error();
                }
            }
            else
            {
                if(!token.empty())
                {
                    throw Error();
                }
            }
        }
        catch (Error)
        {
            error();
        }
        //bookinf.show();
    }
    return 0;
}