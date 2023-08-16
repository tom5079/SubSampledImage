include(ExternalProject)

list(APPEND DEPENDENCIES ep_spng)
ExternalProject_Add(ep_spng
    GIT_REPOSITORY https://github.com/randy408/libspng.git
    GIT_TAG v0.7.4
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot)