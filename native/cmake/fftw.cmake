include(ExternalProject)

list(APPEND DEPENDENCIES ep_fftw)
ExternalProject_Add(ep_fftw
    URL https://www.fftw.org/fftw-3.3.10.tar.gz
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot)