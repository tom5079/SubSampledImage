cmake_minimum_required(VERSION 3.26)

include(CMakePrintHelpers)

cmake_print_variables(GLIB_INCLUDE)

find_library(VIPS NAMES vips PATHS ${THIRD_PARTY_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)

add_library(ssi SHARED ssi.c)
set_target_properties(ssi PROPERTIES PUBLIC_HEADER ssi.h)

target_link_libraries(ssi
    ${VIPS_LIBRARIES})

target_include_directories(ssi PRIVATE
    ${THIRD_PARTY_LIB_PATH}/include
    ${GLIB_INCLUDE_1}
    ${GLIB_INCLUDE_2})

install(TARGETS ssi LIBRARY)
install(TARGETS ssi PUBLIC_HEADER)