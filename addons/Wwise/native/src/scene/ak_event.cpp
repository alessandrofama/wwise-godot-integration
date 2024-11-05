#include "ak_event.h"

void AkEvent2D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkEvent2D::handle_game_event);
	ClassDB::bind_method(D_METHOD("post_event"), &AkEvent2D::post_event);
	ClassDB::bind_method(D_METHOD("stop_event"), &AkEvent2D::stop_event);
	ClassDB::bind_method(D_METHOD("callback_emitter", "data"), &AkEvent2D::callback_emitter);
	ClassDB::bind_method(D_METHOD("set_event", "event"), &AkEvent2D::set_event);
	ClassDB::bind_method(D_METHOD("get_event"), &AkEvent2D::get_event);
	ClassDB::bind_method(D_METHOD("set_stop_fade_time", "stop_fade_time"), &AkEvent2D::set_stop_fade_time);
	ClassDB::bind_method(D_METHOD("get_stop_fade_time"), &AkEvent2D::get_stop_fade_time);
	ClassDB::bind_method(D_METHOD("set_interpolation_mode", "interpolation_mode"), &AkEvent2D::set_interpolation_mode);
	ClassDB::bind_method(D_METHOD("get_interpolation_mode"), &AkEvent2D::get_interpolation_mode);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &AkEvent2D::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &AkEvent2D::get_trigger_on);
	ClassDB::bind_method(D_METHOD("set_stop_on", "stop_on"), &AkEvent2D::set_stop_on);
	ClassDB::bind_method(D_METHOD("get_stop_on"), &AkEvent2D::get_stop_on);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "event", PROPERTY_HINT_NONE), "set_event", "get_event");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stop_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_stop_on", "get_stop_on");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stop_fade_time", PROPERTY_HINT_NONE), "set_stop_fade_time",
			"get_stop_fade_time");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation_mode", PROPERTY_HINT_ENUM,
						 "LOG3,SINE,LOG1,INVSCURVE,LINEAR,SCURVE,EXP1,SINERECIP,EXP3,LASTFADECURVE,CONSTANT"),
			"set_interpolation_mode", "get_interpolation_mode");

	ADD_ALL_AK_EVENT_SIGNALS;
}

void AkEvent2D::_notification(int p_what)
{
	if (p_what == NOTIFICATION_PREDELETE)
	{
		memdelete(cookie);
	}
}

void AkEvent2D::check_signal_connections()
{
	for (const auto& entry : AkUtils::get_singleton()->event_callback_signals)
	{
		AkUtils::AkCallbackType type = entry.first;
		StringName signal = entry.second;

		TypedArray<Dictionary> connections = get_signal_connection_list(signal);

		if (!connections.is_empty())
		{
			callback_type |= type;
		}
	}
}

AkEvent2D::AkEvent2D()
{
	event["name"] = "";
	event["id"] = 0;

	cookie = memnew(WwiseCookie(Callable(this, "callback_emitter")));
}

void AkEvent2D::_enter_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		// note(alex): by caching the value, we eliminate the need to perform the check repeatedly during every frame in
		// the process callback.
		is_editor = true;
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->register_game_obj(this, get_name());
	}

	handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE);
}

void AkEvent2D::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkEvent2D::_exit_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE); }

void AkEvent2D::_process(double delta)
{
	if (is_editor)
	{
		return;
	}

	Wwise::get_singleton()->set_2d_position(this, get_global_transform(), get_z_index());
}

void AkEvent2D::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (trigger_on == game_event)
	{
		post_event();
	}
	if (stop_on == game_event)
	{
		stop_event();
	}
}

void AkEvent2D::post_event()
{
	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		check_signal_connections();

		if (callback_type)
		{
			playing_id = soundengine->post_event_id_callback(
					event.get("id", 0), (AkUtils::AkCallbackType)callback_type, this, cookie);
		}
		else
		{
			playing_id = soundengine->post_event_id(event.get("id", 0), this);
		}
	}
}

void AkEvent2D::stop_event()
{
	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->stop_event(playing_id, stop_fade_time, interpolation_mode);
	}
}

