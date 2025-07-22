@echo off
set SCRIPT_DIR=%~dp0

:: Generate the WAV files
python WwiseProject/GenerateProjectWavFiles.py

:: Generate the SoundBanks
"%WWISEROOT%\Authoring\x64\Release\bin\WwiseConsole.exe" generate-soundbank "%SCRIPT_DIR%WwiseProject/GodotTests.wproj" --root-output-path "%SCRIPT_DIR%GodotProject/GeneratedSoundBanks" --soundbank-path "Base" "%SCRIPT_DIR%GodotProject/GeneratedSoundBanks/Base" --soundbank-path "Windows" "%SCRIPT_DIR%GodotProject/GeneratedSoundBanks/Windows" --output "Base" "%SCRIPT_DIR%GodotProject/GeneratedSoundBanks/Base" --output "Windows" "%SCRIPT_DIR%GodotProject/GeneratedSoundBanks/Windows"

:: Generate GDExtension Editor Library
cd /d "%SCRIPT_DIR%..\addons\Wwise\native\"
scons platform=windows target=editor use_static_cpp=yes wwise_config=profile build_profile="build_profile_editor.json" debug_symbols=yes wwise_sdk="%WWISESDK%"

pause