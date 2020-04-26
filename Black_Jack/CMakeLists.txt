cmake_minimum_required(VERSION 3.14)
project(Black_Jack)

set(CMAKE_CXX_STANDARD 11)

add_executable(Black_Jack main.cpp commonhandler.cpp commonhandler.h gamesettings.cpp gamesettings.h pregamehandler.cpp pregamehandler.h mainhandler.cpp mainhandler.h)