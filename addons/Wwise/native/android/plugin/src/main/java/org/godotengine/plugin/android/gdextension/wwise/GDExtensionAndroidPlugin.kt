package org.godotengine.plugin.android.gdextension.wwise

import android.app.Activity
import android.util.Log
import android.view.View
import org.godotengine.godot.Godot
import org.godotengine.godot.plugin.GodotPlugin
import org.godotengine.godot.plugin.SignalInfo

class GDExtensionAndroidPlugin(godot: Godot) : GodotPlugin(godot) {

    companion object {
        val TAG = GDExtensionAndroidPlugin::class.java.simpleName

        init {
            try {
                Log.v(TAG, "Loading ${BuildConfig.GODOT_PLUGIN_NAME} library")
                System.loadLibrary(BuildConfig.GODOT_PLUGIN_NAME)
            } catch (e: UnsatisfiedLinkError) {
                Log.e(TAG, "Unable to load ${BuildConfig.GODOT_PLUGIN_NAME} shared library")
            }
        }
    }

    external fun initPlugin(activity: Activity)

    override fun getPluginName() = BuildConfig.GODOT_PLUGIN_NAME

    override fun getPluginGDExtensionLibrariesPaths(): Set<String> {
        return setOf("res://addons/Wwise/native/wwise.gdextension")
    }

    override fun onMainCreate(activity: Activity?): View? {
        if (activity != null) {
            initPlugin(activity)
        }
        return null
    }
}
