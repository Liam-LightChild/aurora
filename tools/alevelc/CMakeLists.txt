cmake_minimum_required(VERSION 3.23)
project(aurora)

add_executable(alevelc alevelc.cpp)
target_link_libraries(alevelc PRIVATE aether XercesC::XercesC Boost::headers Boost::program_options nlohmann_json::nlohmann_json)

install(TARGETS alevelc CONFIGURATIONS Release RUNTIME)
