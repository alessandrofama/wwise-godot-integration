#include "ak_event_3d_gizmo_plugin.h"

bool AkEvent3DGizmoPlugin::_has_gizmo(Node3D* for_node_3d) const
{
	if (for_node_3d)
	{
		return for_node_3d->get_class() == "AkEvent3D";
	}
	return false;
}

String AkEvent3DGizmoPlugin::_get_gizmo_name() const { return "AkEvent3DGizmo"; }

int32_t AkEvent3DGizmoPlugin::_get_priority() const { return -1; }

void AkEvent3DGizmoPlugin::_redraw(const Ref<EditorNode3DGizmo>& gizmo)
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
