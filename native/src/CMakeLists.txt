cmake_minimum_required(VERSION 3.26)

project(ssi C)

include(CMakeFindDependencyMacro)

add_library(ssi SHARED ssi.c jni/fileimagesource.c jni/vipsimage.c)
set_target_properties(ssi PROPERTIES PUBLIC_HEADER ssi.h)

find_library(VIPS_LIB NAMES vips PATHS ${CUSTOM_LIB_PATH} PATH_SUFFIXES lib REQUIRED NO_DEFAULT_PATH)
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
find_package(JNI REQUIRED)

target_link_libraries(ssi
    JNI::JNI
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

target_include_directories(ssi PRIVATE
    ${CUSTOM_LIB_PATH}/include
    ${CUSTOM_LIB_PATH}/include/glib-2.0
    ${CUSTOM_LIB_PATH}/lib/glib-2.0/include
    ${JNI_INCLUDE_DIRS})

install(TARGETS ssi LIBRARY)
install(TARGETS ssi PUBLIC_HEADER)