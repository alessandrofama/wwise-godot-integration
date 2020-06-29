# Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := AkAudioInputSource
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkAudioInputSource.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkCompressorFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkCompressorFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkDelayFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkDelayFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkExpanderFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkExpanderFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkFlangerFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkFlangerFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkGainFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkGainFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkGuitarDistortionFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkGuitarDistortionFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkHarmonizerFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkHarmonizerFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkMatrixReverbFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkMatrixReverbFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkMemoryMgr
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkMemoryMgr.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkMeterFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkMeterFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkMusicEngine
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkMusicEngine.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkOpusDecoder
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkOpusDecoder.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkParametricEQFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkParametricEQFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkPeakLimiterFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkPeakLimiterFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkPitchShifterFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkPitchShifterFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkRecorderFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkRecorderFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkRoomVerbFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkRoomVerbFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkSilenceSource
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkSilenceSource.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkSineSource
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkSineSource.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkSoundEngine
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkSoundEngine.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkSpatialAudio
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkSpatialAudio.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkStereoDelayFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkStereoDelayFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkStreamMgr
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkStreamMgr.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkSynthOneSource
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkSynthOneSource.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkTimeStretchFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkTimeStretchFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkToneSource
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkToneSource.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkTremoloFX
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkTremoloFX.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := AkVorbisDecoder
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libAkVorbisDecoder.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := zip
LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libzip.a
include $(PREBUILT_STATIC_LIBRARY)

ifeq ($(PM5_CONFIG),debug_android_armeabi-v7a)
  include $(CLEAR_VARS)
  LOCAL_MODULE    := CommunicationCentral
  LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libCommunicationCentral.a
  include $(PREBUILT_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_x86)
  include $(CLEAR_VARS)
  LOCAL_MODULE    := CommunicationCentral
  LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libCommunicationCentral.a
  include $(PREBUILT_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_arm64-v8a)
  include $(CLEAR_VARS)
  LOCAL_MODULE    := CommunicationCentral
  LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libCommunicationCentral.a
  include $(PREBUILT_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_x86_64)
  include $(CLEAR_VARS)
  LOCAL_MODULE    := CommunicationCentral
  LOCAL_SRC_FILES := $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib/libCommunicationCentral.a
  include $(PREBUILT_STATIC_LIBRARY)
endif

include $(CLEAR_VARS)
LOCAL_MODULE := godot-prebuilt
ifeq ($(APP_ABI),x86)
    ifeq ($(CONFIGURATION),Debug)
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.debug.x86.a
    else
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.release.x86.a
    endif
endif
ifeq ($(APP_ABI),x86_64)
    ifeq ($(CONFIGURATION),Debug)
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.debug.x86_64.a
    else
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.release.x86_64.a
    endif
endif
ifeq ($(APP_ABI),armeabi-v7a)
    ifeq ($(CONFIGURATION),Debug)
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.debug.armv7.a
    else
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.release.armv7.a
    endif
endif
ifeq ($(APP_ABI),arm64-v8a)
    ifeq ($(CONFIGURATION),Debug)
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.debug.arm64v8.a
    else
        LOCAL_SRC_FILES := $(LOCAL_PATH)/../godot-cpp/bin/libgodot-cpp.android.release.arm64v8.a
    endif
