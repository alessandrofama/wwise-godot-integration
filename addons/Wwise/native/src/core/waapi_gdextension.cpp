#include "waapi_gdextension.h"

Waapi* Waapi::singleton = nullptr;

struct PendingCallback
{
	uint64_t subscription_id;
	std::string json_string;

	PendingCallback(uint64_t sub_id, const std::string& json) : subscription_id(sub_id) { json_string.append(json); }
};

CAkLock g_pendingCallbacksLock;
std::vector<PendingCallback*> g_pendingCallbacks;

void WampEventCallback(const uint64_t& subscription_id, const AK::WwiseAuthoringAPI::JsonProvider& rJson)
{
	AkAutoLock<CAkLock> scoped_lock(g_pendingCallbacksLock);
	g_pendingCallbacks.push_back(new PendingCallback(subscription_id, rJson.GetJsonString()));
}

Waapi::Waapi()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

Waapi::~Waapi()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

Waapi* Waapi::get_singleton() { return singleton; }

void Waapi::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("connect_client", "uri", "port"), &Waapi::connect_client);
	ClassDB::bind_method(D_METHOD("is_client_connected"), &Waapi::is_client_connected);
	ClassDB::bind_method(D_METHOD("disconnect_client"), &Waapi::disconnect_client);
	ClassDB::bind_method(D_METHOD("subscribe", "uri", "options"), &Waapi::subscribe);
	ClassDB::bind_method(
			D_METHOD("subscribe_with_timeout", "uri", "options", "timeout_ms"), &Waapi::subscribe_with_timeout);
	ClassDB::bind_method(D_METHOD("unsubscribe", "subscription_id"), &Waapi::unsubscribe);
	ClassDB::bind_method(
			D_METHOD("unsubscribe_with_timeout", "subscription_id", "timeout_ms"), &Waapi::unsubscribe_with_timeout);
	ClassDB::bind_method(D_METHOD("client_call", "uri", "args", "options"), &Waapi::client_call);
	ClassDB::bind_method(D_METHOD("client_call_with_timeout", "uri", "args", "options", "timeout_ms"),
			&Waapi::client_call_with_timeout);
	ClassDB::bind_method(D_METHOD("get_last_string"), &Waapi::get_last_string);
	ClassDB::bind_method(D_METHOD("process_callbacks"), &Waapi::process_callbacks);

	ADD_SIGNAL(MethodInfo("wamp_event", PropertyInfo(Variant::DICTIONARY, "data")));
}

bool Waapi::connect_client(const String& uri, const unsigned int port)
{
	AKASSERT(!uri.is_empty());

	return client.Connect(uri.utf8().get_data(), port);
}

bool Waapi::is_client_connected() { return client.IsConnected(); }

void Waapi::disconnect_client() { client.Disconnect(); }

Dictionary Waapi::subscribe(const String& uri, const String options)
{
	AKASSERT(!uri.is_empty());
	AKASSERT(!options.is_empty());

	uint64_t subscription_id = 0;
	Dictionary result;

	bool call_result = client.Subscribe(
			uri.utf8().get_data(), options.utf8().get_data(), &WampEventCallback, subscription_id, last_string, -1);

	result["subscription_id"] = subscription_id;
	result["subscribe_result"] = call_result;
	result["result_string"] = String(last_string.c_str());

	return result;
}

Dictionary Waapi::subscribe_with_timeout(const String& uri, const String options, const int timeout_ms)
{
	AKASSERT(!uri.is_empty());
	AKASSERT(!options.is_empty());
	AKASSERT(timeout_ms > 0);

	uint64_t subscription_id = 0;
	Dictionary result;

	bool call_result = client.Subscribe(uri.utf8().get_data(), options.utf8().get_data(), &WampEventCallback,
			subscription_id, last_string, timeout_ms);

	result["subscription_id"] = subscription_id;
	result["subscribe_result"] = call_result;
	result["result_string"] = String(last_string.c_str());

	return result;
}

Dictionary Waapi::unsubscribe(const uint64_t subscription_id)
{
	Dictionary result;

	bool call_result = client.Unsubscribe(subscription_id, last_string);

	result["unsubscribe_result"] = call_result;
	result["result_string"] = String(last_string.c_str());

	return result;
}

Dictionary Waapi::unsubscribe_with_timeout(const uint64_t subscription_id, const int timeout_ms)
{
	AKASSERT(timeout_ms > 0);

	Dictionary result;

	bool call_result = client.Unsubscribe(subscription_id, last_string, timeout_ms);

	result["unsubscribe_result"] = call_result;
	result["result_string"] = String(last_string.c_str());

	return result;
}

Dictionary Waapi::client_call(const String& uri, const String args, const String options)
{
	AKASSERT(!uri.is_empty());
	AKASSERT(!args.is_empty());
	AKASSERT(!options.is_empty());

	Dictionary result;

	bool call_result =
			client.Call(uri.utf8().get_data(), args.utf8().get_data(), options.utf8().get_data(), last_string);

	result["call_result"] = call_result;
	result["result_string"] = String(last_string.c_str());

	return result;
}

Dictionary Waapi::client_call_with_timeout(
		const String uri, const String args, const String options, const int timeout_ms)
{
	AKASSERT(!uri.is_empty());
	AKASSERT(!args.is_empty());
	AKASSERT(!options.is_empty());
	AKASSERT(timeout_ms > 0);

	Dictionary result;

	bool call_result = client.Call(
			uri.utf8().get_data(), args.utf8().get_data(), options.utf8().get_data(), last_string, timeout_ms);

	result["call_result"] = call_result;
	result["result_string"] = String(last_string.c_str());

	return result;
}

String Waapi::get_last_string() { return String(last_string.c_str()); }

void Waapi::process_callbacks()
{
	if (!g_pendingCallbacks.empty())
	{
		std::vector<PendingCallback*> callbacks_copy;
		{
			AkAutoLock<CAkLock> scoped_lock(g_pendingCallbacksLock);
			callbacks_copy = g_pendingCallbacks;
			g_pendingCallbacks.clear();
		}

		for (auto pending_callback : callbacks_copy)
		{
			Dictionary data;

			data["subscription_id"] = pending_callback->subscription_id;
			data["json_string"] = pending_callback->json_string.c_str();

			emit_signal("wamp_event", data);

			delete pending_callback;
		}

		callbacks_copy.clear();
	}
}