include(ExternalProject)

list(APPEND DEPENDENCIES ep_tiff)
ExternalProject_Add(ep_tiff
    URL http://download.osgeo.org/libtiff/tiff-4.5.1.tar.gz
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fakeroot -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/fakeroot
)