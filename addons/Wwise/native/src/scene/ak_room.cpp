#include "ak_room.h"

void AkRoom::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_aux_bus", "aux_bus"), &AkRoom::set_aux_bus);
	ClassDB::bind_method(D_METHOD("get_aux_bus"), &AkRoom::get_aux_bus);
	ClassDB::bind_method(D_METHOD("set_reverb_level", "reverb_level"), &AkRoom::set_reverb_level);
	ClassDB::bind_method(D_METHOD("get_reverb_level"), &AkRoom::get_reverb_level);
	ClassDB::bind_method(D_METHOD("set_transmission_loss", "transmission_loss"), &AkRoom::set_transmission_loss);
	ClassDB::bind_method(D_METHOD("get_transmission_loss"), &AkRoom::get_transmission_loss);
	ClassDB::bind_method(D_METHOD("set_associated_geometry", "associated_geometry"), &AkRoom::set_associated_geometry);
	ClassDB::bind_method(D_METHOD("get_associated_geometry"), &AkRoom::get_associated_geometry);
	ClassDB::bind_method(D_METHOD("set_room_tone_event", "room_tone_event"), &AkRoom::set_room_tone_event);
	ClassDB::bind_method(D_METHOD("get_room_tone_event"), &AkRoom::get_room_tone_event);
	ClassDB::bind_method(
			D_METHOD("set_aux_send_level_to_self", "aux_send_level_to_self"), &AkRoom::set_aux_send_level_to_self);
	ClassDB::bind_method(D_METHOD("get_aux_send_level_to_self"), &AkRoom::get_aux_send_level_to_self);
	ClassDB::bind_method(D_METHOD("set_keep_registered", "keep_registered"), &AkRoom::set_keep_registered);
	ClassDB::bind_method(D_METHOD("get_keep_registered"), &AkRoom::get_keep_registered);
	ClassDB::bind_method(D_METHOD("set_room_priority", "room_priority"), &AkRoom::set_room_priority);
	ClassDB::bind_method(D_METHOD("get_room_priority"), &AkRoom::get_room_priority);
	ClassDB::bind_method(D_METHOD("set_distance_behavior", "distance_behavior"), &AkRoom::set_distance_behavior);
	ClassDB::bind_method(D_METHOD("get_distance_behavior"), &AkRoom::get_distance_behavior);
	ClassDB::bind_method(D_METHOD("set_enable_reverb_zone", "enable_reverb_zone"), &AkRoom::set_enable_reverb_zone);
	ClassDB::bind_method(D_METHOD("get_enable_reverb_zone"), &AkRoom::get_enable_reverb_zone);
	ClassDB::bind_method(D_METHOD("set_parent_room", "parent_room"), &AkRoom::set_parent_room);
	ClassDB::bind_method(D_METHOD("get_parent_room"), &AkRoom::get_parent_room);
	ClassDB::bind_method(
			D_METHOD("set_transition_region_width", "transition_region_width"), &AkRoom::set_transition_region_width);
	ClassDB::bind_method(D_METHOD("get_transition_region_width"), &AkRoom::get_transition_region_width);

	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "aux_bus", PROPERTY_HINT_NONE, "WwiseAuxBus"), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(
			PropertyInfo(Variant::FLOAT, "reverb_level", PROPERTY_HINT_NONE), "set_reverb_level", "get_reverb_level");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transmission_loss", PROPERTY_HINT_NONE), "set_transmission_loss",
			"get_transmission_loss");
	ADD_PROPERTY(
			PropertyInfo(Variant::NODE_PATH, "associated_geometry", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AkGeometry"),
			"set_associated_geometry", "get_associated_geometry");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "room_tone_event", PROPERTY_HINT_NONE, "WwiseEvent"),
			"set_room_tone_event", "get_room_tone_event");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "aux_send_level_to_self", PROPERTY_HINT_NONE),
			"set_aux_send_level_to_self", "get_aux_send_level_to_self");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_registered", PROPERTY_HINT_NONE), "set_keep_registered",
			"get_keep_registered");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_priority", PROPERTY_HINT_NONE), "set_room_priority",
			"get_room_priority");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "distance_behavior", PROPERTY_HINT_ENUM, "Substract, Exclude"),
			"set_distance_behavior", "get_distance_behavior");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_reverb_zone", PROPERTY_HINT_NONE), "set_enable_reverb_zone",
			"get_enable_reverb_zone");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "parent_room", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AkRoom"),
			"set_parent_room", "get_parent_room");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transition_region_width", PROPERTY_HINT_NONE),
			"set_transition_region_width", "get_transition_region_width");
}

