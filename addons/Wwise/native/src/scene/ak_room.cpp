#include "ak_room.h"

void AkRoom::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_area_entered", "area"), &AkRoom::_on_area_entered);
	ClassDB::bind_method(D_METHOD("_on_area_exited", "area"), &AkRoom::_on_area_exited);
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

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "aux_bus", PROPERTY_HINT_NONE), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(
			PropertyInfo(Variant::FLOAT, "reverb_level", PROPERTY_HINT_NONE), "set_reverb_level", "get_reverb_level");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transmission_loss", PROPERTY_HINT_NONE), "set_transmission_loss",
			"get_transmission_loss");
	ADD_PROPERTY(
			PropertyInfo(Variant::NODE_PATH, "associated_geometry", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AkGeometry"),
			"set_associated_geometry", "get_associated_geometry");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_registered", PROPERTY_HINT_NONE), "set_keep_registered",
			"get_keep_registered");
}

AkRoom::AkRoom()
{
	aux_bus["name"] = "";
	aux_bus["id"] = 0;
}

void AkRoom::_enter_tree()
{
	RETURN_IF_EDITOR;

	AkGeometry* geometry_node{ nullptr };
	if (!associated_geometry.is_empty())
	{
		geometry_node = get_node<AkGeometry>(associated_geometry);
	}

	Transform3D normalized_transform = get_global_transform().orthonormalized();

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->set_room(this, aux_bus["id"], reverb_level, transmission_loss,
				normalized_transform.get_basis().get_column(2), normalized_transform.get_basis().get_column(1),
				keep_registered, geometry_node);
	}

	connect("area_entered", callable_mp(this, &AkRoom::_on_area_entered));
	connect("area_exited", callable_mp(this, &AkRoom::_on_area_exited));
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

void AkRoom::_on_area_entered(const Area3D* area)
{
	Node* parent = area->get_parent();

	if (parent)
	{
		if (parent->get_class() == "AkEvent3D" || parent->get_class() == "AkListener3D")
		{
			// If we have an AkListener3D or an AkEvent3D, keep track
			// of the room it's entering.
			AkListener3D* listener = Object::cast_to<AkListener3D>(parent);
			AkEvent3D* event = Object::cast_to<AkEvent3D>(parent);
			if (listener)
			{
				listener->set_room_id(static_cast<AkGameObjectID>(this->get_instance_id()));
			}
			else if (event)
			{
				event->set_room_id(static_cast<AkGameObjectID>(this->get_instance_id()));
			}

			Wwise* soundengine = Wwise::get_singleton();

			if (soundengine)
			{
				soundengine->set_game_object_in_room(parent, this);
			}
		}
	}
}

void AkRoom::_on_area_exited(const Area3D* area)
{
	Node* parent = area->get_parent();

	if (parent)
	{
		if (parent->get_class() == "AkEvent3D" || parent->get_class() == "AkListener3D")
		{
			// Check if the AkListener3D or AkEvent3D is leaving the
			// room they are in to go outside.
			//
			// If they enter a different room, then the
			// _on_area_entered() method for the different room will be
			// called, setting the listener/event's room ID to that room.
			//
			// In contrast, if the event/listener goes outside, then
			// there is no call to _on_area_entered() and the
			// event/ listener's room ID is the same as the room they
			// are leaving. In this case, we call
			// remove_game_object_from_room() and set the room
			// to INVALID_ROOM_ID.
			bool isGoingOutside = false;

			AkListener3D* listener = Object::cast_to<AkListener3D>(parent);
			AkEvent3D* event = Object::cast_to<AkEvent3D>(parent);
			if (listener)
			{
				isGoingOutside = listener->get_room_id() == static_cast<AkGameObjectID>(this->get_instance_id());
			}
			else if (event)
			{
				isGoingOutside = event->get_room_id() == static_cast<AkGameObjectID>(this->get_instance_id());
			}

			Wwise* soundengine = Wwise::get_singleton();

			if (soundengine && isGoingOutside)
			{
				soundengine->remove_game_object_from_room(parent);
			}
		}
	}
}

void AkRoom::set_aux_bus(const Dictionary& aux_bus) { this->aux_bus = aux_bus; }

Dictionary AkRoom::get_aux_bus() const { return aux_bus; }

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
