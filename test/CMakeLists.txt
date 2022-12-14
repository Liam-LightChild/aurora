cmake_minimum_required(VERSION 3.23)
project(test)

include(../aurora.cmake)

a_add_executable(test main.cpp assets/test.jpg assets/test.alvl.xml assets/test.obj)
