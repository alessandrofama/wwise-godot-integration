#ifndef AK_EVENT_H
#define AK_EVENT_H

#include "ak_environment_data.h"
#include "wwise_gdextension.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/thread.hpp>

namespace godot
{

class AkEvent2D : public Node2D
{
	GDCLASS(AkEvent2D, Node2D);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	Dictionary event;
	unsigned int playing_id = AK_INVALID_PLAYING_ID;
	unsigned int stop_fade_time{};
	AkUtils::AkCurveInterpolation interpolation_mode = AkUtils::AkCurveInterpolation::AK_CURVE_LINEAR;
	int callback_type{};
	CookieWrapper* cookie{ nullptr };
	bool is_editor{};

	AkUtils::GameEvent trigger_on = AkUtils::GameEvent::GAMEEVENT_NONE;
	AkUtils::GameEvent stop_on = AkUtils::GameEvent::GAMEEVENT_NONE;

	void check_signal_connections();

public:
	AkEvent2D();
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;
	virtual void _process(double delta) override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void post_event();
	void stop_event();

	void callback_emitter(const Dictionary& data);

	void set_event(const Dictionary& event);
	Dictionary get_event() const;

	void set_stop_fade_time(unsigned int stop_fade_time);
	unsigned int get_stop_fade_time() const;

	void set_interpolation_mode(AkUtils::AkCurveInterpolation interpolation_mode);
	AkUtils::AkCurveInterpolation get_interpolation_mode() const;

	void set_trigger_on(AkUtils::GameEvent trigger_on);
	AkUtils::GameEvent get_trigger_on() const;

	void set_stop_on(AkUtils::GameEvent stop_on);
	AkUtils::GameEvent get_stop_on() const;
};

class AkEvent3D : public Node3D
{
	GDCLASS(AkEvent3D, Node3D);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	Dictionary event;
	unsigned int playing_id = AK_INVALID_PLAYING_ID;
	unsigned int stop_fade_time{};
	AkUtils::AkCurveInterpolation interpolation_mode = AkUtils::AkCurveInterpolation::AK_CURVE_LINEAR;
	bool is_environment_aware{};
	int callback_type{};
	CookieWrapper* cookie{ nullptr };
	bool is_editor{};

	AkUtils::GameEvent trigger_on = AkUtils::GameEvent::GAMEEVENT_NONE;
	AkUtils::GameEvent stop_on = AkUtils::GameEvent::GAMEEVENT_NONE;

	void check_signal_connections();

public:
	AkEnvironmentData* environment_data = nullptr;

	AkEvent3D();
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;
	virtual void _process(double delta) override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void post_event();
	void stop_event();

	void callback_emitter(const Dictionary& data);

	void set_event(const Dictionary& event);
	Dictionary get_event() const;

	void set_stop_fade_time(unsigned int stop_fade_time);
	unsigned int get_stop_fade_time() const;

	void set_interpolation_mode(AkUtils::AkCurveInterpolation interpolation_mode);
	AkUtils::AkCurveInterpolation get_interpolation_mode() const;

	void set_trigger_on(AkUtils::GameEvent trigger_on);
	AkUtils::GameEvent get_trigger_on() const;

	void set_stop_on(AkUtils::GameEvent stop_on);
	AkUtils::GameEvent get_stop_on() const;

	void set_is_environment_aware(bool is_environment_aware);
	bool get_is_environment_aware() const;
};

} //namespace godot

#endif