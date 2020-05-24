extends Spatial
class_name AkGameObj

export(bool) var is_environment_aware:bool = true;

func register_game_object(object:Object, gameObjectName:String):
	Wwise.register_game_obj(object, gameObjectName)
