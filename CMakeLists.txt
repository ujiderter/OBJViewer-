cmake_minimum_required(VERSION 3.30)
project(OBJViewer_)

set(CMAKE_CXX_STANDARD 20)

add_executable(OBJViewer_ main.cpp
        OBJModel.h)
