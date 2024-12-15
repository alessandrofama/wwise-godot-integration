#pragma once

#include "core/ak_utils.h"
#include "core/utils.h"
#include "core/wwise_cookie.h"
#include "core/wwise_io_hook.h"
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkVirtualAcoustics.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <AK/Tools/Common/AkMonitorError.h>
#include <AK/Tools/Common/AkObject.h>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

using namespace godot;

class Wwise : public Object
{
	GDCLASS(Wwise, Object);

protected:
	static Wwise* singleton;
	static void _bind_methods();

private:
	String GODOT_WINDOWS_SETTING_POSTFIX = ".Windows";
	String GODOT_MAC_OSX_SETTING_POSTFIX = ".OSX";
	String GODOT_IOS_SETTING_POSTFIX = ".iOS";
	String GODOT_ANDROID_SETTING_POSTFIX = ".Android";
	String GODOT_LINUX_SETTING_POSTFIX = ".Linux";

	String WWISE_COMMON_USER_SETTINGS_PATH = "wwise/common_user_settings/";
	String WWISE_COMMON_ADVANCED_SETTINGS_PATH = "wwise/common_advanced_settings/";
	String WWISE_SPATIAL_AUDIO_PATH = "spatial_audio/";
	String WWISE_COMMUNICATION_SETTINGS_PATH = "wwise/communication_settings/";

	static void event_callback(AkCallbackType callback_type, AkCallbackInfo* callback_info);
	static void bank_callback(AkUInt32 bank_id, const void* in_memory_bank_ptr, AKRESULT load_result, void* in_pCookie);

	Variant get_platform_project_setting(const String& setting);

	bool initialize_wwise_systems();
	bool shutdown_wwise_system();

	static CAkLock callback_data_lock;

	ProjectSettings* project_settings;
	WwiseFileIOHandler low_level_io;

public:
	Wwise();
	~Wwise();
	static Wwise* get_singleton();

	void init();
	void render_audio();
	void shutdown();

	bool set_base_path(const String& base_path);
	void set_current_language(const String& language);
	bool load_bank(const String& bank_name);
	bool load_bank_id(const unsigned int bank_id);
	bool load_bank_async(const String& bank_name, const WwiseCookie* cookie);
	bool load_bank_async_id(const unsigned int bank_id, const WwiseCookie* cookie);
	bool unload_bank(const String& bank_name);
	bool unload_bank_id(const unsigned int bank_id);
	bool unload_bank_async(const String& bank_name, const WwiseCookie* cookie);
	bool unload_bank_async_id(const unsigned int bank_id, const WwiseCookie* cookie);

	bool register_listener(const Object* game_object);
	bool register_game_obj(const Object* game_object, const String& game_object_name);
	bool unregister_game_obj(const Object* game_object);

	bool set_distance_probe(const Object* listener_game_object, const Object* probe_game_object);
	bool reset_distance_probe(const Object* listener_game_object);

	bool set_listeners(const Object* emitter, const Object* listener);
	void set_random_seed(const unsigned int seed);

	bool set_3d_position(const Object* game_object, const Transform3D& transform_3d);
	bool set_2d_position(const Object* game_object, const Transform2D& transform_2d, const float z_depth);
	bool set_multiple_positions_3d(const Object* game_object, const TypedArray<Transform3D>& positions,
			const int num_positions, const AkUtils::MultiPositionType multi_position_type);
	bool set_multiple_positions_2d(const Object* game_object, const TypedArray<Transform2D>& positions,
			const TypedArray<float>& z_depths, const int num_positions,
			const AkUtils::MultiPositionType multi_position_type);
	bool set_game_object_radius(const Object* game_object, const float outer_radius, const float inner_radius);

