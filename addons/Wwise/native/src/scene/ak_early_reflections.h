#pragma once

#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class AkEarlyReflections : public Node
{
	GDCLASS(AkEarlyReflections, Node);

protected:
	static void _bind_methods();

private:
	Dictionary aux_bus;
	NodePath game_object_path{};
	float volume{ 1.0f };

public:
	AkEarlyReflections();
	virtual void _enter_tree() override;

	void set_aux_bus(const Dictionary& aux_bus);
	Dictionary get_aux_bus() const;

	void set_game_object_path(const NodePath& game_object_path);
	NodePath get_game_object_path() const;

	void set_volume(float volume);
	float get_volume() const;
};