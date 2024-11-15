add_library(Graphviz INTERFACE)


find_path(GRAPHVIZ_INCLUDE_DIR graphviz/cgraph.h)

target_include_directories(
    Graphviz
    INTERFACE
        $<BUILD_INTERFACE:${GRAPHVIZ_INCLUDE_DIR}>)


find_library(GRAPHVIZ_GVC_LIBRARY gvc)
find_library(GRAPHVIZ_CGRAPH_LIBRARY cgraph)
find_library(GRAPHVIZ_CDT_LIBRARY cdt)
find_library(GRAPHVIZ_GVPLUGIN_CORE_LIBRARY gvplugin_core)
find_library(GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY gvplugin_dot_layout)
find_library(GRAPHVIZ_GVPLUGIN_GD_LIBRARY gvplugin_gd)
find_library(GRAPHVIZ_GVPLUGIN_GDIPLUS_LIBRARY gvplugin_gdiplus)
find_library(GRAPHVIZ_GVPLUGIN_NEATO_LAYOUT_LIBRARY gvplugin_neato_layout)
find_library(GRAPHVIZ_GVPLUGIN_PANGO_LIBRARY gvplugin_pango)
find_library(GRAPHVIZ_GVPLUGIN_RSVG_LIBRARY gvplugin_rsvg)
find_library(GRAPHVIZ_GVPLUGIN_WEBP_LIBRARY gvplugin_webp)

target_link_libraries(
    Graphviz
    INTERFACE
        ${GRAPHVIZ_GVC_LIBRARY}
        ${GRAPHVIZ_CGRAPH_LIBRARY}
        ${GRAPHVIZ_CDT_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_CORE_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_GD_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_GDIPLUS_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_NEATO_LAYOUT_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_PANGO_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_RSVG_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_WEBP_LIBRARY})


include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/Graphviz-config.cmake.in Graphviz-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Graphviz
    PATH_VARS CMAKE_INSTALL_DATADIR
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Graphviz-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Graphviz
    COMPONENT Graphviz_Development)

install(
    TARGETS Graphviz
    EXPORT Graphviz
    RUNTIME COMPONENT Graphviz_Runtime
    LIBRARY COMPONENT Graphviz_Development
    ARCHIVE COMPONENT Graphviz_Development
    FILE_SET api COMPONENT Graphviz_Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT Graphviz
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Graphviz
    FILE Graphviz-targets.cmake
    NAMESPACE Graphviz::
    COMPONENT Graphviz_Development)
