#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class AkMidiPost : public RefCounted
{
	GDCLASS(AkMidiPost, RefCounted);

protected:
	static void _bind_methods()
	{
		ClassDB::bind_method(D_METHOD("set_by_type", "type"), &AkMidiPost::set_by_type);
		ClassDB::bind_method(D_METHOD("get_by_type"), &AkMidiPost::get_by_type);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_type"), "set_by_type", "get_by_type");

		ClassDB::bind_method(D_METHOD("set_by_chan", "chan"), &AkMidiPost::set_by_chan);
		ClassDB::bind_method(D_METHOD("get_by_chan"), &AkMidiPost::get_by_chan);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_chan"), "set_by_chan", "get_by_chan");

		ClassDB::bind_method(D_METHOD("set_by_param1", "param1"), &AkMidiPost::set_by_param1);
		ClassDB::bind_method(D_METHOD("get_by_param1"), &AkMidiPost::get_by_param1);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_param1"), "set_by_param1", "get_by_param1");

		ClassDB::bind_method(D_METHOD("set_by_param2", "param2"), &AkMidiPost::set_by_param2);
		ClassDB::bind_method(D_METHOD("get_by_param2"), &AkMidiPost::get_by_param2);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_param2"), "set_by_param2", "get_by_param2");

		ClassDB::bind_method(D_METHOD("set_by_velocity", "velocity"), &AkMidiPost::set_by_velocity);
		ClassDB::bind_method(D_METHOD("get_by_velocity"), &AkMidiPost::get_by_velocity);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_velocity"), "set_by_velocity", "get_by_velocity");

		ClassDB::bind_method(D_METHOD("set_by_cc", "cc"), &AkMidiPost::set_by_cc);
		ClassDB::bind_method(D_METHOD("get_by_cc"), &AkMidiPost::get_by_cc);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_cc"), "set_by_cc", "get_by_cc");

		ClassDB::bind_method(D_METHOD("set_by_value", "value"), &AkMidiPost::set_by_value);
		ClassDB::bind_method(D_METHOD("get_by_value"), &AkMidiPost::get_by_value);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_value"), "set_by_value", "get_by_value");

		ClassDB::bind_method(D_METHOD("set_by_note", "note"), &AkMidiPost::set_by_note);
		ClassDB::bind_method(D_METHOD("get_by_note"), &AkMidiPost::get_by_note);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_note"), "set_by_note", "get_by_note");

		ClassDB::bind_method(D_METHOD("set_by_value_lsb", "value_lsb"), &AkMidiPost::set_by_value_lsb);
		ClassDB::bind_method(D_METHOD("get_by_value_lsb"), &AkMidiPost::get_by_value_lsb);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_value_lsb"), "set_by_value_lsb", "get_by_value_lsb");

		ClassDB::bind_method(D_METHOD("set_by_value_msb", "value_msb"), &AkMidiPost::set_by_value_msb);
		ClassDB::bind_method(D_METHOD("get_by_value_msb"), &AkMidiPost::get_by_value_msb);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_value_msb"), "set_by_value_msb", "get_by_value_msb");

		ClassDB::bind_method(D_METHOD("set_by_program_num", "program_num"), &AkMidiPost::set_by_program_num);
		ClassDB::bind_method(D_METHOD("get_by_program_num"), &AkMidiPost::get_by_program_num);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "by_program_num"), "set_by_program_num", "get_by_program_num");

		ClassDB::bind_method(D_METHOD("set_u_cmd", "cmd"), &AkMidiPost::set_u_cmd);
		ClassDB::bind_method(D_METHOD("get_u_cmd"), &AkMidiPost::get_u_cmd);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "u_cmd"), "set_u_cmd", "get_u_cmd");

		ClassDB::bind_method(D_METHOD("set_u_arg", "arg"), &AkMidiPost::set_u_arg);
		ClassDB::bind_method(D_METHOD("get_u_arg"), &AkMidiPost::get_u_arg);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "u_arg"), "set_u_arg", "get_u_arg");

		ClassDB::bind_method(D_METHOD("set_u_offset", "offset"), &AkMidiPost::set_u_offset);
		ClassDB::bind_method(D_METHOD("get_u_offset"), &AkMidiPost::get_u_offset);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "u_offset"), "set_u_offset", "get_u_offset");

		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_INVALID);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_NOTE_OFF);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_NOTE_ON);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_NOTE_AFTERTOUCH);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_CONTROLLER);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_PROGRAM_CHANGE);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_PITCH_BEND);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_SYSEX);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_ESCAPE);
		BIND_ENUM_CONSTANT(MIDI_EVENT_TYPE_WWISE_CMD);
	}

