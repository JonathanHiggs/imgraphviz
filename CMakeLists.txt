cmake_minimum_required(VERSION 3.27)


# Options

option(IMGRAPHVIZ_BUILD_EXAMPLE "Build the example app" OFF)


# Project Setup

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(VcpkgSetup)

file(READ "VERSION.txt" IMGRAPHVIZ_VERSION)

project(
    imgraphviz
    VERSION 0.0.1
    DESCRIPTION "A graphviz plotter for ImGui"
    HOMEPAGE_URL "https://github.com/jonathanhiggs/imgraphviz"
    LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CMAKE_OPTIMIZE_DEPENDENCIES ON)

include(GNUInstallDirs)


# Dependencies

find_package(imgui          CONFIG REQUIRED)

find_package(PkgConfig)

pkg_check_modules(Graphviz REQUIRED IMPORTED_TARGET libgvc)

find_library(GRAPHVIZ_GVPLUGIN_CORE_LIBRARY gvplugin_core)
find_library(GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY gvplugin_dot_layout)
find_library(GRAPHVIZ_GVPLUGIN_GD_LIBRARY gvplugin_gd)
find_library(GRAPHVIZ_GVPLUGIN_GDIPLUS_LIBRARY gvplugin_gdiplus)
find_library(GRAPHVIZ_GVPLUGIN_NEATO_LAYOUT_LIBRARY gvplugin_neato_layout)
find_library(GRAPHVIZ_GVPLUGIN_PANGO_LIBRARY gvplugin_pango)
find_library(GRAPHVIZ_GVPLUGIN_RSVG_LIBRARY gvplugin_rsvg)
find_library(GRAPHVIZ_GVPLUGIN_WEBP_LIBRARY gvplugin_webp)


if(IMGRAPHVIZ_BUILD_EXAMPLE)
    add_definitions(-DGLFW_INCLUDE_NONE)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

    find_package(fmt            CONFIG REQUIRED)
    find_package(glad           CONFIG REQUIRED)
    find_package(glfw3          CONFIG REQUIRED)
    find_package(OpenGL                REQUIRED COMPONENTS OpenGL)
endif()


# Targets

add_subdirectory(src)

if(IMGRAPHVIZ_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
