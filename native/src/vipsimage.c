#include <jni.h>
#include <stdint.h>
#include <vips/vips.h>

#include "arch.h"

JNIEXPORT jint JNICALL
Java_xyz_quaver_graphics_subsampledimage_VipsImageImpl_readPixels(
    JNIEnv *env, jobject this, jbyteArray buffer, jint startX, jint startY,
    jint width, jint height, jint bufferOffset, jint stride) {
    jclass class = (*env)->GetObjectClass(env, this);

    jmethodID getVipsImage =
        (*env)->GetMethodID(env, class, "getVipsImage", "()J");
    VipsImage *in =
        (VipsImage *)((*env)->CallLongMethod(env, this, getVipsImage));

    const jsize length = (*env)->GetArrayLength(env, buffer);

    if (length < bufferOffset + ((height - 1) / stride + 1) * ((width - 1) / stride + 1)) {
        // Buffer too small
        return 1;
    }

    VipsRegion *region = vips_region_new(in);

    VipsRect image = {0, 0, in->Xsize, in->Ysize};
    VipsRect rect = {startX, startY, width, height};

    if (in->BandFmt != VIPS_FORMAT_UCHAR) {
        g_object_unref(region);
        return 2;
    }

    if (!vips_rect_includesrect(&image, &rect)) {
        // Rect out of bound
        g_object_unref(region);
        return 3;
    }

    if (vips_region_prepare(region, &rect)) {
        g_object_unref(region);
        return 4;
    }

    VipsPel *pel = VIPS_REGION_ADDR(region, startX, startY);
    size_t skip = VIPS_REGION_LSKIP(region);
    size_t bands = in->Bands;

    jbyte *buf = (*env)->GetByteArrayElements(env, buffer, NULL);
    jbyte *bufCopy = buf;

    buf += bufferOffset;

    for (int y = 0; y < height; y += stride) {
        VipsPel *pelCopy = pel;

        for (int x = 0; x < width; x += stride) {
            buf[0] = pel[0];
            buf[1] = pel[1];
            buf[2] = pel[2];
            buf[3] = bands == 4 ? pel[3] : 0xFF;

            pel += bands * stride;
            buf += 4;
        }

        pel = pelCopy + skip;
    }

    (*env)->ReleaseByteArrayElements(env, buffer, bufCopy, 0);

    g_object_unref(region);

    return 0;
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

    if (vips_image_guess_interpretation(image) != VIPS_INTERPRETATION_sRGB) {
        VipsImage *tmp;
        if (vips_colourspace(image, &tmp, VIPS_INTERPRETATION_sRGB)) {
            g_object_unref(image);
            return NULL;
        }

        g_object_unref(image);
        image = tmp;
    }

    return newLongObject(env, (jlong) image);
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
    (*env)->SetObjectField(env, this, vipsImageField, NULL);

    return;
}