#include <android/log.h>
#include <jni.h>
#include <string>

class JNISupport
{
private:
	static JavaVM* jvm;
	static jobject jactivity;
	static std::string plugins_dir;

public:
	static void initialize(JNIEnv* env, jobject activity, jstring nativeLibDir);
	static JavaVM* getJavaVM();
	static jobject getActivity();
	static std::string getPluginsDir();
};