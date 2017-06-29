//
// Created by Piasy on 26/06/2017.
//

#include <jni.h>
#include <android/log.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <functional>

#include "vulkan_wrapper/vulkan_wrapper.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "VK-SAMPLE", __VA_ARGS__))

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }

private:
    void initWindow() {
        LOGI("no GLFW in Android :(");
    }

    void initVulkan() {
        if (!InitVulkan()) {
            throw std::runtime_error("InitVulkan fail!");
        }

        createInstance();
    }

    void createInstance() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = 0;
        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("create instance fail!");
        }

        LOGI("createInstance success :)");
    }

    void mainLoop() {

    }

    void cleanUp() {
        vkDestroyInstance(instance, nullptr);
    }

    VkInstance instance;
};

extern "C" {
JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_MainActivity_runVulkan(JNIEnv *env, jobject instance) {
    LOGI("Vulkan app start");

    HelloTriangleApplication app;
    try {
        app.run();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    LOGI("Vulkan app exit");
}
}