extends Node

func _ready():
	var result = Wwise.connect("127.0.0.1", 8080)
	print("Connect Result: ", result)
	
	var dict= Wwise.call("ak.wwise.core.remote.getConnectionStatus", "{}", "{}")
	print(dict)

