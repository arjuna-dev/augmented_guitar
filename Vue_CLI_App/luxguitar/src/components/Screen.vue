<template>
  <div class="screen">
    <Fretboard />
    <AfterFretboardDisplay />
  </div>
</template>

<script>
import Fretboard from "./Fretboard";
import AfterFretboardDisplay from "./AfterFretboardDisplay";

export default {
  components: { Fretboard, AfterFretboardDisplay },
  name: "Screen"
};
</script>

<style scoped>
.screen {
  display: flex;
  flex-direction: row;
  width: 100vw;
  height: 100vh;
}
</style>
