extends "res://addons/wwise/runtime/helpers/ak_event_handler_2d.gd"

var playing_id:int
var cookie:FuncRef

func _enter_tree() -> void:
	if Engine.is_editor_hint():
		self.set_process(false)
		return
	self.set_process(true)
	
	register_game_object(self, self.get_name())
	
	if use_callback:
		cookie = FuncRef.new()
		cookie.set_function("callback_emitter")
		cookie.set_instance(self)
	
func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		post_event()
	if stop_on == game_event:
		stop_event()
		
func post_event() -> void:
	if not use_callback:
		playing_id = Wwise.post_event_id(event.get("Id"), self)
	else:
		playing_id = Wwise.post_event_id_callback(event.get("Id"), callback_flag, self, cookie)
	
func stop_event() -> void:
	Wwise.stop_event(playing_id, stop_fade_time, interpolation_mode)
	
func _process(_delta) -> void:
	if Engine.is_editor_hint():
		return
		
	Wwise.set_2d_position(self, get_global_transform(), self.z_index)
