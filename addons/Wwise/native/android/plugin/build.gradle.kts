import com.android.build.gradle.internal.tasks.factory.dependsOn
import org.gradle.api.tasks.Copy

plugins {
    id("com.android.library")
    id("org.jetbrains.kotlin.android")
}

val pluginName = "WwiseAndroidPlugin"

val pluginPackageName = "org.godotengine.plugin.android.gdextension.wwise"

val skipPlugins = listOf(
    "SineSource",
    "ToneSource",
    "SilenceGenerator",
    "SineTone",
    "ToneGen",
    "SilenceSource",
    "AkAudioInput",
    "Ak3DAudioBedMixer",
    "AkCompressor",
    "AkDelay",
    "AkExpander",
    "AkFlanger",
    "AkGain",
    "AkGuitarDistortion",
    "AkHarmonizer",
    "AkMatrixReverb",
    "AkMeter",
    "AkParametricEQ",
    "AkPeakLimiter",
    "AkPitchShifter",
    "AkRecorder",
    "AkRoomVerb",
    "AkStereoDelay",
    "AkSynthOne",
    "AkTimeStretch",
    "AkTremolo",
    "AkVorbisDecoder",
    "AkSoundEngine",
    "WwiseProjectDatabase",
    "CommunicationCentral",
    "SDL2",
    "AkSinkFactory",
    "AkOpusDecoderFactory",
    "AllPluginsFactories",
    "AllPluginsRegistrationHelpers",
    "AkFXDurationHandler",
    "AkFXParameterChangeHandler",
    "AkFXTailHandler",
    "AkMixerInputMap",
    "AkValueRamp",
    "AkVectorValueRamp",
    "AkMotionSinkGameInputHelpers",
    "gme",
    "GME"
)

val architectures = listOf("arm64-v8a", "armeabi-v7a")
val copyDebugTasks = mutableListOf<TaskProvider<Copy>>()
val copyReleaseTasks = mutableListOf<TaskProvider<Copy>>()

architectures.forEach { abi ->
    val debugTask = tasks.register<Copy>("copyDebugPluginsFor${abi.replace("-", "").capitalize()}") {
        description = "Copies the debug Wwise plugins for $abi to the plugin's addons directory"
        val wwisePluginsDir = "${project.findProperty("WWISE_SDK")}/Android_${abi}/Debug/bin"
        from(wwisePluginsDir) {
            exclude(skipPlugins.map { "*$it*.so" })
            exclude("IntegrationDemo/**")
        }
        into("../../lib/android/debug/$abi/DSP")
    }
    copyDebugTasks.add(debugTask)

    val releaseTask = tasks.register<Copy>("copyReleasePluginsFor${abi.replace("-", "").capitalize()}") {
        description = "Copies the release Wwise plugins for $abi to the plugin's addons directory"
        val wwisePluginsDir = "${project.findProperty("WWISE_SDK")}/Android_${abi}/Release/bin"
        from(wwisePluginsDir) {
            exclude(skipPlugins.map { "*$it*.so" })
            exclude("IntegrationDemo/**")
        }
        into("../../lib/android/release/$abi/DSP")
    }
    copyReleaseTasks.add(releaseTask)
}

