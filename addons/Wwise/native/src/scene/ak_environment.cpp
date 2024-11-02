#include "ak_environment.h"

void AkEnvironment::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_area_entered", "area"), &AkEnvironment::_on_area_entered);
	ClassDB::bind_method(D_METHOD("_on_area_exited", "area"), &AkEnvironment::_on_area_exited);
	ClassDB::bind_method(D_METHOD("set_aux_bus", "aux_bus"), &AkEnvironment::set_aux_bus);
	ClassDB::bind_method(D_METHOD("get_aux_bus"), &AkEnvironment::get_aux_bus);
	ClassDB::bind_method(D_METHOD("set_priority", "priority"), &AkEnvironment::set_priority);
	ClassDB::bind_method(D_METHOD("get_priority"), &AkEnvironment::get_priority);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "aux_bus", PROPERTY_HINT_NONE), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "priority", PROPERTY_HINT_NONE), "set_priority", "get_priority");
}

AkEnvironment::AkEnvironment()
{
	aux_bus["name"] = "";
	aux_bus["id"] = 0;
}

void AkEnvironment::_enter_tree()
{
	RETURN_IF_EDITOR;

	connect("area_entered", callable_mp(this, &AkEnvironment::_on_area_entered));
	connect("area_exited", callable_mp(this, &AkEnvironment::_on_area_exited));
}

void AkEnvironment::_on_area_entered(const Area3D* area)
{
	Node* parent = area->get_parent();

	if (parent)
	{
		if (parent->get_class() == "AkEvent3D")
		{
			AkEvent3D* event = Object::cast_to<AkEvent3D>(parent);
			if (event)
			{
				if (event->get_is_environment_aware())
				{
					event->environment_data->add_environment(this);
				}
			}
		}
	}
}

void AkEnvironment::_on_area_exited(const Area3D* area)
{
	Node* parent = area->get_parent();

	if (parent)
	{
		if (parent->get_class() == "AkEvent3D")
		{
			AkEvent3D* event = Object::cast_to<AkEvent3D>(parent);
			if (event)
			{
				if (event->get_is_environment_aware())
				{
					event->environment_data->remove_environment(this);
				}
			}
		}
	}
}

void AkEnvironment::set_aux_bus(const Dictionary& aux_bus) { this->aux_bus = aux_bus; }

Dictionary AkEnvironment::get_aux_bus() const { return aux_bus; }

void AkEnvironment::set_priority(int priority) { this->priority = priority; }

int AkEnvironment::get_priority() const { return priority; }