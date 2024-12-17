#include <android/log.h>
#include <jni.h>

class JNISupport
{
private:
	static JavaVM* jvm;
	static jobject jactivity;

public:
	static void initialize(JNIEnv* env, jobject activity);
	static JavaVM* getJavaVM();
	static jobject getActivity();
};