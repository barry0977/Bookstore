//
// Created by 24115 on 2023/12/23.
//

#ifndef CODE_ERROR_H
#define CODE_ERROR_H

#include <string>
#include <exception>
class Error {
public:
    Error() {};

private:
    std::string message;
};

void error()
{
    std::cout << "Invalid\n";
}
#endif //CODE_ERROR_H
