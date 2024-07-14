class AkSwitch : public Node
{
	GDCLASS(AkSwitch, Node);

protected:
	static void _bind_methods();

private:
	NodePath ak_event;
	Dictionary switch_group;
	Dictionary switch_value;
	AkUtils::GameEvent trigger_on = AkUtils::GameEvent::GAMEEVENT_NONE;

public:
	AkSwitch();
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void set_switch();

	void set_ak_event(const NodePath& ak_event);
	NodePath get_ak_event() const;

	void set_switch_group(const Dictionary& switch_group);
	Dictionary get_switch_group() const;

	void set_switch_value(const Dictionary& switch_value);
	Dictionary get_switch_value() const;

	void set_trigger_on(AkUtils::GameEvent trigger_on);
	AkUtils::GameEvent get_trigger_on() const;
};