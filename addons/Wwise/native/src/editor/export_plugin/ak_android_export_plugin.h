#ifndef AK_ANDROID_EXPORT_PLUGIN
#define AK_ANDROID_EXPORT_PLUGIN

#include <godot_cpp/classes/editor_export_platform.hpp>
#include <godot_cpp/classes/editor_export_plugin.hpp>

namespace godot
{
class AkAndroidExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkAndroidExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

private:
public:
	virtual String _get_name() const override { return "AkAndroidExportPlugin"; }

	virtual bool _supports_platform(const Ref<EditorExportPlatform>& platform) const override
	{
		return platform->get_os_name() == "Android";
	}

	virtual PackedStringArray _get_android_libraries(
			const Ref<EditorExportPlatform>& platform, bool debug) const override
	{
		String lib_path = "res://addons/Wwise/native/lib/android/";
		if (debug)
		{
			lib_path += "debug/WwiseAndroidPlugin-debug.aar";
		}
		else
		{
			lib_path += "release/WwiseAndroidPlugin-release.aar";
		}

		PackedStringArray packed;

		Array arr;
		arr.append(lib_path);
		packed.append_array(arr);
		return packed;
	}
};

} //namespace godot

#endif