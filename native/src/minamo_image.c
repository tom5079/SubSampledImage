#include <jni.h>
#include <stdint.h>
#include <vips/vips.h>
#include <turbojpeg.h>

#include "arch.h"
#include "minamo_sink_callback.h"

VipsImage* MinamoImage_getVipsImage(JNIEnv *env, jobject this) {
    jclass class = (*env)->GetObjectClass(env, this);

    jmethodID getVipsImage =
        (*env)->GetMethodID(env, class, "getVipsImage", "()J");
    jlong image = (*env)->CallLongMethod(env, this, getVipsImage);

    return (VipsImage *)image;
}

JNIEXPORT jobject JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_decode(JNIEnv *env, jobject this,
                                                  jobject rect) {
    VipsImage* image = MinamoImage_getVipsImage(env, this);

    size_t x, y, width, height;
    {
        jclass rectClass = (*env)->GetObjectClass(env, rect);

        jfieldID xField = (*env)->GetFieldID(env, rectClass, "x", "I");
        jfieldID yField = (*env)->GetFieldID(env, rectClass, "y", "I");
        jfieldID widthField = (*env)->GetFieldID(env, rectClass, "width", "I");
        jfieldID heightField = (*env)->GetFieldID(env, rectClass, "height", "I");

        x = (*env)->GetIntField(env, rect, xField);
        y = (*env)->GetIntField(env, rect, yField);
        width = (*env)->GetIntField(env, rect, widthField);
        height = (*env)->GetIntField(env, rect, heightField);
    }

    VipsRect imageRect = {0, 0, image->Xsize, image->Ysize};
    VipsRect regionRect = {x, y, width, height};

    if (!vips_rect_includesrect(&imageRect, &regionRect)) {
        return NULL;
    }

    if (image->BandFmt != VIPS_FORMAT_UCHAR) {
        return NULL;
    }

    VipsRegion *region = vips_region_new(image);

    if (vips_region_prepare(region, &regionRect)) {
        g_object_unref(region);
        return NULL;
    }

    VipsPel *pel = VIPS_REGION_ADDR(region, x, y);
    size_t skip = VIPS_REGION_LSKIP(region);
    size_t bands = image->Bands;

#ifdef __ANDROID__
    jintArray dataArray = (jintArray) (*env)->NewIntArray(env, width * height);
    {
        jint* data = (*env)->GetIntArrayElements(env, dataArray, NULL);

        for (size_t i = 0; i < height; i++) {
            VipsPel* pelCopy = pel;

            for (size_t j = 0; j < width; j++) {
                if (bands == 1) {
                    data[i * width + j] = (0xFF << 24) | (pel[0] << 16) | (pel[0] << 8) | pel[0];
                } else if (bands == 3) {
                    data[i * width + j] = (0xFF << 24) | (pel[0] << 16) | (pel[1] << 8) | pel[2];
                } else if (bands == 4) {
                    data[i * width + j] = (pel[3] << 24) | (pel[0] << 16) | (pel[1] << 8) | pel[2];
                }
                pel += bands;
            }

            pel = pelCopy + skip;
        }

        (*env)->ReleaseIntArrayElements(env, dataArray, data, 0);
    }

    jobject bitmap;
    {
        jclass bitmapConfigClass = FIND_CLASS(env, "android/graphics/Bitmap$Config");
        jfieldID argb8888Field = (*env)->GetStaticFieldID(env, bitmapConfigClass, "ARGB_8888", "Landroid/graphics/Bitmap$Config;");
        jobject argb8888 = (*env)->GetStaticObjectField(env, bitmapConfigClass, argb8888Field);

        jclass bitmapClass = FIND_CLASS(env, "android/graphics/Bitmap");
        jmethodID createBitmap = (*env)->GetStaticMethodID(env, bitmapClass, "createBitmap", "([IIILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
        bitmap = (*env)->CallStaticObjectMethod(env, bitmapClass, createBitmap, dataArray, width, height, argb8888);
    }

    jobject minamoImage;
    {
        jclass minamoNativeImageClass = FIND_CLASS(env, "xyz/quaver/minamo/MinamoNativeImage");
        jmethodID constructor = (*env)->GetMethodID(env, minamoNativeImageClass, "<init>", "(Landroid/graphics/Bitmap;)V");
        minamoImage = (*env)->NewObject(env, minamoNativeImageClass, constructor, bitmap);
    }

    (*env)->DeleteLocalRef(env, dataArray);
    (*env)->DeleteLocalRef(env, bitmap);
#else
    jobject dataBuffer;
    {
        jclass dataBufferClass = (*env)->FindClass(env, "Ljava/awt/image/DataBufferInt;");
        jmethodID dataBufferConstructor = (*env)->GetMethodID(env, dataBufferClass, "<init>", "(II)V");
        dataBuffer = (*env)->NewObject(env, dataBufferClass, dataBufferConstructor, width * height, 1);

        jmethodID getData = (*env)->GetMethodID(env, dataBufferClass, "getData", "()[I");
        jintArray dataArray = (jintArray)(*env)->CallObjectMethod(env, dataBuffer, getData);

        jint* data = (*env)->GetIntArrayElements(env, dataArray, NULL);

        for (size_t i = 0; i < height; i++) {
            VipsPel* pelCopy = pel;

            for (size_t j = 0; j < width; j++) {
                if (bands == 1) {
                    data[i * width + j] = (pel[0] << 16) | (pel[0] << 8) | pel[0];
                } else if (bands == 3) {
                    data[i * width + j] = (pel[0] << 16) | (pel[1] << 8) | pel[2];
                } else if (bands == 4) {
                    data[i * width + j] = (pel[0] << 16) | (pel[1] << 8) | pel[2];
                }
                pel += bands;
            }

            pel = pelCopy + skip;
        }

        (*env)->ReleaseIntArrayElements(env, dataArray, data, 0);
        (*env)->DeleteLocalRef(env, dataArray);
    }
    g_object_unref(region);

    jobject raster;
    {
        jintArray bandMasks = (*env)->NewIntArray(env, 3);
        jint* bandMasksArray = (*env)->GetIntArrayElements(env, bandMasks, NULL);
        bandMasksArray[0] = 0xFF0000;
        bandMasksArray[1] = 0xFF00;
        bandMasksArray[2] = 0xFF;
        (*env)->ReleaseIntArrayElements(env, bandMasks, bandMasksArray, 0);

        jclass pointClass = (*env)->FindClass(env, "Ljava/awt/Point;");
        jmethodID pointConstructor = (*env)->GetMethodID(env, pointClass, "<init>", "(II)V");
        jobject point = (*env)->NewObject(env, pointClass, pointConstructor, 0, 0);

        jclass rasterClass = (*env)->FindClass(env, "Ljava/awt/image/Raster;");
        jmethodID createPackedRaster = (*env)->GetStaticMethodID(env, rasterClass, "createPackedRaster", "(Ljava/awt/image/DataBuffer;III[ILjava/awt/Point;)Ljava/awt/image/WritableRaster;");
        raster = (*env)->CallStaticObjectMethod(env, rasterClass, createPackedRaster, dataBuffer, width, height, width, bandMasks, point);

        (*env)->DeleteLocalRef(env, bandMasks);
        (*env)->DeleteLocalRef(env, point);
    }

    jobject colorModel;
    {
        jclass colorModelClass = (*env)->FindClass(env, "Ljava/awt/image/DirectColorModel;");

        jmethodID init = (*env)->GetMethodID(env, colorModelClass, "<init>", "(IIII)V");
        colorModel = (*env)->NewObject(env, colorModelClass, init, 24, 0xFF0000, 0xFF00, 0xFF);
    }

    jobject bufferedImage;
    {
        jclass bufferedImageClass = (*env)->FindClass(env, "Ljava/awt/image/BufferedImage;");
        jmethodID constructor = (*env)->GetMethodID(env, bufferedImageClass, "<init>", "(Ljava/awt/image/ColorModel;Ljava/awt/image/WritableRaster;ZLjava/util/Hashtable;)V");
        bufferedImage = (*env)->NewObject(env, bufferedImageClass, constructor, colorModel, raster, JNI_FALSE, NULL);
    }

    jobject minamoImage;
    {
        jclass minamoNativeImageClass = (*env)->FindClass(env, "Lxyz/quaver/minamo/MinamoNativeImage;");
        jmethodID constructor = (*env)->GetMethodID(env, minamoNativeImageClass, "<init>", "(Ljava/awt/Image;)V");
        minamoImage = (*env)->NewObject(env, minamoNativeImageClass, constructor, bufferedImage);
    }

    (*env)->DeleteLocalRef(env, raster);
    (*env)->DeleteLocalRef(env, colorModel);
    (*env)->DeleteLocalRef(env, bufferedImage);
#endif

    return minamoImage;
}

JNIEXPORT jobject JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_resize(JNIEnv *env, jobject this,
    jfloat scale
) {
    VipsImage* image = MinamoImage_getVipsImage(env, this);

    VipsImage* resizedImage;
    if (image->Bands == 4) {
        VipsImage *premultiplied, *resizedPremultiplied;
        vips_premultiply(image, &premultiplied, NULL);

        vips_resize(premultiplied, &resizedPremultiplied, scale, NULL);

        vips_unpremultiply(resizedPremultiplied, &resizedImage, NULL);

        VIPS_UNREF(premultiplied);
        VIPS_UNREF(resizedPremultiplied);
    } else {
        vips_resize(image, &resizedImage, scale, NULL);
    }

    jclass class = (*env)->GetObjectClass(env, this);
    jclass test = (*env)->GetSuperclass(env, class);
    jmethodID constructor = (*env)->GetMethodID(env, class, "<init>", "(J)V");
    
    return (*env)->NewObject(env, class, constructor, (jlong) resizedImage);
}

JNIEXPORT jobject JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_subsample(JNIEnv *env, jobject this,
    jint xFactor, jint yFactor
) {
    VipsImage* image = MinamoImage_getVipsImage(env, this);

    VipsImage* subsampledImage;
    vips_subsample(image, &subsampledImage, xFactor, yFactor, NULL);

    jclass class = (*env)->GetObjectClass(env, this);
    jmethodID constructor = (*env)->GetMethodID(env, class, "<init>", "(J)V");

    return (*env)->NewObject(env, class, constructor, (jlong) subsampledImage);
}

void minamo_sink_notify(VipsImage* image, VipsRect* rect, void* data) {
    MinamoSinkCallback* cb = (MinamoSinkCallback*) data;

    g_object_ref(cb);

    minamo_sink_callback_invoke(cb, image, rect);

    g_object_unref(cb);
}

void minamo_sink_close_cb(VipsObject* object, void* data) {
    MinamoSinkCallback* cb = (MinamoSinkCallback*) data;

    g_object_unref(cb);
}

JNIEXPORT jobject JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_sink(JNIEnv *env, jobject this,
    jobject tileSize,
    jint maxTiles,
    jint priority,
    jobject notify
) {
    jint tileWidth, tileHeight;
    {
        jclass sizeClass = (*env)->GetObjectClass(env, tileSize);
        tileWidth = (*env)->GetIntField(env, tileSize, (*env)->GetFieldID(env, sizeClass, "width", "I"));
        tileHeight = (*env)->GetIntField(env, tileSize, (*env)->GetFieldID(env, sizeClass, "height", "I"));
    }

    VipsImage* image = MinamoImage_getVipsImage(env, this);

    VipsImage* cached = vips_image_new();
    VipsImage* mask = vips_image_new();

    JavaVM* jvm;
    (*env)->GetJavaVM(env, &jvm);

    MinamoSinkCallback* cb = minamo_sink_callback_new(env, notify);

    g_signal_connect(cached, "close", G_CALLBACK(minamo_sink_close_cb), cb);

    if (vips_sink_screen(image, cached, mask, tileWidth, tileHeight, maxTiles, priority, minamo_sink_notify, cb)) {
        VIPS_UNREF(cached);
        VIPS_UNREF(mask);
        printf("Failed to sink\n");
        fflush(stdout);
        return NULL;
    }

    printf("Returning %p %p with data %p\n", cached, mask, cb);
    fflush(stdout);

    jobject cachedMinamoImage, maskMinamoImage;
    {
        jclass class = (*env)->GetObjectClass(env, this);
        jmethodID constructor = (*env)->GetMethodID(env, class, "<init>", "(J)V");

        cachedMinamoImage = (*env)->NewObject(env, class, constructor, (jlong) g_steal_pointer(&cached));
        maskMinamoImage = (*env)->NewObject(env, class, constructor, (jlong) g_steal_pointer(&mask));
    }

    printf("which are: %p %p\n", cachedMinamoImage, maskMinamoImage);
    fflush(stdout);

    jobject retval;
    {
        jclass class = FIND_CLASS(env, "kotlin/Pair");
        jmethodID constructor = (*env)->GetMethodID(env, class, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");

        retval = (*env)->NewObject(env, class, constructor, cachedMinamoImage, maskMinamoImage);
    }

    return retval;
}

JNIEXPORT jboolean JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_hasAlpha(
    JNIEnv *env,
    jobject this,
    jlong image
) {
    return vips_image_hasalpha((VipsImage *)image);
}

JNIEXPORT jint JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_getHeight(
    JNIEnv *env,
    jobject this,
    jlong image
) {
    return vips_image_get_height((VipsImage *)image);
}

JNIEXPORT jint JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_getWidth(
    JNIEnv *env,
    jobject this,
    jlong image
) {
    return vips_image_get_width((VipsImage *)image);
}

JNIEXPORT jlong JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_load(
    JNIEnv *env,
    jobject this,
    jlong source
) {
    VipsImage *image =
        vips_image_new_from_source((VipsSource *)source, "", NULL);

    if (!image) {
        return (jlong) NULL;
    }

    if (vips_image_guess_interpretation(image) != VIPS_INTERPRETATION_sRGB) {
        VipsImage *tmp;
        if (vips_colourspace(image, &tmp, VIPS_INTERPRETATION_sRGB, NULL)) {
            VIPS_UNREF(image);
            return (jlong) NULL;
        }

        VIPS_UNREF(image);
        image = tmp;
    }

    return (jlong) image;
}

JNIEXPORT void JNICALL
Java_xyz_quaver_minamo_MinamoImageImpl_close(JNIEnv *env, jobject this) {
    jclass class = (*env)->GetObjectClass(env, this);

    jmethodID getVipsImage =
        (*env)->GetMethodID(env, class, "getVipsImage", "()J");
    VipsImage *image =
        (VipsImage *)((*env)->CallLongMethod(env, this, getVipsImage));

    VIPS_UNREF(image);

    jfieldID vipsImageField =
        (*env)->GetFieldID(env, class, "_vipsImage", "J");
    (*env)->SetLongField(env, this, vipsImageField, 0L);

    return;
}