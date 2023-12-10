<template>
  <div>
    <MIDIPlayer />
    <UserMIDI />
  </div>
</template>

<script>
import MIDIPlayer from "./MIDIPlayer.vue";
import UserMIDI from "./UserMIDI.vue";

export default {
  name: "MIDI",
  components: {
    MIDIPlayer,
    UserMIDI,
  },
  data() {
    return {};
  },
  methods: {},
  mounted() {},
};
</script>
