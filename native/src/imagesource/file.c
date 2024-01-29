#include <jni.h>
#include <vips/vips.h>

#include "../arch.h"

JNIEXPORT jobject JNICALL
Java_xyz_quaver_graphics_subsampledimage_FileImageSource_load(JNIEnv *env,
                                                              jobject this,
                                                              jstring file) {
    const char *filename = (*env)->GetStringUTFChars(env, file, NULL);

    VipsSource *vipsSource = vips_source_new_from_file(filename);

    if (!vipsSource) {
        return NULL;
    }

    return newLongObject(env, vipsSource);
}

JNIEXPORT void JNICALL
Java_xyz_quaver_graphics_subsampledimage_FileImageSource_close(JNIEnv *env,
                                                               jobject this) {
    jclass class = (*env)->GetObjectClass(env, this);

    jmethodID getVipsSource =
        (*env)->GetMethodID(env, class, "getVipsSource", "()J");
    VipsSource *vipsSource =
        (VipsSource *)((*env)->CallLongMethod(env, this, getVipsSource));

    g_object_unref(vipsSource);

    jfieldID vipsSourceField =
        (*env)->GetFieldID(env, class, "_vipsSource", "Ljava/lang/Long;");
    (*env)->SetObjectField(env, this, vipsSourceField, NULL);
    return;
}