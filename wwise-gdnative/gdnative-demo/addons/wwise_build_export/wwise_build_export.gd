tool
extends EditorPlugin

class WwiseBuildExport:
	extends EditorExportPlugin
	
	class LibPaths:
		class Windows:
			const DEBUG = "res://wwise/bin/win64/debug/WwiseGDNative.dll"
			const RELEASE = "res://wwise/bin/win64/release/WwiseGDNative.dll"
		class OSX:
			const DEBUG = "res://wwise/bin/osx/debug/libWwiseGDNative.dylib"
			const RELEASE = "res://wwise/bin/osx/release/libWwiseGDNative.dylib"
		class Linux:
			const DEBUG = "res://wwise/bin/linux/debug/libWwiseGDNative.so"
			const RELEASE = "res://wwise/bin/linux/release/libWwiseGDNative.so"
		class Android:
			class Arm64v8a:
				const DEBUG = "res://wwise/bin/android/debug/arm64-v8a/libWwiseGDNative.Debug.arm64-v8a.so"
				const RELEASE = "res://wwise/bin/android/release/arm64-v8a/libWwiseGDNative.Release.arm64-v8a.so"
			class Armeabiv7a:
				const DEBUG = "res://wwise/bin/android/debug/armeabi-v7a/libWwiseGDNative.Debug.armeabi-v7a.so"
				const RELEASE = "res://wwise/bin/android/release/armeabi-v7a/libWwiseGDNative.Release.armeabi-v7a.so"
		class IOS:
			class Arm64:
				const DEBUG = "res://wwise/bin/iOS/debug/arm64/libWwiseGDNative.dylib"
				const RELEASE = "res://wwise/bin/iOS/release/arm64/libWwiseGDNative.dylib"
			class Armv7:
				const DEBUG = "res://wwise/bin/iOS/debug/armv7/libWwiseGDNative.dylib"
				const RELEASE = "res://wwise/bin/iOS/release/armv7/libWwiseGDNative.dylib"
			
	var changed_libs:Array = []
	var lib:GDNativeLibrary = preload("res://wwise/bin/wwise-gdnative.gdnlib")

	func _export_begin(features, debug, path, flags):
		if "Windows" in features:
			if debug:
				lib.config_file.set_value("entry", "Windows.64", LibPaths.Windows.DEBUG)
			else:
				lib.config_file.set_value("entry", "Windows.64", LibPaths.Windows.RELEASE)
				changed_libs.append({"entry": "Windows.64", "path": LibPaths.Windows.DEBUG})
		if "OSX" in features:
			if debug:
				lib.config_file.set_value("entry", "OSX.64", LibPaths.OSX.DEBUG)
			else:
				lib.config_file.set_value("entry", "OSX.64", LibPaths.OSX.RELEASE)
				changed_libs.append({"entry": "OSX.64", "path": LibPaths.OSX.DEBUG})
		if "X11" in features:
			if debug:
				lib.config_file.set_value("entry", "X11.64", LibPaths.Linux.DEBUG)
			else:
				lib.config_file.set_value("entry", "X11.64", LibPaths.Linux.RELEASE)
				changed_libs.append({"entry": "X11.64", "path": LibPaths.Linux.DEBUG})
		if "Android" in features:
			if "arm64-v8a" in features:
				if debug:
					lib.config_file.set_value("entry", "Android.arm64-v8a", LibPaths.Android.Arm64v8a.DEBUG)
				else:
					lib.config_file.set_value("entry", "Android.arm64-v8a", LibPaths.Android.Arm64v8a.RELEASE)
					changed_libs.append({"entry": "Android.arm64-v8a", "path": LibPaths.Android.Arm64v8a.DEBUG})
			if "armeabi-v7a" in features:
				if debug:
					lib.config_file.set_value("entry", "Android.armeabi-v7a", LibPaths.Android.Armeabiv7a.DEBUG)
				else:
					lib.config_file.set_value("entry", "Android.armeabi-v7a", LibPaths.Android.Armeabiv7a.RELEASE)
					changed_libs.append({"entry": "Android.armeabi-v7a", "path": LibPaths.Android.Armeabiv7a.DEBUG})
		if "iOS" in features:		
			# Add missing StoreKit framework for iOS
			add_ios_framework("StoreKit.framework")
			if "arm64" in features:
				if debug:
					lib.config_file.set_value("entry", "IOS.arm64", LibPaths.IOS.Arm64.DEBUG)
				else:
					lib.config_file.set_value("entry", "IOS.arm64", LibPaths.IOS.Arm64.RELEASE)
					changed_libs.append({"entry": "IOS.arm64", "path": LibPaths.IOS.Arm64.DEBUG})
			if "armv7" in features:
				if debug:
					lib.config_file.set_value("entry", "IOS.armv7", LibPaths.IOS.Armv7.DEBUG)	
				else:
					lib.config_file.set_value("entry", "IOS.armv7", LibPaths.IOS.Armv7.RELEASE)	
					changed_libs.append({"entry": "IOS.armv7", "path": LibPaths.IOS.Armv7.DEBUG})
	
	func _export_end():
		reset_libs()
	
	# config_file.set_value() stores the new lib path value permanently. So we change back to the debug lib after exporting.
	func reset_libs():
		if changed_libs.size() == 0:
			return
		for library in changed_libs:
			lib.config_file.set_value("entry", library["entry"], library["path"])
		changed_libs.clear()
		
func _init():
	add_export_plugin(WwiseBuildExport.new())
