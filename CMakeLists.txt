cmake_minimum_required(VERSION 3.14)
project(AviaSales)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -Wall -Wextra -Werror -std=c++20")

set(SOURCES 
    aviasales.cpp
    database.cpp
    datetime.cpp
)

add_executable(aviasales ${SOURCES})