tool
extends EditorPlugin

var postProcessedLines = PoolStringArray(["class_name AK"])
var enumBlock = PoolStringArray([])
var correctedLine = ""

var fileDialog
const WWISE_IDS_H_FILE = "Wwise_IDs.h"
const WWISE_IDS_GD_FILE = "wwise_ids.gd"

func _enter_tree():
	add_tool_menu_item("Convert Wwise IDs", self, "_do_work")

	fileDialog = FileDialog.new()
	fileDialog.mode = FileDialog.MODE_OPEN_FILE
	fileDialog.access = FileDialog.ACCESS_RESOURCES
	fileDialog.filters = PoolStringArray([WWISE_IDS_H_FILE + " ; Wwise IDs header file"])

	fileDialog.connect("file_selected", self, "_on_FileDialog_file_selected")

	var editor_interface = get_editor_interface()
	var base_control = editor_interface.get_base_control()
	base_control.add_child(fileDialog)

func _exit_tree():
	fileDialog.queue_free()

func _do_work(ud):
	fileDialog.popup_centered_ratio()

func _on_FileDialog_file_selected(path):
	var inputWwiseIDs = path
	var outputWwiseIDs = ""
	
	if inputWwiseIDs.find(WWISE_IDS_H_FILE) != -1:
		outputWwiseIDs = inputWwiseIDs.replace(WWISE_IDS_H_FILE, WWISE_IDS_GD_FILE)
		_convert(inputWwiseIDs, outputWwiseIDs)

func _convert(inputWwiseIDs, outputWwiseIDs):
	var wwiseIDsHFile = File.new()
	wwiseIDsHFile.open(inputWwiseIDs, File.READ)
	
	var content = wwiseIDsHFile.get_as_text()
	wwiseIDsHFile.close()
	
	content = content.replace("namespace", "class")
	content = content.replace("static const AkUniqueID", "const")
	content = content.replace("{", "")
	content = content.replace("}", "")
	content = content.replace("U;", "")
	
	var contentLines = content.split("\n", false)
	
	for line in contentLines:
		if line.find("class AK") != -1:
			continue
		elif line.find("//") != -1:
			continue
		elif line.find("#") != -1:
			continue
		elif line.find("    ", 0) == 0:
			correctedLine = line.substr(4, line.length() - 4)
			correctedLine = correctedLine.replace("    ", "\t")
			
			if correctedLine.find("class") != -1:
				if enumBlock.size() > 0:
					_appendEnumBlock()			
				postProcessedLines.append("\n" + correctedLine + ":")

			elif correctedLine.find("const") != -1:
				postProcessedLines.append(correctedLine)
				var enumLine = correctedLine.replace("const ", "\t")
				enumBlock.append(enumLine)
				
	if enumBlock.size() > 0:
		_appendEnumBlock()

	# Checking if non-default/empty classes missing, adding empty classes and enums
	# to avoid breaking custom node variable exports
	var joinedLines = postProcessedLines.join("\n")	
	if joinedLines.find("class EVENTS") == -1:
		postProcessedLines.append("class EVENTS:\n\t enum _enum {}")
	if joinedLines.find("class AUX_BUSSES") == -1:
		postProcessedLines.append("class AUX_BUSSES:\n\t enum _enum {}")
	if joinedLines.find("class GAME_PARAMETERS") == -1:
		postProcessedLines.append("class GAME_PARAMETERS:\n\t enum _enum {}")
	
	var outWwiseIDs = postProcessedLines.join("\n")

	var wwiseIDsGDFile = File.new()
	wwiseIDsGDFile.open(outputWwiseIDs, File.WRITE)
	wwiseIDsGDFile.store_string(outWwiseIDs)
	wwiseIDsGDFile.close()
	
	# Running the tool multiple times in a row duplicates everything
	postProcessedLines = PoolStringArray(["class_name AK"])

	print("GDScript WWiseIDs generated at " + outputWwiseIDs)

	get_editor_interface().get_resource_filesystem().scan()

func _appendEnumBlock():
	for line in enumBlock.size() - 1:
		enumBlock[line] = enumBlock[line] + ","

	var numTabs = enumBlock[0].count("\t") - 1
	var tabFill = ""
	for _tab in range(numTabs):
		tabFill = tabFill + "\t"

	var enumHeader = "\n" + tabFill + "enum _enum {"
	enumBlock.insert(0, enumHeader)
	enumBlock.append(tabFill + "}")
	
	var enumLines = enumBlock.join("\n")
	postProcessedLines.append(enumLines)
	enumBlock = PoolStringArray([])	
