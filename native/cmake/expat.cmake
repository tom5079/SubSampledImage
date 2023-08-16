include(ExternalProject)

list(APPEND DEPENDENCIES ep_expat)
ExternalProject_Add(ep_expat
    GIT_REPOSITORY https://github.com/libexpat/libexpat.git
    GIT_TAG R_2_5_0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot
    SOURCE_SUBDIR expat)