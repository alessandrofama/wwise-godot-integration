tool
extends EditorScript

const inputWwiseIDs = "res://Banks/Wwise_IDs.h"
const outputWwiseIDs = "res://Banks/wwise_ids.gd"

var postProcessedLines = PoolStringArray(["class_name AK"])
var enumBlock = PoolStringArray([])
var correctedLine = ""

func _run():
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
	
	var outWwiseIDs = postProcessedLines.join("\n")

	var wwiseIDsGDFile = File.new()
	wwiseIDsGDFile.open(outputWwiseIDs, File.WRITE)
	wwiseIDsGDFile.store_string(outWwiseIDs)
	wwiseIDsGDFile.close()

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
