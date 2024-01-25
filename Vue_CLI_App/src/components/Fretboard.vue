<template>
  <div>
    <div class="the-grid" :style="fretboardGridStyle" @click="debug">
      <div v-bind:style="{ display: 'grid', placeItems: 'center' }" v-for="(fretSlot, index) in draw_fretboard_slots(notePattern)" :key="index">
        <div :style="fretStyle(fretSlot)">
          {{ fretSlot.text }}
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import fretSlot from "../utils/fretSlot";
import MIDIccMessage from "../utils/MIDIccMessage.js";
import { MIDIMessageTypesStrings } from "../utils/MIDIMessageTypes.js";

export default {
  name: "Fretboard",
  data() {
    return {
      midi_message: "",
      is_user_message: false,
      number_of_strings: 6,
      number_of_frets: 20,
      string_length: 647.7,
      all_notes: ["C", "C♯/D♭", "D", "D♯/E♭", "E", "F", "F♯/G♭", "G", "G♯/A♭", "A", "A♯/B♭", "B"],
      capo: 3,
      pressed_notes: [],
      playing_notes: [],
      map_jamstik_strings: [5, 4, 3, 2, 1, 0],
      midi_cc_msg: "",
      are_circles: false,
      is_midi_note: false,
      notePattern: [],
      colors: {
        normal: ["#7FDBFF", "#39CCCC", "#3D9970", "#2ECC40", "#01FF70", "#FFDC00", "#FF851B", "#FF4136", "#85144b", "#F012BE", "#B10DC9", "#0074D9"],
        pressed: "black",
        playing: "black",
      },
    };
  },
  computed: {
    openStringMidiNotes() {
      let openStringMidiNotes = [40, 45, 50, 55, 59, 64];
      for (let i = 0; i < this.number_of_strings; i++) {
        openStringMidiNotes[i] += this.capo;
      }
      return openStringMidiNotes;
    },
    openStringNotes() {
      let openStringNotes = ["E", "A", "D", "G", "B", "E"];
      for (let i = 0; i < this.number_of_strings; i++) {
        let newNoteIndex = (this.all_notes.indexOf(openStringNotes[i]) + this.capo) % 12;
        openStringNotes[i] = this.all_notes[newNoteIndex];
      }
      return openStringNotes;
    },
    fretboardSlotsAmount: function () {
      return this.number_of_strings * this.number_of_frets;
    },
    fretDistances() {
      let fretDist = [10];
      // Adjustment for screen resolution distance discrepancies
      let newLength = this.string_length * 0.867469097;
      // let newLength = this.string_length * 0.85981;
      // let newLength = this.string_length * 0.85981551561;
      for (let i = 0; i < this.number_of_frets - 1; i++) {
        let d = newLength / 17.817;
        fretDist.push(d);
        newLength = newLength - d;
      }
      // We substract the edge of the display on the side of the guitar nut
      fretDist[0] -= 3.37;
      return fretDist;
    },
    fretboardLength() {
      return this.fretDistances.reduce((sum, elem) => sum + elem);
    },
    fretboardGridStyle() {
      let fretDist = this.fretDistances.join("mm ");
      return {
        gridTemplateColumns: fretDist + "mm",
        gridTemplateRows: "repeat(" + this.number_of_strings + ", 1fr )",
        width: this.fretboardLength + "mm",
      };
    },
  },
  methods: {
    debug() {
      console.log(this.fretboardSlots);
    },
    draw_fretboard_slots(note_pattern) {
      if (note_pattern.length == 0) {
        note_pattern = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11];
      }
      let fretboardSlots = [];
      for (let i = this.number_of_strings - 1; i >= 0; i--) {
        let firstNoteOfString = this.openStringNotes[i];
        // let firstNoteOfString = this.openStringMidiNotes[i];
        for (let j = 0; j < this.number_of_frets; j++) {
          let noteIndex = (this.all_notes.indexOf(firstNoteOfString) + j) % 12;
          let MIDI_note = this.openStringMidiNotes[i] + j;
          let octave = Math.floor(MIDI_note / 12) - 1;

          let text;
          if (this.is_midi_note) {
            text = MIDI_note;
          } else {
            text = this.all_notes[noteIndex] + octave;
          }
          let color = "";
          let border = "";
          let text_color = "";
          let z_index = "";

          let note_object = {};
          note_object.note = MIDI_note;

          note_object.string = i;

          const pressed_note = this.pressed_notes.find((obj) => obj.note === note_object.note && obj.string === note_object.string);
          const playing_note = this.playing_notes.find((obj) => obj.note === note_object.note && obj.string === note_object.string);
          const is_in_scale = note_pattern.includes(noteIndex);

          if (is_in_scale && !pressed_note && !playing_note) {
            color = this.colors.normal[noteIndex];
            z_index = "0";
          } else if (!is_in_scale) {
            color = "grey";
            border = "1px solid white";
            text = "";
          }
          if (this.pressed_notes.length > 0 && pressed_note && is_in_scale) {
            color = this.is_user_message ? "purple" : "black";
            border = "10px solid white";
            text_color = "white";
            z_index = "100";
          } else if (this.pressed_notes.length > 0 && pressed_note && !is_in_scale) {
            color = "black";
            border = "10px solid red";
            text_color = "white";
            z_index = "100";
          }
          if (this.playing_notes.length > 0 && playing_note && is_in_scale) {
            console.log("this.playing_notes.color: ", playing_note.color);
            color = this.is_user_message ? "purple" : "black";
            // border = this.is_user_message ? "10px solid orange" : "10px solid white";
            border = "10px solid white";
            border = this.is_user_message && playing_note.color ? "10px solid " + playing_note.color : border;
            text_color = "white";
            z_index = "100";
          } else if (this.playing_notes.length > 0 && playing_note && !is_in_scale) {
            color = "red";
            border = "10px solid black";
            z_index = "100";
          }
          fretboardSlots.push(new fretSlot(MIDI_note, i, j, color, text, this.are_circles, border, text_color, z_index));
        }
      }
      return fretboardSlots;
    },

    update_pressed_notes(midi_message) {
      if (Array.isArray(midi_message.channel) && Array.isArray(midi_message.value)) {
        for (let i = 0; i < midi_message.channel.length; i++) {
          let pressed_string = this.map_jamstik_strings[midi_message.channel[i]];
          if (
            midi_message.message_type == "cc" &&
            !this.pressed_notes.some((obj) => obj.note === midi_message.value[i] && obj.string === pressed_string)
          ) {
            let note_object = {};
            note_object.note = midi_message.value[i];
            note_object.string = pressed_string;

            if (midi_message.value[i] == this.openStringMidiNotes[note_object.string]) {
              this.pressed_notes = this.pressed_notes.filter((obj) => obj.string !== note_object.string);
            } else {
              this.pressed_notes.push(note_object);
            }
          }
        }
      } else {
        let pressed_string = this.map_jamstik_strings[midi_message.channel];

        let noteIsNotInPressedNotes = !this.pressed_notes.some((obj) => obj.note === midi_message.value && obj.string === pressed_string);

        if (midi_message.message_type == "cc" && noteIsNotInPressedNotes) {
          let note_object = {};
          note_object.note = midi_message.value;
          note_object.string = pressed_string;
          let isOpenString = midi_message.value == this.openStringMidiNotes[note_object.string];
          if (isOpenString) {
            // Remove all pressed notes from string
            this.pressed_notes = this.pressed_notes.filter((obj) => obj.string !== note_object.string);
          } else {
            this.pressed_notes.push(note_object);
          }
        }
      }
    },
    add_to_playing_notes(midi_message) {
      if (Array.isArray(midi_message.channel) && Array.isArray(midi_message.note)) {
        for (let i = 0; i < midi_message.channel.length; i++) {
          let pressed_string = this.map_jamstik_strings[midi_message.channel[i]];
          if (
            midi_message.message_type == "note_on" &&
            midi_message.velocity !== 0 &&
            !this.playing_notes.some((obj) => obj.note === midi_message.note[i] && obj.string === pressed_string)
          ) {
            let note_object = {};
            note_object.note = midi_message.note[i];
            note_object.string = pressed_string;
            if (midi_message.color) {
              note_object.color = midi_message.color;
              console.log("yes: ", note_object.color);
            }
            this.playing_notes.push(note_object);
          }
        }
      } else {
        let pressed_string = this.map_jamstik_strings[midi_message.channel];
        if (
          midi_message.message_type == "note_on" &&
          midi_message.velocity !== 0 &&
          !this.playing_notes.some((obj) => obj.note === midi_message.note && obj.string === pressed_string)
        ) {
          let note_object = {};
          note_object.note = midi_message.note;
          note_object.string = pressed_string;
          if (midi_message.color) {
            note_object.color = midi_message.color;
            console.log("yes: ", note_object.color);
          }
          this.playing_notes.push(note_object);
        }
      }
    },
    remove_from_playing_notes(midi_message) {
      // Use MIDI off message to create a lift finger message
      let open_note = this.openStringMidiNotes[this.map_jamstik_strings[midi_message.channel]];
      let midi_message_lift = new MIDIccMessage(midi_message.channel, MIDIMessageTypesStrings.cc, 0, open_note);
      this.update_pressed_notes(midi_message_lift);
      // Actual remove from playing notes
      let pressed_string = this.map_jamstik_strings[midi_message.channel];
      if (
        midi_message.message_type == "note_off" &&
        midi_message.velocity == 0 &&
        this.playing_notes.some((obj) => obj.note === midi_message.note && obj.string === pressed_string)
      ) {
        console.log("remove_from_playing_notes: ", midi_message);
        let note_object = {};
        note_object.note = midi_message.note;
        note_object.string = pressed_string;
        this.playing_notes = this.playing_notes.filter((item) => item.note !== midi_message.note || item.string !== pressed_string);
      }
    },
    fretStyle(fretSlot) {
      if (fretSlot.is_circle) {
        return {
          display: "grid",
          placeItems: "center",
          backgroundColor: fretSlot.color,
          width: "20px",
          height: "20px",
          borderRadius: "50%",
          border: fretSlot.border,
          color: fretSlot.text_color,
          outline: fretSlot.border,
          zIndex: fretSlot.z_index,
        };
      } else {
        return {
          display: "grid",
          placeItems: "center",
          backgroundColor: fretSlot.color,
          width: "100%",
          height: "100%",
          color: fretSlot.text_color,
          outline: fretSlot.border,
          zIndex: fretSlot.z_index,
        };
      }
    },
  },
  mounted() {
    this.$parent.$on("note-pattern", (data) => {
      this.notePattern = data;
    });
    this.$parent.$on("are-circles", (data) => {
      this.are_circles = data;
    });
    this.$parent.$on("is-midi-note", (data) => {
      this.is_midi_note = data;
    });
    this.$parent.$on("capo-fret", (data) => {
      this.capo = data;
    });

    this.$parent.$on("MIDI-message-app", (data) => {
      this.is_user_message = false;
      if (data.message_type == "cc") {
        this.update_pressed_notes(data);
      } else if (data.message_type == "note_on") {
        this.add_to_playing_notes(data);
      } else if (data.message_type == "note_off") {
        this.remove_from_playing_notes(data);
      }
    });
    this.$parent.$on("MIDI-message-user", (data) => {
      console.log("MIDI-message-user: ", data);
      this.is_user_message = true;
      if (data.message_type == "cc") {
        this.update_pressed_notes(data, this.is_user_message);
      } else if (data.message_type == "note_on") {
        this.add_to_playing_notes(data, this.is_user_message);
      } else if (data.message_type == "note_off") {
        this.remove_from_playing_notes(data, this.is_user_message);
      }
    });
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
