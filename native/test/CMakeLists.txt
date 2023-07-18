cmake_minimum_required(VERSION 3.26)

project(ssi_test C)

add_executable(ssi_test test.c)

find_library(SSI NAMES ssi PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)

target_link_libraries(ssi_test ${SSI_LIBRARIES})

target_include_directories(ssi_test PRIVATE
    ${CUSTOM_LIB_PATH}/include
    ${CUSTOM_LIB_PATH}/include/glib-2.0
    ${CUSTOM_LIB_PATH}/lib/glib-2.0/include)