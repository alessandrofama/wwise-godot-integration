#pragma once

#include "core/wwise_external_source_info.h"
#include "core/ak_midi_post.h"
#include "core/ak_utils.h"
#include "core/types/wwise_acoustic_texture.h"
#include "core/utils.h"
#include "core/wwise_bank_manager.h"
#include "core/wwise_io_hook.h"
#include "core/wwise_platform_info.h"
#include "core/wwise_settings.h"
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/AkMidiTypes.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkVirtualAcoustics.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <AK/Tools/Common/AkMonitorError.h>
#include <AK/Tools/Common/AkObject.h>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/templates/hash_set.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <memory>

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
	static void event_callback(AkCallbackType callback_type, AkCallbackInfo* callback_info);
	static void bank_callback(AkUInt32 bank_id, const void* in_memory_bank_ptr, AKRESULT load_result, void* in_pCookie);

	void pre_game_object_api_call(Node* p_node, AkGameObjectID p_id);
	void post_register_game_object(AKRESULT p_result, const Node* p_node, AkGameObjectID p_id);
	void post_unregister_game_object(AKRESULT p_result, const Node* p_node, AkGameObjectID p_id);

	bool initialize_wwise_systems();
	bool shutdown_wwise_system();

	static CAkLock callback_data_lock;

	WwiseFileIOHandler low_level_io;
	static HashSet<AkGameObjectID> registered_game_objects;

