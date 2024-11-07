class WwiseObjectReference : public Resource
{
	GDCLASS(WwiseObjectReference, Resource);

protected:
	static void _bind_methods();

private:
	String object_name{};
	uint32_t id{ AK_INVALID_UNIQUE_ID };
	String guid{};

public:
	void set_object_name(const String& p_object_name);
	String get_object_name() const;

	void set_id(uint32_t p_id);
	uint32_t get_id() const;

	void set_guid(const String& p_guid);
	String get_guid() const;

	virtual String get_display_name() {return object_name;}
	virtual WwiseObjectType get_object_type() = 0;

#if defined(TOOLS_ENABLED)
	static HashMap<WwiseObjectType, HashMap<String, Ref<WwiseObjectReference>>> s_objectReferenceDictionary;

	virtual bool is_complete() { return true; }
	virtual void complete_data() {}

	template <typename T> static Ref<WwiseObjectReference> create(WwiseObjectType wwiseObjectType, String guid)
	{
		static_assert(std::is_base_of<WwiseObjectReference, T>::value, "T must be a subclass of WwiseObjectReference");

		Ref<WwiseObjectReference> objectReference = memnew(T);

		// if constexpr (std::is_same<T, WwiseEventReference>::value)
		// {
		// 	Ref<WwiseEventReference> eventRef = objectReference;
		// 	eventRef->update_is_user_defined_sound_bank();
		// }

		objectReference->set_guid(guid);

		if (!s_objectReferenceDictionary.has(wwiseObjectType))
		{
			fetch_assets_of_type(wwiseObjectType);
		}

		s_objectReferenceDictionary[wwiseObjectType][guid] = objectReference;

		return objectReference;
	}

	static Ref<WwiseObjectReference> find_existing_wwise_object(
			WwiseObjectType wwiseObjectType, const String& guid, const String& path)
	{
		Ref<WwiseObjectReference> asset;
		bool exists = ResourceLoader::get_singleton()->exists(path);
		if (exists)
		{
			asset = ResourceLoader::get_singleton()->load(path);
		}

		if (asset.is_valid())
		{
			return asset;
		}

		// Check if we have a factory function for the type
		// if (!m_WwiseObjectReferenceFunctions.has(wwiseObjectType)) {
		//     return Ref<WwiseObjectReference>();
		// }

		// Fetch assets if not done already
		if (!s_objectReferenceDictionary.has(wwiseObjectType))
		{
			fetch_assets_of_type(wwiseObjectType);
		}

		// Check if the object is in the dictionary
		if (s_objectReferenceDictionary[wwiseObjectType].has(guid))
		{
			return s_objectReferenceDictionary[wwiseObjectType][guid];
		}

		return Ref<WwiseObjectReference>();
	}

	static void fetch_assets_of_type(WwiseObjectType wwiseObjectType)
	{
		// Mapping WwiseObjectType to file extension
		String extension = "tres"; // Adjust this based on your needs

		// // Get the type for asset loading
		// Type type;
		// if (!m_WwiseObjectReferenceClasses.has(wwiseObjectType)) {
		//     type = Type::get_type<WwiseObjectReference>();
		// } else {
		//     type = m_WwiseObjectReferenceClasses[wwiseObjectType];
		// }

		// Get all file paths for the specific type
		Array file_paths = get_all_file_paths("res://addons/Wwise/resource", extension);

		// Create a dictionary for the object references
		HashMap<String, Ref<WwiseObjectReference>> objectReferenceDictionary;

		// Load each asset and add it to the dictionary
		for (int i = 0; i < file_paths.size(); ++i)
		{
			String path = file_paths[i];
			Ref<WwiseObjectReference> asset; 
			bool exists = ResourceLoader::get_singleton()->exists(path);

			if (exists)
			{
				asset = ResourceLoader::get_singleton()->load(path);
			}

			if (asset.is_valid())
			{
				objectReferenceDictionary[asset->get_guid()] = asset;
			}
		}

		// Store the dictionary in the global map
		s_objectReferenceDictionary[wwiseObjectType] = objectReferenceDictionary;
	}

	static bool update_wwise_object_data(const Ref<WwiseObjectReference>& wwiseObjectReference, const String& name)
	{
		uint32_t id = ShortIDGenerator::compute(name);
		UtilityFunctions::print(itos(id));
		bool changed = false;
		if (wwiseObjectReference->object_name != name || wwiseObjectReference->id != id)
			changed = true;

		wwiseObjectReference->set_object_name(name);
		wwiseObjectReference->set_id(id);

		if (!wwiseObjectReference->is_complete())
		{
			changed = true;
			wwiseObjectReference->complete_data();
		}
		return changed;
	}

	static String get_parent_path(WwiseObjectType wwiseObjectType)
	{
		return AkEditorSettings::get_wwise_resource_relative_path().path_join(WwiseObjectTypeStr[wwiseObjectType]);
	}

	static String get_asset_file_name(const String& guid) { return guid + String(".tres"); }

		template <typename T> static Ref<WwiseObjectReference> find_or_create_wwise_object(
			WwiseObjectType wwise_object_type, const String& name, const String& guid)
	{
		String parent_path = get_parent_path(wwise_object_type);
		String path = parent_path.path_join(get_asset_file_name(guid));
		auto asset = find_existing_wwise_object(wwise_object_type, guid, path);
		auto assetExists = asset.is_valid();
		if (!assetExists)
		{
			create_folder(parent_path);
			asset = create<T>(wwise_object_type, guid);
		}

		bool changed = update_wwise_object_data(asset, name);
		if (!assetExists)
		{
			ResourceSaver::get_singleton()->save(asset, path);
		}
		else if (changed)
		{
			//todo set dirty
		}

		return asset;
	}

	static Ref<WwiseObjectReference> find_wwise_object(const WwiseObjectType wwise_object_type, const String& guid)
	{
		String parent_path = get_parent_path(wwise_object_type);
		String path = parent_path.path_join(get_asset_file_name(guid));
		return find_existing_wwise_object(wwise_object_type, guid, path);
	}

	static void update_wwise_object(WwiseObjectType wwise_object_type, const String& name, const String& guid)
	{
		String parent_path = get_parent_path(wwise_object_type);
		String path = parent_path.path_join(get_asset_file_name(guid));
		auto asset = find_existing_wwise_object(wwise_object_type, guid, path);
		if (asset.is_valid() && update_wwise_object_data(asset, name))
		{
			//UnityEditor.EditorUtility.SetDirty(asset);
		}
	}

	static void delete_wwise_object(const WwiseObjectType wwise_object_type, const String& guid)
	{
		String parent_path = get_parent_path(wwise_object_type);
		String path = parent_path.path_join(get_asset_file_name(guid));
		DirAccess::remove_absolute(path);
	}
#endif
};