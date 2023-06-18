#include "register_types.h"

using namespace godot;

static Wwise* wwise_module;
static WwiseSettings* wwise_settings;

void register_wwise_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		ClassDB::register_class<AkUtils>();
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		// Wwise module
		ClassDB::register_class<Wwise>();
		wwise_module = memnew(Wwise);
		Engine::get_singleton()->register_singleton("Wwise", Wwise::get_singleton());

		ClassDB::register_class<WwiseSettings>();
		wwise_settings = memnew(WwiseSettings);

		ClassDB::register_class<AkBank>();
		ClassDB::register_class<AkListener2D>();
		ClassDB::register_class<AkListener3D>();
		ClassDB::register_class<AkState>();
		ClassDB::register_class<AkSwitch>();
	}
}

void unregister_wwise_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		Engine::get_singleton()->unregister_singleton("Wwise");
		memdelete(wwise_module);
		memdelete(wwise_settings);
	}
}

extern "C"
{
	// Initialization.

	GDExtensionBool GDE_EXPORT wwise_library_init(const GDExtensionInterface* p_interface,
			const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(register_wwise_types);
		init_obj.register_terminator(unregister_wwise_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}