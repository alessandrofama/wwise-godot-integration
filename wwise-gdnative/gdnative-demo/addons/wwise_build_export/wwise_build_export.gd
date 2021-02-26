tool
extends EditorPlugin

class WwiseBuildExport:
	extends EditorExportPlugin

	func _export_begin(features, debug, path, flags):
		if "iOS" in features:		
			# Add missing StoreKit framework for iOS
			add_ios_framework("StoreKit.framework")
		
func _init():
	add_export_plugin(WwiseBuildExport.new())
