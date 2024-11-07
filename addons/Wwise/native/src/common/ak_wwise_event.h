class AkWwiseEvent : public AkWwiseBaseType
{
	GDCLASS(AkWwiseEvent, AkWwiseBaseType);

protected:
	static void _bind_methods() { ClassDB::bind_static_method("AkWwiseEvent", D_METHOD("init", "name", "id"), &AkWwiseEvent::init); };

public:
	static AkWwiseEvent* init(const String& p_name, uint32_t p_id)
	{
		AkWwiseEvent* ref = memnew(AkWwiseEvent);
		ref->set_name(p_name);
		ref->set_id(p_id);

        UtilityFunctions::print("called");
		return ref;
	}
};