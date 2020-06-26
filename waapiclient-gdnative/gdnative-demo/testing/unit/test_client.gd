extends "res://addons/gut/test.gd"

class TestClient:
	extends "res://addons/gut/test.gd"
	
	# Using the client without connecting first.
	func test_assert_client_without_connect():
		var dict = Waapi.call("ak.wwise.core.getInfo", "{}", "{}")
		var result = dict["callResult"]
		assert_false(result, "Calling client without connecting should be false")
		
	# Connecting, should not fail.
	func test_assert_client_connect():
		var connectResult = Waapi.connect("127.0.0.1", 8080)
		assert_true(connectResult, "Connecting to client should be true")
		if Waapi.is_connected():
			Waapi.disconnect()
			
	# Calling with the wrong URI.
	func test_assert_client_call_wrong_uri():
		var connectResult = Waapi.connect("127.0.0.1", 8080)
		if connectResult:
			var dict = Waapi.call("ak.wwise.wronguri", "{}", "{}")
			var result = dict["callResult"]
			assert_false(result, "Calling client with wrong URI should be false")
		if Waapi.is_connected():
			Waapi.disconnect()
		
	
	
