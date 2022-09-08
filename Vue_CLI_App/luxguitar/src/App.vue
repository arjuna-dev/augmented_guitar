<template>
  <div id="app">
    <Screen />
  </div>
</template>

<script>
import Screen from "./components/Screen.vue";

export default {
  name: "app",
  components: {
    Screen
  }
};
</script>

<style>
* {
  box-sizing: border-box;
}
body {
  margin: 0;
  padding: 0;
}
</style>
