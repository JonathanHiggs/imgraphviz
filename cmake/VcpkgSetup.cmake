if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(NOT DEFINED "$ENV{VCPKG_ROOT}")
        message(FATAL_ERROR "Unable to setup vcpkg, ensure \"VCPKG_ROOT\" is configured as an environment variable or pass a CMAKE_TOOLCHAIN_FILE")
    endif()
endif()

if(IMGRAPHVIZ_BUILD_EXAMPLE)
    list(APPEND VCPKG_MANIFEST_FEATURES "example")
endif()
