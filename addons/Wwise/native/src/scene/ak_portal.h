#ifndef AK_PORTAL_H
#define AK_PORTAL_H

#include "wwise_gdnative.h"
#include <godot_cpp/classes/area3d.hpp>

namespace godot
{
class AkPortal : public Area3D
{
	GDCLASS(AkPortal, Area3D);

protected:
	static void _bind_methods();

private:
	Vector3 extent;
	NodePath front_room;
	NodePath back_room;
	bool enabled;
	Node* front_room_node;
	Node* back_room_node;

public:
	virtual void _ready() override;
	void set_portal() const;

	void set_front_room(const NodePath& front_room);
	NodePath get_front_room() const;

	void set_back_room(const NodePath& back_room);
	NodePath get_back_room() const;

	void set_enabled(bool enabled);
	bool get_enabled() const;
};
} //namespace godot

#endif