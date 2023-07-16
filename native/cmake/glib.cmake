include(ExternalProject)

list(APPEND DEPENDENCIES ep_glib)
ExternalProject_Add(ep_glib
    URL https://download.gnome.org/sources/glib/2.76/glib-2.76.4.tar.xz
    URL_HASH SHA256=5a5a191c96836e166a7771f7ea6ca2b0069c603c7da3cba1cd38d1694a395dda
    CONFIGURE_COMMAND
        ${Meson_EXECUTABLE} setup --prefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
    BUILD_COMMAND
        ${Meson_EXECUTABLE} compile -C <BINARY_DIR>
    INSTALL_COMMAND
        ${Meson_EXECUTABLE} install -C <BINARY_DIR>
)
list(APPEND EXTRA_CMAKE_ARGS -DGLIB_INCLUDE_1=${THIRD_PARTY_LIB_PATH}/include/glib-2.0 -DGLIB_INCLUDE_2=${THIRD_PARTY_LIB_PATH}/lib/glib-2.0/include)