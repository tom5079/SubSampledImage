declare -A ARCHS=(
    [aarch64]="arm64-v8a"
    [armv7a]="armeabi-v7a"
    [i686]="x86"
    [x86_64]="x86_64"
)

LIBRARIES=(
    "libssi.so"
    "libvips.so"
    "libglib-2.0.so"
    "libgio-2.0.so"
    "libgmodule-2.0.so"
    "libgobject-2.0.so"
    "libgthread-2.0.so"
    "libexpat.so"
    "libjpeg.so"
    "libtiff.so"
    "libspng.so"
    "libfftw3.so"
    "libwebp.so"
    "libintl.so"
    "libsharpyuv.so"
    "libwebpmux.so"
    "libwebpdemux.so"
    "libiconv.so"
    "libffi.so"
    "libheif.so"
    "libde265.so"
    "libdav1d.so"
)

for arch in "${!ARCHS[@]}"; do
    source_folder=$(readlink -f build-ndk-$arch/fakeroot/lib)
    target_folder=$(readlink -f ../library/src/androidMain/jniLibs/${ARCHS[$arch]})

    [ -d $target_folder ] || mkdir $target_folder

    for library in ${LIBRARIES[*]}; do
        [[ -f "$target_folder/$library" ]] || ln -s $source_folder/$library $target_folder/$library
    done
done