void AkEvent2D::callback_emitter(const Dictionary& data)
{
	Thread::set_thread_safety_checks_enabled(false);
	int callback_type = data.get("callback_type", AkUtils::AkCallbackType::AK_END_OF_EVENT);
	AkUtils::AkCallbackType type = (AkUtils::AkCallbackType)callback_type;
	emit_signal(AkUtils::get_singleton()->event_callback_signals[type], data);
}

void AkEvent2D::set_event(const Dictionary& event) { this->event = event; }

Dictionary AkEvent2D::get_event() const { return event; }

void AkEvent2D::set_stop_fade_time(unsigned int stop_fade_time) { this->stop_fade_time = stop_fade_time; }

void AkEvent2D::set_interpolation_mode(AkUtils::AkCurveInterpolation interpolation_mode)
{
	this->interpolation_mode = interpolation_mode;
}

AkUtils::AkCurveInterpolation AkEvent2D::get_interpolation_mode() const { return interpolation_mode; }

unsigned int AkEvent2D::get_stop_fade_time() const { return stop_fade_time; }

void AkEvent2D::set_trigger_on(AkUtils::GameEvent trigger_on) { this->trigger_on = trigger_on; }

AkUtils::GameEvent AkEvent2D::get_trigger_on() const { return trigger_on; }

void AkEvent2D::set_stop_on(AkUtils::GameEvent stop_on) { this->stop_on = stop_on; }

AkUtils::GameEvent AkEvent2D::get_stop_on() const { return stop_on; }

void AkEvent3D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkEvent3D::handle_game_event);
	ClassDB::bind_method(D_METHOD("post_event"), &AkEvent3D::post_event);
	ClassDB::bind_method(D_METHOD("stop_event"), &AkEvent3D::stop_event);
	ClassDB::bind_method(D_METHOD("callback_emitter", "data"), &AkEvent3D::callback_emitter);
	ClassDB::bind_method(D_METHOD("set_event", "event"), &AkEvent3D::set_event);
	ClassDB::bind_method(D_METHOD("get_event"), &AkEvent3D::get_event);
	ClassDB::bind_method(D_METHOD("set_stop_fade_time", "stop_fade_time"), &AkEvent3D::set_stop_fade_time);
	ClassDB::bind_method(D_METHOD("get_stop_fade_time"), &AkEvent3D::get_stop_fade_time);
	ClassDB::bind_method(D_METHOD("set_interpolation_mode", "interpolation_mode"), &AkEvent3D::set_interpolation_mode);
	ClassDB::bind_method(D_METHOD("get_interpolation_mode"), &AkEvent3D::get_interpolation_mode);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &AkEvent3D::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &AkEvent3D::get_trigger_on);
	ClassDB::bind_method(D_METHOD("set_stop_on", "stop_on"), &AkEvent3D::set_stop_on);
	ClassDB::bind_method(D_METHOD("get_stop_on"), &AkEvent3D::get_stop_on);
	ClassDB::bind_method(
			D_METHOD("set_is_environment_aware", "is_environment_aware"), &AkEvent3D::set_is_environment_aware);
	ClassDB::bind_method(D_METHOD("get_is_environment_aware"), &AkEvent3D::get_is_environment_aware);
	ClassDB::bind_method(D_METHOD("set_room_id", "room_id"), &AkEvent3D::set_room_id);
	ClassDB::bind_method(D_METHOD("get_room_id"), &AkEvent3D::get_room_id);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "event", PROPERTY_HINT_NONE), "set_event", "get_event");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stop_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_stop_on", "get_stop_on");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stop_fade_time", PROPERTY_HINT_NONE), "set_stop_fade_time",
			"get_stop_fade_time");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation_mode", PROPERTY_HINT_ENUM,
						 "LOG3,SINE,LOG1,INVSCURVE,LINEAR,SCURVE,EXP1,SINERECIP,EXP3,LASTFADECURVE,CONSTANT"),
			"set_interpolation_mode", "get_interpolation_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_environment_aware", PROPERTY_HINT_NONE), "set_is_environment_aware",
			"get_is_environment_aware");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "room_id", PROPERTY_HINT_NONE), "set_room_id", "get_room_id");

	ADD_ALL_AK_EVENT_SIGNALS;
}

