extends Node

func _input(_ev):
	if Input.is_key_pressed(KEY_1):
		var connectResult = Waapi.connect("127.0.0.1", 8080)
		print("Connect Result: ", connectResult)
		
		if connectResult:
			var dict = Waapi.call("ak.wwise.core.getInfo", "{}", "{}")
			var jsonDocument = JSON.parse(dict["resultString"])
			
			if jsonDocument.error == OK:
				print(jsonDocument.result["branch"])
				print(jsonDocument.result["copyright"])
		else:
			print("Wwise authoring isn't launched!")

		if Waapi.is_connected():
			Waapi.disconnect()
