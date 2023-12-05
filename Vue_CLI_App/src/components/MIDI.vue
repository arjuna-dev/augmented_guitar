<template>
  <div></div>
</template>

<script>
import { hexToDecimal, hex2bin } from "../utils/utils.js";
import MIDIMessage from "../utils/MIDIMessage.js";
import MIDIccMessage from "../utils/MIDIccMessage.js";
import { MIDIMessageTypes } from "../utils/MIDIMessageTypes.js";

export default {
  name: "Fretboard",
  data() {
    return {
      midi_message: "",
      midi: null,
      channel: "",
      type: "",
      midi_message_note: "",
      midi_message_velocity: "",
    };
  },
  methods: {
    onMIDIMessage(event) {
      //Decode MIDI
      let str = "";
      for (const character of event.data) {
        str += `0x${character.toString(16)} `;
      }
      let message_array = str.split(" ");
      let status_byte = hex2bin(message_array[0]);
      let channel = parseInt(status_byte.slice(4), 2);
      let type = status_byte.slice(0, 4);
      let midi_message_note = message_array[1];
      let midi_message_velocity = message_array[2];

      this.channel = channel;
      this.type = MIDIMessageTypes[type];
      this.midi_message_note = hexToDecimal(midi_message_note);
      this.midi_message_velocity = hexToDecimal(midi_message_velocity);

      //Emit MIDI on updating variable
      if (this.type == "cc") {
        this.midi_message = new MIDIccMessage(this.channel, this.type, this.midi_message_note, this.midi_message_velocity);
      } else if (this.type == "note_on" || this.type == "note_off") {
        this.midi_message = new MIDIMessage(this.channel, this.type, this.midi_message_note, this.midi_message_velocity);
      }
    },
    onMIDISuccess(midiAccess) {
      console.log("MIDI ready!");
      this.midi = midiAccess;
      this.listInputsAndOutputs(this.midi);
      this.startLoggingMIDIInput(this.midi);
    },
    onMIDIFailure(msg) {
      console.error(`Failed to get MIDI access - ${msg}`);
    },
    listInputsAndOutputs(midiAccess) {
      for (const entry of midiAccess.inputs) {
        const input = entry[1];
        console.log(
          `Input port [type:'${input.type}']` +
            ` id:'${input.id}'` +
            ` manufacturer:'${input.manufacturer}'` +
            ` name:'${input.name}'` +
            ` version:'${input.version}'`
        );
      }

      for (const entry of midiAccess.outputs) {
        const output = entry[1];
        console.log(
          `Output port [type:'${output.type}'] id:'${output.id}' manufacturer:'${output.manufacturer}' name:'${output.name}' version:'${output.version}'`
        );
      }
    },
    startLoggingMIDIInput(midiAccess) {
      midiAccess.inputs.forEach((entry) => {
        entry.onmidimessage = this.onMIDIMessage;
      });
    },
  },
  watch: {
    midi_message(newValue) {
      this.$parent.$emit("MIDI-message", newValue);
    },
  },
  mounted() {
    navigator.requestMIDIAccess().then(this.onMIDISuccess, this.onMIDIFailure);
  },
};
</script>

<style scoped>
.the-grid {
  display: grid;
  height: 100%;
  background-color: black;
}
</style>
