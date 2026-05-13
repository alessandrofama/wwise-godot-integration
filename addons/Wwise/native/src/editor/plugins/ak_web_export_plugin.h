#pragma once

#include <godot_cpp/classes/editor_export_platform.hpp>
#include <godot_cpp/classes/editor_export_plugin.hpp>

using namespace godot;

class AkWebExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkWebExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

public:
	virtual String _get_name() const override;
	virtual bool _supports_platform(const Ref<EditorExportPlatform>& platform) const override;
	virtual void _export_begin(
			const PackedStringArray& p_features, bool p_is_debug, const String& p_path, uint32_t p_flags) override;
};
