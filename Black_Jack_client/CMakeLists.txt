cmake_minimum_required(VERSION 3.14)
project(Black_Jack_client)

set(CMAKE_CXX_STANDARD 11)

# include_directories(/usr/local/include/)

add_executable(Black_Jack_client main.cpp client_processor.cpp client_processor.h)