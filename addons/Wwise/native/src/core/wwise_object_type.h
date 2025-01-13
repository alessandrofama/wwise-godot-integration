#pragma once

#include <type_traits>

enum class WwiseObjectType
{
	None,
	AuxBus,
	Bus,
	Event,
	Folder,
	PhysicalFolder,
	Project,
	Soundbank,
	State,
	StateGroup,
	Switch,
	SwitchGroup,
	Workunit,
	GameParameter,
	Trigger,
	AcousticTexture,
	Max
};

bool operator==(WwiseObjectType lhs, int rhs) { return static_cast<int>(lhs) == rhs; }
bool operator==(int lhs, WwiseObjectType rhs) { return lhs == static_cast<int>(rhs); }

const char* const wwise_object_type_str[] = {
	"None",
	"Aux Bus",
	"Bus",
	"Event",
	"Folder",
	"Physical Folder",
	"Project",
	"SoundBank",
	"State",
	"State Group",
	"Switch",
	"Switch Group",
	"WorkUnit",
	"Game Parameter",
	"Trigger",
	"Acoustic Texture",
};

static_assert(std::extent<decltype(wwise_object_type_str)>::value == (int)WwiseObjectType::Max,
		"WwiseGodot: wwise_object_type_str array size does not match the number of WwiseObjectType enum values.");