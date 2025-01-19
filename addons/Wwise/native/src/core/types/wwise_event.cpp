#include "wwise_event.h"

void WwiseEvent::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_is_in_user_defined_sound_bank", "is_in_user_defined_soundbank"),
			&WwiseEvent::set_is_in_user_defined_sound_bank);
	ClassDB::bind_method(D_METHOD("get_is_in_user_defined_sound_bank"), &WwiseEvent::get_is_in_user_defined_sound_bank);

	ClassDB::bind_method(D_METHOD("set_bank_id", "bank_id"), &WwiseEvent::set_bank_id);
	ClassDB::bind_method(D_METHOD("get_bank_id"), &WwiseEvent::get_bank_id);

	ClassDB::bind_method(
			D_METHOD("set_is_auto_bank_loaded", "is_auto_bank_loaded"), &WwiseEvent::set_is_auto_bank_loaded);
	ClassDB::bind_method(D_METHOD("get_is_auto_bank_loaded"), &WwiseEvent::get_is_auto_bank_loaded);

	ClassDB::bind_method(D_METHOD("set_playing_id", "playing_id"), &WwiseEvent::set_playing_id);
	ClassDB::bind_method(D_METHOD("get_playing_id"), &WwiseEvent::get_playing_id);

	ClassDB::bind_method(D_METHOD("_on_post_resource_init"), &WwiseEvent::_on_post_resource_init);
	ClassDB::bind_method(D_METHOD("post", "game_object"), &WwiseEvent::post);
	ClassDB::bind_method(D_METHOD("post_callback", "game_object", "flags", "cookie"), &WwiseEvent::post_callback);
	ClassDB::bind_method(D_METHOD("stop", "game_object", "transition_duration", "fade_curve"), &WwiseEvent::stop,
			DEFVAL(0), DEFVAL(AkUtils::AkCurveInterpolation::AK_CURVE_LINEAR));
	ClassDB::bind_method(
			D_METHOD("execute_action", "game_object", "action_on_event_type", "transition_duration", "fade_curve"),
			&WwiseEvent::execute_action);
	ClassDB::bind_method(D_METHOD("post_midi", "game_object", "midi_posts"), &WwiseEvent::post_midi);
	ClassDB::bind_method(D_METHOD("stop_midi", "game_object"), &WwiseEvent::stop_midi);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_in_user_defined_sound_bank"), "set_is_in_user_defined_sound_bank",
			"get_is_in_user_defined_sound_bank");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "bank_id"), "set_bank_id", "get_bank_id");
	ADD_PROPERTY(
			PropertyInfo(Variant::BOOL, "is_auto_bank_loaded"), "set_is_auto_bank_loaded", "get_is_auto_bank_loaded");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "playing_id", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE),
			"set_playing_id", "get_playing_id");
}

void WwiseEvent::_notification(int p_what)
{
	RETURN_IF_EDITOR;

	switch (p_what)
	{
		case NOTIFICATION_POSTINITIALIZE:
		{
			connect("ws_post_resource_init", callable_mp(this, &WwiseEvent::_on_post_resource_init));
			break;
		}
		case NOTIFICATION_PREDELETE:
		{
			disconnect("ws_post_resource_init", callable_mp(this, &WwiseEvent::_on_post_resource_init));
			unload_auto_bank();
			break;
		}
		default:
			break;
	}
}

void WwiseEvent::_on_post_resource_init()
{
	if (Wwise::get_singleton()->is_initialized())
	{
		load_auto_bank();
	}
}

void WwiseEvent::load_auto_bank()
{
	if (get_is_in_user_defined_sound_bank())
	{
		return;
	}

	load_auto_bank_blocking();
}

void WwiseEvent::load_auto_bank_blocking()
{
	set_bank_id(AkBankManager::load_bank(get_name(), get_bank_type()));
	post_load_auto_bank(get_bank_id());
	set_is_auto_bank_loaded(true);
}

void WwiseEvent::post_load_auto_bank(uint32_t p_bank_id)
{
	if (p_bank_id == AK_INVALID_UNIQUE_ID)
	{
		return;
	}

	const AkUniqueID event_id = get_id();
	AkUniqueID event_ids[] = { event_id };
	auto result = AK::SoundEngine::PrepareEvent(AK::SoundEngine::Preparation_Load, event_ids, 1);
	if (result != AK_Success)
	{
		UtilityFunctions::push_error(vformat(
				"WwiseGodot: PrepareEvent for %s failed with result: %s.", get_name(), wwise_error_string(result)));
	}
}

