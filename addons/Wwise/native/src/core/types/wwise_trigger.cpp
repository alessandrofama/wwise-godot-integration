#include "wwise_trigger.h"

void WwiseTrigger::_bind_methods() { ClassDB::bind_method(D_METHOD("post", "game_object"), &WwiseTrigger::post); }

void WwiseTrigger::post(Node* p_game_object) { Wwise::get_singleton()->post_trigger_id(get_id(), p_game_object); }
