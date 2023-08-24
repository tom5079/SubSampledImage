cmake_minimum_required(VERSION 3.26)

project(ssi_test C)

include(CTest)
include(CMakeFindDependencyMacro)

find_library(VIPS_LIB NAMES vips PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(SSI_LIB NAMES ssi PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GLIB_LIB NAMES glib-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GIO_LIB NAMES gio-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GMODULE_LIB NAMES gmodule-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GOBJECT_LIB NAMES gobject-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GTHREAD_LIB NAMES gthread-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(EXPAT_LIB NAMES expat PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)

find_package(mozjpeg PATHS ${CUSTOM_LIB_PATH} REQUIRED NO_DEFAULT_PATH)
find_package(Tiff PATHS ${CUSTOM_LIB_PATH} REQUIRED NO_DEFAULT_PATH)
find_package(WebP PATHS ${CUSTOM_LIB_PATH} REQUIRED NO_DEFAULT_PATH)
find_package(SPNG PATHS ${CUSTOM_LIB_PATH} REQUIRED NO_DEFAULT_PATH)
find_package(FFTW3 PATHS ${CUSTOM_LIB_PATH} REQUIRED NO_DEFAULT_PATH)

set(TEST_LIBRARIES
    ${SSI_LIB}
    ${VIPS_LIB}
    ${GLIB_LIB}
    ${GIO_LIB}
    ${GMODULE_LIB}
    ${GOBJECT_LIB}
    ${GTHREAD_LIB}
    ${EXPAT_LIB}
    mozjpeg::jpeg
    TIFF::tiff
    spng::spng
    FFTW3::fftw3
    WebP::webp
    z
    m)

set(TEST_INCLUDES
    ${CUSTOM_LIB_PATH}/include
    ${CUSTOM_LIB_PATH}/include/glib-2.0
    ${CUSTOM_LIB_PATH}/lib/glib-2.0/include)

add_executable(ssi_test_jpg test_jpg.c)
target_link_libraries(ssi_test_jpg ${TEST_LIBRARIES})
target_include_directories(ssi_test_jpg PRIVATE ${TEST_INCLUDES})

enable_testing()
add_test(NAME ssi_test_jpg COMMAND ssi_test_jpg ${CMAKE_CURRENT_LIST_DIR}/image.jpg)