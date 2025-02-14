#if defined(TOOLS_ENABLED)
#include "editor/plugins/ak_json_builder.cpp"
#include "editor/plugins/ak_soundbank_directory_watcher_plugin.cpp"
#include "editor/wwise_project_database.cpp"
#endif

#include "core/ak_midi_post.h"
#include "core/ak_utils.cpp"
#include "core/types/wwise_acoustic_texture.h"
#include "core/types/wwise_aux_bus.cpp"
#include "core/types/wwise_bank.cpp"
#include "core/types/wwise_base_type.cpp"
#include "core/types/wwise_event.cpp"
#include "core/types/wwise_group_type.cpp"
#include "core/types/wwise_rtpc.cpp"
#include "core/types/wwise_state.cpp"
#include "core/types/wwise_state_group.h"
#include "core/types/wwise_switch.cpp"
#include "core/types/wwise_switch_group.h"
#include "core/types/wwise_trigger.cpp"
#include "core/wwise_external_source_info.h"
#include "core/wwise_gdextension.cpp"
#include "core/wwise_io_hook.cpp"
#include "core/wwise_platform_info.cpp"
#include "core/wwise_plugin_info.cpp"
#include "core/wwise_settings.cpp"
#include "scene/ak_bank.cpp"
#include "scene/ak_early_reflections.cpp"
#include "scene/ak_environment.cpp"
#include "scene/ak_environment_data.cpp"
#include "scene/ak_event.cpp"
#include "scene/ak_game_obj.h"
#include "scene/ak_game_obj_2d.h"
#include "scene/ak_game_obj_3d.h"
#include "scene/ak_geometry.cpp"
#include "scene/ak_listener.cpp"
#include "scene/ak_portal.cpp"
#include "scene/ak_room.cpp"
#include "scene/ak_state.cpp"
#include "scene/ak_switch.cpp"

#if defined(TOOLS_ENABLED)
#include "editor/ak_editor_settings.h"
#include "editor/ak_editor_utils.cpp"
#include "editor/ak_wwise_inspector_picker.cpp"
#include "editor/ak_wwise_tree.cpp"
#include "editor/ak_wwise_tree_data_source.cpp"
#include "editor/ak_wwise_tree_item.cpp"
#include "editor/plugins/ak_android_export_plugin.cpp"
#include "editor/plugins/ak_editor_export_plugin.cpp"
#include "editor/plugins/ak_editor_plugin.cpp"
#include "editor/plugins/ak_event_3d_gizmo_plugin.cpp"
#include "editor/plugins/ak_wwise_editor_inspector_plugin.cpp"
#include "editor/plugins/wwise_browser.cpp"
#include "editor/plugins/wwise_project_info.cpp"
#include "editor/properties/ak_wwise_editor_property.cpp"
#include "editor/properties/ak_wwise_editor_property_acoustic_texture.cpp"
#include "editor/properties/ak_wwise_editor_property_aux_bus.cpp"
#include "editor/properties/ak_wwise_editor_property_bank.cpp"
#include "editor/properties/ak_wwise_editor_property_event.cpp"
#include "editor/properties/ak_wwise_editor_property_rtpc.cpp"
#include "editor/properties/ak_wwise_editor_property_state.cpp"
#include "editor/properties/ak_wwise_editor_property_switch.cpp"
#include "editor/properties/ak_wwise_editor_property_trigger.cpp"
#include "editor/wwise_base_information.cpp"
#include "editor/wwise_project_data.cpp"
#include "editor/wwise_tree_object.cpp"
#include "editor/wwise_tree_object_event.cpp"
#include "gen/doc_data.gen.cpp"
#endif

#if defined(AK_WIN) || defined(AK_MAC_OS_X)
#include "core/waapi_gdextension.cpp"
static Waapi* waapi_module;
#endif

static Wwise* wwise_module;
static WwiseSettings* wwise_settings;
static AkUtils* ak_utils;
#if defined(TOOLS_ENABLED)
static WwiseProjectDatabase* wwise_project_database;
#endif

