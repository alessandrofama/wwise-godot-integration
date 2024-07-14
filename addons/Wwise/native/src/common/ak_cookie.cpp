#include "ak_cookie.h"

void AkCookie::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_cookie", "cookie"), &AkCookie::set_cookie);
	ClassDB::bind_method(D_METHOD("get_cookie"), &AkCookie::get_cookie);

	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "cookie", PROPERTY_HINT_NONE), "set_cookie", "get_cookie");
}

AkCookie::AkCookie(const Callable& cookie) : cookie(cookie) {}

void AkCookie::set_cookie(const Callable& cookie) { this->cookie = cookie; }

Callable AkCookie::get_cookie() const { return cookie; }
