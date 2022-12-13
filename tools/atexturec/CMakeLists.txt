cmake_minimum_required(VERSION 3.23)
project(aurora)

add_executable(atexturec atexturec.cpp)
target_link_libraries(atexturec PRIVATE aether Boost::headers Boost::program_options nlohmann_json::nlohmann_json)

install(TARGETS atexturec CONFIGURATIONS Release RUNTIME)
