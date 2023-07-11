extends Node3D

var tests: Array = []


func before_all():
	pass


func after_all():
	pass


func ak_assert(condition, message: String = ""):
	if not condition:
		if not message.is_empty():
			print("Assertion Error: ", message)
		return false
	return true


func get_method_names() -> Array:
	var methods = []
	for fun in get_method_list():
		if fun["flags"] == METHOD_FLAG_NORMAL:
			var func_name: String = fun["name"]
			if func_name.begins_with("test_"):
				var dict = {"name": func_name, "func": Callable(self, func_name)}
				methods.append(dict)
	return methods


func run_tests(tests, randomize) -> Dictionary:
	before_all()
	var passed = 0
	var failed = 0
	var failed_functions = Array()
	if randomize:
		tests.shuffle()
	for test in tests:
		Wwise.render_audio()
		var result = await test["func"].call()
		if result:
			passed += 1
			print_rich("[color=green]PASS: ", test["name"] + "[/color]")
		else:
			failed += 1
			failed_functions.append(test["name"])
			print_rich("[color=red]FAIL: ", test["name"] + "[/color]")
	after_all()
	print_rich("[color=green]Passed: ", str(passed) + "[/color]")
	print_rich("[color=red]Failed: ", str(failed) + "[/color]")
	return {"passed": passed, "failed": failed, "failed_functions": failed_functions}
