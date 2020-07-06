<template>
  <div class="home">
    <h1>Wwise GDNative installation</h1>
    <installation></installation>
  </div>
</template>

<style scoped>
h1 {
  padding: 20px 0px;
}
</style>

<script>
import Installation from "../components/Installation.vue";

export default {
  components: {
    installation: Installation
  }
};
</script>
