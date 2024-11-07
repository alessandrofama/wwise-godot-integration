class AkWwiseBaseType : public Object
{
	GDCLASS(AkWwiseBaseType, Object);

protected:
	static void _bind_methods();

private:
	String name{};
	uint32_t id{ AK_INVALID_UNIQUE_ID };

public:
	void set_name(const String& p_name);
	String get_name() const;

	void set_id(uint32_t p_id);
	uint32_t get_id() const;
};