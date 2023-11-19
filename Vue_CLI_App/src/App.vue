<template>
  <div id="app">
    <Fretboard />
    <Menu />
    <MIDI />
    <MidiPlayer />
  </div>
</template>

<script>
import Fretboard from "./components/Fretboard.vue";
import Menu from "./components/Menu.vue";
import MIDI from "./components/MIDI.vue";
import MidiPlayer from "./components/MidiPlayer.vue";

export default {
  name: "app",
  components: {
    Fretboard,
    Menu,
    MIDI,
    MidiPlayer,
},
  mounted() {}
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
}
</style>
