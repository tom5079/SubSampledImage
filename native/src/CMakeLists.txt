cmake_minimum_required(VERSION 3.26)

project(ssi C)

find_library(VIPS_LIBRARIES NAMES vips PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
find_library(GLIB_LIBRARIES NAMES glib-2.0 gio-2.0 gmodule-2.0 gobject-2.0 gthread-2.0 PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)

add_library(ssi STATIC ssi.c)
set_target_properties(ssi PROPERTIES PUBLIC_HEADER ssi.h)

target_link_libraries(ssi
    ${VIPS_LIBRARIES}
    ${GLIB_LIBRARIES})

target_include_directories(ssi PRIVATE
    ${CUSTOM_LIB_PATH}/include
    ${GLIB_INCLUDE_1}
    ${GLIB_INCLUDE_2})

install(TARGETS ssi LIBRARY)
install(TARGETS ssi PUBLIC_HEADER)