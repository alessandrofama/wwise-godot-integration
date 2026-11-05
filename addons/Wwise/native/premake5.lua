newoption {
	trigger = "wwise-sdk",
	value = "PATH",
	description = "Path to the Wwise SDK (overrides the WWISESDK environment variable)",
}

local wwiseSdk = _OPTIONS["wwise-sdk"] or "$(WWISESDK)"
local wwiseSdkInclude = path.join(wwiseSdk, "include")
local wwiseSamples = path.join(wwiseSdk, "samples")

local function sconsCommand(target, wwiseConfig, buildProfile, clean)
    local cleanFlag = clean and "-c " or ""
    return 'cd /d "$(ProjectDir).." && scons ' .. cleanFlag ..
        "platform=windows target=" .. target ..
        " use_static_cpp=yes wwise_config=" .. wwiseConfig ..
        ' debug_symbols=yes optimize=speed_trace wwise_sdk="' .. wwiseSdk .. '"' ..
        ' build_profile="' .. buildProfile .. '"'
end

workspace "wwise-gdextension"
    location (_ACTION or "default")
	startproject "wwise-gdextension"
	architecture "x86_64"
	configurations { "editor", "template_debug", "template_release" }

project "wwise-gdextension"
    location (_ACTION or "default")
	kind "Makefile"
	language "C++"
	characterset "Unicode"
	cppdialect "C++20"
	systemversion "latest"

	-- SCons builds one unity translation unit. Show every source in Solution Explorer,
	-- but leave the included implementation files out of the compiler input list.
	files {
		"src/**.cpp",
		"src/**.h",
	}
	filter { "files:src/**.cpp", "not files:src/wwise_gdextension_main.cpp" }
		buildaction "None"
	filter {}

	defines {
		"TYPED_METHOD_BIND",
		"WIN32",
		"_WIN32",
		"_WINDOWS",
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"UNICODE",
	}
	includedirs {
		".",
		"godot-cpp",
		"godot-cpp/gdextension",
		"godot-cpp/include",
		"godot-cpp/gen/include",
		"src",
		path.join(wwiseSdkInclude, "AK", "SoundEngine", "Common"),
		wwiseSdkInclude,
		path.join(wwiseSamples, "SoundEngine", "Common"),
		path.join(wwiseSamples, "SoundEngine", "Win32"),
	}

	filter "configurations:editor"
		defines {
			"TOOLS_ENABLED",
			"AK_PROFILE",
			"NDEBUG",
			"WWISE_DB_STANDARD_TYPES",
			"TESTING_WWISE_PROJECT_DATABASE",
		}
		includedirs {
			path.join(wwiseSamples, "WwiseProjectDatabase"),
			path.join(wwiseSamples, "WwiseProjectDatabase", "WwiseProjectDatabase"),
			path.join(wwiseSamples, "3rdParty", "subprojects", "rapidjson", "include"),
		}
		targetdir "lib/win64/editor/profile"
		targetname "libwwise.windows.editor.profile"
		targetextension ".dll"
		buildcommands { sconsCommand("editor", "profile", "build_profile_editor.json") }
		rebuildcommands {
			sconsCommand("editor", "profile", "build_profile_editor.json", true),
			sconsCommand("editor", "profile", "build_profile_editor.json"),
		}
		cleancommands { sconsCommand("editor", "profile", "build_profile_editor.json", true) }
		optimize "Speed"
		symbols "On"

	filter "configurations:template_debug"
		defines { "AK_PROFILE", "TEMPLATE_DEBUG", "NDEBUG" }
		targetdir "lib/win64/template_debug/profile"
		targetname "libwwise.windows.template_debug.profile"
		targetextension ".dll"
		buildcommands { sconsCommand("template_debug", "profile", "build_profile_runtime.json") }
		rebuildcommands {
			sconsCommand("template_debug", "profile", "build_profile_runtime.json", true),
			sconsCommand("template_debug", "profile", "build_profile_runtime.json"),
		}
		cleancommands { sconsCommand("template_debug", "profile", "build_profile_runtime.json", true) }
		optimize "Speed"
		symbols "On"

	filter "configurations:template_release"
		defines { "AK_RELEASE", "AK_OPTIMIZED", "TEMPLATE_RELEASE", "NDEBUG" }
		targetdir "lib/win64/template_release/release"
		targetname "libwwise.windows.template_release.release"
		targetextension ".dll"
		buildcommands { sconsCommand("template_release", "release", "build_profile_runtime.json") }
		rebuildcommands {
			sconsCommand("template_release", "release", "build_profile_runtime.json", true),
			sconsCommand("template_release", "release", "build_profile_runtime.json"),
		}
		cleancommands { sconsCommand("template_release", "release", "build_profile_runtime.json", true) }
		optimize "Speed"
		symbols "On"

	filter {}
