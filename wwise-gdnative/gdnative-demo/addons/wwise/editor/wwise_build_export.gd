class WwiseBuildExport:
	extends EditorExportPlugin
	
	class LibPaths:
		class Windows:
			const WWISE_DEBUG = "res://addons/wwise/bin/win64/debug/WwiseGDNative.dll"
			const WWISE_RELEASE = "res://addons/wwise/bin/win64/release/WwiseGDNative.dll"
			const WAAPI_DEBUG = "res://addons/wwise/bin/win64/debug/WaapiGDNative.dll"
			const WAAPI_RELEASE = "res://addons/wwise/bin/win64/release/WaapiGDNative.dll"
		class OSX:
			const WWISE_DEBUG = "res://addons/wwise/bin/osx/debug/libWwiseGDNative.dylib"
			const WWISE_RELEASE = "res://addons/wwise/bin/osx/release/libWwiseGDNative.dylib"
			const WAAPI_DEBUG = "res://addons/wwise/bin/osx/debug/libWaapiGDNative.dylib"
			const WAAPI_RELEASE = "res://addons/wwise/bin/osx/release/libWaapiGDNative.dylib"
		class Linux:
			const WWISE_DEBUG = "res://addons/wwise/bin/linux/debug/libWwiseGDNative.so"
			const WWISE_RELEASE = "res://addons/wwise/bin/linux/release/libWwiseGDNative.so"
		class Android:
			class Arm64v8a:
				const WWISE_DEBUG = "res://addons/wwise/bin/android/debug/arm64-v8a/libWwiseGDNative.Debug.arm64-v8a.so"
				const WWISE_RELEASE = "res://addons/wwise/bin/android/release/arm64-v8a/libWwiseGDNative.Release.arm64-v8a.so"
			class Armeabiv7a:
				const WWISE_DEBUG = "res://addons/wwise/bin/android/debug/armeabi-v7a/libWwiseGDNative.Debug.armeabi-v7a.so"
				const WWISE_RELEASE = "res://addons/wwise/bin/android/release/armeabi-v7a/libWwiseGDNative.Release.armeabi-v7a.so"
		class IOS:
			class Arm64:
				const WWISE_DEBUG = "res://addons/wwise/bin/iOS/debug/arm64/libWwiseGDNative.dylib"
				const WWISE_RELEASE = "res://addons/wwise/bin/iOS/release/arm64/libWwiseGDNative.dylib"
			class Armv7:
				const WWISE_DEBUG = "res://addons/wwise/bin/iOS/debug/armv7/libWwiseGDNative.dylib"
				const WWISE_RELEASE = "res://addons/wwise/bin/iOS/release/armv7/libWwiseGDNative.dylib"
			
	var changed_wwise_libs:Array = []
	var changed_waapi_libs:Array = []
	var wwise_lib:GDNativeLibrary = load("res://addons/wwise/bin/wwise-gdnative.gdnlib")
	var waapi_lib:GDNativeLibrary = load("res://addons/wwise/bin/waapi-gdnative.gdnlib")
	
	func _export_begin(features, debug, path, flags):
		if "Windows" in features:
			if debug:
				wwise_lib.config_file.set_value("entry", "Windows.64", LibPaths.Windows.WWISE_DEBUG)
				waapi_lib.config_file.set_value("entry", "Windows.64", LibPaths.Windows.WAAPI_DEBUG)
			else:
				wwise_lib.config_file.set_value("entry", "Windows.64", LibPaths.Windows.WWISE_RELEASE)
				waapi_lib.config_file.set_value("entry", "Windows.64", LibPaths.Windows.WAAPI_RELEASE)
				
				changed_wwise_libs.append({"entry": "Windows.64", "path": LibPaths.Windows.WWISE_DEBUG})
				changed_waapi_libs.append({"entry": "Windows.64", "path": LibPaths.Windows.WAAPI_DEBUG})
		if "OSX" in features:
			if debug:
				wwise_lib.config_file.set_value("entry", "OSX.64", LibPaths.OSX.WWISE_DEBUG)
			else:
				wwise_lib.config_file.set_value("entry", "OSX.64", LibPaths.OSX.WWISE_RELEASE)
				
				changed_wwise_libs.append({"entry": "OSX.64", "path": LibPaths.OSX.WWISE_DEBUG})
				changed_waapi_libs.append({"entry": "OSX.64", "path": LibPaths.OSX.WAAPI_DEBUG})
		if "X11" in features:
			if debug:
				wwise_lib.config_file.set_value("entry", "X11.64", LibPaths.Linux.WWISE_DEBUG)
			else:
				wwise_lib.config_file.set_value("entry", "X11.64", LibPaths.Linux.WWISE_RELEASE)
				
				changed_wwise_libs.append({"entry": "X11.64", "path": LibPaths.Linux.WWISE_DEBUG})
		if "Android" in features:
			if "arm64-v8a" in features:
				if debug:
					wwise_lib.config_file.set_value("entry", "Android.arm64-v8a", LibPaths.Android.Arm64v8a.WWISE_DEBUG)
				else:
					wwise_lib.config_file.set_value("entry", "Android.arm64-v8a", LibPaths.Android.Arm64v8a.WWISE_RELEASE)
					changed_wwise_libs.append({"entry": "Android.arm64-v8a", "path": LibPaths.Android.Arm64v8a.WWISE_DEBUG})
			if "armeabi-v7a" in features:
				if debug:
					wwise_lib.config_file.set_value("entry", "Android.armeabi-v7a", LibPaths.Android.Armeabiv7a.WWISE_DEBUG)
				else:
					wwise_lib.config_file.set_value("entry", "Android.armeabi-v7a", LibPaths.Android.Armeabiv7a.WWISE_RELEASE)
					changed_wwise_libs.append({"entry": "Android.armeabi-v7a", "path": LibPaths.Android.Armeabiv7a.WWISE_DEBUG})
		if "iOS" in features:		
			# Add missing StoreKit framework for iOS
			add_ios_framework("StoreKit.framework")
			if "arm64" in features:
				if debug:
					wwise_lib.config_file.set_value("entry", "iOS.arm64", LibPaths.IOS.Arm64.WWISE_DEBUG)
				else:
					wwise_lib.config_file.set_value("entry", "iOS.arm64", LibPaths.IOS.Arm64.WWISE_RELEASE)
					changed_wwise_libs.append({"entry": "iOS.arm64", "path": LibPaths.IOS.Arm64.WWISE_DEBUG})
			if "armv7" in features:
				if debug:
					wwise_lib.config_file.set_value("entry", "iOS.armv7", LibPaths.IOS.Armv7.WWISE_DEBUG)	
				else:
					wwise_lib.config_file.set_value("entry", "iOS.armv7", LibPaths.IOS.Armv7.WWISE_RELEASE)	
					changed_wwise_libs.append({"entry": "iOS.armv7", "path": LibPaths.IOS.Armv7.WWISE_DEBUG})
	
	func _export_file(path, type, features):

		var base_path:String = ProjectSettings.get_setting("wwise/common_user_settings/base_path")
		if !base_path.empty():
			for i in features.size():
				if path.findn(base_path) != -1:
					if features[i] == "Windows":
						if path.findn("Linux") != -1 || path.findn("Mac") != -1 || path.findn("Android") != -1 || path.findn("iOS") != -1:
							skip()
					if features[i] == "OSX":
						if path.findn("Windows") != -1 || path.findn("Linux") != -1 || path.findn("Android") != -1 || path.findn("iOS") != -1:
							skip()
					if features[i] == "X11":
						if path.findn("Windows") != -1 || path.findn("Mac") != -1 || path.findn("Android") != -1 || path.findn("iOS") != -1:
							skip()
					if features[i] == "Android":
						if path.findn("Windows") != -1 || path.findn("Mac") != -1 || path.findn("Linux") != -1 || path.findn("iOS") != -1:
							skip()
					if features[i] == "iOS":
						if path.findn("Windows") != -1 || path.findn("Mac") != -1 || path.findn("Linux") != -1 || path.findn("Android") != -1:
							skip()
		
	func _export_end():
		reset_wwise_libs()
		reset_waapi_libs()
	
	# config_file.set_value() stores the new wwise_lib path value permanently. So we change back to the debug wwise_lib after exporting.
	func reset_wwise_libs():
		if changed_wwise_libs.size() == 0:
			return
		for library in changed_wwise_libs:
			wwise_lib.config_file.set_value("entry", library["entry"], library["path"])
		changed_wwise_libs.clear()

	func reset_waapi_libs():
		if changed_waapi_libs.size() == 0:
			return
		for library in changed_waapi_libs:
			waapi_lib.config_file.set_value("entry", library["entry"], library["path"])
		changed_waapi_libs.clear()
