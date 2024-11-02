cmake_minimum_required(VERSION 3.22.1)

# Set default Android ABI to arm64-v8a if not specified
if (NOT ANDROID_ABI)
    set(ANDROID_ABI "arm64-v8a")
endif ()

# Map Android ABI to GODOT_CPP_LIB_ABI
if (ANDROID_ABI STREQUAL "armeabi-v7a")
    set(GODOT_CPP_LIB_ABI "arm32")
elseif (ANDROID_ABI STREQUAL "x86")
    set(GODOT_CPP_LIB_ABI "x86_32")
elseif (ANDROID_ABI STREQUAL "x86_64")
    set(GODOT_CPP_LIB_ABI "x86_64")
else ()
    set(GODOT_CPP_LIB_ABI "arm64")
endif ()

# Set default Android platform to android-21 if not specified
if (NOT ANDROID_PLATFORM)
    set(ANDROID_PLATFORM "android-21")
endif ()

# Set default build type to Debug if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# Add debug or release-specific definitions and build type suffix
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
    set(GODOT_CPP_LIB_BUILD_TYPE debug)
else ()
    add_definitions(-DNDEBUG)
    set(GODOT_CPP_LIB_BUILD_TYPE release)
endif ()

# Set default Android STL to c++_shared if not specified
if (NOT (ANDROID_STL STREQUAL "c++_shared"))
    set(ANDROID_STL "c++_shared")
endif ()

# Check if ANDROID_NDK is set or use the environment variable
if (NOT ANDROID_NDK)
    if (DEFINED ENV{ANDROID_NDK_HOME})
        set(ANDROID_NDK $ENV{ANDROID_NDK_HOME})
    else ()
        message(WARNING "ANDROID_NDK_HOME is not set")
    endif ()
endif ()

# Check if CMAKE_TOOLCHAIN_FILE is set
if (NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${ANDROID_NDK}/build/cmake/android.toolchain.cmake")
endif ()

# Determine BITS based on pointer size (32 or 64)
if (NOT DEFINED BITS)
    set(BITS 32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BITS 64)
    endif ()
endif ()

# Set C++ standard and flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Create the project
project(WwiseAndroidPlugin LANGUAGES CXX)

set(GODOT_COMPILE_FLAGS)
set(GODOT_LINKER_FLAGS)

set(GODOT_LINKER_FLAGS "-Wl")

set(GODOT_COMPILE_FLAGS "-fPIC -g -Wwrite-strings")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wchar-subscripts -Wcomment -Wdisabled-optimization")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wformat -Wformat=2 -Wformat-security -Wformat-y2k")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wimport -Winit-self -Winline -Winvalid-pch")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wlong-long -Wmissing-braces -Wmissing-format-attribute")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wpointer-arith")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wredundant-decls -Wreturn-type -Wsequence-point")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wswitch -Wswitch-enum -Wtrigraphs")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused-label")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wunused-value -Wvariadic-macros -Wvolatile-register-var -Wno-error=attributes -Wno-format-nonliteral")

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wno-ignored-attributes")
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -fno-omit-frame-pointer -O0")
else ()
    set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -O3")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

# Set GODOT_CPP_DIR and GODOT-CPP library
set(GODOT_CPP_DIR "${CMAKE_SOURCE_DIR}/../../godot-cpp")
set(GODOT-CPP "godot-cpp")

# Use the godot-cpp prebuilt static binary
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(GODOT_CPP_STATIC_LIB "${GODOT_CPP_DIR}/bin/libgodot-cpp.android.template_debug.dev.${GODOT_CPP_LIB_ABI}.a")
else ()
    set(GODOT_CPP_STATIC_LIB "${GODOT_CPP_DIR}/bin/libgodot-cpp.android.template_${GODOT_CPP_LIB_BUILD_TYPE}.${GODOT_CPP_LIB_ABI}.a")
endif ()

list(APPEND GODOT_CPP_INCLUDE_DIRECTORIES "${GODOT_CPP_DIR}/include" "${GODOT_CPP_DIR}/gen/include" "${GODOT_CPP_DIR}/gdextension")

add_library(${GODOT-CPP} STATIC IMPORTED GLOBAL INTERFACE_INCLUDE_DIRECTORIES "${GODOT_CPP_INCLUDE_DIRECTORIES}")
set_target_properties(${GODOT-CPP} PROPERTIES IMPORTED_LOCATION ${GODOT_CPP_STATIC_LIB})

