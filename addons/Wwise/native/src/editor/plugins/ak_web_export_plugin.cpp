#include "ak_web_export_plugin.h"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

String AkWebExportPlugin::_get_name() const { return "AkWebExportPlugin"; }

bool AkWebExportPlugin::_supports_platform(const Ref<EditorExportPlatform>& platform) const
{
	return platform->get_os_name() == "Web";
}

void AkWebExportPlugin::_export_begin(
		const PackedStringArray& p_features, bool p_is_debug, const String& p_path, uint32_t p_flags)
{
	const String worklet_src = "res://addons/Wwise/native/lib/web/WwiseAudioWorklet.processor.js";

	if (!FileAccess::file_exists(worklet_src))
	{
		UtilityFunctions::push_warning(vformat(
				"WwiseGodot: AkWebExportPlugin: %s not found. Copy it from the Wwise SDK "
				"samples/SoundEngine/Emscripten/ directory.",
				worklet_src));
		return;
	}

	Ref<FileAccess> file = FileAccess::open(worklet_src, FileAccess::ModeFlags::READ);
	if (!file.is_valid())
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkWebExportPlugin: Failed to open %s.", worklet_src));
		return;
	}

	PackedByteArray data = file->get_buffer(file->get_length());
	file->close();

	String export_dir = p_path.get_base_dir();
	String dst_path = export_dir.path_join("WwiseAudioWorklet.processor.js");

	Ref<FileAccess> out = FileAccess::open(dst_path, FileAccess::ModeFlags::WRITE);
	if (!out.is_valid())
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkWebExportPlugin: Failed to write %s.", dst_path));
		return;
	}
	out->store_buffer(data);
	out->close();

	UtilityFunctions::print(vformat("WwiseGodot: Deployed WwiseAudioWorklet.processor.js to %s.", dst_path));
}
