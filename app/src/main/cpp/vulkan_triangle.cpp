//
// Created by Piasy on 26/06/2017.
//

#include <jni.h>
#include <android/log.h>
#include<android/native_window_jni.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <functional>

#include "vulkan_wrapper/vulkan_wrapper.h"

#define APP_NAME "VK-TUTORIAL"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__))

const std::vector<const char *> validationLayers = {
        "VK_LAYER_LUNARG_image",
        "VK_LAYER_GOOGLE_threading",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_GOOGLE_unique_objects",
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugReportCallbackEXT(
        VkInstance instance,
        const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugReportCallbackEXT *pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance,
                                                                           "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance,
                                                                            "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

struct QueueFamilyIndices {
    int graphicsFamily = -1;

    bool isComplete() {
        return graphicsFamily >= 0;
    }
};

class HelloTriangleApplication {
public:
    void run(ANativeWindow *window) {
        initWindow(window);
        initVulkan();
        mainLoop();
        cleanUp();
    }

private:
    void initWindow(ANativeWindow *window) {
        this->window = window;
    }

    void initVulkan() {
        if (!InitVulkan()) {
            throw std::runtime_error("InitVulkan fail!");
        }

        createInstance();
        setUpDebugCallback();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
    }

    void createInstance() {
        if (enableValidationLayers && !supportValidationLayers()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

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
        auto extensions = requiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("create instance fail!");
        }

        LOGI("createInstance success :)");
    }

    void setUpDebugCallback() {
        if (!enableValidationLayers) {
            return;
        }

        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
                           | VK_DEBUG_REPORT_WARNING_BIT_EXT
                           | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        createInfo.pfnCallback = debugCallback;

        if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug callback!");
        }
    }

    void createSurface() {
        VkAndroidSurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        createInfo.window = window;

        if (vkCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        LOGI("device count: %d", deviceCount);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto &device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        uint32_t graphicsFamily = static_cast<uint32_t>(indices.graphicsFamily);

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = graphicsFamily;
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0F;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
    }

    bool isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);

        return indices.isComplete();
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
                break;
            }
            i++;
        }

        return indices;
    }

    bool supportValidationLayers() {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        LOGI("layer count: %d", layerCount);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto &layer : availableLayers) {
            LOGI("\t layer: %s", layer.layerName);
        }

        for (const char *layerName : validationLayers) {
            bool found = false;
            for (const auto &layer : availableLayers) {
                if (strcmp(layerName, layer.layerName) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char *> requiredExtensions() {
        std::vector<const char *> extensionNames;
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        LOGI("extension count: %d", extensionCount);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        for (const auto &extension : extensions) {
            if (strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, extension.extensionName) == 0) {
                if (enableValidationLayers) {
                    extensionNames.push_back(extension.extensionName);
                }
            } else {
                extensionNames.push_back(extension.extensionName);
            }

            LOGI("\t extension: %s", extension.extensionName);
        }
        return extensionNames;
    }

    void mainLoop() {

    }

    void cleanUp() {
        vkDestroyDevice(device, nullptr);
        DestroyDebugReportCallbackEXT(instance, callback, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        ANativeWindow_release(window);
        vkDestroyInstance(instance, nullptr);
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objType,
            uint64_t obj,
            size_t location,
            int32_t code,
            const char *layerPrefix,
            const char *msg,
            void *userData) {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            __android_log_print(ANDROID_LOG_ERROR,
                                APP_NAME,
                                "ERROR: [%s] Code %i : %s",
                                layerPrefix, code, msg);
        } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
            __android_log_print(ANDROID_LOG_WARN,
                                APP_NAME,
                                "WARNING: [%s] Code %i : %s",
                                layerPrefix, code, msg);
        } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
            __android_log_print(ANDROID_LOG_WARN,
                                APP_NAME,
                                "PERFORMANCE WARNING: [%s] Code %i : %s",
                                layerPrefix, code, msg);
        } else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
            __android_log_print(ANDROID_LOG_INFO,
                                APP_NAME,
                                "INFO: [%s] Code %i : %s",
                                layerPrefix, code, msg);
        } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
            __android_log_print(ANDROID_LOG_VERBOSE,
                                APP_NAME,
                                "DEBUG: [%s] Code %i : %s",
                                layerPrefix, code, msg);
        }

        // Returning false tells the layer not to stop when the event occurs, so
        // they see the same behavior with and without validation layers enabled.
        return VK_FALSE;
    }

    VkInstance instance;
    VkDebugReportCallbackEXT callback;
    ANativeWindow* window;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
};

extern "C" {
JNIEXPORT void JNICALL
Java_com_github_piasy_vulkantutorial_MainActivity_runVulkan(
        JNIEnv *env, jobject instance, jobject surface) {
    LOGI("Vulkan app start");

    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("get window from surface fail!");
        return;
    }

    HelloTriangleApplication app;
    try {
        app.run(window);
    } catch (const std::runtime_error &e) {
        LOGE("runtime error: %s", e.what());
    }

    LOGI("Vulkan app exit");
}
}
