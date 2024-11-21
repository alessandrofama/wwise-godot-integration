#include <algorithm>
#include <memory>

#include "core/ak_utils.cpp"
#include "core/wwise_cookie.cpp"
#include "core/wwise_gdextension.cpp"
#include "core/wwise_io_hook.cpp"
#include "core/wwise_settings.cpp"
#include "scene/ak_bank.cpp"
#include "scene/ak_early_reflections.cpp"
#include "scene/ak_environment.cpp"
#include "scene/ak_environment_data.cpp"
#include "scene/ak_event.cpp"
#include "scene/ak_geometry.cpp"
#include "scene/ak_listener.cpp"
#include "scene/ak_portal.cpp"
#include "scene/ak_room.cpp"
#include "scene/ak_state.cpp"
#include "scene/ak_switch.cpp"

#if defined(TOOLS_ENABLED)
#include "gen/doc_data.gen.cpp"
#include "editor/ak_android_export_plugin.cpp"
#include "editor/ak_editor_export_plugin.cpp"
#include "editor/ak_editor_plugin.cpp"
#include "editor/ak_editor_utils.cpp"
#include "editor/ak_event_3d_gizmo_plugin.cpp"
#include "editor/ak_inspector_plugin.cpp"
#endif

#if defined(AK_WIN) || defined(AK_MAC_OS_X)
#include "core/waapi_gdextension.cpp"
#if defined(TOOLS_ENABLED)
#include "editor/ak_waapi_picker.cpp"
#endif
static Waapi* waapi_module;
#endif

static Wwise* wwise_module;
static WwiseSettings* wwise_settings;
static AkUtils* ak_utils;

#if defined(TOOLS_ENABLED)
static AkEditorUtils* ak_editor_utils;
#endif

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
		ClassDB::register_class<AkEditorUtils>();
		ak_editor_utils = memnew(AkEditorUtils);
		Engine::get_singleton()->register_singleton("AkEditorUtils", AkEditorUtils::get_singleton());
		ClassDB::register_class<AkInspectorEditor>();
		ClassDB::register_class<AkInspectorTree>();
		ClassDB::register_class<AkInspectorEditorInspectorPlugin>();
		ClassDB::register_class<AkInspectorEditorProperty>();
		ClassDB::register_class<AkEvent3DGizmoPlugin>();
		ClassDB::register_class<AkEditorExportPlugin>();
		ClassDB::register_class<AkAndroidExportPlugin>();
		ClassDB::register_class<AkEditorPlugin>();
		EditorPlugins::add_by_type<AkEditorPlugin>();
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

		ClassDB::register_class<WwiseSettings>();
		wwise_settings = memnew(WwiseSettings);

		ClassDB::register_class<WwiseCookie>();
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
		if (Engine::get_singleton()->has_singleton("AkEditorUtils"))
		{
			Engine::get_singleton()->unregister_singleton("AkEditorUtils");
			memdelete(ak_editor_utils);
		}
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