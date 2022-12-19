cmake_minimum_required(VERSION 3.23)
project(aurora)

add_library(aether STATIC aether.cpp aether.h a_shader.cpp a_texture.cpp a_mesh.cpp a_opt_mesh.cpp a_level_controller.cpp a_level_object.cpp a_level.cpp)
target_include_directories(aether PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(aether PUBLIC glm::glm nlohmann_json::nlohmann_json)
