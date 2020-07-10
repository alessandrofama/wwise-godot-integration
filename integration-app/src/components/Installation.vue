<template>
  <div>
    <div v-if="!installing && !installed && !installationFailed">
      <form @submit.prevent="onSubmit">
        <div class="form-group">
          <p></p>
          <label for="godotProject">Select Godot's project file.</label>
          <div class="custom-file" style="margin-top: 1%;">
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
            style="margin-top: 3%; font-size: large;"
          >
            The Wwise Godot Integration is already installed. Please update or
            uninstall the integration.
          </span>
          <span
            v-else
            class="form-text"
            style="margin-top: 3%; font-size: large;"
          >
            Installing the Wwise Godot integration will copy the required files
            to your Godot Project.
          </span>
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
      <p style="margin-top: 5%; font-size: large;">{{ installText }}</p>
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
      <p style="margin-top: 5%;">{{ installText }}</p>
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
      <p style="margin-top: 5%;">{{ installText }}</p>
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
import copydir from "copy-dir";
import openExplorer from "open-file-explorer";
import dlRelease from "download-github-release";
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
      progress: 0,
      max: 100,
      gitDownloadDestionationPath: path.join(
        remote.app.getPath("temp"),
        "wwise_gdnative"
      ),
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
    },
    onSubmit() {
      this.installing = true;
      this.installHeader = "Installation";
      this.getIntegrationFiles();
    },
    previewFilePath(event) {
      let filePath = event.target.files[0].path;
      filePath = filePath.replace(/\\/g, "/");
      filePath = path.normalize(filePath);
      this.previewGodotProjectFilePath = filePath;

      this.setGodotProjectPaths(filePath);
    },

    setGodotProjectPaths(godotFilePath) {
      this.setGodotProjectFilePath(godotFilePath);

      let dirPath = path.dirname(godotFilePath);
      this.setGodotProjectPath(dirPath);
      this.checkIntegrationAlreadyInstalled();
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

      function filterAsset() {
        return true;
      }

      dlRelease(
        "alessandrofama",
        "testrepo",
        this.gitDownloadDestionationPath,
        filterRelease,
        filterAsset,
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
      var vm = this;

      this.updateProgressTextandBar(
        "Copying integration files to Godot project",
        50
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
        "Open your Godot project and activate the Wwise integration in the addons tab of the project settings.",
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
      var overrideFilePath = path.join(this.godotProjectPath, "override.cfg");

      this.removeDirectory(wwisePath);
      this.removeDirectory(wwiseAddonsPath);
      this.removeDirectory(wwiseIdsConverterPath);
      this.removeDirectory(wwiseBuildExportPath);

      this.removeStringFromFile(
        `WwiseSettings="res://wwise/wwise_settings.gd"`,
        this.godotProjectFilePath
      );
      this.removeStringFromFile(
        `Wwise="*res://wwise/bin/wwise-gdnative-debug.gdns"`,
        this.godotProjectFilePath
      );

      fs.unlinkSync(overrideFilePath);

      this.updateProgressTextandBar("", 100);

      this.installed = true;
      this.installing = false;
    },
    removeStringFromFile(string, file) {
      var data = fs.readFileSync(file, "utf-8");
      var stringToRemove = string;

      var newValue = data.replace(stringToRemove, "");
      fs.writeFileSync(file, newValue, "utf-8");
    },
    openProjectInExplorer() {
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
  transition: opacity 3s;
  opacity: 0;
}
</style>
