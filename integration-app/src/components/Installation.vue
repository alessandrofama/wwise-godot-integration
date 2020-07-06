<template>
  <div>
    <form @submit.prevent="onSubmit">
      <div class="form-group">
        <label for="godotProject">Godot project target selection</label>
        <div class="custom-file">
          <input
            type="file"
            class="custom-file-input"
            id="customFile"
            accept=".godot"
            @change="previewFilePath"
          />
          <label class="custom-file-label" for="customFile">{{
            previewGodotProjectFilePath
          }}</label>
          <span class="form-text text-muted">Select Godot's project file.</span>
        </div>
        <span
          v-if="isIntegrationInstalled"
          class="form-text"
          style="margin-top:1%"
          >The Wwise Godot Integration is already installed. Please update or
          uninstall the integration.</span
        >
        <span v-else class="form-text" style="margin-top:1%"
          >Installing the Wwise Godot integration will copy the required files
          to your Godot Project.</span
        >
      </div>

      <div class="from-group b-button-toolbar ">
        <b-button-group class="mx-1">
          <button name="submit" type="submit" class="btn btn-primary">
            <span v-if="isIntegrationInstalled">Update</span>
            <span v-else>Install</span>
          </button>
        </b-button-group>
        <b-button-group class="mx-1">
          <button
            v-if="isIntegrationInstalled"
            name="submit"
            type="submit"
            class="btn btn-secondary"
          >
            <span>Uninstall</span>
          </button>
        </b-button-group>
      </div>
    </form>
    <div class="progressing">
      <transition name="fadeInstall">
        <div v-if="installing" class="progress">
          <div
            class="progress-bar"
            role="progressbar"
            :style="{ width: progress + '%' }"
            aria-valuenow="100"
            aria-valuemin="0"
            aria-valuemax="100"
          ></div>
        </div>
      </transition>
    </div>
  </div>
</template>

<script>
import { mapActions } from "vuex";
import unzipper from "unzipper";
import fs from "fs";
import path from "path";

export default {
  data() {
    return {
      previewGodotProjectFilePath: "Choose File",
      installing: false
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
    }
  },

  methods: {
    ...mapActions([
      "setGodotProjectPath",
      "setGodotProjectFilePath",
      "setIntegrationInstalled"
    ]),

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
    extractIntegration() {
      fs.createReadStream(
        // eslint-disable-next-line no-undef
        path.join(__static, "Wwise-Godot-Integration.zip")
      ).pipe(
        unzipper
          .Extract({ path: this.godotProjectPath })
          .on("error", err => console.error("error", err))
          .on("finish", () => this.updateGodotProjectFile())
      );
    },
    updateGodotProjectFile() {
      try {
        var projectFileData = fs.readFileSync(
          // eslint-disable-next-line no-undef
          path.join(__static, "projectFileData.txt"),
          "utf8"
        );
      } catch (e) {
        console.log("Error:", e.stack);
      }

      fs.appendFile(this.godotProjectFilePath, projectFileData, function(err) {
        if (err) {
          console.log("Error:", err.stack);
        } else {
          console.log("append succeeded");
        }
      });
    },
    onSubmit() {
      this.installing = true;
      this.extractIntegration();
    }
  }
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
  transition: opacity 1s;
  opacity: 0;
}
</style>
