class AkBuildExport:
	extends EditorExportPlugin

	func _get_name() -> String:
		return "AkBuildExport"

	func _export_file(path: String, type: String, features: PackedStringArray):
		var base_path: String = ProjectSettings.get_setting("wwise/common_user_settings/base_path")
		if !base_path.is_empty():
			for i in features.size():
				if path.findn(base_path) != -1:
					if features[i] == "windows":
						if (
							path.findn("Linux") != -1
							|| path.findn("Mac") != -1
							|| path.findn("Android") != -1
							|| path.findn("iOS") != -1
						):
							skip()
					if features[i] == "macos":
						if (
							path.findn("Windows") != -1
							|| path.findn("Linux") != -1
							|| path.findn("Android") != -1
							|| path.findn("iOS") != -1
						):
							skip()
					if features[i] == "linux":
						if (
							path.findn("Windows") != -1
							|| path.findn("Mac") != -1
							|| path.findn("Android") != -1
							|| path.findn("iOS") != -1
						):
							skip()
					if features[i] == "android":
						if (
							path.findn("Windows") != -1
							|| path.findn("Mac") != -1
							|| path.findn("Linux") != -1
							|| path.findn("iOS") != -1
						):
							skip()
					if features[i] == "ios":
						if (
							path.findn("Windows") != -1
							|| path.findn("Mac") != -1
							|| path.findn("Linux") != -1
							|| path.findn("Android") != -1
						):
							skip()
