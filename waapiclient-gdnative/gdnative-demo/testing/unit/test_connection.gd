extends "res://addons/gut/test.gd"

class TestConnection:
	extends "res://addons/gut/test.gd"
	
		# Using the client without connecting first.
	func test_assert_client_without_connect():
		if Waapi.is_client_connected():
			Waapi.disconnect_client()
		var dict = Waapi.client_call("ak.wwise.core.getInfo", "{}", "{}")
		var result = dict["callResult"]
		assert_false(result, "Calling client without connecting should be false")
		
		# Connecting, should not fail.
	func test_assert_client_connect():
		var connectResult = Waapi.connect_client("127.0.0.1", 8080)
		assert_true(connectResult, "Connecting to client should be true")
		if Waapi.is_client_connected():
			Waapi.disconnect_client()
