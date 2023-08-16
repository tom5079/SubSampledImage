include(ExternalProject)

list(APPEND DEPENDENCIES ep_webp)
ExternalProject_Add(ep_webp
    GIT_REPOSITORY https://chromium.googlesource.com/webm/libwebp
    GIT_TAG v1.2.1
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot -DBUILD_SHARED_LIBS=ON)