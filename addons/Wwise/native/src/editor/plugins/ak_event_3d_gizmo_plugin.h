#pragma once

#include "editor/editor_utils.h"
#include "editor/wwise_project_data.h"
#include <godot_cpp/classes/editor_node3d_gizmo_plugin.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/theme.hpp>

using namespace godot;

class AkEvent3DGizmoPlugin : public EditorNode3DGizmoPlugin
{
	GDCLASS(AkEvent3DGizmoPlugin, EditorNode3DGizmoPlugin);

protected:
	static void _bind_methods() {}

public:
	AkEvent3DGizmoPlugin();
	virtual bool _has_gizmo(Node3D* for_node_3d) const override;
	virtual String _get_gizmo_name() const override;
	virtual int32_t _get_priority() const override;
	virtual void _redraw(const Ref<EditorNode3DGizmo>& p_gizmo) override;
};