PackedStringArray AkRoom::_get_configuration_warnings() const
{
	PackedStringArray warnings;
	if (aux_bus.is_null())
	{
		warnings.push_back("Assign an AuxBus to register this AkRoom with Wwise.");
	}

	if (!enable_reverb_zone)
	{
		return warnings;
	}

	if (transition_region_width < 0.0f)
	{
		warnings.push_back("Transition region width cannot be negative. It will be clamped to 0 at runtime.");
	}

	if (parent_room.is_empty() || !is_inside_tree())
	{
		return warnings;
	}

	AkRoom* parent_room_node = get_parent_room_node();

	if (parent_room_node == this)
	{
		warnings.push_back(
				"A Room cannot be its own parent. The parent will be reset to the 'Outdoors' room at runtime.");
	}
	else
	{
		if (is_a_parent_of(parent_room_node))
		{
			warnings.push_back("Parent Room creates or contains a circular dependency. "
							   "The parent will be reset to the 'Outdoors' room at runtime.");
		}
	}

	return warnings;
}

void AkRoom::_ready()
{
	RETURN_IF_EDITOR;

	if (aux_bus.is_null())
	{
		WwiseLogger::warning_format("Trying to set a Room, but the AuxBus property in the "
									"AkRoom node: %s is not set (null).",
				get_name());
		return;
	}

	AkGeometry* geometry_node{ nullptr };
	if (!associated_geometry.is_empty())
	{
		geometry_node = get_node<AkGeometry>(associated_geometry);
	}

	Transform3D normalized_transform = get_global_transform().orthonormalized();

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		if (aux_bus.is_valid())
		{
			room_registered = soundengine->set_room(this, aux_bus->get_id(), reverb_level, transmission_loss,
					normalized_transform.get_basis().get_column(2), normalized_transform.get_basis().get_column(1),
					room_tone_event.is_valid() || keep_registered,
					geometry_node != nullptr ? geometry_node->geometry_instance_object : nullptr,
					aux_send_level_to_self, room_priority);

			if (room_registered)
			{
				if (room_tone_event.is_valid())
				{
					room_tone_event->post(this);
				}

				if (enable_reverb_zone)
				{
					validate_reverb_zone_properties();
					AkRoom* parent_room_node = get_parent_room_node();
					soundengine->set_reverb_zone(this, parent_room_node, transition_region_width);
				}
			}
		}
	}
}

void AkRoom::_exit_tree()
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		if (room_registered)
		{
			if (enable_reverb_zone)
			{
				soundengine->remove_reverb_zone(this);
			}

			if (room_tone_event.is_valid())
			{
				room_tone_event->stop(this);
			}

			soundengine->remove_room(this);
		}
	}
}

void AkRoom::set_aux_bus(const Ref<WwiseAuxBus>& aux_bus)
{
	this->aux_bus = aux_bus;

#if defined(TOOLS_ENABLED)
	notify_property_list_changed();
	if (is_inside_tree())
	{
		update_configuration_warnings();
	}
#endif
}

Ref<WwiseAuxBus> AkRoom::get_aux_bus() const { return aux_bus; }

void AkRoom::set_reverb_level(float reverb_level) { this->reverb_level = reverb_level; }

float AkRoom::get_reverb_level() const { return reverb_level; }

void AkRoom::set_transmission_loss(float transmission_loss) { this->transmission_loss = transmission_loss; }

float AkRoom::get_transmission_loss() const { return transmission_loss; }

