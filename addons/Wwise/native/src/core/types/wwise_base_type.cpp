#include "wwise_base_type.h"
#include "core/types/wwise_aux_bus.h"
#include "core/types/wwise_event.h"

void WwiseBaseType::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_name", "name"), &WwiseBaseType::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &WwiseBaseType::get_name);
	ClassDB::bind_method(D_METHOD("set_id", "id"), &WwiseBaseType::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &WwiseBaseType::get_id);
	ClassDB::bind_method(D_METHOD("set_guid", "guid"), &WwiseBaseType::set_guid);
	ClassDB::bind_method(D_METHOD("get_guid"), &WwiseBaseType::get_guid);
	ClassDB::bind_method(D_METHOD("set_dummy", "dummy"), &WwiseBaseType::set_dummy);
	ClassDB::bind_method(D_METHOD("get_dummy"), &WwiseBaseType::get_dummy);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name", PROPERTY_HINT_NONE, ""), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id", PROPERTY_HINT_NONE, ""), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "guid", PROPERTY_HINT_NONE, ""), "set_guid", "get_guid");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dummy", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_dummy",
			"get_dummy");

	ADD_SIGNAL(MethodInfo("ws_post_resource_init"));
}

void WwiseBaseType::set_name(const String& p_name)
{
	name = p_name;
	emit_changed();
}

String WwiseBaseType::get_name() const { return name; }

void WwiseBaseType::set_id(uint32_t p_id)
{
	id = p_id;
	emit_changed();
}

uint32_t WwiseBaseType::get_id() const { return id; }

void WwiseBaseType::set_guid(const String& p_guid)
{
	guid = p_guid;
	emit_changed();
}

String WwiseBaseType::get_guid() const
{
	if (guid.is_empty())
	{
		return "00000000-0000-0000-0000-000000000000";
	}
	else
	{
		return guid;
	}
}

void WwiseBaseType::set_dummy(bool p_dummy)
{
	dummy = p_dummy;
	emit_changed();
	RETURN_IF_EDITOR;

	// note (alex): this is a workaround to ensure all properties are fully constructed before they are accessed in
	// WwiseEvent. Godot automatically calls the setter for properties that are set to non-default values. We explicitly
	// emit a signal here, which is the last property to be initialized, to indicate that all properties are ready. In
	// WwiseEvent::_notification, we connect to this signal to trigger the bank load once the object is fully
	// initialized.

	emit_signal("ws_post_resource_init");
}

bool WwiseBaseType::get_dummy() const { return dummy; }

#ifdef TOOLS_ENABLED
template <typename T> Ref<T> WwiseBaseType::create(WwiseObjectType wwise_object_type, String guid)
{
	static_assert(std::is_base_of<WwiseBaseType, T>::value, "T must be a subclass of WwiseBaseType");

	Ref<T> object_reference = memnew(T);
	object_reference.instantiate();
	object_reference->set_guid(guid.to_upper());
	object_reference->set_dummy(true);
	return object_reference;
}

template <typename T>
Ref<T> WwiseBaseType::find_existing_wwise_object(WwiseObjectType wwise_object_type, const String& path)
{
	Ref<T> asset;
	bool exists = ResourceLoader::get_singleton()->exists(path, T::get_class_static());
	if (exists)
	{
		asset = ResourceLoader::get_singleton()->load(path);
	}

	if (asset.is_valid())
	{
		return asset;
	}

	return Ref<WwiseBaseType>();
}

template <typename T>
bool WwiseBaseType::update_wwise_object_data(const Ref<T>& wwise_object_reference, const String& name)
{
	uint32_t id = ShortIDGenerator::compute(name);
	bool changed = false;
	if (wwise_object_reference->get_name() != name || wwise_object_reference->get_id() != id)
		changed = true;

	wwise_object_reference->set_name(name);
	wwise_object_reference->set_id(id);

	return changed;
}

template <typename T>
inline Ref<T> WwiseBaseType::find_or_create_wwise_object(
		WwiseObjectType wwise_object_type, const String& name, const String& guid)
{
	String parent_path = get_parent_path(wwise_object_type);
	String path = parent_path.path_join(get_asset_file_name(guid));
	Ref<T> asset = find_existing_wwise_object<T>(wwise_object_type, path);
	auto asset_exists = asset.is_valid();
	if (!asset_exists)
	{
		make_dir_recursive(parent_path);
		asset = create<T>(wwise_object_type, guid);
	}

	bool changed = update_wwise_object_data<T>(asset, name);

	if (wwise_object_type == WwiseObjectType::Event)
	{
		Ref<WwiseEvent> event = asset;
		event->update_is_user_defined_soundbank();
	}
	else if (wwise_object_type == WwiseObjectType::AuxBus)
	{
		Ref<WwiseAuxBus> aux_bus = asset;
		aux_bus->update_is_user_defined_soundbank();
	}

	if (!asset_exists || changed)
	{
		ResourceSaver::get_singleton()->save(asset, path);
	}

	ResourceSaver::get_singleton()->save(asset, path);
	EditorInterface::get_singleton()->get_resource_filesystem()->scan();
	EditorInterface::get_singleton()->get_resource_filesystem()->update_file(path);
	asset = ResourceLoader::get_singleton()->load(path);

	return asset;
}

template <typename T>
void WwiseBaseType::update_wwise_object(WwiseObjectType wwise_object_type, const String& name, const String& guid)
{
	String parent_path = get_parent_path(wwise_object_type);
	String asset_path = parent_path.path_join(get_asset_file_name(guid));

	Ref<T> wwise_asset = find_existing_wwise_object<T>(wwise_object_type, asset_path);
	if (!wwise_asset.is_valid())
	{
		return;
	}

	bool data_changed = update_wwise_object_data<T>(wwise_asset, name);

	bool user_bank_changed = false;
	if (wwise_object_type == WwiseObjectType::Event)
	{
		Ref<WwiseEvent> event_asset = wwise_asset;
		user_bank_changed = event_asset->update_is_user_defined_soundbank();
	}
	else if (wwise_object_type == WwiseObjectType::AuxBus)
	{
		Ref<WwiseAuxBus> aux_bus = wwise_asset;
		user_bank_changed = aux_bus->update_is_user_defined_soundbank();
	}

	if (data_changed || user_bank_changed)
	{
		auto save_result = ResourceSaver::get_singleton()->save(wwise_asset, asset_path);
		if (save_result != Error::OK)
		{
			UtilityFunctions::push_warning(vformat("WwiseGodot: Failed to save Wwise Object at path: %s", asset_path));
			return;
		}

		auto editor_interface = EditorInterface::get_singleton();
		editor_interface->get_resource_filesystem()->update_file(asset_path);
	}
}
#endif