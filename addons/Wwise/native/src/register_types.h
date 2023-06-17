#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "wwise_gdnative.h"
#include "wwise_settings.h"


using namespace godot;

void register_wwise_types(ModuleInitializationLevel p_level);
void unregister_wwise_types(ModuleInitializationLevel p_level);

#endif // REGISTER_TYPES_H