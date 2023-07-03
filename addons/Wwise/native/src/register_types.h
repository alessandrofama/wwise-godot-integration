#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "ak_utils.h"
#include "scene/ak_bank.h"
#include "scene/ak_environment.h"
#include "scene/ak_environment_data.h"
#include "scene/ak_event.h"
#include "scene/ak_geometry.h"
#include "scene/ak_listener.h"
#include "scene/ak_portal.h"
#include "scene/ak_state.h"
#include "scene/ak_switch.h"
#include "waapi/waapi_gdextension.h"
#include "wwise_gdnative.h"
#include "wwise_settings.h"

using namespace godot;

void register_wwise_types(ModuleInitializationLevel p_level);
void unregister_wwise_types(ModuleInitializationLevel p_level);

#endif // REGISTER_TYPES_H