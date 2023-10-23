#ifndef AK_EVENT_3D_GIZMO_PLUGIN_H
#define AK_EVENT_3D_GIZMO_PLUGIN_H

#include <godot_cpp/classes/editor_node3d_gizmo_plugin.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

namespace godot
{

class AkEvent3DGizmoPlugin : public EditorNode3DGizmoPlugin
{
	GDCLASS(AkEvent3DGizmoPlugin, EditorNode3DGizmoPlugin);

protected:
	static void _bind_methods() {}

private:
	const String ak_event_speaker_path = "res://addons/Wwise/editor/images/wwise_audio_speaker.svg";
	Ref<Texture2D> texture;

public:
	virtual bool _has_gizmo(Node3D* for_node_3d) const override
	{
		if (for_node_3d)
		{
			return for_node_3d->get_class() == "AkEvent3D";
		}
		return false;
	}
	virtual String _get_gizmo_name() const override { return "AkEvent3DGizmo"; }
	virtual int32_t _get_priority() const override { return -1; }
	virtual void _redraw(const Ref<EditorNode3DGizmo>& gizmo) override
	{
		gizmo->clear();

		if (!texture.is_valid())
		{
			texture = ResourceLoader::get_singleton()->load(ak_event_speaker_path);

			if (!texture.is_valid())
			{
				return;
			}

			create_icon_material("ak_event_3d_icon_material", texture);
		}

		Ref<StandardMaterial3D> material = get_material("ak_event_3d_icon_material", gizmo);
		gizmo->add_unscaled_billboard(material, 0.05);
	}
};

} //namespace godot

#endif