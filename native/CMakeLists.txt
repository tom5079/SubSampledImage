cmake_minimum_required(VERSION 3.26)

option (USE_SUPERBUILD "superbuild" ON)

if (USE_SUPERBUILD)
    project(superbuild NONE)

    include(CTest)

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

    include("cmake/expat.cmake")
    include("cmake/glib.cmake")
    include("cmake/mozjpeg.cmake")
    include("cmake/tiff.cmake")
    include("cmake/webp.cmake")
    include("cmake/spng.cmake")
    include("cmake/fftw.cmake")
    include("cmake/vips.cmake")
    include("cmake/ssi.cmake")
    include("cmake/test.cmake")
    return()
else()
    project(ssi C)
endif()

add_subdirectory(src)