cmake_minimum_required(VERSION 3.23)
project(aurora)

add_executable(ameshc ameshc.cpp)
target_link_libraries(ameshc PRIVATE aether Boost::headers Boost::program_options nlohmann_json::nlohmann_json)
