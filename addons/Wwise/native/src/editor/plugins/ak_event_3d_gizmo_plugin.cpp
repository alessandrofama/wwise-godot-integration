#include "ak_event_3d_gizmo_plugin.h"

AkEvent3DGizmoPlugin::AkEvent3DGizmoPlugin()
{
	create_icon_material("ak_event_3d_icon",
			EditorInterface::get_singleton()->get_editor_theme()->get_icon(
					SNAME("Gizmo3DSamplePlayer"), SNAME("EditorIcons")));
	create_material("ak_event_3d_material_billboard", Color(1, 1, 1), true, false, true);
}

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

void AkEvent3DGizmoPlugin::_redraw(const Ref<EditorNode3DGizmo>& p_gizmo)
{
	p_gizmo->clear();

	float r = 0;
	AkEvent3D* ak_event = Object::cast_to<AkEvent3D>(p_gizmo->get_node_3d());
	if (ak_event)
	{
		auto event = ak_event->get_event();
		if (event.is_valid())
		{
			auto project_data = WwiseProjectInfo::get_singleton()->get_data();
			if (project_data.is_valid())
			{
				r = project_data->get_event_max_attenuation(event->get_id());
			}
		}
	}

	// note (afama): Based on code from:
	// https://github.com/godotengine/godot/blob/4.4/editor/plugins/gizmos/audio_stream_player_3d_gizmo_plugin.cpp
	// Used to resemble the AudioStreamPlayer3D gizmo.
	// License: godotengine.org/license
	const Ref<Material> lines_billboard_material = get_material("ak_event_3d_material_billboard", p_gizmo);

	PackedVector3Array points_billboard;

	for (int i = 0; i < 120; i++)
	{
		// Create a circle.
		const float ra = Math::deg_to_rad((float)(i * 3));
		const float rb = Math::deg_to_rad((float)((i + 1) * 3));
		const Point2 a = Vector2(Math::sin(ra), Math::cos(ra)) * r;
		const Point2 b = Vector2(Math::sin(rb), Math::cos(rb)) * r;

		// Draw a billboarded circle.
		points_billboard.push_back(Vector3(a.x, a.y, 0));
		points_billboard.push_back(Vector3(b.x, b.y, 0));
	}

	Color color = Color(0.4, 0.8, 1);

	if (r > CMP_EPSILON)
	{
		color.set_h(color.get_h() + 0.5);
	}

	p_gizmo->add_lines(points_billboard, lines_billboard_material, true, color);

	const Ref<Material> icon = get_material("ak_event_3d_icon", p_gizmo);
	p_gizmo->add_unscaled_billboard(icon, 0.05);
}
