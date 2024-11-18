add_library(imgraphviz STATIC)
add_library(imgraphviz::imgraphviz ALIAS imgraphviz)

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE header_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(
    imgraphviz
    PRIVATE ${source_files}
    PUBLIC
        FILE_SET api
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
        FILES ${header_files})


target_include_directories(
    imgraphviz
    PUBLIC
        $<BUILD_INTERFACE:${Graphviz_INCLUDE_DIRS}>)

target_link_libraries(imgraphviz
    PUBLIC
        imgui::imgui
        PkgConfig::Graphviz
        ${GRAPHVIZ_GVPLUGIN_CORE_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_DOT_LAYOUT_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_GD_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_GDIPLUS_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_NEATO_LAYOUT_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_PANGO_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_RSVG_LIBRARY}
        ${GRAPHVIZ_GVPLUGIN_WEBP_LIBRARY}
)


include(CMakePackageConfigHelpers)

configure_package_config_file(
    imgraphviz-config.cmake.in imgraphviz-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/imgraphviz
    PATH_VARS CMAKE_INSTALL_DATADIR
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/imgraphviz-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/imgraphviz
    COMPONENT imgraphviz_Development)

install(
    TARGETS imgraphviz
    EXPORT imgraphviz
    RUNTIME COMPONENT imgraphviz_Runtime
    LIBRARY COMPONENT imgraphviz_Development
    ARCHIVE COMPONENT imgraphviz_Development
    FILE_SET api COMPONENT imgraphviz_Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT imgraphviz
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/imgraphviz
    FILE imgraphviz-targets.cmake
    NAMESPACE imgraphviz::
    COMPONENT imgraphviz_Development)
