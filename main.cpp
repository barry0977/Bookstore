//
// Created by 24115 on 2023/12/23.
//
#include "blockchain.h"
#include "book.h"
#include "user.h"
#include "sentenceslice.h"
#include "error.h"

int main()
{
    try
    {
        Bookinf bookinf;
        Userinf userinf;
        while (true)
        {
            std::vector<std::string> token = readtoken();
            int length = token.size();
            if (token[0] == "exit" || token[0] == "quit")
            {
                if (length == 1)
                {
                    break;
                }
                else
                {
                    throw Error();
                }
            }
            else if (token[0] == "su")
            {

            }
            else if (token[0] == "logout")
            {

            }
            else if (token[0] == "register")
            {

            }
            else if (token[0] == "passwd")
            {

            }
            else if (token[0] == "useradd")
            {

            }
            else if (token[0] == "delete")
            {

            }
            else if (token[0] == "show")
            {
                if (token[1] == "finance") {}
            }
            else if (token[0] == "buy")
            {

            }
            else if (token[0] == "select")
            {

            }
            else if (token[0] == "modify")
            {

            }
            else if (token[0] == "import")
            {

            }
            else if (token[0] == "log")
            {

            }
            else if (token[0] == "report")
            {

            }
            else
            {
                throw Error();
            }
        }
    }
    catch(Error)
    {
        error();
    }
    return 0;
}