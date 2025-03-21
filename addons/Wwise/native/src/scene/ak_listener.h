#pragma once

#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class AkListener2D : public Node2D
{
	GDCLASS(AkListener2D, Node2D);

protected:
	static void _bind_methods();

private:
	bool is_default_listener{ true };

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	void set_is_default_listener(bool p_is_default);
	bool get_is_default_listener() const;
};

class AkListener3D : public Node3D
{
	GDCLASS(AkListener3D, Node3D);

protected:
	static void _bind_methods();

private:
	bool is_default_listener{ true };
	bool is_spatial{ true };
	uint64_t room_id{ INVALID_ROOM_ID };

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	void set_is_default_listener(bool p_is_default);
	bool get_is_default_listener() const;

	void set_is_spatial(bool p_is_spatial);
	bool get_is_spatial() const;

	void set_room_id(uint64_t p_room_id);
	uint64_t get_room_id() const;
};