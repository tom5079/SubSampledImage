cmake_minimum_required(VERSION 3.26)

option (USE_SUPERBUILD "superbuild" ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

if (USE_SUPERBUILD)
    project(superbuild NONE)

    set(THIRD_PARTY_LIB_PATH ${CMAKE_BINARY_DIR}/third-party)
    set_directory_properties(PROPERTIES EP_PREFIX ${THIRD_PARTY_LIB_PATH})

    set(DEPENDENCIES)
    set(EXTRA_CMAKE_ARGS)

    find_program(Meson_EXECUTABLE meson)
    if(NOT Meson_EXECUTABLE)
        message(FATAL_ERROR "Meson is required")
    endif()

    find_program(Ninja_EXECUTABLE ninja)
    if(NOT Meson_EXECUTABLE)
        message(FATAL_ERROR "Ninja is required")
    endif()

    include("cmake/glib.cmake")
    include("cmake/libvips.cmake")
    include("cmake/ssi.cmake")
    include("cmake/ssi_test.cmake")
    return()
else()
    project(ssi C)
endif()

add_subdirectory(src)