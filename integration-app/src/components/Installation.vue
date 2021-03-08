<template>
  <div>
    <div v-if="!installing && !installed && !installationFailed">
      <div class="form-group">
        <div>
          <label style="font-size: large">Platform Selection</label>
          <br />
          <div class="custom-control custom-checkbox custom-control-inline">
            <input
              name="platform"
              id="platform_1"
              type="checkbox"
              class="custom-control-input"
              value="Win64"
              v-model="platforms"
            />
            <label for="platform_1" class="custom-control-label">Windows</label>
          </div>
          <div class="custom-control custom-checkbox custom-control-inline">
            <input
              name="platform"
              id="platform_2"
              type="checkbox"
              class="custom-control-input"
              value="macOS"
              v-model="platforms"
            />
            <label for="platform_2" class="custom-control-label">macOS</label>
          </div>
          <div class="custom-control custom-checkbox custom-control-inline">
            <input
              name="platform"
              id="platform_3"
              type="checkbox"
              class="custom-control-input"
              value="Linux"
              v-model="platforms"
            />
            <label for="platform_3" class="custom-control-label">Linux</label>
          </div>
          <div class="custom-control custom-checkbox custom-control-inline">
            <input
              name="platform"
              id="platform_4"
              type="checkbox"
              class="custom-control-input"
              value="Android"
              v-model="platforms"
            />
            <label for="platform_4" class="custom-control-label">Android</label>
          </div>
          <div class="custom-control custom-checkbox custom-control-inline">
            <input
              name="platform"
              id="platform_5"
              type="checkbox"
              class="custom-control-input"
              value="iOS"
              v-model="platforms"
            />
            <label for="platform_5" class="custom-control-label">iOS</label>
          </div>
        </div>
      </div>

      <form @submit.prevent="onSubmit" style="margin-top: 4%">
        <div class="form-group">
          <label for="godotProject" style="font-size: large"
            >Select Godot's project file:</label
          >
          <div class="custom-file" style="margin-top: 1%">
            <input
              type="file"
              class="custom-file-input"
              id="customFile"
              accept=".godot"
              @change="previewFilePath"
            />
            <label class="custom-file-label" for="customFile">
              {{ previewGodotProjectFilePath }}
            </label>
          </div>
          <span
            v-if="isIntegrationInstalled"
            class="form-text"
            style="margin-top: 5%; font-size: large"
          >
            The Wwise Godot Integration is already installed. Please update or
            uninstall the integration.
          </span>
          <span
            v-else
            class="form-text"
            style="margin-top: 5%; font-size: large"
          >
            Installing the Wwise Godot integration will copy the required files
            to your Godot Project.
          </span>
          <span
            class="form-text"
            style="margin-top: 4%; font-size: large; color: red"
            >{{ errorHint }}</span
          >
        </div>

        <div class="from-group b-button-toolbar">
          <b-button-group>
            <button
              v-if="!isIntegrationInstalled"
              name="submit"
              type="submit"
              class="btn btn-primary"
            >
              <span>Install</span>
            </button>
            <button
              v-if="isIntegrationInstalled"
              name="submit"
              type="submit"
              class="btn btn-primary"
            >
              <span>Update</span>
            </button>
          </b-button-group>
          <b-button-group class="mx-1">
            <button
              v-if="isIntegrationInstalled"
              class="btn btn-secondary"
              v-on:click="uninstallIntegration"
            >
              <span>Uninstall</span>
            </button>
          </b-button-group>
        </div>
      </form>
    </div>
    <div v-if="installed">
      <h3>{{ installHeader }} completed successfully</h3>
      <p style="margin-top: 5%; font-size: large">{{ installText }}</p>
      <div class="b-button-toolbar">
        <b-button-group>
          <button v-on:click="openProjectInExplorer" class="btn btn-primary">
            <span>Open Project directory</span>
          </button>
          <button v-on:click="onInit" class="btn btn-primary mx-1">
            <span>Start over</span>
          </button>
        </b-button-group>
      </div>
    </div>
    <div v-if="installing">
      <h3>Installing...</h3>
      <p style="margin-top: 5%">{{ installText }}</p>
      <div class="progressing">
        <transition name="fadeInstall">
          <div v-show="installing">
            <b-progress
              :max="max"
              show-progress
              animated
              :value="progress"
            ></b-progress>
          </div>
        </transition>
      </div>
    </div>
    <div v-if="installationFailed">
      <h3>{{ installHeader }} failed</h3>
      <p style="margin-top: 5%">{{ installText }}</p>
      <button v-on:click="onInit" class="btn btn-primary">
        <span>Start over</span>
      </button>
    </div>
  </div>
