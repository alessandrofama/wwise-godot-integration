#ifndef WWISE_EDITOR_SCALE_H
#define WWISE_EDITOR_SCALE_H

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot
{

class WwiseEditorScale : public EditorPlugin
{
	GDCLASS(WwiseEditorScale, EditorPlugin);

protected:
	static void _bind_methods() {}

private:
	static WwiseEditorScale* singleton;

public:
	WwiseEditorScale()
	{
		ERR_FAIL_COND(singleton != nullptr);
		singleton = this;
	}

	~WwiseEditorScale()
	{
		ERR_FAIL_COND(singleton != this);
		singleton = nullptr;
	}

	static WwiseEditorScale* get_singleton() { return singleton; }

	double get_editor_scale()
	{
		EditorInterface* editor_interface = get_editor_interface();
		if (editor_interface)
		{
			return editor_interface->get_editor_scale();
		}

		return 1.0;
	}
};

} //namespace godot

#endif
