//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_SENTENCESLICE_H
#define BOOKSTORE_SENTENCESLICE_H
#include "error.h"
#include <iostream>
#include<sstream>
#include<vector>
std::vector<std::string> readtoken()//把读入的语句切片
{
    std::string input;
    std::getline(std::cin,input);//把一整行读入
    std::istringstream iss(input);
    std::vector<std::string>token;//储存以空格为分隔符的子串
    std::string substring;
    while (iss >> substring) {
        token.push_back(substring);
    }
}

std::string integerToString(int n)
{
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

int stringToInteger(std::string str)
{

    std::istringstream stream(str);
    int value;
    stream >> value;
    if (!stream.eof()) stream >> std::ws;
    if (stream.fail() || !stream.eof())
    {
        throw Error();
    }
    return value;
}

std::string realToString(double d)
{
    std::ostringstream stream;
    stream << std::uppercase << d;
    return stream.str();
}

double stringToReal(std::string str)
{
    std::istringstream stream(str);
    double value;
    stream >> value;
    if (!stream.eof()) stream >> std::ws;
    if (stream.fail() || !stream.eof())
    {
        throw Error();
    }
    return value;
}
#endif
