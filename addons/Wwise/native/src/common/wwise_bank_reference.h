class WwiseBankReference : public WwiseObjectReference
{
	GDCLASS(WwiseBankReference, WwiseObjectReference);

protected:
	static void _bind_methods() {};

public:
	virtual WwiseObjectType get_object_type() override {return AKTYPE_SOUNDBANK;} ;
};