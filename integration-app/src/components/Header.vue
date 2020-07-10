<template>
  <b-navbar type="light" variant="light">
    <router-link to="/">
      <b-navbar-brand>Wwise Godot Integration App</b-navbar-brand></router-link
    >
    <b-navbar-nav class="ml-auto">
      <b-nav-item-dropdown text="Help" right>
        <b-dropdown-item
          v-on:click="
            openLinkInBrowser(
              'https://github.com/alessandrofama/wwise-godot-integration'
            )
          "
          >GitHub Repository</b-dropdown-item
        >
        <b-dropdown-item
          v-on:click="
            openLinkInBrowser(
              'https://github.com/alessandrofama/wwise-godot-integration/wiki'
            )
          "
          >Documentation</b-dropdown-item
        >
      </b-nav-item-dropdown>
    </b-navbar-nav>
  </b-navbar>
</template>

<script>
export default {
  methods: {
    openLinkInBrowser(url) {
      var shell = require("electron").shell;
      shell.openExternal(url);
    },
  },
};
</script>
