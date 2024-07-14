class AkState : public Node
{
	GDCLASS(AkState, Node);

protected:
	static void _bind_methods();

private:
	Dictionary state_group;
	Dictionary state_value;
	AkUtils::GameEvent trigger_on = AkUtils::GameEvent::GAMEEVENT_NONE;

public:
	AkState();
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void set_state();

	void set_state_group(const Dictionary& state_group);
	Dictionary get_state_group() const;

	void set_state_value(const Dictionary& state_value);
	Dictionary get_state_value() const;

	void set_trigger_on(AkUtils::GameEvent trigger_on);
	AkUtils::GameEvent get_trigger_on() const;
};