#ifndef AK_BANK_H
#define AK_BANK_H

#include "wwise_gdextension.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/project_settings.hpp>

namespace godot
{
class AkBank : public Node
{
	GDCLASS(AkBank, Node);

protected:
	static void _bind_methods();

private:
	Dictionary bank;
	AkUtils::GameEvent load_on = AkUtils::GameEvent::GAMEEVENT_NONE;
	AkUtils::GameEvent unload_on = AkUtils::GameEvent::GAMEEVENT_NONE;
	bool use_soundbank_names{ true };

public:
	AkBank();
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void load_bank();
	void unload_bank();

	void set_bank(const Dictionary& bank);
	Dictionary get_bank() const;

	void set_load_on(AkUtils::GameEvent load_on);
	AkUtils::GameEvent get_load_on() const;

	void set_unload_on(AkUtils::GameEvent unload_on);
	AkUtils::GameEvent get_unload_on() const;
};
} //namespace godot

#endif // AK_BANK_H