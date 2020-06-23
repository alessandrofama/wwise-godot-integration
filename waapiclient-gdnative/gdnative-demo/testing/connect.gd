extends Node

func _input(_ev):
	if Input.is_key_pressed(KEY_1):
		var result = Wwise.connect("127.0.0.1", 8080)
		print("Connect Result: ", result)
		if result:
			var dict = Wwise.call("ak.wwise.core.getInfo", "{}", "{}")
			print(dict)