public:
	enum MidiEventType
	{
		MIDI_EVENT_TYPE_INVALID = 0x00,
		MIDI_EVENT_TYPE_NOTE_OFF = 0x80,
		MIDI_EVENT_TYPE_NOTE_ON = 0x90,
		MIDI_EVENT_TYPE_NOTE_AFTERTOUCH = 0xA0,
		MIDI_EVENT_TYPE_CONTROLLER = 0xB0,
		MIDI_EVENT_TYPE_PROGRAM_CHANGE = 0xC0,
		MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH = 0xD0,
		MIDI_EVENT_TYPE_PITCH_BEND = 0xE0,
		MIDI_EVENT_TYPE_SYSEX = 0xF0,
		MIDI_EVENT_TYPE_ESCAPE = 0xF7,
		MIDI_EVENT_TYPE_WWISE_CMD = 0xFE,
	};

private:
	uint8_t by_type;
	uint8_t by_chan;
	uint8_t by_param1;
	uint8_t by_param2;
	uint8_t by_velocity;
	uint8_t by_cc;
	uint8_t by_value;
	uint8_t by_note;
	uint8_t by_value_lsb;
	uint8_t by_value_msb;
	uint8_t by_program_num;
	uint16_t u_cmd;
	uint32_t u_arg;
	uint64_t u_offset;

public:
	AkMidiPost() :
			by_type(0),
			by_chan(0),
			by_param1(0),
			by_param2(0),
			by_velocity(0),
			by_cc(0),
			by_value(0),
			by_note(0),
			by_value_lsb(0),
			by_value_msb(0),
			by_program_num(0),
			u_cmd(0),
			u_arg(0),
			u_offset(0)
	{
	}

	// Getters and Setters for each field
	void set_by_type(uint8_t type) { by_type = type; }
	uint8_t get_by_type() const { return by_type; }

	void set_by_chan(uint8_t chan) { by_chan = chan; }
	uint8_t get_by_chan() const { return by_chan; }

	void set_by_param1(uint8_t param) { by_param1 = param; }
	uint8_t get_by_param1() const { return by_param1; }

	void set_by_param2(uint8_t param) { by_param2 = param; }
	uint8_t get_by_param2() const { return by_param2; }

	void set_by_velocity(uint8_t velocity) { by_velocity = velocity; }
	uint8_t get_by_velocity() const { return by_velocity; }

	void set_by_cc(uint8_t cc) { by_cc = cc; }
	uint8_t get_by_cc() const { return by_cc; }

	void set_by_value(uint8_t value) { by_value = value; }
	uint8_t get_by_value() const { return by_value; }

	void set_by_note(uint8_t note) { by_note = note; }
	uint8_t get_by_note() const { return by_note; }

	void set_by_value_lsb(uint8_t lsb) { by_value_lsb = lsb; }
	uint8_t get_by_value_lsb() const { return by_value_lsb; }

	void set_by_value_msb(uint8_t msb) { by_value_msb = msb; }
	uint8_t get_by_value_msb() const { return by_value_msb; }

	void set_by_program_num(uint8_t program_num) { by_program_num = program_num; }
	uint8_t get_by_program_num() const { return by_program_num; }

	void set_u_cmd(uint16_t cmd) { u_cmd = cmd; }
	uint16_t get_u_cmd() const { return u_cmd; }

	void set_u_arg(uint32_t arg) { u_arg = arg; }
	uint32_t get_u_arg() const { return u_arg; }

	void set_u_offset(uint64_t offset) { u_offset = offset; }
	uint64_t get_u_offset() const { return u_offset; }
};

VARIANT_ENUM_CAST(AkMidiPost::MidiEventType);
