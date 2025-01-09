#include "jni_support.h"

#define LOG_TAG "WwiseGDExtension"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

JavaVM* JNISupport::jvm = nullptr;
jobject JNISupport::jactivity = nullptr;
std::string JNISupport::plugins_dir = "";

void JNISupport::initialize(JNIEnv* env, jobject activity, jstring nativeLibDir)
{
	if (env->GetJavaVM(&jvm) != JNI_OK)
	{
		ALOGE("WwiseGodot: Failed to get the JavaVM");
		return;
	}
	jactivity = env->NewGlobalRef(activity);

	const char* nativeLibDirCStr = env->GetStringUTFChars(nativeLibDir, nullptr);
	if (nativeLibDirCStr != nullptr)
	{
		plugins_dir = nativeLibDirCStr;
		env->ReleaseStringUTFChars(nativeLibDir, nativeLibDirCStr);

		ALOGE("WwiseGodot: Native Library Directory: %s", plugins_dir.c_str());
	}
	else
	{
		ALOGE("WwiseGodot: Failed to retrieve the Native Library directory.");
	}
}

JavaVM* JNISupport::getJavaVM() { return jvm; }

jobject JNISupport::getActivity() { return jactivity; }

std::string JNISupport::getPluginsDir() { return plugins_dir; }

extern "C" JNIEXPORT void JNICALL
Java_org_godotengine_plugin_android_gdextension_wwise_GDExtensionAndroidPlugin_initPlugin(
		JNIEnv* env, jobject thiz, jobject activity, jstring nativeLibDir)
{
	JNISupport::initialize(env, activity, nativeLibDir);
}