endif

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := WwiseGDNative.android.$(CONFIGURATION).$(APP_ABI)
LOCAL_MODULE_FILENAME := libWwiseGDNative
FILE_LIST_CORE := $(wildcard $(LOCAL_PATH)/../godot-cpp/src/core/*.cpp)
FILE_LIST_GEN := $(wildcard $(LOCAL_PATH)/../godot-cpp/src/gen/*.cpp)
FILE_LIST_10 := $(wildcard $(LOCAL_PATH)/../godot-cpp/src/*.cpp)

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../godot-cpp/godot_headers \
$(LOCAL_PATH)/../godot-cpp/include \
$(LOCAL_PATH)/../godot-cpp/include/core \
$(LOCAL_PATH)/../godot-cpp/include/gen \
src

LOCAL_SRC_FILES := src/wwise_gdnative.cpp src/gdlibrary.cpp $(WWISESDK)/samples/SoundEngine/Android/AkDefaultIOHookBlocking.cpp $(WWISESDK)/samples/SoundEngine/Android/AkDefaultIOHookDeferred.cpp $(WWISESDK)/samples/SoundEngine/Android/AkFileHelpers.cpp $(WWISESDK)/samples/SoundEngine/Common/AkFileLocationBase.cpp $(WWISESDK)/samples/SoundEngine/Common/AkFilePackage.cpp $(WWISESDK)/samples/SoundEngine/Common/AkFilePackageLUT.cpp
LOCAL_SRC_FILES += \
$(FILE_LIST_CORE:$(LOCAL_PATH)/%=%) \
$(FILE_LIST_GEN:$(LOCAL_PATH)/%=%) \
$(FILE_LIST_10:$(LOCAL_PATH)/%=%) \

ifeq ($(PM5_CONFIG),debug_android_armeabi-v7a)
  LOCAL_C_INCLUDES += $(WWISESDK)/samples/SoundEngine/Android/libzip/lib $(LOCAL_PATH)/. $(WWISESDK)/samples/SoundEngine/Common $(WWISESDK)/samples/SoundEngine/Android $(WWISESDK)/include 
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := CommunicationCentral AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Debug/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -D_DEBUG -mfloat-abi=softfp -mfpu=neon -g  -frtti 
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14 
  LOCAL_CONLYFLAGS := -std=c14
  cmd-strip :=
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_x86)
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../SoundEngine/Android/libzip/lib $(ndkroot)/sources/android/native_app_glue $(LOCAL_PATH)/. $(LOCAL_PATH)/../Common $(LOCAL_PATH)/../FreetypeRenderer $(LOCAL_PATH)/../FreetypeRenderer/Assets $(LOCAL_PATH)/../MenuSystem $(LOCAL_PATH)/../../SoundEngine/Android $(LOCAL_PATH)/../../SoundEngine/Common $(LOCAL_PATH)/$(WWISESDK)/include $(LOCAL_PATH)/../DemoPages $(LOCAL_PATH)/../WwiseProject/GeneratedSoundBanks
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := CommunicationCentral AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Debug/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -D_DEBUG -g -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_arm64-v8a)
  LOCAL_C_INCLUDES += $(WWISESDK)/samples/SoundEngine/Android/libzip/lib $(WWISESDK)/samples/SoundEngine/Common $(WWISESDK)/samples/SoundEngine/Android $(WWISESDK)/include 
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := CommunicationCentral AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Debug/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -D_DEBUG -g -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_x86_64)
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../SoundEngine/Android/libzip/lib $(ndkroot)/sources/android/native_app_glue $(LOCAL_PATH)/. $(LOCAL_PATH)/../Common $(LOCAL_PATH)/../FreetypeRenderer $(LOCAL_PATH)/../FreetypeRenderer/Assets $(LOCAL_PATH)/../MenuSystem $(LOCAL_PATH)/../../SoundEngine/Android $(LOCAL_PATH)/../../SoundEngine/Common $(LOCAL_PATH)/$(WWISESDK)/include $(LOCAL_PATH)/../DemoPages $(LOCAL_PATH)/../WwiseProject/GeneratedSoundBanks
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := CommunicationCentral AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Debug/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -D_DEBUG -g -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_armeabi-v7a)
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../SoundEngine/Android/libzip/lib $(ndkroot)/sources/android/native_app_glue $(LOCAL_PATH)/. $(LOCAL_PATH)/../Common $(LOCAL_PATH)/../FreetypeRenderer $(LOCAL_PATH)/../FreetypeRenderer/Assets $(LOCAL_PATH)/../MenuSystem $(LOCAL_PATH)/../../SoundEngine/Android $(LOCAL_PATH)/../../SoundEngine/Common $(LOCAL_PATH)/$(WWISESDK)/include $(LOCAL_PATH)/../DemoPages $(LOCAL_PATH)/../WwiseProject/GeneratedSoundBanks
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Release/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -DNDEBUG -DAK_OPTIMIZED -mfloat-abi=softfp -mfpu=neon -ffunction-sections -fdata-sections -O3 -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_x86)
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../SoundEngine/Android/libzip/lib $(ndkroot)/sources/android/native_app_glue $(LOCAL_PATH)/. $(LOCAL_PATH)/../Common $(LOCAL_PATH)/../FreetypeRenderer $(LOCAL_PATH)/../FreetypeRenderer/Assets $(LOCAL_PATH)/../MenuSystem $(LOCAL_PATH)/../../SoundEngine/Android $(LOCAL_PATH)/../../SoundEngine/Common $(LOCAL_PATH)/$(WWISESDK)/include $(LOCAL_PATH)/../DemoPages $(LOCAL_PATH)/../WwiseProject/GeneratedSoundBanks
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Release/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -DNDEBUG -DAK_OPTIMIZED -ffunction-sections -fdata-sections -O3 -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_arm64-v8a)
  LOCAL_C_INCLUDES += $(WWISESDK)/samples/SoundEngine/Android/libzip/lib $(LOCAL_PATH)/src $(WWISESDK)/samples/SoundEngine/Common $(WWISESDK)/samples/SoundEngine/Android $(WWISESDK)/samples/SoundEngine/Common $(WWISESDK)/include
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Release/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -DNDEBUG -DAK_OPTIMIZED -ffunction-sections -fdata-sections -O3 -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_x86_64)
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../SoundEngine/Android/libzip/lib $(ndkroot)/sources/android/native_app_glue $(LOCAL_PATH)/. $(LOCAL_PATH)/../Common $(LOCAL_PATH)/../FreetypeRenderer $(LOCAL_PATH)/../FreetypeRenderer/Assets $(LOCAL_PATH)/../MenuSystem $(LOCAL_PATH)/../../SoundEngine/Android $(LOCAL_PATH)/../../SoundEngine/Common $(LOCAL_PATH)/$(WWISESDK)/include $(LOCAL_PATH)/../DemoPages $(LOCAL_PATH)/../WwiseProject/GeneratedSoundBanks
  LOCAL_CPP_FEATURES := exceptions
  LOCAL_STATIC_LIBRARIES := AkAudioInputSource AkCompressorFX AkDelayFX AkExpanderFX AkFlangerFX AkGainFX AkGuitarDistortionFX AkHarmonizerFX AkMatrixReverbFX AkMeterFX AkOpusDecoder AkParametricEQFX AkPeakLimiterFX AkPitchShifterFX AkRecorderFX AkRoomVerbFX AkSilenceSource AkSineSource AkStereoDelayFX AkSynthOneSource AkTimeStretchFX AkToneSource AkTremoloFX zip AkVorbisDecoder AkMusicEngine AkSpatialAudio AkStreamMgr AkMemoryMgr AkSoundEngine
  LOCAL_LDLIBS := $(addprefix -L, $(WWISESDK)/Android_$(ArchAbi)/$(Configuration)/lib $(WWISESDK)/Android_$(TARGET_ARCH_ABI)/Release/lib $(WWISESDK)/Android_$(APP_ABI)/$(CONFIGURATION)/lib) $(addprefix -l, log OpenSLES android EGL GLESv1_CM z)
  LOCAL_CFLAGS := -DNDEBUG -DAK_OPTIMIZED -ffunction-sections -fdata-sections -O3 -DAK_SUPPORT_WCHAR -frtti
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-invalid-offsetof -Wno-deprecated-declarations -fvisibility=hidden -fno-exceptions -DLUA_USE_POSIX -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_SHARED_LIBRARY)
endif

$(info $(LOCAL_C_INCLUDES))

all: $(LOCAL_INSTALLED)