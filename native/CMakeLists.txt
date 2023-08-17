cmake_minimum_required(VERSION 3.26)

option (USE_SUPERBUILD "superbuild" ON)

if (USE_SUPERBUILD)
    project(superbuild NONE)

    include(CTest)
    include(CMakeFindDependencyMacro)

    set(THIRD_PARTY_LIB_PATH ${CMAKE_BINARY_DIR}/fakeroot)
    set_directory_properties(PROPERTIES EP_PREFIX ${THIRD_PARTY_LIB_PATH})

    set(DEPENDENCIES)
    set(EXTRA_CMAKE_ARGS)

    find_program(Meson_EXECUTABLE meson)
    if(NOT Meson_EXECUTABLE)
        message(FATAL_ERROR "Meson is required")
    endif()

    find_program(Ninja_EXECUTABLE ninja)
    if(NOT Ninja_EXECUTABLE)
        message(FATAL_ERROR "Ninja is required")
    endif()

    find_program(CTest_EXECUTABLE ctest)
    if(NOT CTest_EXECUTABLE)
        message(FATAL_ERROR "CTest is required")
    endif()

    find_library(EXPAT_LIB NAMES expat PATHS ${THIRD_PARTY_LIB_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    if(NOT EXPAT_LIB)
        include("cmake/expat.cmake")
    endif()

    find_library(GLIB_LIB NAMES glib-2.0 PATHS ${THIRD_PARTY_LIB_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    if(NOT GLIB_LIB)
        include("cmake/glib.cmake")
    endif()

    find_package(mozjpeg PATHS ${THIRD_PARTY_LIB_PATH} NO_DEFAULT_PATH)
    if(NOT mozjpeg_FOUND)
        include("cmake/mozjpeg.cmake")
    endif()

    find_package(Tiff PATHS ${THIRD_PARTY_LIB_PATH} NO_DEFAULT_PATH)
    if(NOT Tiff_FOUND)
        include("cmake/tiff.cmake")
    endif()

    find_package(WebP PATHS ${THIRD_PARTY_LIB_PATH} NO_DEFAULT_PATH)
    if(NOT WebP_FOUND)
        include("cmake/webp.cmake")
    endif()

    find_package(SPNG PATHS ${THIRD_PARTY_LIB_PATH} NO_DEFAULT_PATH)
    if(NOT SPNG_FOUND)
        include("cmake/spng.cmake")
    endif()

    find_package(FFTW3 PATHS ${THIRD_PARTY_LIB_PATH} NO_DEFAULT_PATH)
    if(NOT FFTW3_FOUND)
        include("cmake/fftw.cmake")
    endif()

    find_library(VIPS_LIB NAMES vips PATHS ${THIRD_PARTY_LIB_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    if(NOT VIPS_LIB)
        include("cmake/vips.cmake")
    endif()
    include("cmake/ssi.cmake")
    include("cmake/test.cmake")
    return()
else()
    project(ssi C)
endif()

add_subdirectory(src)