#ifndef AK_UTILS_H
#define AK_UTILS_H

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot 
{
class AkUtils : public RefCounted
{
	GDCLASS(AkUtils, RefCounted);

  protected:
	static void _bind_methods();

  public:
	enum GameEvent
	{
		GAMEEVENT_NONE,
		GAMEEVENT_ENTER_TREE,
		GAMEEVENT_READY,
		GAMEEVENT_EXIT_TREE
	};

	enum AkType
	{
		AKTYPE_EVENT,
		AKTYPE_BANK,
		AKTYPE_RTPC,
		AKTYPE_STATE_GROUP,
		AKTYPE_STATE,
		AKTYPE_SWITCH_GROUP,
		AKTYPE_SWITCH,
		AKTYPE_BUS,
		AKTYPE_AUX_BUS
	};
};
}

VARIANT_ENUM_CAST(AkUtils::GameEvent);
VARIANT_ENUM_CAST(AkUtils::AkType);

#endif // AK_UTILS_H