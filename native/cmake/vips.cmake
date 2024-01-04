include(ExternalProject)

list(APPEND DEPENDENCIES ep_vips)

if ($ENV{TARGET} MATCHES ".*android.*")
    set(FIX_ANDROID_BUILD sed -i "s! /usr/lib/libz.so!!g" <BINARY_DIR>/build.ninja &&)
endif()

ExternalProject_Add(ep_vips
    GIT_REPOSITORY      https://github.com/libvips/libvips.git
    GIT_TAG             v8.14.5
    CONFIGURE_COMMAND
        PKG_CONFIG_PATH=${THIRD_PARTY_LIB_PATH}/lib/pkgconfig LD_LIBRARY_PATH=${THIRD_PARTY_LIB_PATH}/lib:$ENV{LD_LIBRARY_PATH} ${Meson_EXECUTABLE} setup ${MESON_CROSS_FILE_ARG} --default-library shared --prefix=<INSTALL_DIR>
            -Dexamples=false
            -Dintrospection=false
            -Dmagick=disabled
            -Dpangocairo=disabled
            -Dpoppler=disabled
            -Dopenexr=disabled
            -Djpeg-xl=disabled
            -Dlcms=disabled
            -Dexif=disabled
            -Dopenjpeg=disabled
            -Dorc=disabled
            -Dpng=disabled
            -Dspng=enabled
            -Dcplusplus=false
            -Drsvg=disabled
            -Dpdfium=disabled
            -Dopenslide=disabled
            -Dnifti=disabled
            -Dmatio=disabled
            <BINARY_DIR> <SOURCE_DIR>
    BUILD_COMMAND
        ${FIX_ANDROID_BUILD} ${Ninja_EXECUTABLE} -C <BINARY_DIR>
    INSTALL_COMMAND
        ${Ninja_EXECUTABLE} -C <BINARY_DIR> install
)