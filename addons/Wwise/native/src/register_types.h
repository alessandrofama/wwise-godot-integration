#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "ak_utils.h"
#include "editor/ak_editor_utils.h"
#include "editor/event_gizmo/ak_event_3d_gizmo_plugin.h"
#include "editor/export_plugin/ak_android_export_plugin.h"
#include "editor/export_plugin/ak_editor_export_plugin.h"
#include "editor/inspector_plugin/ak_inspector_plugin.h"
#include "scene/ak_bank.h"
#include "scene/ak_early_reflections.h"
#include "scene/ak_environment.h"
#include "scene/ak_environment_data.h"
#include "scene/ak_event.h"
#include "scene/ak_geometry.h"
#include "scene/ak_listener.h"
#include "scene/ak_portal.h"
#include "scene/ak_room.h"
#include "scene/ak_state.h"
#include "scene/ak_switch.h"
#include "waapi/waapi_gdextension.h"
#include "wwise_editor.h"
#include "wwise_gdextension.h"
#include "wwise_settings.h"

#if defined(AK_WIN) || defined(AK_MAC_OS_X)
#include "editor/waapi_picker/waapi_picker.h"
#endif

using namespace godot;

void register_wwise_types(ModuleInitializationLevel p_level);
void unregister_wwise_types(ModuleInitializationLevel p_level);

#endif // REGISTER_TYPES_H