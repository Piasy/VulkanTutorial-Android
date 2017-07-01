# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# build vulkan app
set(SRC_DIR src/main/cpp)
set(VK_WRAPPER_DIR src/main/cpp/vulkan_wrapper)
set(VK_VAL_LAYER_SRC_DIR ${ANDROID_NDK}/sources/third_party/vulkan/src)
set(GLM_SRC_DIR src/main/cpp/glm)

add_library( # Sets the name of the library.
             vulkan-tutorial

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${VK_WRAPPER_DIR}/vulkan_wrapper.cpp
             ${SRC_DIR}/vulkan_tutorial.cpp
             ${SRC_DIR}/vulkan_tutorial_jni.cpp
             )

include_directories(${VK_WRAPPER_DIR} ${VK_VAL_LAYER_SRC_DIR}/include  ${GLM_SRC_DIR})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       vulkan-tutorial

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       android)
