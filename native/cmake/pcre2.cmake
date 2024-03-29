include(ExternalProject)

list(APPEND DEPENDENCIES ep_pcre2)
ExternalProject_Add(ep_pcre2
    GIT_REPOSITORY      https://github.com/PCRE2Project/pcre2
    GIT_TAG             pcre2-10.42
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
)

