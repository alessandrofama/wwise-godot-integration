extends Spatial
class_name AkGameObj

func register_game_object(object:Object, gameObjectName:String):
	Wwise.register_game_obj(object, gameObjectName)

func get_listener() -> Spatial: 
	return get_tree().get_root().find_node("AkListener", true, false) as Spatial
	# haaacky and slow, needs a better solution ^_^"
