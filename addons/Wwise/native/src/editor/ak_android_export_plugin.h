#pragma once

#include <godot_cpp/classes/editor_export_platform.hpp>
#include <godot_cpp/classes/editor_export_plugin.hpp>

using namespace godot;

class AkAndroidExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkAndroidExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

public:
	virtual String _get_name() const override;
	virtual bool _supports_platform(const Ref<EditorExportPlatform>& platform) const override;
	virtual PackedStringArray _get_android_libraries(
			const Ref<EditorExportPlatform>& platform, bool debug) const override;
};