extends "res://wwise/runtime/helpers/ak_event_handler.gd"

export(AK.EVENTS._enum) var event:int
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time:int = 0
export(AkUtils.AkCurveInterpolation) var stop_interpolation_curve:int = AkUtils.AkCurveInterpolation.LINEAR
var playing_id:int

export(bool) var use_callback:bool = false
export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_EndOfEvent
export(NodePath) var callback_receiver:NodePath

export(bool) var is_environment_aware:bool = false;
var listener:Spatial
var ray:RayCast
var colliding_objects:Array = []
var ak_environment_data

export(bool) var is_spatial:bool = false

func _enter_tree():
	if Engine.is_editor_hint():
		return
	else:
		register_game_object(self, self.get_name())
	
func _ready() -> void:
	if Engine.is_editor_hint():
		self.set_process(false)
		return
	else:
		self.set_process(true)
		
	if use_callback:
		connect_signals(callback_receiver, callback_type)
	# If is_environment_aware is checked, Wwise.set_game_obj_aux_send_values will
	# be called. Each Event will instantiate the AkGameObjeckEnvironmentData class,
	# this instance holds an Array with currently active environments for this
	# Event. Activate environments are added by AkEnvironment nodes themselves 
	# (by checking if any Event is inside the Environment area). 
	if is_environment_aware:
		listener = get_listener()
		# This ray is used in set_obstruction_and_occlusion to check whether
		# there are any objects between Event and Listener.
		ray = set_up_raycast(self)
		ak_environment_data = preload("res://wwise/runtime/helpers/ak_game_obj_environment_data.gd").new()
		
func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		post_event()
	if stop_on == game_event:
		stop_event()
		
func post_event() -> void:
	if not use_callback:
		playing_id = Wwise.post_event_id(event, self)
	else:
		playing_id = Wwise.post_event_id_callback(event, callback_type, self)
	
func stop_event() -> void:
	Wwise.stop_event(playing_id, stop_fade_time, stop_interpolation_curve)
		
func _process(_delta) -> void:
	Wwise.set_3d_position(self, get_global_transform())
	
	if is_environment_aware:
		ak_environment_data.update_aux_send(self, self.get_global_transform().origin)
	
func _physics_process(_delta) -> void:
		if listener and is_environment_aware:
			set_obstruction_and_occlusion(self, listener, colliding_objects, ray, 0)