# Set Wwise paths and libraries
set(WWISE_SDK ${WWISE_SDK_PATH})
message("wwise sdk+ ${WWISE_SDK_PATH}")
set(WWISE_SOUNDENGINE_SAMPLE_PATH "${WWISE_SDK}/samples/SoundEngine/Android/")

if (ANDROID_ABI STREQUAL "arm64-v8a")
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set(WWISE_SDK_LIBS_PATH "${WWISE_SDK}/Android_arm64-v8a/Debug/lib/")
    else ()
        set(WWISE_SDK_LIBS_PATH "${WWISE_SDK}/Android_arm64-v8a/Release/lib/")
    endif ()
else ()
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set(WWISE_SDK_LIBS_PATH "${WWISE_SDK}/Android_armeabi-v7a/Debug/lib/")
    else ()
        set(WWISE_SDK_LIBS_PATH "${WWISE_SDK}/Android_armeabi-v7a/Release/lib/")
    endif ()
endif ()

set(WWISE_SOUNDENGINE_SAMPLE_COMMON_PATH "${WWISE_SDK}/samples/SoundEngine/Common/")

include_directories("${WWISE_SDK}/include" "${WWISE_SDK}/samples/SoundEngine/Android")

link_directories("${WWISE_SDK_LIBS_PATH}")

# Define Wwise core and plugins libraries
set(WWISE_CORE_LIBRARIES "AkSoundEngine" "AkStreamMgr" "AkMusicEngine" "AkMemoryMgr")
set(WWISE_PLUGINS_LIBRARY "AkGainFX" "AkHarmonizerFX" "AkStereoDelayFX" "AkTimeStretchFX" "AkTremoloFX" "AkGuitarDistortionFX" "AkVorbisDecoder" "AkOpusDecoder" "AkMeterFX" "AkPitchShifterFX" "AkRoomVerbFX" "AkPeakLimiterFX" "AkExpanderFX" "AkCompressorFX" "AkMatrixReverbFX" "AkParametricEQFX" "AkDelayFX" "AkSilenceSource" "AkToneSource" "AkFlangerFX" "AkSineSource" "AkRecorderFX" "AkAudioInputSource" "AkSynthOneSource" "AkSpatialAudio" "Ak3DAudioBedMixerFX")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND WWISE_PLUGINS_LIBRARY CommunicationCentral)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DAK_ENABLE_ASSERTS)
else ()
    add_definitions(-DAK_OPTIMIZED)
endif ()

if ("motion" IN_LIST plugins)
    list(APPEND WWISE_PLUGINS_LIBRARY "AkMotionSink" "AkMotionSourceSource")
    add_definitions(-DAK_MOTION)
endif ()

add_definitions(-DLUA_USE_POSIX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--start-group -Wl,--end-group")

# Define source files
set(WWISE_SOURCES
    ${CMAKE_SOURCE_DIR}/../../src/wwise_gdextension_main.cpp
)

file(GLOB WWISE_HEADERS
        ${WWISE_SOUNDENGINE_SAMPLE_PATH}libzip/lib/*.h
        ${WWISE_SDK}/samples/SoundEngine/POSIX/*.h
        )

list(APPEND WWISE_INCLUDE_DIRECTORIES
        "${CMAKE_SOURCE_DIR}/../../src/"
        "${WWISE_SOUNDENGINE_SAMPLE_PATH}libzip/lib/"
        "${WWISE_SDK}/samples/SoundEngine/POSIX/"
        )

message(${CMAKE_PROJECT_NAME} )

add_library(${CMAKE_PROJECT_NAME} SHARED ${WWISE_SOURCES} ${WWISE_HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC
        ${WWISE_INCLUDE_DIRECTORIES}
        ${GODOT_CPP_INCLUDE_DIRECTORIES}
        )

target_link_libraries(${CMAKE_PROJECT_NAME}
        ${WWISE_CORE_LIBRARIES}
        ${WWISE_PLUGINS_LIBRARY}
        android
        log
        zip
        OpenSLES
        z
        EGL
        GLESv1_CM
        ${GODOT-CPP}
        )

# Add the compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${GODOT_COMPILE_FLAGS})
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${GODOT_LINKER_FLAGS})