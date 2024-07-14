class AkCookie : public Object
{
	GDCLASS(AkCookie, Object);

protected:
	static void _bind_methods();

private:
	Callable cookie{};

public:
	AkCookie(){};
	AkCookie(const Callable& cookie);

	void set_cookie(const Callable& cookie);
	Callable get_cookie() const;
};