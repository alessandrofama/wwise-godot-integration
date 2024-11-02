#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>

using namespace godot;

class WwiseCookie : public Object
{
	GDCLASS(WwiseCookie, Object);

protected:
	static void _bind_methods();

private:
	Callable cookie{};

public:
	WwiseCookie(){};
	WwiseCookie(const Callable& cookie);

	void set_cookie(const Callable& cookie);
	Callable get_cookie() const;
};