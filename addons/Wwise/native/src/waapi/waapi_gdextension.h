#ifndef WAAPI_GDEXTENTION_H
#define WAAPI_GDEXTENTION_H

#include <AK/SoundEngine/Common/AkTypes.h>
#include <godot_cpp/classes/node.hpp>

#include <AK/Tools/Common/AkAutoLock.h>
#include <AK/Tools/Common/AkLock.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/WwiseAuthoringAPI/AkAutobahn/Client.h>
#include <AK/WwiseAuthoringAPI/AkAutobahn/JsonProvider.h>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
class Waapi : public Node
{
	GDCLASS(Waapi, Node);

protected:
	static void _bind_methods();
	static Waapi* singleton;

public:
	Waapi();
	~Waapi();
	static Waapi* get_singleton();

	bool connect_client(const String& uri, const unsigned int port);
	bool is_client_connected();
	void disconnect_client();

	Dictionary subscribe(const String& uri, const String options);
	Dictionary subscribe_with_timeout(const String& uri, const String options, const int timeout_ms);
	Dictionary unsubscribe(const uint64_t subscription_id);
	Dictionary unsubscribe_with_timeout(const uint64_t subscription_id, const int timeout_ms);

	Dictionary client_call(const String& uri, const String args, const String options);
	Dictionary client_call_with_timeout(
			const String uri, const String args, const String options, const int timeout_ms);

	String get_last_string();

	void process_callbacks();

	AK::WwiseAuthoringAPI::Client client;
	std::string last_string;
};
} // namespace godot

#endif