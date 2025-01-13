#include "ak_soundbank_directory_watcher_plugin.h"

void AkSoundBankDirectoryWatcherPlugin::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_timer_timeout"), &AkSoundBankDirectoryWatcherPlugin::_on_timer_timeout);
}

void AkSoundBankDirectoryWatcherPlugin::_on_timer_timeout()
{
	WwiseSettings* settings = WwiseSettings::get_singleton();

	if (init_callback_required.load())
	{
		init_callback_required.store(false);
		WwiseProjectDatabase::get_singleton()->post_init_callback();
	}

	String root_output_path = settings->get_setting(settings->common_user_settings.root_output_path);
	if (root_output_path.is_empty())
	{
		if (!empty_base_path_error_was_logged)
		{
			UtilityFunctions::push_warning(
					"WwiseGodot: The 'Root Output Path' setting in Wwise's Common User Settings (found under Godot Project > "
					"Project Settings...) is empty. SoundBanks must be generated inside your Godot project "
					"folder. Please generate your SoundBanks within the Godot project and update the 'Root Output Path' setting "
					"in your project settings to match the SoundBanks location in order to complete the setup "
					"process.");
			empty_base_path_error_was_logged = true;
		}
		return;
	}

	String project_json_path = root_output_path.path_join(AkEditorSettings::get_wwise_project_info_file_name());

	bool file_exists = FileAccess::file_exists(project_json_path);
	if (!file_exists)
	{
		if (!missing_json_file_error_was_logged)
		{
			UtilityFunctions::printerr(vformat(
					"WwiseGodot: ProjectInfo.json not found at %s. Ensure the 'Generate JSON Metadata' option is "
					"enabled in the Wwise Project Settings under the SoundBanks tab, then regenerate the SoundBanks.",
					project_json_path));
			missing_json_file_error_was_logged = true;
		}
		return;
	}

	String custom_platform_name = settings->get_setting(settings->project_settings.custom_platform_name);

	if ((Time::get_singleton()->get_unix_time_from_system() - last_file_check) > s_between_checks)
	{
		uint64_t time = FileAccess::get_modified_time(root_output_path);

		task = std::async(std::launch::async,
				[this, root_output_path, time, custom_platform_name]()
				{ this->init_project_db(root_output_path, time, custom_platform_name); });

		last_file_check = Time::get_singleton()->get_unix_time_from_system();
	}
}

void AkSoundBankDirectoryWatcherPlugin::_enter_tree()
{
	timer = memnew(Timer);
	timer->set_wait_time(s_between_checks);
	timer->set_one_shot(false);
	add_child(timer);
	timer->connect("timeout", callable_mp(this, &AkSoundBankDirectoryWatcherPlugin::_on_timer_timeout));
	timer->start();
	_on_timer_timeout();
}

void AkSoundBankDirectoryWatcherPlugin::_exit_tree()
{
	if (task.valid())
	{
		task.wait();
	}

	timer->stop();
	timer->queue_free();
}

void AkSoundBankDirectoryWatcherPlugin::init_project_db(
		const String& p_root_directory_path, uint64_t p_time, String custom_platform_name)
{
	if (p_time > last_soundbank_directory_update)
	{
		last_soundbank_directory_update = p_time;
		WwiseProjectDatabase::get_singleton()->init(
				p_root_directory_path, AkEditorSettings::get_platform_name(custom_platform_name));

		WwiseSettings* settings = WwiseSettings::get_singleton();
		String current_language = settings->get_setting(settings->common_user_settings.startup_language);
		if (current_language.is_empty())
		{
			UtilityFunctions::printerr("WwiseGodot: Startup Language property in the advanced Wwise-Godot Project "
									   "Settings is empty. Defaulting to English(US).");
			current_language = "English(US)";
		}

		WwiseProjectDatabase::get_singleton()->set_current_language(current_language);

		init_callback_required.store(true);
	}
}