	unsigned int post_event(const String& event_name, const Object* game_object);
	unsigned int post_event_callback(const String& event_name, const AkUtils::AkCallbackType flags,
			const Object* game_object, const WwiseCookie* cookie);
	unsigned int post_event_id(const unsigned int event_id, const Object* game_object);
	unsigned int post_event_id_callback(const unsigned int event_id, const AkUtils::AkCallbackType flags,
			const Object* game_object, const WwiseCookie* cookie);
	void stop_event(const unsigned int playing_id, const unsigned int fade_time,
			const AkUtils::AkCurveInterpolation interpolation);

	bool set_switch(const String& switch_group, const String& switch_value, const Object* game_object);
	bool set_switch_id(
			const unsigned int switch_group_id, const unsigned int switch_value_id, const Object* game_object);
	bool set_state(const String& state_group, const String& state_value);
	bool set_state_id(const unsigned int state_group_id, const unsigned int state_value_id);

	float get_rtpc_value(const String& rtpc_name, const Object* game_object);
	float get_rtpc_value_id(const unsigned int rtpc_id, const Object* game_object);
	bool set_rtpc_value(const String& rtpc_name, const float rtpc_value, const Object* game_object);
	bool set_rtpc_value_id(const unsigned int rtpc_id, const float rtpc_value, const Object* game_object);

	bool post_trigger(const String& trigger_name, const Object* game_object);
	bool post_trigger_id(const unsigned int trigger_id, const Object* game_object);

	unsigned int post_external_source(const String& event_name, const Object* game_object,
			const String& source_object_name, const String& filename, const AkUtils::AkCodecID id_codec);
	unsigned int post_external_source_id(const unsigned int event_id, const Object* game_object,
			const unsigned int source_object_id, const String& filename, const AkUtils::AkCodecID id_codec);

	int get_source_play_position(const unsigned int playing_id, const bool extrapolate);
	Dictionary get_playing_segment_info(const unsigned int playing_id, const bool extrapolate);

	bool set_game_object_output_bus_volume(const Object* game_object, const Object* listener, float f_control_value);
	bool set_game_object_aux_send_values(
			const Object* game_object, const Array ak_aux_send_values, const unsigned int num_send_values);
	bool set_object_obstruction_and_occlusion(
			const Object* game_object, const Object* listener, float f_calculated_obs, float f_calculated_occ);

	bool set_geometry(const Array vertices, const Array triangles, const Dictionary& acoustic_texture,
			float transmission_loss_value, const Object* game_object, bool enable_diffraction,
			bool enable_diffraction_on_boundary_edges);
	bool remove_geometry(const Object* game_object);
	bool set_geometry_instance(
			const Object* associated_geometry, const Transform3D& transform, const Object* geometry_instance);
	bool remove_geometry_instance(const Object* geometry_instance);
	bool register_spatial_listener(const Object* game_object);
	bool set_room(const Object* game_object, const unsigned int ak_aux_bus_id, const float reverb_level,
			const float transmission_loss, const Vector3& front_vector, const Vector3& up_vector, bool keep_registered,
			const Object* associated_geometry_instance);
	bool remove_room(const Object* game_object);
	bool set_portal(const Object* game_object, const Transform3D transform, const Vector3& extent,
			const Object* front_room, const Object* back_room, bool enabled);
	bool remove_portal(const Object* game_object);
	bool set_portal_obstruction_and_occlusion(
			const Object* portal, const float obstruction_value, const float occlusion_value);
	bool set_game_object_to_portal_obstruction(
			const Object* game_object, const Object* portal, const float obstruction_value);
	bool set_portal_to_portal_obstruction(const Object* portal0, const Object* portal1, const float obstruction_value);
	bool set_game_object_in_room(const Object* game_object, const Object* room);
	bool remove_game_object_from_room(const Object* game_object);
	bool set_early_reflections_aux_send(const Object* game_object, const unsigned int aux_bus_id);
	bool set_early_reflections_volume(const Object* game_object, const float volume);

	bool add_output(const String& share_set, const unsigned int output_id);
	bool remove_output(const unsigned int output_id);
	bool suspend(bool render_anyway);
	bool wakeup_from_suspend();
	bool is_initialized();
};