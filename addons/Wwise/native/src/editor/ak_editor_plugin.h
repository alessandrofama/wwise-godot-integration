class AkEditorPlugin : public EditorPlugin
{
	GDCLASS(AkEditorPlugin, EditorPlugin);

protected:
	static void _bind_methods() {}

private:
	Ref<AkEvent3DGizmoPlugin> gizmo_plugin;
	Ref<AkEditorExportPlugin> export_plugin;
	Ref<AkAndroidExportPlugin> android_export_plugin;
	Ref<AkInspectorEditorInspectorPlugin> inspector_plugin;

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
};