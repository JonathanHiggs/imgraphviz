cmake_minimum_required(VERSION 3.27)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

file(READ "VERSION.txt" IMGRAPHVIZ_VERSION)

project(
    imgraphviz
    VERSION 0.0.1
    DESCRIPTION "A graphviz plotter for ImGui"
    HOMEPAGE_URL "https://github.com/jonathanhiggs/imgraphviz"
    LANGUAGES CXX)

include(GNUInstallDirs)

message(STATUS "CMAKE_INSTALL_INCLUDEDIR    ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR        ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "CMAKE_INSTALL_DATADIR       ${CMAKE_INSTALL_DATADIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CMAKE_OPTIMIZE_DEPENDENCIES ON)

option(IMGRAPHVIZ_BUILD_EXAMPLE "Build the example app" OFF)


# Dependencies

add_definitions(-DGLFW_INCLUDE_NONE)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

find_package(fmt            CONFIG REQUIRED)
find_package(glad           CONFIG REQUIRED)
find_package(glfw3          CONFIG REQUIRED)
find_package(imgui          CONFIG REQUIRED)
find_package(Graphviz              REQUIRED)
find_package(OpenGL                REQUIRED COMPONENTS OpenGL)


# Targets

add_subdirectory(src)

if(IMGRAPHVIZ_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