void WwiseEvent::unload_auto_bank()
{
	if (get_bank_id() != AK_INVALID_UNIQUE_ID && get_is_auto_bank_loaded())
	{
		const AkUniqueID event_id = get_id();
		AkUniqueID event_ids[] = { event_id };
		AK::SoundEngine::PrepareEvent(AK::SoundEngine::Preparation_Unload, event_ids, 1);
		AkBankManager::unload_bank(get_name());
		set_bank_id(AK_INVALID_UNIQUE_ID);
	}
}
AkBankTypeEnum WwiseEvent::get_bank_type() const
{
	switch (get_object_type())
	{
		case WwiseObjectType::AuxBus:
			return AkBankTypeEnum::AkBankType_Bus;
		case WwiseObjectType::Event:
			return AkBankTypeEnum::AkBankType_Event;
		default:
			return AkBankTypeEnum::AkBankType_User;
	}
}

uint32_t WwiseEvent::post(Node* p_node)
{
	if (p_node)
	{
		playing_id = Wwise::get_singleton()->post_event_id(get_id(), p_node);
		if (get_is_in_user_defined_sound_bank() && playing_id == 0)
		{
			UtilityFunctions::push_error(
					"WwiseGodot: Post Event failed. This Event is in a User Defined Soundbank. Make sure to add an "
					"AkBank Node to the Scene Tree and load the corresponding SoundBank before posting the Event.");
		}
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Could not post Event (name: %s, ID: %d). The Node "
											   "(GameObject) to post the event on has been deleted or is now invalid.",
				get_name(), get_id()));
	}

	return playing_id;
}

uint32_t WwiseEvent::post_callback(Node* p_node, AkUtils::AkCallbackType p_flags, const Callable& cookie)
{
	if (p_node)
	{
		playing_id = Wwise::get_singleton()->post_event_id_callback(get_id(), p_flags, p_node, cookie);
		if (get_is_in_user_defined_sound_bank() && playing_id == 0)
		{
			UtilityFunctions::push_error(
					"WwiseGodot: Post Event failed. This Event is in a User Defined Soundbank. Make sure to add an "
					"AkBank Node to the Scene Tree and load the corresponding SoundBank before posting the Event.");
		}
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Could not post Event (name: %s, ID: %d). The Node "
											   "(GameObject) to post the event on has been deleted or is now invalid.",
				get_name(), get_id()));
	}

	return playing_id;
}

void WwiseEvent::stop(Node* p_node, int p_transition_duration, AkUtils::AkCurveInterpolation p_fade_curve)
{
	execute_action(p_node, AkUtils::AK_ACTION_ON_EVENT_STOP, p_transition_duration, p_fade_curve);
}

void WwiseEvent::execute_action(Node* p_node, AkUtils::AkActionOnEventType p_action_on_event_type,
		int p_transition_duration, AkUtils::AkCurveInterpolation p_fade_curve)
{
	if (p_node)
	{
		Wwise::get_singleton()->execute_action_on_event_id(
				get_id(), p_action_on_event_type, p_node, p_transition_duration, p_fade_curve);
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Could not stop Event (name: %s, ID: %d). The Node "
											   "(GameObject) has been deleted or is now invalid.",
				get_name(), get_id()));
	}
}

void WwiseEvent::post_midi(Node* p_node, TypedArray<AkMidiPost> p_midi_posts)
{
	if (p_node)
	{
		Wwise::get_singleton()->post_midi_on_event_id(get_id(), p_node, p_midi_posts);
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Could not post Midi on Event (name: %s, ID: %d). The Node "
											   "(GameObject) has been deleted or is now invalid.",
				get_name(), get_id()));
	}
}

void WwiseEvent::stop_midi(Node* p_node)
{
	if (p_node)
	{
		Wwise::get_singleton()->stop_midi_on_event_id(get_id(), p_node);
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Could not stop Midi on Event (name: %s, ID: %d). The Node "
											   "(GameObject) has been deleted or is now invalid.",
				get_name(), get_id()));
	}
}

void WwiseEvent::set_is_in_user_defined_sound_bank(bool p_is_in_user_defined_soundbank)
{
	is_in_user_defined_sound_bank = p_is_in_user_defined_soundbank;
	emit_changed();
}

bool WwiseEvent::get_is_in_user_defined_sound_bank() const { return is_in_user_defined_sound_bank; }

void WwiseEvent::set_bank_id(uint32_t p_bank_id)
{
	bank_id = p_bank_id;
	emit_changed();
}

uint32_t WwiseEvent::get_bank_id() const { return bank_id; }

void WwiseEvent::set_is_auto_bank_loaded(bool p_is_auto_bank_loaded)
{
	is_auto_bank_loaded = p_is_auto_bank_loaded;
	emit_changed();
}

bool WwiseEvent::get_is_auto_bank_loaded() const { return is_auto_bank_loaded; }

void WwiseEvent::set_playing_id(AkPlayingID p_playing_id)
{
	playing_id = p_playing_id;
}

AkPlayingID WwiseEvent::get_playing_id() const { return playing_id; }