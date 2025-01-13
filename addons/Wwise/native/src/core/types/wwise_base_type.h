#pragma once

#if defined(TOOLS_ENABLED)
#include "editor/ak_editor_settings.h"
#endif

#include "core/utils.h"
#include "core/wwise_object_type.h"
#include "editor/short_id_generator.h"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

using namespace godot;

class WwiseBaseType : public Resource
{
	GDCLASS(WwiseBaseType, Resource);

protected:
	static void _bind_methods();

private:
	String name{};
	uint32_t id{ AK_INVALID_UNIQUE_ID };
	String guid{};
	bool dummy{};

public:
	void set_name(const String& p_name);
	String get_name() const;

	void set_id(uint32_t p_id);
	uint32_t get_id() const;

	void set_guid(const String& p_guid);
	String get_guid() const;

	void set_dummy(bool p_dummy);
	bool get_dummy() const;

	virtual String get_display_name() { return name; }
	virtual WwiseObjectType get_object_type() const = 0;

#if defined(TOOLS_ENABLED)
	template <typename T> static Ref<T> create(WwiseObjectType wwise_object_type, String guid);

	template <typename T>
	static Ref<T> find_existing_wwise_object(WwiseObjectType wwise_object_type, const String& path);

	template <typename T>
	static bool update_wwise_object_data(const Ref<T>& wwise_object_reference, const String& name);

	template <typename T>
	static Ref<T> find_or_create_wwise_object(
			WwiseObjectType wwise_object_type, const String& name, const String& guid);

	template <typename T>
	static void update_wwise_object(WwiseObjectType wwise_object_type, const String& name, const String& guid);

	static String get_parent_path(WwiseObjectType wwiseObjectType)
	{
		return AkEditorSettings::get_wwise_resource_relative_path().path_join(
				wwise_object_type_str[(int)wwiseObjectType]);
	}

	static String get_asset_file_name(const String& guid) { return guid.to_upper() + String(".tres"); }

#endif
};
