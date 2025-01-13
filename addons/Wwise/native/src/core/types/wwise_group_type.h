#pragma once

#include "core/types/wwise_base_type.h"

using namespace godot;

class WwiseGroupType : public WwiseBaseType
{
	GDCLASS(WwiseGroupType, WwiseBaseType);

protected:
	static void _bind_methods();

private:
	uint32_t group_id{ AK_INVALID_UNIQUE_ID };
	Ref<WwiseBaseType> group_ref;

public:
	void set_group_id(uint32_t p_id);
	uint32_t get_group_id() const;

	void set_group_ref(const Ref<WwiseBaseType>& p_group_ref);
	Ref<WwiseBaseType> get_group_ref() const;

	virtual String get_display_name() override;

#ifdef TOOLS_ENABLED
	template <typename T>
	void setup_group_object_reference(const Ref<WwiseGroupType>& p_child_ref, const WwiseObjectType p_type,
			const String& p_name, const String& p_guid)
	{
		Ref<T> object_reference = find_or_create_wwise_object<T>(p_type, p_name, p_guid);
		if (object_reference.is_valid())
		{
			p_child_ref->set_group_ref(object_reference);
			ResourceSaver::get_singleton()->save(p_child_ref, p_child_ref->get_path());
			EditorInterface::get_singleton()->get_resource_filesystem()->scan();
			EditorInterface::get_singleton()->get_resource_filesystem()->update_file(p_child_ref->get_path());
		}
	}
#endif
};