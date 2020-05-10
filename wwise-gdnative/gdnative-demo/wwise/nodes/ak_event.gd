extends AkGameObj
class_name AkEvent

export(int) var event
export(AkUtils.GameEvent) var trigger_on
export(AkUtils.GameEvent) var stop_on
export(int) var stop_fade_time = 0
export (AkUtils.AkInterpolationMode) var interpolation_mode = 4

var playingID:int

func _init():
	register_game_object(self, String(event))
	handle_game_event(AkUtils.GameEvent.INIT);

func _enter_tree():
	handle_game_event(AkUtils.GameEvent.TREE_ENTER);

func _exit_tree():
	handle_game_event(AkUtils.GameEvent.TREE_EXIT)
	
func _ready():
	handle_game_event(AkUtils.GameEvent.READY);
	
func handle_game_event(gameEvent:int):
	if trigger_on == gameEvent:
		post_event()
	if stop_on == gameEvent:
		stop_event()
		
func post_event():
	playingID = Wwise.post_event_id(event, self)
	
func stop_event():
	Wwise.stop_event(playingID, stop_fade_time, interpolation_mode)
	
func _process(_delta):
	Wwise.set_3d_position(self, get_global_transform())
	
