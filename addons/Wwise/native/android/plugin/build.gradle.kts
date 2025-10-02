import com.android.build.gradle.internal.tasks.factory.dependsOn
import org.gradle.api.tasks.Copy
import org.gradle.api.tasks.Delete

plugins {
    id("com.android.library")
    id("org.jetbrains.kotlin.android")
}

val pluginName = "WwiseAndroidPlugin"
val pluginPackageName = "org.godotengine.plugin.android.gdextension.wwise"

val skipPlugins = listOf(
    "SineSource", "ToneSource", "SilenceGenerator", "SineTone", "ToneGen",
    "SilenceSource", "AkAudioInput", "Ak3DAudioBedMixer", "AkCompressor",
    "AkDelay", "AkExpander", "AkFlanger", "AkGain", "AkGuitarDistortion",
    "AkHarmonizer", "AkMatrixReverb", "AkMeter", "AkParametricEQ",
    "AkPeakLimiter", "AkPitchShifter", "AkRecorder", "AkRoomVerb",
    "AkStereoDelay", "AkSynthOne", "AkTimeStretch", "AkTremolo",
    "AkVorbisDecoder", "AkSoundEngine", "WwiseProjectDatabase",
    "CommunicationCentral", "SDL2", "AkSinkFactory", "AkOpusDecoderFactory",
    "AllPluginsFactories", "AllPluginsRegistrationHelpers", "AkFXDurationHandler",
    "AkFXParameterChangeHandler", "AkFXTailHandler", "AkMixerInputMap",
    "AkValueRamp", "AkVectorValueRamp", "AkMotionSinkGameInputHelpers",
    "gme", "GME"
)

val architectures = listOf("arm64-v8a", "armeabi-v7a")
val copyDebugTasks = mutableListOf<TaskProvider<Copy>>()
val copyReleaseTasks = mutableListOf<TaskProvider<Copy>>()

// --- Copy tasks per ABI ---
architectures.forEach { abi ->
    val debugTask = tasks.register<Copy>("copyDebugPluginsFor${abi.replace("-", "").capitalize()}") {
        description = "Copies debug Wwise plugins for $abi"
        val wwisePluginsDir = "${project.findProperty("WWISE_SDK")}/Android_${abi}/Debug/bin"
        from(wwisePluginsDir) {
            exclude(skipPlugins.map { "*$it*.so" })
            exclude("IntegrationDemo/**")
        }
        into("../../lib/android/debug/$abi/DSP")
    }
    copyDebugTasks.add(debugTask)

    val releaseTask = tasks.register<Copy>("copyReleasePluginsFor${abi.replace("-", "").capitalize()}") {
        description = "Copies release Wwise plugins for $abi"
        val wwisePluginsDir = "${project.findProperty("WWISE_SDK")}/Android_${abi}/Release/bin"
        from(wwisePluginsDir) {
            exclude(skipPlugins.map { "*$it*.so" })
            exclude("IntegrationDemo/**")
        }
        into("../../lib/android/release/$abi/DSP")
    }
    copyReleaseTasks.add(releaseTask)
}

// --- Android configuration ---
android {
    namespace = pluginPackageName
    compileSdk = 33

    buildFeatures { buildConfig = true }

    defaultConfig {
        minSdk = 21

        externalNativeBuild {
            cmake {
                cppFlags("")
                arguments(
                    "-DWWISE_SDK_PATH=" + project.findProperty("WWISE_SDK")?.toString().orEmpty(),
                    "-Dprecision=" + project.findProperty("precision")?.toString().orEmpty()
                )
            }
        }

        ndk {
            abiFilters.addAll(architectures)
            ndkVersion = "26.1.10909125"
        }

        manifestPlaceholders["godotPluginName"] = pluginName
        manifestPlaceholders["godotPluginPackageName"] = pluginPackageName
        buildConfigField("String", "GODOT_PLUGIN_NAME", "\"$pluginName\"")
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
    kotlinOptions { jvmTarget = "17" }

    ndkVersion = "23.2.8568313"
    buildToolsVersion = "33.0.2"
}

// --- Dependencies ---
dependencies {
    implementation("org.godotengine:godot:4.3.0.stable")
}

// --- Cleanup tasks ---
val cleanAssetsAddons by tasks.registering(Copy::class) {
    delete("src/main/assets/addons")
}

val cleanDemoAddons by tasks.registering(Delete::class) {
    delete("../../lib/android/")
}

// --- Copy export scripts ---
val copyExportScriptsTemplate by tasks.registering(Copy::class) {
    description = "Copies export scripts templates to plugin assets"
    dependsOn(cleanAssetsAddons)
    from(file("../../wwise.gdextension"))
    into("src/main/assets/addons/Wwise/native")
}

// --- Copy artifacts ---
val copyDebugAARToDemoAddons by tasks.registering(Copy::class) {
    description = "Copies debug AAR to demo"
    from("build/outputs/aar") { include("$pluginName-debug.aar") }
    into("../../lib/android/debug")
}

val copyReleaseAARToDemoAddons by tasks.registering(Copy::class) {
    description = "Copies release AAR to demo"
    from("build/outputs/aar") { include("$pluginName-release.aar") }
    into("../../lib/android/release")
}

val copyDebugSharedLibs by tasks.registering(Copy::class) {
    description = "Copies debug .so shared libraries"
    from("build/intermediates/cmake/debug/obj")
    into("../../lib/android/debug")
}

val copyReleaseSharedLibs by tasks.registering(Copy::class) {
    description = "Copies release .so shared libraries"
    from("build/intermediates/cmake/release/obj")
    into("../../lib/android/release")
}

// --- Combined copy tasks ---
val copyDebugAddonsToDemo by tasks.registering {
    description = "Copies all debug artifacts for all ABIs"
    dependsOn(copyDebugTasks)
    finalizedBy(copyDebugAARToDemoAddons)
    finalizedBy(copyDebugSharedLibs)
}

val copyReleaseAddonsToDemo by tasks.registering {
    description = "Copies all release artifacts for all ABIs"
    dependsOn(copyReleaseTasks)
    finalizedBy(copyReleaseAARToDemoAddons)
    finalizedBy(copyReleaseSharedLibs)
}

// --- Assemble debug/release tasks ---
tasks.register("assembleDebugAll") {
    group = "build"
    description = "Builds debug for all ABIs"
    dependsOn("assembleDebug")
    finalizedBy(copyDebugAddonsToDemo)
}

tasks.register("assembleReleaseAll") {
    group = "build"
    description = "Builds release for all ABIs"
    dependsOn("assembleRelease")
    finalizedBy(copyReleaseAddonsToDemo)
}

// --- Wire up export scripts ---
tasks.named("preBuild").dependsOn(copyExportScriptsTemplate)
tasks.named("assemble").configure {
    dependsOn(copyExportScriptsTemplate)
    // Remove previous copyAddonsToDemo dependency
}

// --- Clean tasks ---
tasks.named<Delete>("clean").apply {
    dependsOn(cleanDemoAddons)
    dependsOn(cleanAssetsAddons)
}