void register_wwise_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
		ClassDB::register_class<Waapi>();
		waapi_module = memnew(Waapi);
		Engine::get_singleton()->register_singleton("Waapi", Waapi::get_singleton());
#endif

#if defined(AK_WIN) || defined(AK_MAC_OS_X) || defined(AK_LINUX)
#if defined(TOOLS_ENABLED)
		ClassDB::register_abstract_class<AkWwiseEditorProperty>();
		ClassDB::register_class<AkWwiseEditorInspectorPlugin>();
		ClassDB::register_class<AkWwiseEditorPropertyEvent>();
		ClassDB::register_class<AkWwiseEditorPropertyBank>();
		ClassDB::register_class<AkWwiseEditorPropertyAcousticTexture>();
		ClassDB::register_class<AkWwiseEditorPropertyState>();
		ClassDB::register_class<AkWwiseEditorPropertySwitch>();
		ClassDB::register_class<AkWwiseEditorPropertyRTPC>();
		ClassDB::register_class<AkWwiseEditorPropertyTrigger>();
		ClassDB::register_class<AkWwiseEditorPropertyAuxBus>();

		ClassDB::register_class<AkWwiseTreeItem>();
		ClassDB::register_class<AkWwiseTree>();
		ClassDB::register_class<AkWwiseTreeDataSource>();
		ClassDB::register_class<AkWwiseInspectorPicker>();

		ClassDB::register_class<AkEvent3DGizmoPlugin>();
		ClassDB::register_class<AkEditorExportPlugin>();
		ClassDB::register_class<AkAndroidExportPlugin>();
		ClassDB::register_class<WwiseBaseInformation>();
		ClassDB::register_class<WwiseTreeObject>();
		ClassDB::register_class<WwiseTreeObjectEvent>();
		ClassDB::register_class<WwiseProjectData>();
		ClassDB::register_class<WwiseProjectDatabase>();
		wwise_project_database = memnew(WwiseProjectDatabase);
		Engine::get_singleton()->register_singleton("WwiseProjectDatabase", WwiseProjectDatabase::get_singleton());
		ClassDB::register_class<WwiseProjectInfo>();
		EditorPlugins::add_by_type<WwiseProjectInfo>();
		ClassDB::register_class<AkSoundBankDirectoryWatcherPlugin>();
		EditorPlugins::add_by_type<AkSoundBankDirectoryWatcherPlugin>();
		ClassDB::register_class<AkJSONBuilder>();
		EditorPlugins::add_by_type<AkJSONBuilder>();
		ClassDB::register_class<AkEditorPlugin>();
		EditorPlugins::add_by_type<AkEditorPlugin>();
		ClassDB::register_class<WwiseBrowser>();
		EditorPlugins::add_by_type<WwiseBrowser>();
		ClassDB::register_class<AkEditorUtils>();
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

		ClassDB::register_abstract_class<WwiseBaseType>();
		ClassDB::register_abstract_class<WwiseGroupType>();
		ClassDB::register_class<WwiseEvent>();
		ClassDB::register_class<WwiseBank>();
		ClassDB::register_class<WwiseAcousticTexture>();
		ClassDB::register_class<WwiseState>();
		ClassDB::register_class<WwiseStateGroup>();
		ClassDB::register_class<WwiseSwitch>();
		ClassDB::register_class<WwiseSwitchGroup>();
		ClassDB::register_class<WwiseRTPC>();
		ClassDB::register_class<WwiseTrigger>();
		ClassDB::register_class<WwiseAuxBus>();
		ClassDB::register_class<AkMidiPost>();
		ClassDB::register_class<WwiseExternalSourceInfo>();
		ClassDB::register_class<WwisePluginInfo>();
		ClassDB::register_class<WwisePlatformInfo>();

		ClassDB::register_class<AkGameObj>();
		ClassDB::register_class<AkGameObj2D>();
		ClassDB::register_class<AkGameObj3D>();
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
		if (Engine::get_singleton()->has_singleton("WwiseProjectDatabase"))
		{
			Engine::get_singleton()->unregister_singleton("WwiseProjectDatabase");
			memdelete(wwise_project_database);
		}
#endif
#endif
	}
}

extern "C"
{
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