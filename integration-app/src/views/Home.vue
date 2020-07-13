<template>
  <div class="home">
    <h1></h1>
    <installation></installation>
  </div>
</template>

<style scoped>
h1 {
  padding: 15px 0px;
}
</style>

<script>
import Installation from "../components/Installation.vue";

export default {
  components: {
    installation: Installation,
  },
};
</script>
