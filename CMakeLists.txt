cmake_minimum_required(VERSION 3.23)
project(aurora)

if(VCPKG_PATH)
    include(${VCPKG_PATH})
endif()

set(CMAKE_CXX_STANDARD 20)

find_package(XercesC CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS log program_options)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SailC++ CONFIG REQUIRED)

add_library(aether STATIC aurora/aether/aether.cpp aurora/aether/aether.h)
target_include_directories(aether PUBLIC .)
target_link_libraries(aether PUBLIC glm::glm nlohmann_json::nlohmann_json)

add_subdirectory(tools)

add_library(aurora STATIC aurora/instance.cpp aurora/instance.h aurora/asset_loader.cpp aurora/asset_loader.h aurora/glimpl/opengl_impl.h aurora/graphics/implementation.cpp aurora/graphics/implementation.h aurora/graphics/implementation_finder.cpp aurora/graphics/implementation_finder.h aurora/glimpl/opengl_impl_node.cpp aurora/glimpl/opengl_impl_node.h aurora/glimpl/opengl_impl_node.cpp aurora/glimpl/opengl32_impl.cpp aurora/global.cpp aurora/global.h aurora/graphics/graphics.h aurora/graphics/obj_ref_base.cpp aurora/graphics/obj_ref_base.h aurora/resources/shader.cpp aurora/resources/shader.h aurora/window.cpp aurora/window.h aurora/resources.h aurora/application.cpp aurora/application.h aurora/resources/buffer.cpp aurora/resources/buffer.h aurora/resources/texture_2d.cpp aurora/resources/texture_2d.h aurora/resources/draw_object.cpp aurora/resources/draw_object.h aurora/resources/texture_1d.cpp)
target_link_libraries(aurora PUBLIC glfw GLEW::GLEW aether Boost::headers Boost::log Boost::program_options glm::glm SAIL::sail-c++)
target_include_directories(aurora PUBLIC .)

add_subdirectory(test)
