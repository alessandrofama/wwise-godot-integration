module.exports = {
  pluginOptions: {
    electronBuilder: {
      externals: ["electron", "child_process", "fs", "path", "unzipper"],
      nodeIntegration: true
    }
  }
};
