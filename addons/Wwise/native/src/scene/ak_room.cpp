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
	ClassDB::bind_method(D_METHOD("set_keep_registered", "keep_registered"), &AkRoom::set_keep_registered);
	ClassDB::bind_method(D_METHOD("get_keep_registered"), &AkRoom::get_keep_registered);
	ClassDB::bind_method(D_METHOD("set_room_priority", "room_priority"), &AkRoom::set_room_priority);
	ClassDB::bind_method(D_METHOD("get_room_priority"), &AkRoom::get_room_priority);

	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "aux_bus", PROPERTY_HINT_NONE, "WwiseAuxBus"), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(
			PropertyInfo(Variant::FLOAT, "reverb_level", PROPERTY_HINT_NONE), "set_reverb_level", "get_reverb_level");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transmission_loss", PROPERTY_HINT_NONE), "set_transmission_loss",
			"get_transmission_loss");
	ADD_PROPERTY(
			PropertyInfo(Variant::NODE_PATH, "associated_geometry", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AkGeometry"),
			"set_associated_geometry", "get_associated_geometry");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_registered", PROPERTY_HINT_NONE), "set_keep_registered",
			"get_keep_registered");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_priority", PROPERTY_HINT_NONE), "set_room_priority",
			"get_room_priority");
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
			soundengine->set_room(this, aux_bus->get_id(), reverb_level, transmission_loss,
					normalized_transform.get_basis().get_column(2), normalized_transform.get_basis().get_column(1),
					keep_registered, geometry_node->geometry_instance_object, room_priority);
		}
	}
}

void AkRoom::_exit_tree()
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->remove_room(this);
	}
}

void AkRoom::set_aux_bus(const Ref<WwiseAuxBus>& aux_bus)
{
	this->aux_bus = aux_bus;
	notify_property_list_changed();
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

void AkRoom::set_keep_registered(bool keep_registered) { this->keep_registered = keep_registered; }

bool AkRoom::get_keep_registered() const { return keep_registered; }

void AkRoom::set_room_priority(float p_room_priority) { room_priority = p_room_priority; }

float AkRoom::get_room_priority() const { return room_priority; }
