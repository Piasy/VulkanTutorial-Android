//
// Created by Piasy on 29/06/2017.
//

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include "vulkan_tutorial.h"

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_github_piasy_vulkantutorial_VulkanTutorialApplication_create(
        JNIEnv *env, jclass type, jobject assetManager_, jstring vertexShader_,
        jstring fragmentShader_) {

    AAssetManager *assetManager = AAssetManager_fromJava(env, assetManager_);
    if (assetManager == nullptr) {
        LOGE("get assetManager fail!");
        return 0;
    }

    const char *vertexShader = env->GetStringUTFChars(vertexShader_, 0);
    const char *fragmentShader = env->GetStringUTFChars(fragmentShader_, 0);

    VulkanTutorialApplication *app = new VulkanTutorialApplication(assetManager, vertexShader,
                                                                 fragmentShader);

    env->ReleaseStringUTFChars(vertexShader_, vertexShader);
    env->ReleaseStringUTFChars(fragmentShader_, fragmentShader);

    return (jlong) app;
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_VulkanTutorialApplication_run__JLandroid_view_Surface_2(
        JNIEnv *env, jclass type, jlong nativeHandle, jobject surface) {
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("get window from surface fail!");
        return;
    }

    VulkanTutorialApplication *app = reinterpret_cast<VulkanTutorialApplication *>(nativeHandle);
    app->run(window);
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_VulkanTutorialApplication_pause__J(JNIEnv *env, jclass type,
                                                                       jlong nativeHandle) {
    VulkanTutorialApplication *app = reinterpret_cast<VulkanTutorialApplication *>(nativeHandle);
    app->pause();
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_VulkanTutorialApplication_resume__J(JNIEnv *env, jclass type,
                                                                        jlong nativeHandle) {
    VulkanTutorialApplication *app = reinterpret_cast<VulkanTutorialApplication *>(nativeHandle);
    app->resume();
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_VulkanTutorialApplication_surfaceChanged__J(
        JNIEnv *env, jclass type, jlong nativeHandle) {
    VulkanTutorialApplication *app = reinterpret_cast<VulkanTutorialApplication *>(nativeHandle);
    app->surfaceChanged();
}

JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_VulkanTutorialApplication_stop__J(JNIEnv *env, jclass type,
                                                                      jlong nativeHandle) {
    VulkanTutorialApplication *app = reinterpret_cast<VulkanTutorialApplication *>(nativeHandle);
    app->stop();
}

}
