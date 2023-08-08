class_name AK

class EVENTS:

	const MUSIC = 3991942870 
	const PLAY_CHIMES_WITH_MARKER = 3900723121 
	const MUSIC_2D = 1939884427 
	const FOOTSTEPS = 2385628198 
	const EXTERNAL_SOURCE_EVENT = 1014047073 
	const SYNTH_ONE_TEST = 2343727715 
	const MUSIC_LONG_ATT = 1272633243 
	const GEOMETRYDEMO = 2626652388 
	const RANDOM_SEED_TEST = 1357555555 
	const CONVOLUTION = 3260090173 
	const SOUNDSEED_WIND = 623594218 
	const SOUNDSEED_GRAIN = 3404211905 
	const SOUNDSEED_IMPACT = 480515238 
	const MOTION_AUX = 2297584640 
	const MOTION_SOURCE = 2209794929 
	const REFLECT = 243379636 

	const _dict = { 
	 "Music": MUSIC,
	 "Play_chimes_with_marker": PLAY_CHIMES_WITH_MARKER,
	 "Music_2D": MUSIC_2D,
	 "Footsteps": FOOTSTEPS,
	 "External_Source_Event": EXTERNAL_SOURCE_EVENT,
	 "Synth_One_Test": SYNTH_ONE_TEST,
	 "Music_Long_Att": MUSIC_LONG_ATT,
	 "GeometryDemo": GEOMETRYDEMO,
	 "Random_Seed_Test": RANDOM_SEED_TEST,
	 "Convolution": CONVOLUTION,
	 "SoundSeed_Wind": SOUNDSEED_WIND,
	 "SoundSeed_Grain": SOUNDSEED_GRAIN,
	 "SoundSeed_Impact": SOUNDSEED_IMPACT,
	 "Motion_Aux": MOTION_AUX,
	 "Motion_Source": MOTION_SOURCE,
	 "Reflect": REFLECT
	} 

class STATES:

	class MUSICSTATE:
		const GROUP = 1021618141 

		class STATE:
			const NONE = 748895195 
			const CALM = 3753286132 
			const INTENSE = 4223512837 

	class EXAMPLESTATE:
		const GROUP = 2727507960 

		class STATE:
			const NONE = 748895195 
			const ONE = 1064933119 
			const TWO = 678209053 

	const _dict = { 
		"MusicState": {
			"GROUP": 1021618141,
			"STATE": {
				"None": 748895195,
				"Calm": 3753286132,
				"Intense": 4223512837,
			} 
		}, 
		"ExampleState": {
			"GROUP": 2727507960,
			"STATE": {
				"None": 748895195,
				"One": 1064933119,
				"Two": 678209053
			} 
		} 
	} 

class SWITCHES:

	class FOOTSTEPSSWITCH:
		const GROUP = 3586861854 

		class SWITCH:
			const GRAVEL = 2185786256 
			const GRASS = 4248645337 
			const WOOD = 2058049674 
			const WATER = 2654748154 

	const _dict = { 
		"FootstepsSwitch": {
			"GROUP": 3586861854,
			"SWITCH": {
				"Gravel": 2185786256,
				"Grass": 4248645337,
				"Wood": 2058049674,
				"Water": 2654748154
			} 
		} 
	} 

class GAME_PARAMETERS:

	const MUSICVOLUME = 2346531308 
	const ENEMIES = 2242381963 

	const _dict = { 
	 "MusicVolume": MUSICVOLUME,
	 "Enemies": ENEMIES
	} 

class TRIGGERS:

	const MUSICTRIGGER = 1927797142 

	const _dict = { 
	 "MusicTrigger": MUSICTRIGGER
	} 

class BANKS:

	const INIT = 1355168291 
	const TESTBANK = 3291379323 

	const _dict = { 
	 "Init": INIT,
	 "TestBank": TESTBANK
	} 

class BUSSES:

	const MASTER_AUDIO_BUS = 3803692087 
	const MUSIC = 3991942870 
	const GEOMETRYBUS = 4209325213 
	const OBJECTBUS = 25206862 

	const _dict = { 
	 "Master Audio Bus": MASTER_AUDIO_BUS,
	 "Music": MUSIC,
	 "GeometryBus": GEOMETRYBUS,
	 "ObjectBus": OBJECTBUS
	} 

class AUX_BUSSES:

	const ROOMVERB = 1572913279 
	const LARGEVERB = 2757439665 
	const SMALLVERB = 117523933 

	const _dict = { 
	 "RoomVerb": ROOMVERB,
	 "LargeVerb": LARGEVERB,
	 "SmallVerb": SMALLVERB
	} 

class AUDIO_DEVICES:

	const NO_OUTPUT = 2317455096 
	const SYSTEM = 3859886410 

	const _dict = { 
	 "No_Output": NO_OUTPUT,
	 "System": SYSTEM
	} 

class EXTERNAL_SOURCES:

	const EXTERNAL_SOURCE = 618371124 

	const _dict = { 
	 "External_Source": EXTERNAL_SOURCE
	} 

