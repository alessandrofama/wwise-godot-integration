extends Node

var active_environments:Array = []
var ak_aux_data
var last_position:Vector3 = Vector3(0, 0, 0)
var hasEnvironmentListChanged:bool = true

func _init():
	ak_aux_data = load("res://addons/wwise/runtime/helpers/ak_aux_data.gd").new()
	
func add_environment(env:Object) -> void:
	if not env:
		return
	var index = active_environments.bsearch_custom(env, self, "compare_by_priority")
	active_environments.insert(index, env)
	hasEnvironmentListChanged = true

func remove_environment(env:Object):
	var index = active_environments.find(env)
	if not index == -1:
		active_environments.remove(index)
		hasEnvironmentListChanged = true

func compare_by_priority(a:Object, b:Object):
	if a.env_priority < b.env_priority: 
		return true
	elif a.env_priority == b.env_priority:
		return false
	else:
		return false
		
func add_highest_priority_environments():
	if ak_aux_data.aux_array.size() < active_environments.size():
		for i in active_environments.size():
			var env = active_environments[i]
			if i == 0 and !ak_aux_data.aux_array.has(env.aux_bus):
				var aux_data = {"aux_bus_id": env.aux_bus.get("Id"), "control_value":  1.0}
				ak_aux_data.aux_array.append(aux_data)

func update_aux_send(event:Object, event_position:Vector3) -> void:
	if !hasEnvironmentListChanged && event_position == last_position:
		return
	ak_aux_data.reset()
	add_highest_priority_environments()
	ak_aux_data.set_values(event)
	last_position = event_position
	hasEnvironmentListChanged = false