void AkEvent3D::_notification(int p_what)
{
	if (p_what == NOTIFICATION_PREDELETE)
	{
		memdelete(cookie);
	}
}

void AkEvent3D::check_signal_connections()
{
	for (const auto& entry : AkUtils::get_singleton()->event_callback_signals)
	{
		AkUtils::AkCallbackType type = entry.first;
		StringName signal = entry.second;

		TypedArray<Dictionary> connections = get_signal_connection_list(signal);

		if (!connections.is_empty())
		{
			callback_type |= type;
		}
	}
}

AkEvent3D::AkEvent3D()
{
	event["name"] = "";
	event["id"] = 0;

	cookie = memnew(WwiseCookie(Callable(this, "callback_emitter")));
}

void AkEvent3D::_enter_tree()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		// note(alex): by caching the value, we eliminate the need to perform the check repeatedly during every frame in
		// the process callback.
		is_editor = true;
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->register_game_obj(this, get_name());
	}

	if (is_environment_aware)
	{
		// todo(alex): set up raycast and get the listener for AK::SoundEngine::SetObjectObstructionAndOcclusion
		environment_data = memnew(AkEnvironmentData);
	}

	handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE);
}

void AkEvent3D::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkEvent3D::_exit_tree()
{
	if (is_environment_aware)
	{
		if (environment_data)
		{
			memdelete(environment_data);
		}
	}

	handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE);
}

void AkEvent3D::_process(double delta)
{
	if (is_editor)
	{
		return;
	}

	Wwise::get_singleton()->set_3d_position(this, get_global_transform());

	if (is_environment_aware)
	{
		environment_data->update_aux_send(this, get_global_transform().get_origin());
	}
}

void AkEvent3D::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (trigger_on == game_event)
	{
		post_event();
	}
	if (stop_on == game_event)
	{
		stop_event();
	}
}

void AkEvent3D::post_event()
{
	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		check_signal_connections();

		if (callback_type)
		{
			playing_id = soundengine->post_event_id_callback(
					event.get("id", 0), (AkUtils::AkCallbackType)callback_type, this, cookie);
		}
		else
		{
			playing_id = soundengine->post_event_id(event.get("id", 0), this);
		}
	}
}

void AkEvent3D::stop_event()
{
	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->stop_event(playing_id, stop_fade_time, interpolation_mode);
	}
}

void AkEvent3D::callback_emitter(const Dictionary& data)
{
	Thread::set_thread_safety_checks_enabled(false);
	int callback_type = data.get("callback_type", AkUtils::AkCallbackType::AK_END_OF_EVENT);
	AkUtils::AkCallbackType type = (AkUtils::AkCallbackType)callback_type;
	emit_signal(AkUtils::get_singleton()->event_callback_signals[type], data);
}

void AkEvent3D::set_event(const Dictionary& event) { this->event = event; }

Dictionary AkEvent3D::get_event() const { return event; }

void AkEvent3D::set_stop_fade_time(unsigned int stop_fade_time) { this->stop_fade_time = stop_fade_time; }

void AkEvent3D::set_interpolation_mode(AkUtils::AkCurveInterpolation interpolation_mode)
{
	this->interpolation_mode = interpolation_mode;
}

AkUtils::AkCurveInterpolation AkEvent3D::get_interpolation_mode() const { return interpolation_mode; }

unsigned int AkEvent3D::get_stop_fade_time() const { return stop_fade_time; }

void AkEvent3D::set_trigger_on(AkUtils::GameEvent trigger_on) { this->trigger_on = trigger_on; }

AkUtils::GameEvent AkEvent3D::get_trigger_on() const { return trigger_on; }

void AkEvent3D::set_stop_on(AkUtils::GameEvent stop_on) { this->stop_on = stop_on; }

AkUtils::GameEvent AkEvent3D::get_stop_on() const { return stop_on; }

void AkEvent3D::set_is_environment_aware(bool is_environment_aware)
{
	this->is_environment_aware = is_environment_aware;
}

bool AkEvent3D::get_is_environment_aware() const { return is_environment_aware; }

void AkEvent3D::set_room_id(uint64_t room_id) { this->room_id = room_id; }

uint64_t AkEvent3D::get_room_id() const { return room_id; }