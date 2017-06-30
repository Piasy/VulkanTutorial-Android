//
// Created by Piasy on 29/06/2017.
//

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include "vulkan_triangle.h"

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_github_piasy_vulkantutorial_HelloTriangleApplication_create(
        JNIEnv *env, jclass type, jobject assetManager_, jstring vertexShader_,
        jstring fragmentShader_) {

    AAssetManager *assetManager = AAssetManager_fromJava(env, assetManager_);
    if (assetManager == nullptr) {
        LOGE("get assetManager fail!");
        return 0;
    }

    const char *vertexShader = env->GetStringUTFChars(vertexShader_, 0);
    const char *fragmentShader = env->GetStringUTFChars(fragmentShader_, 0);

    HelloTriangleApplication *app = new HelloTriangleApplication(assetManager, vertexShader,
                                                                 fragmentShader);

    env->ReleaseStringUTFChars(vertexShader_, vertexShader);
    env->ReleaseStringUTFChars(fragmentShader_, fragmentShader);

    return (jlong) app;
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_HelloTriangleApplication_run(
        JNIEnv *env, jclass type, jlong nativeHandle, jobject surface) {
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("get window from surface fail!");
        return;
    }

    HelloTriangleApplication *app = reinterpret_cast<HelloTriangleApplication *>(nativeHandle);
    app->run(window);
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_HelloTriangleApplication_pause(JNIEnv *env, jclass type,
                                                                       jlong nativeHandle) {
    HelloTriangleApplication *app = reinterpret_cast<HelloTriangleApplication *>(nativeHandle);
    app->pause();
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_HelloTriangleApplication_resume(JNIEnv *env, jclass type,
                                                                        jlong nativeHandle) {
    HelloTriangleApplication *app = reinterpret_cast<HelloTriangleApplication *>(nativeHandle);
    app->resume();
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_HelloTriangleApplication_surfaceChanged(
        JNIEnv *env, jclass type, jlong nativeHandle) {
    HelloTriangleApplication *app = reinterpret_cast<HelloTriangleApplication *>(nativeHandle);
    app->surfaceChanged();
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_HelloTriangleApplication_stop(JNIEnv *env, jclass type,
                                                                      jlong nativeHandle) {
    HelloTriangleApplication *app = reinterpret_cast<HelloTriangleApplication *>(nativeHandle);
    app->stop();
}

}
