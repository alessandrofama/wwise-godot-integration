extends Node3D

var banks_loaded = false

@export var wwise_tests: Dictionary = {}


func load_test_banks():
	Wwise.load_bank_id(AK.BANKS.INIT)
	Wwise.load_bank_id(AK.BANKS.TESTBANK)
	banks_loaded = true


func unload_test_banks():
	Wwise.unload_bank_id(AK.BANKS.TESTBANK)
	Wwise.unload_bank_id(AK.BANKS.INIT)
	banks_loaded = false


func before_all():
	load_test_banks()


func after_all():
	unload_test_banks()


func _ready():
	print()
	print_rich(
		"[wave amp=50 freq=2]≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈ ≈ 🆁🆄🅽🅽🅸🅽🅶 ​ 🆃🅴🆂🆃🆂 ≈ ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈[/wave]\n"
	)
	before_all()
	var infos = Array()
	for test in wwise_tests:
		print_rich("[color=white]" + test + ":[/color]")
		var node = load(str(wwise_tests[test])).new()
		add_child(node)
		node.owner = self
		node.call("before_all")
		var info = await node.run(false)
		node.call("after_all")
		infos.append(info)
		node.free()
		print()
	after_all()
