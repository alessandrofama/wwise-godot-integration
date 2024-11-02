#include "wwise_cookie.h"

void WwiseCookie::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_cookie", "cookie"), &WwiseCookie::set_cookie);
	ClassDB::bind_method(D_METHOD("get_cookie"), &WwiseCookie::get_cookie);

	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "cookie", PROPERTY_HINT_NONE), "set_cookie", "get_cookie");
}

WwiseCookie::WwiseCookie(const Callable& cookie) : cookie(cookie) {}

void WwiseCookie::set_cookie(const Callable& cookie) { this->cookie = cookie; }

Callable WwiseCookie::get_cookie() const { return cookie; }