android {
    namespace = pluginPackageName
    compileSdk = 33

    buildFeatures {
        buildConfig = true
    }

    defaultConfig {
        minSdk = 21

        externalNativeBuild {
            cmake {
                cppFlags("")
                arguments(
                "-DWWISE_SDK_PATH="+ project.findProperty("WWISE_SDK")?.toString() ?: "",
                "-Dprecision=" + project.findProperty("precision")?.toString().orEmpty()
                )
                
            }
        }
        ndk {
            abiFilters.add("arm64-v8a")
            abiFilters.add("armeabi-v7a")

            ndkVersion = "26.1.10909125"
        }

        manifestPlaceholders["godotPluginName"] = pluginName
        manifestPlaceholders["godotPluginPackageName"] = pluginPackageName
        buildConfigField("String", "GODOT_PLUGIN_NAME", "\"${pluginName}\"")
        setProperty("archivesBaseName", pluginName)
    }
    externalNativeBuild {
        cmake {
            path("CMakeLists.txt")
            version = "3.22.1"
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = "17"
    }
    ndkVersion = "23.2.8568313"
    buildToolsVersion = "33.0.2"
}

dependencies {
    implementation("org.godotengine:godot:4.3.0.stable")
}

// 1. Define more granular clean tasks for the output directories
val cleanAssetsAddons by tasks.registering(Copy::class) {
    delete("src/main/assets/addons")
}

val cleanDebugDemoLibs by tasks.registering(Delete::class) {
    description = "Deletes the debug libraries from the output directory"
    delete("../../lib/android/debug")
}

val cleanReleaseDemoLibs by tasks.registering(Delete::class) {
    description = "Deletes the release libraries from the output directory"
    delete("../../lib/android/release")
}

val cleanAllDemoLibs by tasks.registering(Delete::class) {
    description = "Deletes all libraries from the output directory"
    delete("../../lib/android/")
}


// 2. Your existing individual copy tasks remain the same
val copyExportScriptsTemplate by tasks.registering(Copy::class) {
    description = "Copies the export scripts templates to the plugin's addons directory"
    dependsOn(cleanAssetsAddons)
    from(file("../../wwise.gdextension"))
    into("src/main/assets/addons/Wwise/native")
}

val copyDebugAARToDemoAddons by tasks.registering(Copy::class) {
    description = "Copies the generated debug AAR binary to the plugin's addons directory"
    from("build/outputs/aar")
    include("$pluginName-debug.aar")
    into("../../lib/android/debug")
}

val copyReleaseAARToDemoAddons by tasks.registering(Copy::class) {
    description = "Copies the generated release AAR binary to the plugin's addons directory"
    from("build/outputs/aar")
    include("$pluginName-release.aar")
    into("../../lib/android/release")
}

val copyDebugSharedLibs by tasks.registering(Copy::class) {
    description = "Copies the generated debug .so shared library to the plugin's addons directory"
    from("build/intermediates/cmake/debug/obj")
    into("../../lib/android/debug")
}

val copyReleaseSharedLibs by tasks.registering(Copy::class) {
    description = "Copies the generated release .so shared library to the plugin's addons directory"
    from("build/intermediates/cmake/release/obj")
    into("../../lib/android/release")
}


// 3. Create separate "package" tasks to group debug and release actions
val packageDebugAddons by tasks.registering {
    description = "Gathers all debug artifacts and copies them to the output directory"
    group = "Distribution"
    dependsOn(cleanDebugDemoLibs)
    dependsOn(copyDebugTasks) // This is your list of tasks from the forEach loop
    dependsOn(copyDebugAARToDemoAddons)
    dependsOn(copyDebugSharedLibs)
}

val packageReleaseAddons by tasks.registering {
    description = "Gathers all release artifacts and copies them to the output directory"
    group = "Distribution"
    dependsOn(cleanReleaseDemoLibs)
    dependsOn(copyReleaseTasks) // This is your list of tasks from the forEach loop
    dependsOn(copyReleaseAARToDemoAddons)
    dependsOn(copyReleaseSharedLibs)
}


// 4. Hook the package tasks into the specific Android assemble tasks
tasks.named("preBuild").dependsOn(copyExportScriptsTemplate)

tasks.named("assembleDebug").configure {
    finalizedBy(packageDebugAddons)
}

tasks.named("assembleRelease").configure {
    finalizedBy(packageReleaseAddons)
}


// 5. Update the main clean task to use the new comprehensive clean task
tasks.named<Delete>("clean").apply {
    dependsOn(cleanAllDemoLibs)
    dependsOn(cleanAssetsAddons)
}