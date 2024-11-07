#include <algorithm>
#include <memory>

#include <gdextension_interface.h>
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/editor_export_platform.hpp>
#include <godot_cpp/classes/editor_export_plugin.hpp>
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_node3d_gizmo_plugin.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_data_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkVirtualAcoustics.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkObject.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <AK/Tools/Common/AkLock.h>
#include <AK/Tools/Common/AkMonitorError.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

#if TOOLS_ENABLED
#endif

using namespace godot;

#if defined(AK_ANDROID)
#include "platform/android/jni_support.cpp"
#endif

#include "common/ak_wwise_base_type.cpp"
#include "common/ak_wwise_event.h"

#include "common/wwise_object_type.h"
#include "common/utils.h"
#include "common/ak_utils.cpp"
#include "common/ak_io_hook.cpp"
#include "common/ak_cookie.cpp"
#include "common/wwise_gdextension.cpp"
#include "common/ak_settings.cpp"
#include "scene/ak_bank.cpp"
#include "scene/ak_early_reflections.cpp"
#include "scene/ak_event.cpp"
#include "scene/ak_environment.cpp"
#include "scene/ak_environment_data.cpp"
#include "scene/ak_geometry.cpp"
#include "scene/ak_listener.cpp"
#include "scene/ak_portal.cpp"
#include "scene/ak_room.cpp"
#include "scene/ak_state.cpp"
#include "scene/ak_switch.cpp"

#if defined(TOOLS_ENABLED)
#include "gen/doc_data.gen.cpp"
#include "editor/short_id_generator.cpp"
#include "editor/ak_editor_settings.h"
#include "common/wwise_object_reference.cpp"
#include "common/wwise_bank_reference.cpp"
#include "editor/ak_android_export_plugin.cpp"
#include "editor/ak_editor_export_plugin.cpp"
#include "editor/ak_editor_utils.cpp"
#include "editor/ak_event_3d_gizmo_plugin.cpp"
#include "editor/ak_inspector_plugin.cpp"
#include "editor/ak_editor_plugin.cpp"
#include "WwiseProjectDatabase.h"
#endif


#if defined(AK_WIN) || defined(AK_MAC_OS_X)
#include <AK/WwiseAuthoringAPI/AkAutobahn/Client.h>
#include <AK/WwiseAuthoringAPI/AkAutobahn/JsonProvider.h>
#include "common/waapi_gdextension.cpp"
#if defined(TOOLS_ENABLED)
#include "editor/ak_waapi_picker.cpp"
#endif
static Waapi* waapi_module;
#endif

static Wwise* wwise_module;
static AkSettings* wwise_settings;
static AkUtils* ak_utils;

void register_wwise_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
		ClassDB::register_class<Waapi>();
		waapi_module = memnew(Waapi);
		Engine::get_singleton()->register_singleton("Waapi", Waapi::get_singleton());
#if defined(TOOLS_ENABLED)
		ClassDB::register_class<AkWaapiPicker>();
		EditorPlugins::add_by_type<AkWaapiPicker>();
#endif
#endif

#if defined(AK_WIN) || defined(AK_MAC_OS_X) || defined(AK_LINUX)
#if defined(TOOLS_ENABLED)
		ClassDB::register_class<AkInspectorEditor>();
		ClassDB::register_class<AkInspectorTree>();
		ClassDB::register_class<AkInspectorEditorInspectorPlugin>();
		ClassDB::register_class<AkInspectorEditorProperty>();
		ClassDB::register_class<AkWwiseEventEditorProperty>();
		ClassDB::register_class<AkEvent3DGizmoPlugin>();
		ClassDB::register_class<AkEditorExportPlugin>();
		ClassDB::register_class<AkAndroidExportPlugin>();
		ClassDB::register_class<AkEditorPlugin>();
		EditorPlugins::add_by_type<AkEditorPlugin>();

		ClassDB::register_abstract_class<AkWwiseBaseType>();
		ClassDB::register_class<AkWwiseEvent>();
#endif
#endif
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<AkUtils>();
		ak_utils = memnew(AkUtils);
		Engine::get_singleton()->register_singleton("AkUtils", AkUtils::get_singleton());

		ClassDB::register_class<Wwise>();
		wwise_module = memnew(Wwise);
		Engine::get_singleton()->register_singleton("Wwise", Wwise::get_singleton());

		ClassDB::register_class<AkSettings>();
		wwise_settings = memnew(AkSettings);

		ClassDB::register_class<AkCookie>();
		ClassDB::register_class<AkBank>();
		ClassDB::register_class<AkListener2D>();
		ClassDB::register_class<AkListener3D>();
		ClassDB::register_class<AkState>();
		ClassDB::register_class<AkSwitch>();
		ClassDB::register_class<AkEnvironmentData>();
		ClassDB::register_class<AkEnvironment>();
		ClassDB::register_class<AkEvent2D>();
		ClassDB::register_class<AkEvent3D>();
		ClassDB::register_class<AkPortal>();
		ClassDB::register_class<AkGeometry>();
		ClassDB::register_class<AkRoom>();
		ClassDB::register_class<AkEarlyReflections>();
	}
}

void unregister_wwise_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		if (Engine::get_singleton()->has_singleton("Wwise"))
		{
			Engine::get_singleton()->unregister_singleton("Wwise");
			memdelete(wwise_module);
		}
		if (Engine::get_singleton()->has_singleton("AkUtils"))
		{
			Engine::get_singleton()->unregister_singleton("AkUtils");
			memdelete(ak_utils);
		}

		memdelete(wwise_settings);
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
		if (Engine::get_singleton()->has_singleton("Waapi"))
		{
			Engine::get_singleton()->unregister_singleton("Waapi");
			memdelete(waapi_module);
		}
#endif

#if defined(AK_WIN) || defined(AK_MAC_OS_X) || defined(AK_LINUX)
#if defined(TOOLS_ENABLED)
#endif
#endif
	}
}

extern "C"
{
	// Initialization.

	GDExtensionBool GDE_EXPORT wwise_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
			GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(register_wwise_types);
		init_obj.register_terminator(unregister_wwise_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

		return init_obj.init();
	}
}