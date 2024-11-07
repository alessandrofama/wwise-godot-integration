enum WwiseObjectType
{
	AKTYPE_NONE,
	AKTYPE_AUX_BUS,
	AKTYPE_BUS,
	AKTYPE_EVENT,
	AKTYPE_FOLDER,
	AKTYPE_PHYSICAL_FOLDER,
	AKTYPE_PROJECT,
	AKTYPE_SOUNDBANK,
	AKTYPE_STATE,
	AKTYPE_STATE_GROUP,
	AKTYPE_SWITCH,
	AKTYPE_SWITCH_GROUP,
	AKTYPE_WORKUNIT,
	AKTYPE_GAME_PARAMETER,
	AKTYPE_TRIGGER,
	AKTYPE_ACOUSTIC_TEXTURE,
	AKTYPE_MAX
};

const char* const WwiseObjectTypeStr[] =
{
    "None",              // AKTYPE_NONE
    "Aux Bus",           // AKTYPE_AUX_BUS
    "Bus",               // AKTYPE_BUS
    "Event",             // AKTYPE_EVENT
    "Folder",            // AKTYPE_FOLDER
    "Physical Folder",   // AKTYPE_PHYSICAL_FOLDER
    "Project",           // AKTYPE_PROJECT
    "SoundBank",         // AKTYPE_SOUNDBANK
    "State",             // AKTYPE_STATE
    "State Group",       // AKTYPE_STATE_GROUP
    "Switch",            // AKTYPE_SWITCH
    "Switch Group",      // AKTYPE_SWITCH_GROUP
    "WorkUnit",          // AKTYPE_WORKUNIT
    "Game Parameter",    // AKTYPE_GAME_PARAMETER
    "Trigger",           // AKTYPE_TRIGGER
    "Acoustic Texture",   // AKTYPE_ACOUSTIC_TEXTURE
};

static_assert(std::extent<decltype(WwiseObjectTypeStr)>::value == AKTYPE_MAX,
              "WwiseObjectTypeStr array size does not match the number of WwiseObjectType enum values.");