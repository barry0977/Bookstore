//
// Created by 24115 on 2023/12/23.
//

#ifndef CODE_ERROR_H
#define CODE_ERROR_H

#include <string>
#include <exception>
class ErrorException : public std::exception {
public:
    explicit ErrorException();

    std::string getMessage() const;

private:
    std::string message;
};

#endif //CODE_ERROR_H
