tool
extends EditorScript

const inputWwiseIDs = "res://Banks/Wwise_IDs.h"
const outputWwiseIDs = "res://Banks/wwise_ids.gd"

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
	var postProcessedLines = PoolStringArray(["class_name AK"])
	
	for line in contentLines:
		if line.find("class AK") != -1:
			continue
		elif line.find("//") != -1:
			continue
		elif line.find("#") != -1:
			continue
		elif line.find("    ", 0) == 0:
			var correctedLine = line.substr(4, line.length() - 4)
			
			if correctedLine.find("class") != -1:
				postProcessedLines.append("\n" + correctedLine + ":")
				continue
			if correctedLine.find("const") != -1:
				postProcessedLines.append(correctedLine)
	
	var outWwiseIDs = postProcessedLines.join("\n")

	var wwiseIDsGDFile = File.new()
	wwiseIDsGDFile.open(outputWwiseIDs, File.WRITE)
	wwiseIDsGDFile.store_string(outWwiseIDs)
	wwiseIDsGDFile.close()

	print("GDScript WWiseIDs generated at " + outputWwiseIDs)

	get_editor_interface().get_resource_filesystem().scan()
