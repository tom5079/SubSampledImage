#ifdef __ANDROID__

#include <jni.h>

JNIEXPORT jint JNICALL
Java_xyz_quaver_graphics_subsampledimage_Test_test(JNIEnv *env,
                                                   jobject this) {
    return 42;
}

#endif