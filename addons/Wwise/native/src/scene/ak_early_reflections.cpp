#include "ak_early_reflections.h"

void AkEarlyReflections::_bind_methods()
{
	ClassDB::bind_method(
			D_METHOD("set_early_reflections_volume", "volume"), &AkEarlyReflections::set_early_reflections_volume);
	ClassDB::bind_method(D_METHOD("set_aux_bus", "aux_bus"), &AkEarlyReflections::set_aux_bus);
	ClassDB::bind_method(D_METHOD("get_aux_bus"), &AkEarlyReflections::get_aux_bus);
	ClassDB::bind_method(
			D_METHOD("set_game_object_path", "game_object_path"), &AkEarlyReflections::set_game_object_path);
	ClassDB::bind_method(D_METHOD("get_game_object_path"), &AkEarlyReflections::get_game_object_path);
	ClassDB::bind_method(D_METHOD("set_volume", "volume"), &AkEarlyReflections::set_volume);
	ClassDB::bind_method(D_METHOD("get_volume"), &AkEarlyReflections::get_volume);

	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "aux_bus", PROPERTY_HINT_NONE, "WwiseAuxBus"), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "game_object_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node"),
			"set_game_object_path", "get_game_object_path");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume", PROPERTY_HINT_NONE), "set_volume", "get_volume");
}

void AkEarlyReflections::_enter_tree()
{
	RETURN_IF_EDITOR;

	set_early_reflections_volume(get_volume());
}

void AkEarlyReflections::set_early_reflections_volume(float volume)
{
	if (game_object_path.is_empty())
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: GameObject path in AkEarlyReflections Node: %s is empty", get_name()));
		return;
	}

	if (aux_bus.is_null())
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: Trying to set Eraly Reflections, but the AuxBus property in the "
						"AkEarlyReflections node: %s is not set (null).",
						get_name()));
		return;
	}

	Node* game_object = get_node_or_null(game_object_path);
	if (game_object)
	{
		Wwise* soundengine = Wwise::get_singleton();
		if (soundengine)
		{
			soundengine->set_early_reflections_aux_send(game_object, aux_bus->get_id());
			soundengine->set_early_reflections_volume(game_object, volume);
		}
	}
}

void AkEarlyReflections::set_aux_bus(const Ref<WwiseAuxBus>& aux_bus)
{
	this->aux_bus = aux_bus;
	notify_property_list_changed();
}

Ref<WwiseAuxBus> AkEarlyReflections::get_aux_bus() const { return aux_bus; }

void AkEarlyReflections::set_game_object_path(const NodePath& game_object_path)
{
	this->game_object_path = game_object_path;
}

NodePath AkEarlyReflections::get_game_object_path() const { return game_object_path; }

void AkEarlyReflections::set_volume(float volume)
{
	this->volume = volume;
	RETURN_IF_EDITOR;
	set_early_reflections_volume(volume);
}

float AkEarlyReflections::get_volume() const { return volume; }