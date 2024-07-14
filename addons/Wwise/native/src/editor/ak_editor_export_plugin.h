class AkEditorExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkEditorExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

public:
	virtual String _get_name() const override;
	virtual void _export_file(const String& path, const String& type, const PackedStringArray& features) override;
};