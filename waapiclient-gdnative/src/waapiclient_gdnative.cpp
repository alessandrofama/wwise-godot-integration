#include "waapiclient_gdnative.h"

using namespace godot;

#if defined(AK_ENABLE_ASSERTS)
void WaapiAssertHook(const char * in_pszExpression, const char * in_pszFileName, int in_lineNumber)
{
	String msg = "AKASSERT: " + String(in_pszExpression);
	Godot::print_warning(msg, "WaapiAssertHook", String(in_pszFileName), in_lineNumber);
	AKPLATFORM::OutputDebugMsg(msg.alloc_c_string());
}

AkAssertHook g_pAssertHook = WaapiAssertHook;
#endif

struct PendingCallback
{
	uint64_t subscriptionID;
	std::string jsonString;

	PendingCallback(uint64_t SubId, const std::string& Json) : subscriptionID(SubId)
	{
		jsonString.append(Json);
	}
};

CAkLock g_pendingCallbacksLock;
std::vector<PendingCallback*> g_pendingCallbacks;

void WampEventCallback(const uint64_t& in_subscriptionId, const AK::WwiseAuthoringAPI::JsonProvider& in_rJson)
{
	AkAutoLock<CAkLock> ScopedLock(g_pendingCallbacksLock);
	g_pendingCallbacks.push_back(new PendingCallback(in_subscriptionId, in_rJson.GetJsonString()));
}

Waapi::~Waapi()
{
}

void Waapi::_register_methods()
{
	register_method("_process", &Waapi::_process);
	register_method("connect", &Waapi::connect);
	register_method("is_connected", &Waapi::isConnected);
	register_method("disconnect", &Waapi::disconnect);
	register_method("subscribe", &Waapi::subscribe);
	register_method("subscribe_with_timeout", &Waapi::subscribeWithTimeout);
	register_method("unsusbscribe", &Waapi::unsubscribe);
	register_method("unsubscribe_with_timeout", &Waapi::unsubscribeWithTimeout);
	register_method("call", &Waapi::call);
	register_method("call_with_timetout", &Waapi::callWithTimeout);
	register_method("get_last_string", &Waapi::getLastString);

	register_signal<Waapi>("wamp_event", "data", GODOT_VARIANT_TYPE_DICTIONARY);
}

void Waapi::_init()
{

}

void Waapi::_process(const float delta)
{
	processCallbacks();
}

bool Waapi::connect(const String uri, const unsigned int port)
{
	AKASSERT(!uri.empty());

	return client.Connect(uri.alloc_c_string(), port);
}

bool Waapi::isConnected()
{
	return client.IsConnected();
}

void Waapi::disconnect()
{
	client.Disconnect();
}

Dictionary Waapi::subscribe(const String uri, const String options)
{
	AKASSERT(!uri.empty());
	AKASSERT(!options.empty());

	uint64_t subscriptionId = 0;
	Dictionary result;

	bool callResult = client.Subscribe(uri.alloc_c_string(), options.alloc_c_string(), &WampEventCallback, subscriptionId, lastString, -1);

	result["subscriptionId"] = subscriptionId;
	result["subscribeResult"] = callResult;
	result["resultString"] = String(lastString.c_str());

	return result;
}

Dictionary Waapi::subscribeWithTimeout(const String uri, const String options, const int timeoutMs)
{
	AKASSERT(!uri.empty());
	AKASSERT(!options.empty());
	AKASSERT(timeoutMs > 0);

	uint64_t subscriptionId = 0;
	Dictionary result;

	bool callResult = client.Subscribe(uri.alloc_c_string(), options.alloc_c_string(), &WampEventCallback, subscriptionId, lastString, timeoutMs);

	result["subscriptionId"] = subscriptionId;
	result["subscribeResult"] = callResult;
	result["resultString"] = String(lastString.c_str());

	return result;
}

Dictionary Waapi::unsubscribe(const uint64_t subscriptionId)
{
	Dictionary result;

	bool callResult = client.Unsubscribe(subscriptionId, lastString);

	result["unsubscribeResult"] = callResult;
	result["resultString"] = String(lastString.c_str());

	return result;
}

Dictionary Waapi::unsubscribeWithTimeout(const uint64_t subscriptionId, const int timeoutMs)
{
	AKASSERT(timeoutMs > 0);

	Dictionary result;

	bool callResult = client.Unsubscribe(subscriptionId, lastString, timeoutMs);

	result["unsubscribeResult"] = callResult;
	result["resultString"] = String(lastString.c_str());

	return result;
}

Dictionary Waapi::call(const String uri, const String args, const String options)
{
	AKASSERT(!uri.empty());
	AKASSERT(!args.empty());
	AKASSERT(!options.empty());

	Dictionary result;

	bool callResult = client.Call(uri.alloc_c_string(), args.alloc_c_string(), options.alloc_c_string(), lastString);

	result["callResult"] = callResult;
	result["resultString"] = String(lastString.c_str());

	return result;
}

Dictionary Waapi::callWithTimeout(const String uri, const String args, const String options, const int timeoutMs)
{
	AKASSERT(!uri.empty());
	AKASSERT(!args.empty());
	AKASSERT(!options.empty());
	AKASSERT(timeoutMs > 0);

	Dictionary result;

	bool callResult = client.Call(uri.alloc_c_string(), args.alloc_c_string(), options.alloc_c_string(), lastString, timeoutMs);

	result["callResult"] = callResult;
	result["resultString"] = String(lastString.c_str());

	return result;
}

String Waapi::getLastString()
{
	return String(lastString.c_str());
}

void Waapi::processCallbacks()
{
	if (!g_pendingCallbacks.empty())
	{
		std::vector<PendingCallback*> callbacksCopy;
		{
			AkAutoLock<CAkLock> ScopedLock(g_pendingCallbacksLock);
			callbacksCopy = g_pendingCallbacks;
			g_pendingCallbacks.clear();
		}

		for (auto pendingCallback : callbacksCopy)
		{
			Dictionary data;

			data["subscriptionID"] = pendingCallback->subscriptionID;
			data["jsonString"] = pendingCallback->jsonString.c_str();

			emit_signal("wamp_event", data);

			delete pendingCallback;
		}

		callbacksCopy.clear();
	}
}
