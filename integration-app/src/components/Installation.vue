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
        </div>
      </div>

      <span class="form-text text-muted">Select Godot's project file.</span>

      <div class="form-group">
        <button name="submit" type="submit" class="btn btn-primary">
          Install
        </button>
      </div>
    </form>
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
    }
  },

  methods: {
    ...mapActions(["setGodotProjectPath", "setGodotProjectFilePath"]),

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
    },
    extractIntegration() {
      // eslint-disable-next-line no-undef
      fs.createReadStream(path.join(__static, "test.zip")).pipe(
        unzipper
          .Extract({ path: this.godotProjectPath })
          .on("error", err => console.error("error", err))
          .on("finish", () => console.log("finish"))
      );
    },
    onSubmit() {
      this.installing = true;
      this.extractIntegration();
    }
  }
};
</script>

<style>
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
