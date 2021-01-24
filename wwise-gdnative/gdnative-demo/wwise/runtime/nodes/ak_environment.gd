extends Area

export(AK.AUX_BUSSES._enum) var aux_bus:int = AK.AUX_BUSSES._enum.values()[0]

export var env_priority = 0

func _ready():
	#warning-ignore:return_value_discarded
	self.connect("area_entered", self, "_on_area_enter")
	#warning-ignore:return_value_discarded
	self.connect("area_exited", self, "_on_area_exit")

func _on_area_enter(area):
	var parent = area.get_parent()
	if parent.get_class() == "AkEvent":
		if parent.is_environment_aware:
			parent.ak_environment_data.add_environment(self)

func _on_area_exit(area):
	var parent = area.get_parent()
	if parent.get_class() == "AkEvent":
		if parent.is_environment_aware:
			parent.ak_environment_data.remove_environment(self)
