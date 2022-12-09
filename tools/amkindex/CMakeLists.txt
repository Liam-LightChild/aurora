cmake_minimum_required(VERSION 3.23)
project(aurora)

add_executable(amkindex amkindex.cpp)
target_link_libraries(amkindex PRIVATE aether Boost::headers Boost::program_options)
