cmake_minimum_required(VERSION 3.23)
project(aurora)

add_executable(ashaderc ashaderc.cpp)
target_link_libraries(ashaderc PRIVATE aether XercesC::XercesC Boost::headers Boost::program_options nlohmann_json::nlohmann_json)
