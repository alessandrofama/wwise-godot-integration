#pragma once

#include "editor/wwise_project_database.h"
#include "editor/ak_editor_settings.h"
#include <future>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class AkSoundBankDirectoryWatcherPlugin : public EditorPlugin
{
	GDCLASS(AkSoundBankDirectoryWatcherPlugin, EditorPlugin)

protected:
	static void _bind_methods();

private:
	static constexpr int s_between_checks = 1;
	Timer* timer{ nullptr };
	double last_file_check{};
	bool empty_base_path_error_was_logged{};
	bool missing_json_file_error_was_logged{};
	uint64_t last_soundbank_directory_update{};
	std::future<void> task;
	std::atomic<bool> init_callback_required = false;
	void _on_timer_timeout();

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	void init_project_db(const String& p_root_directory_path, uint64_t p_time, String custom_platform_name);
};