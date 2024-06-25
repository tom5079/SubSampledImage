include(ExternalProject)

ExternalProject_Add(ep_highway
    GIT_REPOSITORY      https://github.com/google/highway
    GIT_TAG             1.2.0
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
        -DHWY_ENABLE_TESTS=OFF
        -DBUILD_SHARED_LIBS=ON
)

