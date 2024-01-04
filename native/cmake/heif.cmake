include(ExternalProject)

list(APPEND DEPENDENCIES ep_heif)
ExternalProject_Add(ep_heif
    GIT_REPOSITORY      https://github.com/strukturag/libheif.git
    GIT_TAG             v1.17.6
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DWITH_GDK_PIXBUF=OFF
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
        --preset=release
)