cmake_minimum_required(VERSION 3.26)

project(ssi_test C)

include(CTest)
include(CMakePrintHelpers)

set(CMAKE_FIND_DEBUG_MODE ON)

add_executable(ssi_test test.c)

find_library(VIPS_LIBRARIES NAMES vips PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(SSI_LIBRARIES NAMES ssi PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GLIB_LIBRARIES NAMES glib-2.0 gio-2.0 gmodule-2.0 gobject-2.0 gthread-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)

cmake_print_variables(GLIB_LIBRARIES)

target_link_libraries(ssi_test ${SSI_LIBRARIES} ${VIPS_LIBRARIES} ${GLIB_LIBRARIES})

target_include_directories(ssi_test PRIVATE
    ${CUSTOM_LIB_PATH}/include
    ${CUSTOM_LIB_PATH}/include/glib-2.0
    ${CUSTOM_LIB_PATH}/lib/glib-2.0/include)

enable_testing()
add_test(NAME ssi_test COMMAND ssi_test)