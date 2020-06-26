extends Node

func _init():
	var connectResult = Waapi.connect_client("127.0.0.1", 8080)
	print("Connect Result: ", connectResult)
	
	if connectResult:
		var dict = Waapi.client_call("ak.wwise.core.getInfo", "{}", "{}")
		var jsonDocument = JSON.parse(dict["resultString"])
		
		if jsonDocument.error == OK:
			print(jsonDocument.result["branch"])
			print(jsonDocument.result["copyright"])
	else:
		print("Wwise authoring isn't launched!")

	if Waapi.is_client_connected():
		Waapi.disconnect_client()
