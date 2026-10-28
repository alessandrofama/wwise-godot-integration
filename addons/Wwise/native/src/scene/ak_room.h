#pragma once

#include "core/types/wwise_aux_bus.h"
#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include "scene/ak_event.h"
#include "scene/ak_geometry.h"
#include "scene/ak_listener.h"
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/node_path.hpp>

using namespace godot;

class AkRoom : public Node3D
{
	GDCLASS(AkRoom, Node3D);

protected:
	static void _bind_methods();

private:
	Ref<WwiseAuxBus> aux_bus;
	float reverb_level{ 1.0f };
	float transmission_loss{ 1.0f };
	NodePath associated_geometry{};
	Ref<WwiseEvent> room_tone_event;
	float aux_send_level_to_self { 0.0f };
	bool keep_registered{ false };
	float room_priority{ 100.0f };
	AkUtils::AkRoomDistanceBehavior distance_behavior{ (
			AkUtils::AkRoomDistanceBehavior)AkRoomDistanceBehavior_Default };
	bool room_registered{ false };

public:
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void set_aux_bus(const Ref<WwiseAuxBus>& aux_bus);
	Ref<WwiseAuxBus> get_aux_bus() const;

	void set_reverb_level(float reverb_level);
	float get_reverb_level() const;

	void set_transmission_loss(float transmission_loss);
	float get_transmission_loss() const;

	void set_associated_geometry(const NodePath& associated_geometry);
	NodePath get_associated_geometry() const;

	void set_room_tone_event(const Ref<WwiseEvent>& p_room_tone_event);
	Ref<WwiseEvent> get_room_tone_event() const;

	void set_aux_send_level_to_self(float p_aux_send_level_to_self);
	float get_aux_send_level_to_self() const;

	void set_keep_registered(bool keep_registered);
	bool get_keep_registered() const;

	void set_room_priority(float p_room_priority);
	float get_room_priority() const;

	void set_distance_behavior(AkUtils::AkRoomDistanceBehavior p_distance_behavior);
	AkUtils::AkRoomDistanceBehavior get_distance_behavior() const;
};