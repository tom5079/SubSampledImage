include(ExternalProject)

list(APPEND DEPENDENCIES ep_ffi)

if ($ENV{TARGET} MATCHES ".*android.*")
    set(HOST_FLAG --host $ENV{TARGET})
endif()

ExternalProject_Add(ep_ffi
    GIT_REPOSITORY      https://github.com/libffi/libffi.git
    GIT_TAG             v3.4.4
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND
        <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure ${HOST_FLAG} --prefix ${CMAKE_BINARY_DIR}/fakeroot
    BUILD_COMMAND
        ${Make_EXECUTABLE}
    INSTALL_COMMAND
        ${Make_EXECUTABLE} install
)