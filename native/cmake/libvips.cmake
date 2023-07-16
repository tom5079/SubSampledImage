include(ExternalProject)

list(APPEND DEPENDENCIES ep_vips)
ExternalProject_Add(ep_vips
    GIT_REPOSITORY      https://github.com/libvips/libvips.git
    GIT_TAG             v8.14.2
    CONFIGURE_COMMAND
        ${Meson_EXECUTABLE} setup --prefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
    BUILD_COMMAND
        ${Ninja_EXECUTABLE} -C <BINARY_DIR>
    INSTALL_COMMAND
        ${Ninja_EXECUTABLE} -C <BINARY_DIR> install
)