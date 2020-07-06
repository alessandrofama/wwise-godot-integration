import Vue from "vue";
import Vuex from "vuex";

import installation from "./modules/installation";

Vue.use(Vuex);

export default new Vuex.Store({
  modules: { installation }
});
