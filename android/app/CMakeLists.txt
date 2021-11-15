# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions")

set(libs ${CMAKE_SOURCE_DIR}/src/main/jniLibs) # declare the variable libs

# Specifies a path to native header files.
include_directories(${CMAKE_SOURCE_DIR}/src/main/jniIncludes)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.
add_library(lib_opencv SHARED IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${libs}/${ANDROID_ABI}/libopencv_java4.so)

add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/Drawer.cpp
        src/main/cpp/Listener.cpp
)

find_library(
        # Defines the name of the path variable that stores the
        # location of the NDK library

        # Specifies the name of the NDK library that
        # CMake needs to locate.
        log-lib log
)

# Links your native library against one or more other native libraries.
target_link_libraries(native-lib lib_opencv ${log-lib})

find_library(LIB_BITMAP jnigraphics)
target_link_libraries(native-lib ${LIB_BITMAP})

