class AkInspectorEditor;

class AkInspectorTree : public Tree
{
	GDCLASS(AkInspectorTree, Tree);

protected:
	static void _bind_methods();

private:
	TreeItem* root_item{};
	WwiseObjectType ak_type{ WwiseObjectType::AKTYPE_EVENT };
	Ref<Texture2D> icon{};
	AkInspectorEditor* window{};

	Dictionary get_wwise_ids(const WwiseObjectType ak_type);

public:
	LineEdit* search_text{};
	Dictionary user_data{};

	void initialize(const WwiseObjectType item_type, const Dictionary& user_data_ = Dictionary());
	void populate_browser(const String& text_filter);
	void _on_text_changed(const String& text_filter);
	void _on_size_changed();
};

class AkInspectorEditor : public AcceptDialog
{
	GDCLASS(AkInspectorEditor, AcceptDialog);

protected:
	static void _bind_methods(){};

public:
	VBoxContainer* root_vbox{};
	VBoxContainer* search_vbox{};
	LineEdit* search_text{};
	AkInspectorTree* tree{};
	void initialize();
};

class AkInspectorEditorProperty : public EditorProperty
{
	GDCLASS(AkInspectorEditorProperty, EditorProperty);

protected:
	static void _bind_methods();

private:
	Button* property_control{};
	Ref<Texture2D> icon{};
	AkWwiseBaseType* current_value{nullptr};
	bool updating{};
	AkInspectorEditor* window{};
	WwiseObjectType ak_type{ WwiseObjectType::AKTYPE_EVENT };
	PopupMenu* event_popup{};
	Dictionary user_data{};

	void open_popup();
	void close_popup();

public:
	struct AkInspectorEditorPropertyInfo
	{
		const char* text;
		const char* placeholder;
	};

	virtual AkInspectorEditorPropertyInfo get_ak_inspector_property_info()
	{
		return AkInspectorEditorPropertyInfo{ "Default Select Label", "Default Search Label" };

		WwiseObjectType type = AKTYPE_GAME_PARAMETER;
		switch (type)
		{
			case WwiseObjectType::AKTYPE_EVENT:
				return AkInspectorEditorPropertyInfo{ "Select Event...", "Search Events..." };
			case WwiseObjectType::AKTYPE_SOUNDBANK:
				return AkInspectorEditorPropertyInfo{ "Select Bank...", "Search Banks..." };
			case WwiseObjectType::AKTYPE_GAME_PARAMETER:
				return AkInspectorEditorPropertyInfo{ "Select Game Parameter...", "Search Game Parameters..." };
			case WwiseObjectType::AKTYPE_STATE_GROUP:
				return AkInspectorEditorPropertyInfo{ "Select State Group...", "Search State Groups..." };
			case WwiseObjectType::AKTYPE_STATE:
				return AkInspectorEditorPropertyInfo{ "Select State...", "Search States..." };
			case WwiseObjectType::AKTYPE_SWITCH_GROUP:
				return AkInspectorEditorPropertyInfo{ "Select Switch Group...", "Search Switch Groups..." };
			case WwiseObjectType::AKTYPE_SWITCH:
				return AkInspectorEditorPropertyInfo{ "Select Switch...", "Search Switches..." };
			case WwiseObjectType::AKTYPE_BUS:
				return AkInspectorEditorPropertyInfo{ "Select Bus...", "Search Busses..." };
			case WwiseObjectType::AKTYPE_AUX_BUS:
				return AkInspectorEditorPropertyInfo{ "Select Aux Bus...", "Search Aux Busses..." };
			default:
				return AkInspectorEditorPropertyInfo{ "Default Select Label", "Default Search Label" };
		}
	}

	void init(const WwiseObjectType type, const Dictionary& user_data_ = Dictionary());
	virtual void _update_property() override;
	void reset();
	void _on_button_pressed();
	void _on_item_selected();
};

class AkWwiseEventEditorProperty : public AkInspectorEditorProperty
{
	GDCLASS(AkWwiseEventEditorProperty, AkInspectorEditorProperty);

	protected:
	static void _bind_methods() {};

	public:
	virtual AkInspectorEditorPropertyInfo get_ak_inspector_property_info() override 
	{
		return AkInspectorEditorPropertyInfo{ "Select Event...", "Search Events..." };
	}

};

class AkInspectorEditorInspectorPlugin : public EditorInspectorPlugin
{
	GDCLASS(AkInspectorEditorInspectorPlugin, EditorInspectorPlugin);

protected:
	static void _bind_methods(){};

public:
	virtual bool _can_handle(Object* object) const override;
	virtual bool _parse_property(Object* object, Variant::Type type, const String& name, PropertyHint hint_type,
			const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide) override;
};