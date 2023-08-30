#ifndef AK_EDITOR_EXPORT_PLUGIN
#define AK_EDITOR_EXPORT_PLUGIN

#include <godot_cpp/classes/editor_export_plugin.hpp>
#include <godot_cpp/classes/project_settings.hpp>

namespace godot
{
class AkEditorExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkEditorExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

private:
public:
	virtual String _get_name() const override { return "AkEditorExportPlugin"; }
	virtual void _export_file(const String& path, const String& type, const PackedStringArray& features) override
	{
		String base_path = ProjectSettings::get_singleton()->get_setting("wwise/common_user_settings/base_path");

		if (!base_path.is_empty())
		{
			for (int i = 0; i < features.size(); ++i)
			{
				if (path.findn(base_path) != -1)
				{
					if (features[i] == "windows")
					{
						if (path.findn("Linux") != -1 || path.findn("Mac") != -1 || path.findn("Android") != -1 ||
								path.findn("iOS") != -1)
						{
							skip();
						}
					}
					else if (features[i] == "macos")
					{
						if (path.findn("Windows") != -1 || path.findn("Linux") != -1 || path.findn("Android") != -1 ||
								path.findn("iOS") != -1)
						{
							skip();
						}
					}
					else if (features[i] == "linux")
					{
						if (path.findn("Windows") != -1 || path.findn("Mac") != -1 || path.findn("Android") != -1 ||
								path.findn("iOS") != -1)
						{
							skip();
						}
					}
					else if (features[i] == "android")
					{
						if (path.findn("Windows") != -1 || path.findn("Mac") != -1 || path.findn("Linux") != -1 ||
								path.findn("iOS") != -1)
						{
							skip();
						}
					}
					else if (features[i] == "ios")
					{
						if (path.findn("Windows") != -1 || path.findn("Mac") != -1 || path.findn("Linux") != -1 ||
								path.findn("Android") != -1)
						{
							skip();
						}
					}
				}
			}
		}
	}
};

} //namespace godot

#endif