</template>

<script>
import { mapActions } from "vuex";
import fs from "fs";
import path from "path";
import { remote } from "electron";

export default {
  data() {
    return {
      previewGodotProjectFilePath: "Select File",
      installing: false,
      installed: false,
      installationFailed: false,
      installHeader: "",
      installText: "",
      errorHint: "",
      progress: 0,
      max: 100,
      gitDownloadDestionationPath: path.join(
        remote.app.getPath("temp"),
        "wwise_gdnative"
      ),
      binariesDestionationPath: path.join(
        remote.app.getPath("temp"),
        "wwise_gdnative/wwise/bin"
      ),
      platforms: [],
    };
  },

  computed: {
    godotProjectFilePath() {
      return this.$store.getters.godotProjectFilePath;
    },
    godotProjectPath() {
      return this.$store.getters.godotProjectPath;
    },
    isIntegrationInstalled() {
      return this.$store.getters.isIntegrationInstalled;
    },
  },

  methods: {
    ...mapActions([
      "setGodotProjectPath",
      "setGodotProjectFilePath",
      "setIntegrationInstalled",
    ]),
    onInit() {
      this.installed = false;
      this.installing = false;
      this.installationFailed = false;
      this.previewGodotProjectFilePath = "Select File";
      this.setIntegrationInstalled(false);
      this.errorHint = "";
    },
    onSubmit() {
      if (path.extname(this.godotProjectFilePath) != ".godot") {
        this.errorHint = "Please select a valid .godot file.";
        return;
      }

      this.installing = true;
      this.installHeader = "Installation";
      this.printProjectIni(this.godotProjectFilePath);
      this.getIntegrationFiles();
    },
    getKeyByValue(object, value) {
      return Object.keys(object).filter((key) => object[key] === value);
    },
    previewFilePath(event) {
      let filePath = event.target.files[0].path;
      filePath = filePath.replace(/\\/g, "/");
      filePath = path.normalize(filePath);
      this.previewGodotProjectFilePath = filePath;

      this.setGodotProjectPaths(filePath);
      if (path.extname(this.godotProjectFilePath) != ".godot") {
        this.errorHint = "Please select a valid .godot file.";
        return;
      } else {
        this.errorHint = "";
      }
    },

    setGodotProjectPaths(godotFilePath) {
      this.setGodotProjectFilePath(godotFilePath);

      let dirPath = path.dirname(godotFilePath);
      this.setGodotProjectPath(dirPath);
      this.checkIntegrationAlreadyInstalled();
    },
    printProjectIni(godotFilePath) {
      const fs = require("fs").promises;
      const multiini = require("multi-ini");

      var globalOutputText;
      var serializedIni;
      var output;
      var globalClassesAlreadyPresent = false;
      var iniObj = multiini.read(godotFilePath, { keep_quotes: false });

      if (!("autoload" in iniObj)) {
        iniObj.autoload = {};
        iniObj.autoload.WwiseSettings = '"res://wwise/wwise_settings.gd"';
        iniObj.autoload.Wwise = '"*res://wwise/bin/wwise-gdnative.gdns"';
        iniObj.autoload.Waapi = '"*res://wwise/bin/waapi-gdnative-debug.gdns"';
      } else {
        if (!("Wwise" in iniObj.autoload)) {
          iniObj.autoload.WwiseSettings = '"res://wwise/wwise_settings.gd"';
          iniObj.autoload.Wwise = '"*res://wwise/bin/wwise-gdnative.gdns"';
          iniObj.autoload.Waapi = '"*res://wwise/bin/waapi-gdnative-debug.gdns"';
        }
      }

      if (!("editor_plugins" in iniObj)) {
        iniObj.editor_plugins = {};
        iniObj.editor_plugins.enabled =
          'PoolStringArray( "wwise_custom_nodes", "wwise_ids_converter", "waapi_picker", "wwise_build_export" )';
      } else {
        if (!("enabled" in iniObj.editor_plugins)) {
          iniObj.editor_plugins.enabled =
            'PoolStringArray( "wwise_custom_nodes", "wwise_ids_converter", "waapi_picker", "wwise_build_export" )';
        } else {
          if (!iniObj.editor_plugins.enabled.includes("wwise")) {
            // Wwise not present in enabled plugins
            var toReplace = iniObj.editor_plugins.enabled.replace(
              "PoolStringArray(",
              'PoolStringArray( "wwise_custom_nodes", "wwise_ids_converter", "waapi_picker", "wwise_build_export", '
            );
            iniObj.editor_plugins.enabled = toReplace;
          }
        }
      }

  for (var item in iniObj) {
     var arr = this.getKeyByValue(iniObj[item], "{")
     if (arr.length > 0)
     {
       delete iniObj[item]
     }
  }
      var serializer = new multiini.Serializer({ keep_quotes: false });
      serializedIni = serializer.serialize(iniObj);
      serializedIni = serializedIni.replace('"Pool', "Pool");
      serializedIni = serializedIni.replace(')"', ")");

      fs.readFile(godotFilePath, "utf-8").then((txt) => {
        if (txt.includes('class": "AK",')) {
          globalClassesAlreadyPresent = true;
        } else {
          globalClassesAlreadyPresent = false;
        }
        if (
          txt.includes("_global_script_classes=[ {") &&
          !globalClassesAlreadyPresent
        ) {
           globalOutputText = txt.replace(
            "_global_script_classes=[",
            `
_global_script_classes=[ {
"base": "Reference",
"class": "AK",
"language": "GDScript",
"path": "res://wwise/GeneratedSoundBanks/wwise_ids.gd"
}, {
"base": "Reference",
"class": "AkUtils",
"language": "GDScript",
"path": "res://wwise/runtime/helpers/ak_utils.gd"
},`
          );

         
        } else if (
          txt.includes("_global_script_classes=[") &&
          !globalClassesAlreadyPresent
        ) {
          globalOutputText = txt.replace(
            "_global_script_classes=[ ",
            `
_global_script_classes=[ {
"base": "Reference",
"class": "AK",
"language": "GDScript",
"path": "res://wwise/GeneratedSoundBanks/wwise_ids.gd"
}, {
"base": "Reference",
"class": "AkUtils",
"language": "GDScript",
"path": "res://wwise/runtime/helpers/ak_utils.gd"
}`
          );

        } else if (
          !txt.includes("_global_script_classes=[ {") &&
          !globalClassesAlreadyPresent
        ) {
          globalOutputText = txt + `
_global_script_classes=[ {
"base": "Reference",
"class": "AK",
"language": "GDScript",
"path": "res://wwise/GeneratedSoundBanks/wwise_ids.gd"
}, {
"base": "Reference",
"class": "AkUtils",
"language": "GDScript",
"path": "res://wwise/runtime/helpers/ak_utils.gd"
} ]
_global_script_class_icons={
"AK": "",
"AkUtils": ""
} ]`;
        } else {
          globalOutputText = txt;
        }
        output = globalOutputText + "\n\n" + serializedIni;
       fs.writeFile(godotFilePath, output);
      });
    },
    checkIntegrationAlreadyInstalled() {
      var wwisePath = path.join(this.godotProjectPath, "wwise");
      var addonsPath = path.join(this.godotProjectPath, "addons");
      var wwiseAddonsPath = path.join(addonsPath, "wwise");
      var wwiseIdsConverterPath = path.join(addonsPath, "wwise_ids_converter");

      if (fs.existsSync(wwisePath)) {
        this.setIntegrationInstalled(true);
      } else if (fs.existsSync(wwiseAddonsPath)) {
        this.setIntegrationInstalled(true);
      } else if (fs.existsSync(wwiseIdsConverterPath)) {
        this.setIntegrationInstalled(true);
      } else {
        this.setIntegrationInstalled(false);
      }
    },
    getIntegrationFiles() {
      var dlRelease = require("download-github-release");

      var vm = this;

      this.updateProgressTextandBar(
        "Getting integration files from repository",
        20
      );

      if (!fs.existsSync(this.gitDownloadDestionationPath)) {
        fs.mkdirSync(this.gitDownloadDestionationPath);
      }

      function filterRelease(release) {
        return release.prerelease === false;
      }

      function filterAsset(asset) {
        return asset.name.indexOf("Integration.zip") >= 0;
      }

      dlRelease(
        "alessandrofama",
        "wwise-godot-integration",
        this.gitDownloadDestionationPath,
        filterRelease,
        filterAsset,
        false,
        false
      )
        .then(function () {
          vm.getBinaries();
        })
        .catch(function (err) {
          vm.displayFailureMessage(err);
        });
    },
    getBinaries() {
      var dlRelease = require("download-github-release");

      var vm = this;

      this.updateProgressTextandBar("Getting binaries from repository", 40);

      function filterRelease(release) {
        return release.prerelease === false;
      }

      function filterAsset(asset) {
        for (let i = 0; i < vm.platforms.length; i++) {
          if (asset.name.indexOf(vm.platforms[i]) >= 0) {
            return true;
          }
        }
      }

      dlRelease(
        "alessandrofama",
        "wwise-godot-integration",
        this.binariesDestionationPath,
        filterRelease,
        filterAsset,
        false,
        false
      )
        .then(function () {
          vm.copyIntegrationFilesToProject();
        })
        .catch(function (err) {
          vm.displayFailureMessage(err);
        });
    },
    copyIntegrationFilesToProject() {
      var copydir = require("copy-dir");

      var vm = this;

      this.updateProgressTextandBar(
        "Copying integration files to Godot project",
        60
      );

      try {
        copydir.sync(this.gitDownloadDestionationPath, this.godotProjectPath, {
          utimes: true,
          mode: true,
          cover: true,
        });
      } catch (err) {
        vm.displayFailureMessage(err);
        return;
      }
      vm.finishInstallation();
    },
    finishInstallation() {
      this.updateProgressTextandBar(
        "The Wwise Godot integration was installed successfully and is ready to use.",
        100
      );

      this.removeDirectory(this.gitDownloadDestionationPath);

      this.installed = true;
      this.installing = false;
    },
    uninstallIntegration() {
      this.installHeader = "Uninstalling the integration";
      var wwisePath = path.join(this.godotProjectPath, "wwise");
      var addonsPath = path.join(this.godotProjectPath, "addons");
      var wwiseAddonsPath = path.join(addonsPath, "wwise_custom_nodes");
      var wwiseIdsConverterPath = path.join(addonsPath, "wwise_ids_converter");
      var wwiseBuildExportPath = path.join(addonsPath, "wwise_build_export");
      var waapiPickerAddonPath = path.join(addonsPath, "waapi_picker");
      var overrideFilePath = path.join(this.godotProjectPath, "override.cfg");

      this.removeDirectory(wwisePath);
      this.removeDirectory(wwiseAddonsPath);
      this.removeDirectory(wwiseIdsConverterPath);
      this.removeDirectory(wwiseBuildExportPath);
      this.removeDirectory(waapiPickerAddonPath);

      if (fs.existsSync(overrideFilePath)) {
        fs.unlinkSync(overrideFilePath);
      }

      this.updateProgressTextandBar("", 100);

      this.installed = true;
      this.installing = false;
    },
    openProjectInExplorer() {
      var openExplorer = require("open-file-explorer");
      var vm = this;
      openExplorer(this.godotProjectPath, (err) => {
        if (err) {
          vm.displayFailureMessage(err);
        }
      });
    },
    updateProgressTextandBar(text, bar) {
      this.installText = text;
      this.progress = bar;
    },
    displayFailureMessage(err) {
      var message;

      err == Error ? (message = err.message) : (message = err);

      this.updateProgressTextandBar(`Error: ${message} `, 0);

      this.removeDirectory(this.gitDownloadDestionationPath);

      this.installed = false;
      this.installing = false;
      this.installationFailed = true;
    },
    removeDirectory(path) {
      if (fs.existsSync(path)) {
        fs.rmdirSync(path, {
          recursive: true,
        });
      }
    },
  },
};
</script>

<style>
.progressing {
  margin-top: 4%;
}
.fadeInstall-enter {
  opacity: 0;
}

.fadeInstall-enter-active {
  transition: opacity 1s;
}

.fadeInstall-leave-active {
  transition: opacity 4s;
  opacity: 0;
}
</style>
