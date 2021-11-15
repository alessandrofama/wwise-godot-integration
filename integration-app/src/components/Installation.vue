<template>
  <div>
    <div v-if="!installing && !installed && !installationFailed">
      <div class="form-group">
        <div>
          <label style="font-size: large">Wwise Integration Version</label>
          <br />
          <select
            class="form-control"
            id="exampleFormControlSelect1"
            v-model="selectedRelease"
          >
            <option
              v-for="release in releases"
              :key="release.name"
              :value="release"
            >
              {{ release.name }}
            </option>
          </select>
          <label style="font-size: large; margin-top: 4%"
            >Platform Selection</label
          >
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
import { Octokit } from "octokit";

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
      releases: [],
      selectedRelease: null,
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
  mounted: function () {
    const octokit = new Octokit({});

    var vm = this;

    octokit.rest.repos
      .listReleases({
        owner: "alessandrofama",
        repo: "wwise-godot-integration",
      })
      .then(function (releases) {
        vm.releases = releases.data;
        vm.selectedRelease = releases.data[0];
      })
      .catch(function (err) {
        vm.displayFailureMessage(err);
      });
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

      var serializedIni;
      var output;
      var iniObj = multiini.read(godotFilePath, { keep_quotes: false });

      if (!("editor_plugins" in iniObj)) {
        iniObj.editor_plugins = {};
        iniObj.editor_plugins.enabled =
          'PoolStringArray( "res://addons/wwise_setup/plugin.cfg" )';
      } else {
        if (!("enabled" in iniObj.editor_plugins)) {
          iniObj.editor_plugins.enabled =
            'PoolStringArray( "res://addons/wwise_setup/plugin.cfg" )';
        } else {
          if (
            !iniObj.editor_plugins.enabled.includes(
              "res://addons/wwise_setup/plugin.cfg"
            )
          ) {
            // Wwise not present in enabled plugins
            var toReplace = iniObj.editor_plugins.enabled.replace(
              "PoolStringArray(",
              'PoolStringArray( "res://addons/wwise_setup/plugin.cfg", '
            );
            iniObj.editor_plugins.enabled = toReplace;
          }
        }
      }

      for (var item in iniObj) {
        var arr = this.getKeyByValue(iniObj[item], "{");
        if (arr.length > 0) {
          delete iniObj[item];
        }
      }
      var serializer = new multiini.Serializer({ keep_quotes: false });
      serializedIni = serializer.serialize(iniObj);
      serializedIni = serializedIni.replace('"Pool', "Pool");
      serializedIni = serializedIni.replace(')"', ")");

      fs.readFile(godotFilePath, "utf-8").then(() => {
        // eslint-disable-line no-unused-vars
        output = serializedIni;
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
      const Downloader = require("nodejs-file-downloader");

      this.updateProgressTextandBar(
        "Getting integration files from repository",
        20
      );

      var integrationPackage = this.selectedRelease.assets.find(function (
        data
      ) {
        return data.name === "Integration.zip";
      });

      var vm = this;
      (async () => {
        const downloader = new Downloader({
          url: integrationPackage.browser_download_url,
          directory: vm.gitDownloadDestionationPath,
        });
        try {
          await downloader.download();
          vm.updateProgressTextandBar(
            "Getting integration files from repository",
            40
          );
          vm.getBinaries();
        } catch (error) {
          vm.displayFailureMessage(error);
        }
      })();
    },
    getBinaries() {
      const Downloader = require("nodejs-file-downloader");
      var vm = this;
      vm.updateProgressTextandBar("Getting binaries from repository", 60);

      var platformSelection = [];

      for (var i = 0; i < vm.platforms.length; i++) {
        this.selectedRelease.assets.find(function (data) {
          if (data.name === vm.platforms[i] + ".zip") {
            platformSelection.push(data.browser_download_url);
          }
        });
      }

      (async () => {
        for (var j = 0; j < platformSelection.length; j++) {
          const downloader = new Downloader({
            url: platformSelection[j],
            directory: vm.binariesDestionationPath,
          });
          try {
            await downloader.download();
          } catch (error) {
            vm.displayFailureMessage(error);
          }
        }
      })().then(function () {
        vm.copyIntegrationFilesToProject();
      });
    },
    copyIntegrationFilesToProject() {
      this.updateProgressTextandBar("Copying files to the project.", 80);

      const unzipper = require("unzipper");

      var vm = this;

      var files = fs.readdirSync(this.gitDownloadDestionationPath);

      for (var i = 0; i < files.length; i++) {
        var file = files[i];
        var filePath = path.join(this.gitDownloadDestionationPath, file);

        if (file === "Integration.zip") {
          var readStream = fs.createReadStream(filePath);
          readStream.on("error", function (err) {
            vm.displayFailureMessage(err);
          });
          readStream
            .pipe(unzipper.Extract({ path: vm.godotProjectPath }))
            .on("close", () => {
              var binaries = fs.readdirSync(this.binariesDestionationPath);
              var numExtractedBinaries = 0;
              for (var j = 0; j < binaries.length; j++) {
                var binary = binaries[j];
                var binaryPath = path.join(
                  this.binariesDestionationPath,
                  binary
                );

                readStream = fs.createReadStream(binaryPath);
                readStream.on("error", function (err) {
                  vm.displayFailureMessage(err);
                });

                readStream
                  .pipe(
                    unzipper.Extract({
                      path: path.join(vm.godotProjectPath, "wwise/bin"),
                    })
                  )
                  .on("close", () => {
                    numExtractedBinaries++;

                    if (numExtractedBinaries == binaries.length - 1) {
                      vm.finishInstallation();
                    }

                    if (binaries.length == 1 && numExtractedBinaries == 1) {
                      vm.finishInstallation();
                    }
                  });
              }
            });
        }
      }
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
      var wwiseSetupAddonPath = path.join(addonsPath, "wwise_setup");
      var overrideFilePath = path.join(this.godotProjectPath, "override.cfg");

      this.removeDirectory(wwisePath);
      this.removeDirectory(wwiseAddonsPath);
      this.removeDirectory(wwiseIdsConverterPath);
      this.removeDirectory(wwiseBuildExportPath);
      this.removeDirectory(waapiPickerAddonPath);
      this.removeDirectory(wwiseSetupAddonPath);

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
