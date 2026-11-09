#include "ak_environment.h"

void AkEnvironment::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_area_entered", "area"), &AkEnvironment::_on_area_entered);
	ClassDB::bind_method(D_METHOD("_on_area_exited", "area"), &AkEnvironment::_on_area_exited);
	ClassDB::bind_method(D_METHOD("set_aux_bus", "aux_bus"), &AkEnvironment::set_aux_bus);
	ClassDB::bind_method(D_METHOD("get_aux_bus"), &AkEnvironment::get_aux_bus);
	ClassDB::bind_method(D_METHOD("set_environment_priority", "environment_priority"), &AkEnvironment::set_environment_priority);
	ClassDB::bind_method(D_METHOD("get_environment_priority"), &AkEnvironment::get_environment_priority);

	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "aux_bus", PROPERTY_HINT_NONE, "WwiseAuxBus"), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "environment_priority", PROPERTY_HINT_NONE), "set_environment_priority", "get_environment_priority");
}

void AkEnvironment::_enter_tree()
{
	RETURN_IF_EDITOR;

	connect("area_entered", callable_mp(this, &AkEnvironment::_on_area_entered));
	connect("area_exited", callable_mp(this, &AkEnvironment::_on_area_exited));
}

void AkEnvironment::_exit_tree() 
{
	RETURN_IF_EDITOR;

	disconnect("area_entered", callable_mp(this, &AkEnvironment::_on_area_entered));
	disconnect("area_exited", callable_mp(this, &AkEnvironment::_on_area_exited));
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

void AkEnvironment::set_aux_bus(const Ref<WwiseAuxBus>& p_aux_bus)
{
	aux_bus = p_aux_bus;
	notify_property_list_changed();
}

Ref<WwiseAuxBus> AkEnvironment::get_aux_bus() const { return aux_bus; }

void AkEnvironment::set_environment_priority(int p_priority) { environment_priority = p_priority; }

int AkEnvironment::get_environment_priority() const { return environment_priority; }