public:
	Wwise();
	~Wwise();
	static Wwise* get_singleton();

	void init();
	void render_audio();
	void shutdown();

	void set_banks_path(const String& p_banks_path);
	void set_current_language(const String& language);
	bool load_bank(const String& bank_name);
	bool load_bank_id(const unsigned int bank_id);
	bool load_bank_async(const String& bank_name, const Callable& cookie);
	bool load_bank_async_id(const unsigned int bank_id, const Callable& cookie);
	bool unload_bank(const String& bank_name);
	bool unload_bank_id(const unsigned int bank_id);
	bool unload_bank_async(const String& bank_name, const Callable& cookie);
	bool unload_bank_async_id(const unsigned int bank_id, const Callable& cookie);

	bool register_listener(const Node* game_object);
	bool register_game_obj(const Node* game_object, const String& game_object_name);
	bool unregister_game_obj(const Node* game_object);

	bool set_distance_probe(Node* listener_game_object, Node* probe_game_object);
	bool reset_distance_probe(Node* listener_game_object);

	bool set_listeners(Node* emitter, Node* listener);
	void set_random_seed(const unsigned int seed);

	bool set_3d_position(const Node* game_object, const Transform3D& transform_3d);
	bool set_2d_position(const Node* game_object, const Transform2D& transform_2d, const float z_depth);
	bool set_multiple_positions_3d(const Node* game_object, const TypedArray<Transform3D>& positions,
			const int num_positions, const AkUtils::MultiPositionType multi_position_type);
	bool set_multiple_positions_2d(const Node* game_object, const TypedArray<Transform2D>& positions,
			const TypedArray<float>& z_depths, const int num_positions,
			const AkUtils::MultiPositionType multi_position_type);
	bool set_game_object_radius(const Node* game_object, const float outer_radius, const float inner_radius);

	AkPlayingID post_event(const String& event_name, Node* game_object);
	AkPlayingID post_event_callback(
			const String& event_name, const AkUtils::AkCallbackType flags, Node* game_object, const Callable& cookie);
	AkPlayingID post_event_id(uint32_t event_id, Node* game_object);
	AkPlayingID post_event_id_callback(
			uint32_t event_id, AkUtils::AkCallbackType flags, Node* game_object, const Callable& cookie);
	AkPlayingID post_midi_on_event_id(const AkUniqueID p_event_id, Node* p_game_object,
			TypedArray<AkMidiPost> p_midi_posts, bool p_absolute_offsets = false);
	void stop_event(const unsigned int playing_id, const unsigned int fade_time,
			const AkUtils::AkCurveInterpolation interpolation);
	bool stop_midi_on_event_id(const AkUniqueID p_event_id, Node* p_game_object);
	bool execute_action_on_event_id(AkUniqueID p_event_id, AkUtils::AkActionOnEventType p_action_type,
			Node* game_object, int p_transition_duration = 0,
			AkUtils::AkCurveInterpolation p_fade_curve = AkUtils::AkCurveInterpolation::AK_CURVE_LINEAR,
			AkPlayingID p_playing_id = AK_INVALID_PLAYING_ID);
	bool set_switch(const String& switch_group, const String& switch_value, Node* game_object);
	bool set_switch_id(const unsigned int switch_group_id, const unsigned int switch_value_id, Node* game_object);
	bool set_state(const String& state_group, const String& state_value);
	bool set_state_id(const unsigned int state_group_id, const unsigned int state_value_id);

	float get_rtpc_value(const String& rtpc_name, Node* game_object);
	float get_rtpc_value_id(const unsigned int rtpc_id, Node* game_object);
	bool set_rtpc_value(const String& rtpc_name, const float rtpc_value, Node* game_object);
	bool set_rtpc_value_id(const unsigned int rtpc_id, const float rtpc_value, Node* game_object);

	bool post_trigger(const String& trigger_name, Node* game_object);
	bool post_trigger_id(const unsigned int trigger_id, Node* game_object);

	unsigned int post_external_source(const String& event_name, Node* game_object, const String& source_object_name,
			const String& filename, const AkUtils::AkCodecID id_codec);
	unsigned int post_external_source_id(const unsigned int event_id, Node* game_object,
			const unsigned int source_object_id, const String& filename, const AkUtils::AkCodecID id_codec);
	unsigned int post_external_sources(
			const unsigned int event_id, Node* game_object, TypedArray<WwiseExternalSourceInfo> external_source_info);

	int get_source_play_position(const unsigned int playing_id, const bool extrapolate);
	Dictionary get_playing_segment_info(const unsigned int playing_id, const bool extrapolate);

	bool set_game_object_output_bus_volume(Node* game_object, Node* listener, float f_control_value);
	bool set_game_object_aux_send_values(
			Node* game_object, const Array ak_aux_send_values, const unsigned int num_send_values);
	bool set_object_obstruction_and_occlusion(
			Node* game_object, Node* listener, float f_calculated_obs, float f_calculated_occ);

	bool set_geometry(const Array vertices, const Array triangles, const Ref<WwiseAcousticTexture>& acoustic_texture,
			float transmission_loss_value, const Node* game_object, bool enable_diffraction,
			bool enable_diffraction_on_boundary_edges);
	bool remove_geometry(const Object* game_object);
	bool set_geometry_instance(
			const Object* associated_geometry, const Transform3D& transform, const Object* geometry_instance);
	bool remove_geometry_instance(const Object* geometry_instance);
	bool register_spatial_listener(Node* game_object);
	bool set_room(const Node* game_object, const unsigned int ak_aux_bus_id, const float reverb_level,
			const float transmission_loss, const Vector3& front_vector, const Vector3& up_vector, bool keep_registered,
			const Object* associated_geometry_instance);
	bool remove_room(const Node* game_object);
	bool set_portal(const Node* game_object, const Transform3D transform, const Vector3& extent, const Node* front_room,
			const Node* back_room, bool enabled);
	bool remove_portal(const Node* game_object);
	bool set_portal_obstruction_and_occlusion(
			const Node* portal, const float obstruction_value, const float occlusion_value);
	bool set_game_object_to_portal_obstruction(
			const Node* game_object, const Node* portal, const float obstruction_value);
	bool set_portal_to_portal_obstruction(const Node* portal0, const Node* portal1, const float obstruction_value);
	bool set_game_object_in_room(Node* game_object, const Node* room);
	bool remove_game_object_from_room(Node* game_object);
	bool set_early_reflections_aux_send(Node* game_object, const unsigned int aux_bus_id);
	bool set_early_reflections_volume(Node* game_object, const float volume);

	bool add_output(const String& share_set, const unsigned int output_id);
	bool remove_output(const unsigned int output_id);
	bool suspend(bool render_anyway);
	bool wakeup_from_suspend();
	void stop_all(Node* game_object = nullptr);
	uint64_t get_sample_tick();
	uint32_t get_id_from_string(const String& p_string);
	bool is_initialized();
};