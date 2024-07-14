struct WaapiPickerIconData
{
	Ref<Texture2D> project;
	Ref<Texture2D> folder;
	Ref<Texture2D> event;
	Ref<Texture2D> switch_group;
	Ref<Texture2D> switch_;
	Ref<Texture2D> state_group;
	Ref<Texture2D> state;
	Ref<Texture2D> soundbank;
	Ref<Texture2D> bus;
	Ref<Texture2D> aux_bus;
	Ref<Texture2D> acoustic_texture;
	Ref<Texture2D> work_unit;
};

struct WaapiPickerSceneData
{
	Control* control;
	VBoxContainer* parent_vbox_container;
	Button* refresh_project_button;
	Button* export_soundbanks_button;
	Button* generate_ids_button;
	LineEdit* search_text;
	Tree* objects_tree;
};

struct WaapiPickerData
{
	WaapiPickerIconData icon_data;
	WaapiPickerSceneData scene_data;
};

class AkWaapiPicker : public EditorPlugin
{
	GDCLASS(AkWaapiPicker, EditorPlugin);

protected:
	static void _bind_methods();

private:
	WaapiPickerData picker_data{};
	Viewport* editor_viewport{};
	JSON* json{};
	Error parse_result{ Error::FAILED };
	bool is_showing_viewport{};
	TreeItem* selected_item{};
	Ref<PackedScene> scene{};
	String text_filter{};
	FileDialog* file_dialog{};
	String final_text{};

	void create_project_objects_tree();
	void generate_ids(const Array& data);
	void create_class(const Array& data, const String& type);
	void create_state_switch_class(
			const Array& parent_array, const Array& child_array, const String& parent_type, const String& child_type);
	void create_empty_class(const String& type);

public:
	virtual void _enter_tree() override;
	virtual void _process(double delta) override;
	virtual void _exit_tree() override;

	void _on_resized_editor_viewport();
	void _on_visibility_changed_editor_viewport();
	void _on_refresh_project_button_up();
	void _on_export_soundbanks_button_up();
	void _on_generate_ids_button_up();
	void _on_file_dialog_file_selected(const String& path);
	void _on_search_text_changed(const String& text_filter);
};