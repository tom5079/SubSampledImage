#include <jni.h>
#include <vips/vips.h>

JNIEXPORT void JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_readPixels(
    JNIEnv *env, jobject this, jintArray buffer, jint startX, jint startY,
    jint width, jint height, jint bufferOffset, jint stride) {
    jclass class = (*env)->GetObjectClass(env, this);

    jmethodID getVipsImage =
        (*env)->GetMethodID(env, class, "getVipsImage", "()J");
    VipsImage *in =
        (VipsImage *)((*env)->CallLongMethod(env, this, getVipsImage));

    VipsImage *out = vips_image_new();

    if (vips_image_pipelinev(out, VIPS_DEMAND_STYLE_THINSTRIP, in, NULL)) {
        g_object_unref(out);
        return;
    }

    out->Type = VIPS_INTERPRETATION_MULTIBAND;
    out->BandFmt = VIPS_FORMAT_INT;
    out->Bands = 1;

    if (vips_image_generate(out, vips_start_one, vips_sRGB2ARGB, vips_stop_one,
                            in, NULL)) {
        g_object_unref(out);
        return;
    }

    return;
}

JNIEXPORT jboolean JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_hasAlpha(JNIEnv *env,
                                                                jobject this,
                                                                jlong image) {
    return vips_image_hasalpha((VipsImage *)image);
}

JNIEXPORT jint JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_getHeight(JNIEnv *env,
                                                                 jobject this,
                                                                 jlong image) {
    return vips_image_get_height((VipsImage *)image);
}

JNIEXPORT jint JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_getWidth(JNIEnv *env,
                                                                jobject this,
                                                                jlong image) {
    return vips_image_get_width((VipsImage *)image);
}

JNIEXPORT jobject JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_load(JNIEnv *env,
                                                            jobject this,
                                                            jlong source) {
    VipsImage *image =
        vips_image_new_from_source((VipsSource *)source, "", NULL);

    if (!image) {
        return NULL;
    }

    printf(
        "format is %d, coding is %d, interpretation is %d, %d bands, %d pages",
        vips_image_guess_format(image), vips_image_get_coding(image),
        vips_image_guess_interpretation(image), vips_image_get_bands(image),
        vips_image_get_n_pages(image));

    if (vips_image_guess_interpretation(image) != VIPS_INTERPRETATION_sRGB) {
        VipsImage *tmp;
        if (vips_colourspace(image, &tmp, VIPS_INTERPRETATION_sRGB)) {
            g_object_unref(image);
            return NULL;
        }

        g_object_unref(image);
        image = tmp;
    }

    jclass longClass = (*env)->FindClass(env, "Ljava/lang/Long;");
    jmethodID longConstructor =
        (*env)->GetMethodID(env, longClass, "<init>", "(J)V");

    jobject longObject =
        (*env)->NewObject(env, longClass, longConstructor, image);
    return longObject;
}

JNIEXPORT void JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_close(JNIEnv *env,
                                                             jobject this) {
    jclass class = (*env)->GetObjectClass(env, this);

    jmethodID getVipsImage =
        (*env)->GetMethodID(env, class, "getVipsImage", "()J");
    VipsImage *image =
        (VipsImage *)((*env)->CallLongMethod(env, this, getVipsImage));

    g_object_unref(G_OBJECT(image));

    jfieldID vipsImageField =
        (*env)->GetFieldID(env, class, "_vipsImage", "Ljava/lang/Long;");
    (*env)->SetObjectField(env, this, vipsImageField, 0L);

    return;
}