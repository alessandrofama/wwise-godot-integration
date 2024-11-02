#include "ak_early_reflections.h"

void AkEarlyReflections::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_aux_bus", "aux_bus"), &AkEarlyReflections::set_aux_bus);
	ClassDB::bind_method(D_METHOD("get_aux_bus"), &AkEarlyReflections::get_aux_bus);
	ClassDB::bind_method(
			D_METHOD("set_game_object_path", "game_object_path"), &AkEarlyReflections::set_game_object_path);
	ClassDB::bind_method(D_METHOD("get_game_object_path"), &AkEarlyReflections::get_game_object_path);
	ClassDB::bind_method(D_METHOD("set_volume", "volume"), &AkEarlyReflections::set_volume);
	ClassDB::bind_method(D_METHOD("get_volume"), &AkEarlyReflections::get_volume);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "aux_bus", PROPERTY_HINT_NONE), "set_aux_bus", "get_aux_bus");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "game_object_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node"),
			"set_game_object_path", "get_game_object_path");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume", PROPERTY_HINT_NONE), "set_volume", "get_volume");
}

AkEarlyReflections::AkEarlyReflections()
{
	aux_bus["name"] = "";
	aux_bus["id"] = 0;
}

void AkEarlyReflections::_enter_tree()
{
	RETURN_IF_EDITOR;

	if (game_object_path.is_empty())
	{
		UtilityFunctions::push_warning(
				"GameObject NodePath in AkEarlyReflections is empty, id: " + String::num_int64(get_instance_id()));
		return;
	}

	Node* game_object = get_node<Node>(game_object_path);
	if (game_object)
	{
		Wwise* soundengine = Wwise::get_singleton();
		if (soundengine)
		{
			soundengine->set_early_reflections_aux_send(game_object, aux_bus.get("id", 0));
			soundengine->set_early_reflections_volume(game_object, volume);
		}
	}
}

void AkEarlyReflections::set_aux_bus(const Dictionary& aux_bus) { this->aux_bus = aux_bus; }

Dictionary AkEarlyReflections::get_aux_bus() const { return aux_bus; }

void AkEarlyReflections::set_game_object_path(const NodePath& game_object_path)
{
	this->game_object_path = game_object_path;
}

NodePath AkEarlyReflections::get_game_object_path() const { return game_object_path; }

void AkEarlyReflections::set_volume(float volume) { this->volume = volume; }

float AkEarlyReflections::get_volume() const { return volume; }