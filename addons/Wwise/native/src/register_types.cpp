#include "register_types.h"

using namespace godot;

static Wwise* wwise_module;
static Waapi* waapi_module;
static WwiseSettings* wwise_settings;
static AkUtils* ak_utils;
static AkEditorUtils* ak_editor_utils;

void register_wwise_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
		ClassDB::register_class<Waapi>();
		waapi_module = memnew(Waapi);
		Engine::get_singleton()->register_singleton("Waapi", Waapi::get_singleton());

		ClassDB::register_class<WaapiPicker>();
		EditorPlugins::add_by_type<WaapiPicker>();
#endif

#if defined(AK_WIN) || defined(AK_MAC_OS_X) || defined(AK_LINUX)
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
		ClassDB::register_class<WwiseEditorPlugin>();
		EditorPlugins::add_by_type<WwiseEditorPlugin>();
#endif
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<AkUtils>();
		ak_utils = memnew(AkUtils);
		Engine::get_singleton()->register_singleton("AkUtils", AkUtils::get_singleton());

		// Wwise module
		ClassDB::register_class<Wwise>();
		wwise_module = memnew(Wwise);
		Engine::get_singleton()->register_singleton("Wwise", Wwise::get_singleton());

		ClassDB::register_class<WwiseSettings>();
		wwise_settings = memnew(WwiseSettings);

		ClassDB::register_class<CookieWrapper>();
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
		if (Engine::get_singleton()->has_singleton("AkEditorUtils"))
		{
			Engine::get_singleton()->unregister_singleton("AkEditorUtils");
			memdelete(ak_editor_utils);
		}
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