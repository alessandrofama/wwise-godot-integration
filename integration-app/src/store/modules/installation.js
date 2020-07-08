const state = {
  godotProjectPath: "",
  godotProjectFilePath: "",
  isIntegrationInstalled: false,
};

const mutations = {
  SET_GODOT_PROJECT_PATH(state, payload) {
    state.godotProjectPath = payload;
  },

  SET_GODOT_PROJECT_FILE_PATH(state, payload) {
    state.godotProjectFilePath = payload;
  },

  SET_INTEGRATION_INSTALLED(state, payload) {
    state.isIntegrationInstalled = payload;
  },
};

const actions = {
  setGodotProjectPath: ({ commit }, payload) => {
    commit("SET_GODOT_PROJECT_PATH", payload);
  },

  setGodotProjectFilePath: ({ commit }, payload) => {
    commit("SET_GODOT_PROJECT_FILE_PATH", payload);
  },

  setIntegrationInstalled: ({ commit }, payload) => {
    commit("SET_INTEGRATION_INSTALLED", payload);
  },
};

const getters = {
  godotProjectPath: (state) => {
    return state.godotProjectPath;
  },

  godotProjectFilePath: (state) => {
    return state.godotProjectFilePath;
  },

  isIntegrationInstalled: (state) => {
    return state.isIntegrationInstalled;
  },
};

export default {
  state,
  mutations,
  actions,
  getters,
};
