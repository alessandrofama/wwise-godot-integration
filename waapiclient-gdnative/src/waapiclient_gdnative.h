#ifndef WAAPI_GDNATIVE_H
#define WAAPI_GDNATIVE_H

#include <AK/SoundEngine/Common/AkTypes.h>
#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <Node.hpp>

#include <AK/WwiseAuthoringAPI/AkAutobahn/Client.h>
#include <AK/WwiseAuthoringAPI/AkAutobahn/JsonProvider.h>
#include <AK/Tools/Common/AkLock.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

namespace godot
{
	class Waapi : public Node
	{
		GODOT_CLASS(Waapi, Node)

	public:
		explicit Waapi() = default;
		~Waapi();

		static void _register_methods();
		void _init();
		void _process(const float delta);

		bool connectClient(const String uri, const unsigned int port);
		bool isClientConnected();
		void disconnectClient();

		Dictionary subscribe(const String uri, const String options);
		Dictionary subscribeWithTimeout(const String uri, const String options, const int timeoutMs);
		Dictionary unsubscribe(const uint64_t subscriptionId);
		Dictionary unsubscribeWithTimeout(const uint64_t subscriptionId, const int timeoutMs);

		Dictionary clientCall(const String uri, const String args, const String options);
		Dictionary clientCallWithTimeout(const String uri, const String args, const String options, const int timeoutMs);

		String getLastString();

	private:
		void processCallbacks();

		AK::WwiseAuthoringAPI::Client client;
		std::string lastString;
	};
}

#endif // of WAAPI_GDNATIVE_H