//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_SENTENCESLICE_H
#define BOOKSTORE_SENTENCESLICE_H
#include "error.h"
#include <iostream>
#include<sstream>
#include<vector>
std::vector<std::string> readtoken()//把读入的语句以空格为分隔符切片
{
    std::string input;
    std::getline(std::cin, input);//把一整行读入
    std::istringstream iss(input);
    std::vector<std::string>token;//储存以空格为分隔符的子串
    std::string substring;
    while (iss >> substring) {
        token.push_back(substring);
    }
    return token;
}

std::vector<std::string> readtokens(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        result.push_back(token);
    }

    return result;
}

std::vector<std::string> readkey(std::string input)//把读入的语句以|为分隔符切片
{
    std::vector<std::string> subStrings;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, '|'))
    {
        subStrings.push_back(token);
    }
    return subStrings;
}

bool is1(const std::string& str) {//是否都是字母数字下划线
    for (char c : str) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}

bool is2(const std::string& str) {//是否都是除不可见字符以外ASCII字符
    for (char c : str) {
        if (!isprint(c)) {
            return false;
        }
    }
    return true;
}

bool is3(const std::string& str) {//是否都是除不可见字符和英文双引号以外ASCII字符
    for (char c : str) {
        if (!isprint(c) && c != '"') {
            return false;
        }
    }
    return true;
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

std::string trim(std::string str) {
    int finish = str.length() - 1;
    while (finish >= 0 && isspace(str[finish])) {
        finish--;
    }
    int start = 0;
    while (start <= finish && isspace(str[start])) {
        start++;
    }
    return str.substr(start, finish - start + 1);
}
#endif
