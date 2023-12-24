//
// Created by 24115 on 2023/12/23.
//
#include "blockchain.h"
#include "book.h"
#include "user.h"
#include "sentenceslice.h"
#include "error.h"
#include "financelog.h"

std::vector<User>stack;
Bookinf bookinf;
Userinf userinf;
Financelog<double> financeinf;

int main()
{
    try {
        std::string input;
        while (std::getline(std::cin,input))
        {
            std::vector<std::string> token = readtokens(input);
            int l = token.size();
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
                if (l== 2)
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
                    if(token[1].size()>30||token[2].size()>30||!is1(token[1])||!is1(token[2]))
                    {
                        throw Error();
                    }
                    char id[35],pw[35];
                    strcpy(id, token[1].c_str());
                    strcpy(pw,token[2].c_str());
                    userinf.su(id,pw);
                }
                else
                {
                    throw Error();
                }
            }
            else if (token[0] == "logout")
            {
                if(l>1)
                {
                    throw Error();
                }
                userinf.logout();
            }
            else if (token[0] == "register")
            {
                if(l!=4)
                {
                    throw Error();
                }
                if(token[1].size()>30||token[2].size()>30||token[3].size()>30||!is1(token[1])||!is1(token[2])||!is2(token[3]))
                {
                    throw Error();
                }
                char id[35],pw[35],name[35];
                strcpy(id,token[1].c_str());
                strcpy(pw,token[2].c_str());
                strcpy(name,token[3].c_str());
                userinf.regist(id,pw,name);
            }
            else if (token[0] == "passwd")
            {
                if(l==3)
                {
                    if(token[1].size()>30||token[2].size()||!is1(token[1])||!is1(token[2]))
                    {
                        throw Error();
                    }
                    char id[35],npw[35];
                    strcpy(id,token[1].c_str());
                    strcpy(npw,token[2].c_str());
                    userinf.passwd(id,npw);
                }
                else if(l==4)
                {
                    if(token[1].size()>30||token[2].size()||token[3].size()>30||!is1(token[1])||!is1(token[2])||!is1(token[3]))
                    {
                        throw Error();
                    }
                    char id[35],npw[35],cpw[35];
                    strcpy(id,token[1].c_str());
                    strcpy(cpw,token[2].c_str());
                    strcpy(npw,token[3].c_str());
                    userinf.passwd(id,npw,cpw);
                }
            }
            else if (token[0] == "useradd")
            {
                if(l!=5)
                {
                    throw Error();
                }
                if(token[1].size()>30||token[2].size()>30||token[4].size()||!is1(token[1])||!is1(token[2])||!is2(token[4]))
                {
                    throw Error();
                }
                char id[35],pw[35],name[35];
                int pri= stringToInteger(token[3]);
                if(pri!=1||pri!=3||pri!=7)
                {
                    throw Error();
                }
                strcpy(id,token[1].c_str());
                strcpy(pw,token[2].c_str());
                strcpy(name,token[3].c_str());
                userinf.useradd(id,pw,pri,name);
            }
            else if (token[0] == "delete")
            {
                if(l!=2)
                {
                    throw Error();
                }
                if(token[1].size()>30||!is1(token[1]))
                {
                    throw Error();
                }
                char id[35];
                strcpy(id,token[1].c_str());
                userinf.Delete(id);
            }
            else if (token[0] == "show")
            {
                if(l==1)
                {
                    bookinf.show();
                }
                else if(l==2)
                {
                    if(token[1]=="finance")
                    {
                        financeinf.show();
                    }
                    else
                    {
                        bookinf.show(token[1]);
                    }
                }
                else if(l==3&&token[1]=="finance")
                {
                    long long count= stringToInteger(token[2]);
                    financeinf.show(count);
                }
                else
                {
                    throw Error();
                }
            }
            else if (token[0] == "buy")
            {
                if(l!=3)
                {
                    throw Error();
                }
                if(token[1].size()>20||!is2(token[1]))
                {
                    throw Error();
                }
                long long quantity= stringToInteger(token[2]);
                char isbn[25];
                strcpy(isbn,token[1].c_str());
                bookinf.buy(isbn,quantity);
            }
            else if (token[0] == "select")
            {
                if(l!=2)
                {
                    throw Error();
                }
                if(token[1].size()>20||!is2(token[1]))
                {
                    throw Error();
                }
                char isbn[25];
                strcpy(isbn,token[1].c_str());
                userinf.userselect(isbn);
            }
            else if (token[0] == "modify")
            {
                bool is[5]{false};
                for(int i=1;i<l;i++)
                {
                    if(token[i][1]=='I')
                    {
                        if(is[0])
                        {
                            throw Error();
                        }
                        is[0]=true;
                        bookinf.modify(token[i]);
                    }
                    if(token[i][1]=='n')
                    {
                        if(is[1])
                        {
                            throw Error();
                        }
                        is[1]=true;
                        bookinf.modify(token[i]);
                    }
                    if(token[i][1]=='a')
                    {
                        if(is[2])
                        {
                            throw Error();
                        }
                        is[2]=true;
                        bookinf.modify(token[i]);
                    }
                    if(token[i][1]=='k')
                    {
                        if(is[3])
                        {
                            throw Error();
                        }
                        is[3]=true;
                        bookinf.modify(token[i]);
                    }
                    if(token[i][1]=='p')
                    {
                        if(is[4])
                        {
                            throw Error();
                        }
                        is[4]=true;
                        bookinf.modify(token[i]);
                    }
                    else
                    {
                        throw Error();
                    }
                }
            }
            else if (token[0] == "import")
            {
                if(l!=3)
                {
                    throw Error();
                }
                long long num= stringToInteger(token[1]);
                double cost= stringToReal(token[2]);
                bookinf.import(num,cost);
            }
            else if (token[0] == "log")
            {
                if(l!=1)
                {
                    throw Error();
                }
            }
            else if (token[0] == "report")
            {
                if(l!=2)
                {
                    throw Error();
                }
            }
            else
            {
                throw Error();
            }
        }
    }catch (Error )
    {
        error();
    }
    return 0;
}