#include "jni_support.h"

#define LOG_TAG "WwiseGDExtension"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

JavaVM* JNISupport::jvm = nullptr;
jobject JNISupport::jactivity = nullptr;

void JNISupport::initialize(JNIEnv* env, jobject activity)
{
	if (env->GetJavaVM(&jvm) != JNI_OK)
	{
		ALOGE("WwiseGodot: Failed to get the JavaVM");
		return;
	}
	jactivity = env->NewGlobalRef(activity);
}

JavaVM* JNISupport::getJavaVM() { return jvm; }

jobject JNISupport::getActivity() { return jactivity; }

extern "C" JNIEXPORT void JNICALL
Java_org_godotengine_plugin_android_gdextension_wwise_GDExtensionAndroidPlugin_initPlugin(
		JNIEnv* env, jobject thiz, jobject activity)
{
	JNISupport::initialize(env, activity);
}