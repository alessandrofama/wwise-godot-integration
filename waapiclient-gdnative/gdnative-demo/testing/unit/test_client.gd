extends "res://addons/gut/test.gd"

class TestClient:
	extends "res://addons/gut/test.gd"
	
	var connectResult
	
	func before_all():
		connectResult = Waapi.connect_client("127.0.0.1", 8080)
		
	func after_all():
		if Waapi.is_client_connected():
			Waapi.disconnect_client()
		
	# Calling with the wrong URI.
	func test_assert_client_call_wrong_uri():
		if connectResult:
			var dict = Waapi.client_call("ak.wwise.wronguri", "{}", "{}")
			var result = dict["callResult"]
			assert_false(result, "Calling client with wrong URI should be false")

	# Calling with a right URI.
	func test_assert_client_call_uri():
		if connectResult:
			var dict = Waapi.client_call("ak.wwise.core.getInfo", "{}", "{}")
			var result = dict["callResult"]
			assert_true(result, "Client callResult should be true")
	
	# Calling with timeout
	func test_assert_client_call_timeout():
		if connectResult:
			var dict = Waapi.client_call_with_timeout("ak.wwise.core.getInfo", "{}", "{}", 5)
			var result = dict["callResult"]
			assert_true(result, "Client callResult with timeout should be true")
	
	# Calling with wrong arguments
	func test_assert_client_call_wrong_key():
		if connectResult:
			var jsonDict = {"invalidKey": ["invalidValue"]}
			var dict = Waapi.client_call("ak.wwise.core.getInfo", JSON.print(jsonDict), "{}")
			var result = dict["callResult"]
			assert_false(result, "Calling client with wrong argument key should be false")

	# Subscribing with wrong URI
	func test_assert_client_subscribe_wrong_uri():
		if connectResult:
			var options = {"return": ["id", "name"]}
			var dict = Waapi.subscribe("ak.wwise.wronguri", JSON.print(options))
			var result = dict["subscribeResult"]
			assert_false(result, "Subscribing with wrong URI should be false")

	# Subscribing 
	func test_assert_client_subscribe():
		if connectResult:
			var options = {"return": ["id", "name"]}
			var dict = Waapi.subscribe("ak.wwise.ui.selectionChanged", JSON.print(options))
			var result = dict["subscribeResult"]
			assert_true(result, "subscribeResult should be true")

	# Subscribing with timeout
	func test_assert_client_subscribe_timeout():
		if connectResult:
			var options = {"return": ["id", "name"]}
			var dict = Waapi.subscribe_with_timeout("ak.wwise.ui.selectionChanged", JSON.print(options), 3)
			var result = dict["subscribeResult"]
			assert_true(result, "subscribeResult with timeout should be true")

	# Unsubscribing 
	func test_assert_client_unsubscribe():
		if connectResult:
			var options = {"return": ["id", "name"]}
			var subscription_dict = Waapi.subscribe("ak.wwise.ui.selectionChanged", JSON.print(options))
			var subscription_id = subscription_dict["subscriptionId"]
			var unsubscription_dict = Waapi.unsusbscribe(subscription_id)
			var result = unsubscription_dict["unsubscribeResult"]
			assert_true(result, "unsubscribeResult should be true")
	
	# Unsubscribing with wrong id
	func test_assert_client_unsubscribe_wrong_id():
		if connectResult:
			var subscription_id = 54262452624
			var unsubscription_dict = Waapi.unsusbscribe(subscription_id)
			var result = unsubscription_dict["unsubscribeResult"]
			assert_false(result, "unsubscribeResult should be false")
	
	# Unsubscribing with timeout
	func test_assert_client_unsubscribe_timeout():
		if connectResult:
			var options = {"return": ["id", "name"]}
			var subscription_dict = Waapi.subscribe_with_timeout("ak.wwise.ui.selectionChanged", JSON.print(options), 5)
			var subscription_id = subscription_dict["subscriptionId"]
			var unsubscription_dict = Waapi.unsubscribe_with_timeout(subscription_id, 3)
			var result = unsubscription_dict["unsubscribeResult"]
			assert_true(result, "unsubscribeResult should be true")

	# Getting last string
	func test_assert_get_last_string():
		if connectResult:
			var _dict = Waapi.client_call("ak.wwise.core.getInfo", "{}", "{}")
			var last_string:String = Waapi.get_last_string()
			assert_false(last_string.empty(), "Last String should not be empty")