void AkRoom::set_associated_geometry(const NodePath& associated_geometry)
{
	this->associated_geometry = associated_geometry;
}

NodePath AkRoom::get_associated_geometry() const { return associated_geometry; }

void AkRoom::set_room_tone_event(const Ref<WwiseEvent>& p_room_tone_event)
{
	room_tone_event = p_room_tone_event;

#if defined(TOOLS_ENABLED)
	set_keep_registered(room_tone_event.is_valid());
	notify_property_list_changed();
#endif
}

Ref<WwiseEvent> AkRoom::get_room_tone_event() const { return room_tone_event; }

void AkRoom::set_aux_send_level_to_self(float p_aux_send_level_to_self)
{
	aux_send_level_to_self = p_aux_send_level_to_self;
}

float AkRoom::get_aux_send_level_to_self() const { return aux_send_level_to_self; }

void AkRoom::set_keep_registered(bool keep_registered) { this->keep_registered = keep_registered; }

bool AkRoom::get_keep_registered() const { return keep_registered; }

void AkRoom::set_room_priority(float p_room_priority) { room_priority = p_room_priority; }

float AkRoom::get_room_priority() const { return room_priority; }

void AkRoom::set_distance_behavior(AkUtils::AkRoomDistanceBehavior p_distance_behavior)
{
	distance_behavior = p_distance_behavior;
}

AkUtils::AkRoomDistanceBehavior AkRoom::get_distance_behavior() const { return distance_behavior; }

void AkRoom::set_enable_reverb_zone(bool p_enable)
{
	enable_reverb_zone = p_enable;

#if defined(TOOLS_ENABLED)
	if (is_inside_tree())
	{
		update_configuration_warnings();
	}
#endif
}

bool AkRoom::get_enable_reverb_zone() const { return enable_reverb_zone; }

void AkRoom::set_parent_room(const NodePath& p_parent_room)
{
	parent_room = p_parent_room;

#if defined(TOOLS_ENABLED)
	if (is_inside_tree())
	{
		update_configuration_warnings();
	}
#endif
}

NodePath AkRoom::get_parent_room() const { return parent_room; }

void AkRoom::set_transition_region_width(float p_width)
{
	transition_region_width = p_width;

#if defined(TOOLS_ENABLED)
	if (is_inside_tree())
	{
		update_configuration_warnings();
	}
#endif
}

float AkRoom::get_transition_region_width() const { return transition_region_width; }

AkRoom* AkRoom::get_parent_room_node() const
{
	return parent_room.is_empty() ? nullptr : Object::cast_to<AkRoom>(get_node_or_null(parent_room));
}

void AkRoom::validate_reverb_zone_properties()
{
	if (transition_region_width < 0.0f)
	{
		WwiseLogger::warning_format("Transition region width cannot be a negative number. "
							 "It has been clamped to 0. Node: %s", get_name());
		transition_region_width = 0.0f;
	}

	AkRoom* parent_room_node = get_parent_room_node();

	if (!parent_room_node)
		return;

	if (get_instance_id() == parent_room_node->get_instance_id())
	{
		WwiseLogger::warning_format("%s has an invalid parent Room. This Room and the assigned parent Room "
									"are the same. The parent Room will be reset to the 'Outdoors' room.",
				get_name());
		parent_room = NodePath();
		return;
	}

	if (is_a_parent_of(parent_room_node))
	{
		WwiseLogger::warning_format(
				"%s has an invalid parent Room. The assigned parent Room creates or "
				"contains a circular dependency. The parent Room will be reset to the 'Outdoors' room.",
				get_name());
		parent_room = NodePath();
	}
}

bool AkRoom::is_a_parent_of(const AkRoom* p_room) const
{
	HashSet<uint64_t> visited;
	while (p_room && p_room->enable_reverb_zone)
	{
		const uint64_t room_id = p_room->get_instance_id();
		if (visited.has(room_id))
		{
			return true;
		}
		visited.insert(room_id);
		p_room = p_room->get_parent_room_node();
		if (p_room && get_instance_id() == p_room->get_instance_id())
		{
			return true;
		}
	}

	return false;
}