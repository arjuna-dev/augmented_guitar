<template>
  <div id="app">
    <Fretboard />
    <Menu />
    <MIDI />
  </div>
</template>

<script>
import Fretboard from "./components/Fretboard.vue";
import Menu from "./components/Menu.vue";
import MIDI from "./components/MIDI.vue";

export default {
  name: "app",
  components: {
    Fretboard,
    Menu,
    MIDI,
  },
  mounted() {},
};
</script>

<style>
* {
  box-sizing: border-box;
}
body {
  margin: 0;
  padding: 0;
  font-family: Tahoma, sans-serif;
}

@media (min-aspect-ratio: 4/1) {
  #app {
    display: flex;
    flex-direction: row;
    width: 100vw;
    height: 100vh;
  }
}

#app {
  display: flex;
  flex-direction: column;
  width: 100vw;
  height: 100vh;
  background-color: rgb(41, 41, 41);
}

.menu {
  width: 100%;
  color: azure;
  padding: 15px 0px 0px 30px;
  overflow-y: scroll;
  display: flex;
  row-gap: 15px;
}

.menu-title {
  color: azure;
  margin: 0px;
}

.menu-container {
  margin-bottom: 15px;
}

.row {
  display: flex;
  flex-direction: row;
}

.column {
  display: flex;
  flex-direction: column;
}

.menu-item {
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  margin-right: 15px;
}
</style>
