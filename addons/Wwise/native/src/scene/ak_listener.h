class AkListener2D : public Node2D
{
	GDCLASS(AkListener2D, Node2D);

protected:
	static void _bind_methods();

public:
	virtual void _enter_tree() override;
	virtual void _process(double p_delta) override;
};

class AkListener3D : public Node3D
{
	GDCLASS(AkListener3D, Node3D);

protected:
	static void _bind_methods();

private:
	bool is_spatial{};

public:
	virtual void _enter_tree() override;
	virtual void _process(double p_delta) override;

	void set_is_spatial(bool is_spatial);
	bool get_is_spatial() const;
};