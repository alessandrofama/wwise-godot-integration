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

        val abi = project.findProperty("abi") as? String

        externalNativeBuild {
            cmake {
                cppFlags("")
                arguments.add("-DWWISE_SDK_PATH=${project.findProperty("WWISE_SDK")}")
                if (abi != null) {
                    arguments.add("-DANDROID_ABI=$abi")
                }
            }
        }
        ndk {
            abiFilters.clear()
            if (abi != null) {
                abiFilters.add(abi)
            } else {
                abiFilters.addAll(listOf("arm64-v8a", "armeabi-v7a"))
            }

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

val cleanAssetsAddons by tasks.registering(Copy::class) {
    delete("src/main/assets/addons")
}

val cleanDemoAddons by tasks.registering(Delete::class) {
    delete("../../lib/android/")
}

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

val copyAddonsToDemo by tasks.registering(Copy::class) {
    description = "Copies the plugin's output artifact to the output directory"
    dependsOn(cleanDemoAddons)
    dependsOn(copyDebugTasks)
    dependsOn(copyReleaseTasks)
    finalizedBy(copyDebugAARToDemoAddons)
    finalizedBy(copyReleaseAARToDemoAddons)
    finalizedBy(copyDebugSharedLibs)
    finalizedBy(copyReleaseSharedLibs)
}

tasks.named("preBuild").dependsOn(copyExportScriptsTemplate)

tasks.named("assemble").configure {
    dependsOn(copyExportScriptsTemplate)
    finalizedBy(copyAddonsToDemo)
}

tasks.named<Delete>("clean").apply {
    dependsOn(cleanDemoAddons)
    dependsOn(cleanAssetsAddons)
}