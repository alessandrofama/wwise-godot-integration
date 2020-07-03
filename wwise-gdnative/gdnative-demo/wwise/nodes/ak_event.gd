extends AkEventHandler
class_name AkEvent

export(AK.EVENTS._enum) var event:int
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time = 0
export(AkUtils.AkCurveInterpolation) var interpolation_mode = AkUtils.AkCurveInterpolation.LINEAR
var playingID:int

export(bool) var use_callback = false
export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_EndOfEvent
export(NodePath) var callback_receiver:NodePath

export(bool) var is_environment_aware:bool = false;
var listener:Spatial
var ray:RayCast
var colliding_objects:Array = []
var ak_environment_data:AkGameObjectEnvironmentData

export(bool) var is_spatial = false
var last_position:Vector3

func _enter_tree():
	register_game_object(self, self.get_name())
	
func _ready() -> void:
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
		ak_environment_data = AkGameObjectEnvironmentData.new()
		
func handle_game_event(gameEvent:int) -> void:
	if trigger_on == gameEvent:
		post_event()
	if stop_on == gameEvent:
		stop_event()
		
func post_event() -> void:
	if not use_callback:
		playingID = Wwise.post_event_id(event, self)
	else:
		playingID = Wwise.post_event_id_callback(event, callback_type, self)
	
func stop_event() -> void:
	Wwise.stop_event(playingID, stop_fade_time, interpolation_mode)
		
func _process(_delta) -> void:
	# Checking if the position of the Event hasn't changed. In that case not calling
	# Wwise.set_3d_position and saving API calls.
	if not get_global_transform().origin == last_position:
		Wwise.set_3d_position(self, get_global_transform())
		last_position = get_global_transform().origin
	
	if is_environment_aware:
		ak_environment_data.update_aux_send(self, self.get_global_transform().origin)
	
func _physics_process(_delta) -> void:
		if listener and not is_spatial:
			set_obstruction_and_occlusion(self, listener, colliding_objects, ray, 0)

