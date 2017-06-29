//
// Created by Piasy on 29/06/2017.
//

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include <stdexcept>

#include "vulkan_triangle.h"

extern "C" {
JNIEXPORT void JNICALL Java_com_github_piasy_vulkantutorial_MainActivity_runVulkan(
        JNIEnv *env,
        jobject instance,
        jobject surface,
        jobject assetManager_,
        jstring vertexShader_,
        jstring fragmentShader_) {
    LOGI("Vulkan app start");

    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("get window from surface fail!");
        return;
    }
    AAssetManager *assetManager = AAssetManager_fromJava(env, assetManager_);
    if (assetManager == nullptr) {
        LOGE("get assetManager fail!");
        return;
    }

    const char *vertexShader = env->GetStringUTFChars(vertexShader_, 0);
    const char *fragmentShader = env->GetStringUTFChars(fragmentShader_, 0);

    HelloTriangleApplication app(assetManager, vertexShader, fragmentShader);
    try {
        app.run(window);
    } catch (const std::runtime_error &e) {
        LOGE("runtime error: %s", e.what());
    }

    env->ReleaseStringUTFChars(vertexShader_, vertexShader);
    env->ReleaseStringUTFChars(fragmentShader_, fragmentShader);

    LOGI("Vulkan app exit");
}
}
