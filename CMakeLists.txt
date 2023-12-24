cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(code main.cpp
        blockchain.h
        sentenceslice.h
        book.h
        user.h
        error.h
        financelog.h
)
