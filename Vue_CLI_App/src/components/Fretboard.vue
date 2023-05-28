<template>
  <div>
    <div class="the-grid" :style="fretboardGridStyle" @click="debug">
      <div
        v-bind:style="{ display: 'grid', placeItems: 'center' }"
        v-for="(fretSlot, index) in draw_fretboard_slots(notePattern)"
        :key="index"
      >
        <div :style="fretStyle(fretSlot)">
          {{ fretSlot.text }}
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import fretSlot from "../utils/fretSlot";

export default {
  name: "Fretboard",
  data() {
    return {
      midi_message: "",
      number_of_strings: 6,
      number_of_frets: 20,
      string_length: 647.7,
      all_notes: ["C", "C♯/D♭", "D", "D♯/E♭", "E", "F", "F♯/G♭", "G", "G♯/A♭", "A", "A♯/B♭", "B"],
      tuning: ["E", "A", "D", "G", "B", "E"],
      open_string_MIDI_notes: [40, 45, 50, 55, 59, 64],
      pressed_notes: [],
      playing_notes: [],
      midi_cc_msg: "",
      are_circles: false,
      notePattern: [],
      colors: {
        normal: [
          "#7FDBFF",
          "#39CCCC",
          "#3D9970",
          "#2ECC40",
          "#01FF70",
          "#FFDC00",
          "#FF851B",
          "#FF4136",
          "#85144b",
          "#F012BE",
          "#B10DC9",
          "#0074D9",
        ],
        pressed: "black",
        playing: "black",
      },
    };
  },
  computed: {
    fretboardSlotsAmount: function() {
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
    draw_fretboard_slots(scale_notes_steps) {
      if (scale_notes_steps.length == 0) {
        scale_notes_steps = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11];
      }
      let fretboardSlots = [];
      for (let i = this.number_of_strings - 1; i >= 0; i--) {
        let firstNoteOfString = this.tuning[i];
        for (let j = 0; j < this.number_of_frets; j++) {
          let noteIndex = (this.all_notes.indexOf(firstNoteOfString) + j) % 12;
          let MIDI_note = this.open_string_MIDI_notes[i] + j;
          // let text = MIDI_note;
          let text = this.all_notes[noteIndex];
          let color = "";
          let border = "";
          let text_color = "";
          let z_index = "";

          let note_object = {};
          note_object.note = MIDI_note;
          note_object.fret = j;

          const is_pressed = this.pressed_notes.some((obj) => obj.note === note_object.note && obj.fret === note_object.fret);
          const is_playing = this.playing_notes.some((obj) => obj.note === note_object.note && obj.fret === note_object.fret);
          const is_in_scale = scale_notes_steps.includes(noteIndex);

          if (is_in_scale && !is_pressed && !is_playing) {
            color = this.colors.normal[noteIndex];
            z_index = "0";
          } else if (!is_in_scale) {
            color = "grey";
            border = "1px solid white";
            text = "";
          }
          if (this.pressed_notes.length > 0 && is_pressed && is_in_scale) {
            color = "black";
            border = "";
            text_color = "white";
            z_index = "100";
          } else if (this.pressed_notes.length > 0 && is_pressed && !is_in_scale) {
            color = "black";
            border = "10px solid red";
            text_color = "white";
            z_index = "100";
          }
          if (this.playing_notes.length > 0 && is_playing && is_in_scale) {
            color = "black";
            border = "10px solid white";
            text_color = "white";
            z_index = "100";
          } else if (this.playing_notes.length > 0 && is_playing && !is_in_scale) {
            color = "red";
            border = "10px solid black";
            z_index = "100";
          }
          fretboardSlots.push(new fretSlot(MIDI_note, i, j, color, text, this.are_circles, border, text_color, z_index));
        }
      }
      return fretboardSlots;
    },
    add_to_pressed_notes(midi_message, pressed_fret) {
      if (
        midi_message.message_type == "note_on" &&
        midi_message.velocity == 0 &&
        !this.pressed_notes.some((obj) => obj.note === midi_message.note && obj.fret === pressed_fret) &&
        pressed_fret != 0
      ) {
        let note_object = {};
        note_object.note = midi_message.note;
        note_object.fret = pressed_fret;
        this.pressed_notes.push(note_object);
      }
    },
    remove_from_pressed_notes(midi_message, lifted_fret) {
      if (
        midi_message.message_type == "note_on" &&
        midi_message.velocity == 0 &&
        this.pressed_notes.some((obj) => obj.note === midi_message.note && obj.fret === lifted_fret)
      ) {
        let note_object = {};
        note_object.note = midi_message.note;
        note_object.fret = lifted_fret;
        this.pressed_notes = this.pressed_notes.filter((obj) => obj.note !== note_object.note || obj.fret !== note_object.fret);
      }
    },
    add_to_playing_notes(midi_message, pressed_fret) {
      if (
        midi_message.message_type == "note_on" &&
        midi_message.velocity !== 0 &&
        !this.playing_notes.some((obj) => obj.note === midi_message.note && obj.fret === pressed_fret)
      ) {
        let note_object = {};
        note_object.note = midi_message.note;
        note_object.fret = pressed_fret;
        this.playing_notes.push(note_object);
      }
    },
    remove_from_playing_notes(midi_message, pressed_fret) {
      if (
        midi_message.message_type == "note_off" &&
        midi_message.velocity == 0 &&
        this.playing_notes.some((obj) => obj.note === midi_message.note && obj.fret === pressed_fret)
      ) {
        let note_object = {};
        note_object.note = midi_message.note;
        note_object.fret = pressed_fret;
        this.playing_notes = this.playing_notes.filter((item) => item.note !== midi_message.note || item.fret !== pressed_fret);
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

    this.$parent.$on("MIDI-message", (data) => {
      if (data.message_type == "cc") {
        this.midi_cc_msg = data;
      } else if (data.message_type == "note_on" || data.message_type == "note_off") {
        switch (this.midi_cc_msg.cc) {
          case 20:
            this.add_to_playing_notes(data, this.midi_cc_msg.value);
            break;
          case 21:
            this.remove_from_playing_notes(data, this.midi_cc_msg.value);
            break;
          case 22:
            this.add_to_pressed_notes(data, this.midi_cc_msg.value);
            break;
          case 23:
            this.remove_from_pressed_notes(data, this.midi_cc_msg.value);
            break;

          default:
            break;
        }
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
