const state = {
  godotProjectPath: "",
  godotProjectFilePath: ""
};

const mutations = {
  SET_GODOT_PROJECT_PATH(state, payload) {
    state.godotProjectPath = payload;
  },

  SET_GODOT_PROJECT_FILE_PATH(state, payload) {
    state.godotProjectFilePath = payload;
  }
};

const actions = {
  setGodotProjectPath: ({ commit }, payload) => {
    commit("SET_GODOT_PROJECT_PATH", payload);
  },
  setGodotProjectFilePath: ({ commit }, payload) => {
    commit("SET_GODOT_PROJECT_FILE_PATH", payload);
  }
};

const getters = {
  godotProjectPath: state => {
    return state.godotProjectPath;
  },

  godotProjectFilePath: state => {
    return state.godotProjectFilePath;
  }
};

export default {
  state,
  mutations,
  actions,
  getters
};
