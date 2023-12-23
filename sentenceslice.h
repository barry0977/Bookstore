//
// Created by 24115 on 2023/12/23.
//

#ifndef BOOKSTORE_SENTENCESLICE_H
#define BOOKSTORE_SENTENCESLICE_H
#include <iostream>
#include<sstream>
#include<vector>
std::vector<std::string> readtoken() {
    std::string input;
    std::getline(std::cin,input);//把一整行读入
    std::istringstream iss(input);
    std::vector<std::string>token;//储存以空格为分隔符的子串
    std::string substring;
    while (iss >> substring) {
        token.push_back(substring);
    }
}
#endif
