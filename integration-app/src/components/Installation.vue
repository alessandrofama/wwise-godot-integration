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
            style="margin-top: 3%;"
          >
            The Wwise Godot Integration is already installed. Please update or
            uninstall the integration.
          </span>
          <span v-else class="form-text" style="margin-top: 3%;">
            Installing the Wwise Godot integration will copy the required files
            to your Godot Project.
          </span>
        </div>

        <div class="from-group b-button-toolbar">
          <b-button-group>
            <button name="submit" type="submit" class="btn btn-primary">
              <span v-if="isIntegrationInstalled">Update</span>
              <span v-else>Install</span>
            </button>
          </b-button-group>
          <b-button-group class="mx-1">
            <button v-if="isIntegrationInstalled" class="btn btn-secondary">
              <span>Uninstall</span>
            </button>
          </b-button-group>
        </div>
      </form>
    </div>
    <div v-if="installing && !installationFailed">
      <h3 v-if="installing">Installing...</h3>
      <h3 v-else>Installation completed successfully</h3>
      <p style="margin-top: 5%;">{{ installText }}</p>
      <div v-if="installed" class="b-button-toolbar">
        <b-button-group>
          <button v-on:click="openProjectInExplorer" class="btn btn-primary">
            <span>Open Project directory</span>
          </button>
        </b-button-group>
        <b-button-group class="mx-1">
          <button v-if="isIntegrationInstalled" class="btn btn-secondary">
            <span>Uninstall</span>
          </button>
        </b-button-group>
      </div>
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
      <h3>Installation failed</h3>
      <p style="margin-top: 5%;">{{ installText }}</p>
    </div>
  </div>
</template>

<script>
import { mapActions } from "vuex";
import fs from "fs";
import path from "path";
import fetchRepoDir from "fetch-repo-dir";
import copydir from "copy-dir";
import openExplorer from "open-file-explorer";
import { remote } from "electron";

export default {
  data() {
    return {
      previewGodotProjectFilePath: "Select File",
      installing: false,
      installed: false,
      installationFailed: false,
      installText: "",
      progress: 0,
      max: 100,
      gitDownloadDestionationPath:
        remote.app.getPath("temp") + "/wwise_gdnative",
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
    onSubmit() {
      this.installing = true;
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
      if (fs.existsSync(path.join(this.godotProjectPath, "wwise"))) {
        this.setIntegrationInstalled(true);
      } else {
        this.setIntegrationInstalled(false);
      }
    },
    async getIntegrationFiles() {
      this.updateProgressTextandBar(
        "Getting integration files from repository",
        20
      );

      console.log(fs.existsSync(this.gitDownloadDestionationPath));

      if (fs.existsSync(this.gitDownloadDestionationPath)) {
        fs.rmdirSync(this.gitDownloadDestionationPath, {
          recursive: true,
        });
      }

      await fetchRepoDir({
        src: "alessandrofama/fmod-love/fmod-love",
        dir: this.gitDownloadDestionationPath,
        replace: true,
      }).then(
        () => {
          this.copyIntegrationFilesToProject();
        },
        (err) => {
          this.failInstallation(err);
        }
      );
    },
    copyIntegrationFilesToProject() {
      var vm = this;

      this.updateProgressTextandBar(
        "Copying integration files to Godot project",
        40
      );

      try {
        copydir.sync(this.gitDownloadDestionationPath, this.godotProjectPath, {
          utimes: true,
          mode: true,
          cover: true,
        });
      } catch (err) {
        vm.failInstallation(err);
        return;
      }

      vm.updateGodotProjectFile();
    },
    updateGodotProjectFile() {
      var vm = this;

      this.updateProgressTextandBar("Updating Godot project file", 80);
      try {
        var projectFileData = fs.readFileSync(
          // eslint-disable-next-line no-undef
          path.join(__static, "projectFileData.txt"),
          "utf8"
        );
      } catch (err) {
        vm.failInstallation(err);
        return;
      }

      try {
        fs.appendFileSync(this.godotProjectFilePath, projectFileData);
      } catch (err) {
        vm.failInstallation(err);
        return;
      }

      vm.finishInstallation();
    },
    finishInstallation() {
      this.updateProgressTextandBar(
        "Open your Godot project and activate the Wwise integration in the addons tab of the project settings.",
        100
      );

      this.installed = true;
      this.installing = false;
    },
    openProjectInExplorer() {
      var vm = this;
      openExplorer(this.godotProjectPath, (err) => {
        if (err) {
          vm.failInstallation(err);
        }
      });
    },
    updateProgressTextandBar(text, bar) {
      this.installText = text;
      this.progress = bar;
    },
    failInstallation(err) {
      this.updateProgressTextandBar(`Installation failed: ${err.message} `, 0);
      this.installed = false;
      this.installing = false;
      this.installationFailed = true;
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
