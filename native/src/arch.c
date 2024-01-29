#include "arch.h"

#ifdef __ANDROID__

jobject newLongObject(JNIEnv *env, jlong value) {
    jclass longClass = (*env)->FindClass(env, "java/lang/Long");
    jmethodID longConstructor = (*env)->GetStaticMethodID(env, longClass, "valueOf", "(J)Ljava/lang/Long;");

    jobject longObject = (*env)->CallStaticObjectMethod(env, longClass, longConstructor, value);

    return longObject;
}

#else

jobject newLongObject(JNIEnv *env, jlong value) {
    jclass longClass = (*env)->FindClass(env, "Ljava/lang/Long;");
    jmethodID longConstructor = (*env)->GetMethodID(env, longClass, "<init>", "(J)V");

    jobject longObject =
        (*env)->NewObject(env, longClass, longConstructor, value);
    return longObject;
}

#endif