include(ExternalProject)

list(APPEND DEPENDENCIES ep_mozjpeg)
ExternalProject_Add(ep_mozjpeg
    GIT_REPOSITORY      https://github.com/mozilla/mozjpeg.git
    GIT_TAG             v4.1.1
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot
        -DCMAKE_INSTALL_DEFAULT_LIBDIR=lib
        -DPNG_SUPPORTED=off
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
)