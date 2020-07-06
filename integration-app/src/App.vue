<template>
  <div id="container">
    <app-header></app-header>
    <div class="row">
      <div class="col-12">
        <transition name="fade" mode="out-in">
          <router-view></router-view>
        </transition>
      </div>
    </div>
  </div>
</template>

<script>
import Header from "./components/Header.vue";

export default {
  components: {
    appHeader: Header
  },
  created() {}
};
</script>

<style>
body {
  padding: 30px;
}

.fade-enter {
  opacity: 0;
}

.fade-enter-active {
  transition: opacity 0.1s;
}

.fade-leave-active {
  transition: opacity 0.1s;
  opacity: 0;
}
</style>
