tool
extends EditorPlugin

class WwiseBuildExport:
	extends EditorExportPlugin
	
	func _add_dir_contents(dir: Directory, files: Array, directories: Array):
		var file_name = dir.get_next()
	
		while (file_name != ""):
			var path = dir.get_current_dir() + "/" + file_name
	
			if dir.current_is_dir():
				print("Found Wwise directory: %s" % path)
				var subDir = Directory.new()
				subDir.open(path)
				subDir.list_dir_begin(true, false)
				directories.append(path)
				_add_dir_contents(subDir, files, directories)
			else:
				print("Found Wwise file: %s" % path)
				files.append(path)
	
			file_name = dir.get_next()
	
		dir.list_dir_end()

	func _export_begin(features, debug, path, flags):		
		# Get the base path for Wwise data
		var basePath = ProjectSettings.get_setting("wwise/common_user_settings/base_path")
		print("Wwise base path: ", basePath)
		
		# Only copy banks if we are exporting for iOS
		if "iOS" in features:
			print("Copying Wwise data to build...")

			var generatedSoundBanksPath = basePath + "/iOS/"
			
			var dir = Directory.new()
			
			if dir.open(generatedSoundBanksPath) == OK:
				var baseDir = path.get_base_dir() + "/"
				var destinationDir = baseDir + generatedSoundBanksPath.replace("res://", "")
				print("Destination dir: ", destinationDir)
				dir.make_dir_recursive(destinationDir)
				
				var files = []
				var directories = []
				
				dir.list_dir_begin(true, false)
				_add_dir_contents(dir, files, directories)
				dir.list_dir_end()
				
				for directory in directories:
					dir.make_dir_recursive(baseDir + directory.replace("res://", ""))
					
				for file in files:
					dir.copy(file, baseDir + file.replace("res://", ""))
			else:
				print("Error when trying to open the generated soundbanks path")			

			# Add missing StoreKit framework for iOS
			add_ios_framework("StoreKit.framework")
		
func _init():
	add_export_plugin(WwiseBuildExport.new())
