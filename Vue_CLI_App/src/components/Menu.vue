<template>
  <div class="menu column">
    <div>
      <div>Root Note</div>
      <select v-model="selected_root_note" v-on:change="emit_steps(selected_scale, selected_root_note)">
        <option v-for="(note, index) in notes" :value="note" :key="index">
          {{ note }}
        </option>
      </select>
    </div>

    <div>
      <div>Scale</div>
      <select v-model="selected_scale" v-on:change="emit_steps(selected_scale, selected_root_note)">
        <option v-for="(scale, index) in scales" :value="scale.name" :key="index">
          {{ scale.name }}
        </option>
      </select>
    </div>

    <div>
      <div>Style</div>
      <select v-model="selected_style" v-on:change="setFretSlotStyle(selected_style)">
        <option value="false" selected>Full Color</option>
        <option value="true">Circles</option>
      </select>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      selected_style: "false",
      selected_scale: "Chromatic",
      selected_root_note: "C",
      name: "Menu",
      note_mapping: {
        "1": 0,
        "♯1": 1,
        "♭2": 1,
        "2": 2,
        "♯2": 3,
        "♭3": 3,
        "3": 4,
        "4": 5,
        "♯4": 6,
        "♭5": 6,
        "5": 7,
        "♯5": 8,
        "♭6": 8,
        "6": 9,
        "♯6": 10,
        "♭7": 10,
        "7": 11
      },
      notes: ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"],
      scales: [
        {
          name: "All Notes",
          pattern: ["1", "♯1", "2", "♯2", "3", "4", "♯4", "5", "♯5", "6", "♯6", "7"]
        },
        {
          name: "Acoustic",
          pattern: ["1", "2", "3", "♯4", "5", "6", "♭7"]
        },
        {
          name: "Aeolian Mode",
          pattern: ["1", "2", "♭3", "4", "5", "♭6", "♭7"]
        },
        {
          name: "Algerian",
          pattern: ["1", "2", "♭3", "♯4", "5", "♭6", "7"]
        },
        {
          name: "Altered",
          pattern: ["1", "♭2", "♭3", "♭4", "♭5", "♭6", "♭7"]
        },
        {
          name: "Augmented",
          pattern: ["1", "♭3", "3", "5", "♯5", "7"]
        },
        {
          name: "Bebop Dominant",
          pattern: ["1", "2", "3", "4", "5", "6", "♭7", "7"]
        },
        {
          name: "Blues",
          pattern: ["1", "♭3", "4", "♭5", "5", "♭7"]
        },
        {
          name: "Chromatic",
          pattern: ["1", "♯1", "2", "♯2", "3", "4", "♯4", "5", "♯5", "6", "♯6", "7"]
        },
        {
          name: "Dorian Mode",
          pattern: ["1", "2", "♭3", "4", "5", "6", "♭7"]
        },
        {
          name: "Double harmonic",
          pattern: ["1", "♭2", "3", "4", "5", "♭6", "7"]
        },
        {
          name: "Enigmatic",
          pattern: ["1", "♭2", "3", "♯4", "♯5", "♯6", "7"]
        },
        {
          name: "Flamenco Mode",
          pattern: ["1", "♭2", "3", "4", "5", "♭6", "7"]
        },
        {
          name: "Gypsy",
          pattern: ["1", "2", "♭3", "♯4", "5", "♭6", "♭7"]
        },
        {
          name: "Half Diminished",
          pattern: ["1", "2", "♭3", "4", "♭5", "♭6", "♭7"]
        },
        {
          name: "Harmonic Major",
          pattern: ["1", "2", "3", "4", "5", "♭6", "7"]
        },
        {
          name: "Harmonic Minor",
          pattern: ["1", "2", "♭3", "4", "5", "♭6", "7"]
        },
        {
          name: "Hirajoshi",
          pattern: ["1", "3", "♯4", "5", "7"]
        },
        {
          name: "Hungarian Gypsy",
          pattern: ["1", "2", "♭3", "♯4", "5", "♭6", "7"]
        },
        {
          name: "Hungarian Major",
          pattern: ["1", "♯2", "3", "♯4", "5", "6", "♭7"]
        },
        {
          name: "In",
          pattern: ["1", "♭2", "4", "5", "♭6"]
        },
        {
          name: "Insen",
          pattern: ["1", "♭2", "4", "5", "♭7"]
        },
        {
          name: "Major",
          pattern: ["1", "2", "3", "4", "5", "6", "7"]
        },
        {
          name: "Istrian",
          pattern: ["1", "♭2", "♭3", "♭4", "♭5", "5"]
        },
        { name: "Iwato", pattern: ["1", "♭2", "4", "♭5", "♭7"] },
        {
          name: "Locrian Mode",
          pattern: ["1", "♭2", "♭3", "4", "♭5", "♭6", "♭7"]
        },
        {
          name: "Lydian Augmented",
          pattern: ["1", "2", "3", "♯4", "♯5", "6", "7"]
        },
        {
          name: "Lydian Mode",
          pattern: ["1", "2", "3", "♯4", "5", "6", "7"]
        },
        {
          name: "Major Bebop",
          pattern: ["1", "2", "3", "4", "5", "♭6", "6", "7"]
        },
        {
          name: "Major Locrian",
          pattern: ["1", "2", "3", "4", "♭5", "♭6", "♭7"]
        },
        {
          name: "Major Pentatonic",
          pattern: ["1", "2", "3", "5", "6"]
        },
        {
          name: "Melodic Minor",
          pattern: ["1", "2", "♭3", "4", "5", "6", "7"]
        },
        {
          name: "Minor Pentatonic",
          pattern: ["1", "♭3", "4", "5", "♭7"]
        },
        {
          name: "Mixolydian Mode",
          pattern: ["1", "2", "3", "4", "5", "6", "♭7"]
        },
        {
          name: "Neapolitan Major",
          pattern: ["1", "♭2", "♭3", "4", "5", "6", "7"]
        },
        {
          name: "Neapolitan Minor",
          pattern: ["1", "♭2", "♭3", "4", "5", "♭6", "7"]
        },
        {
          name: "Octatonic 1",
          pattern: ["1", "2", "♭3", "4", "♭5", "♭6", "6", "7"]
        },
        {
          name: "Octatonic 2",
          pattern: ["1", "♭2", "♭3", "3", "♯4", "5", "6", "♭7"]
        },
        {
          name: "Persian",
          pattern: ["1", "♭2", "3", "4", "♭5", "♭6", "7"]
        },
        {
          name: "Phrygian Dominant",
          pattern: ["1", "♭2", "3", "4", "5", "♭6", "♭7"]
        },
        {
          name: "Phrygian Mode",
          pattern: ["1", "♭2", "♭3", "4", "5", "♭6", "♭7"]
        },
        {
          name: "Prometheus",
          pattern: ["1", "2", "3", "♯4", "6", "♭7"]
        },
        {
          name: "Scale of Harmonics",
          pattern: ["1", "♭3", "3", "4", "5", "6"]
        },
        {
          name: "Tritone",
          pattern: ["1", "♭2", "3", "♭5", "5", "♭7"]
        },
        {
          name: "Two-semitone Tritone",
          pattern: ["1", "♭2", "2", "♯4", "5", "♭6"]
        },
        {
          name: "Ukrainian Dorian",
          pattern: ["1", "2", "♭3", "♯4", "5", "6", "♭7"]
        },
        {
          name: "Whole Tone",
          pattern: ["1", "2", "3", "♯4", "♯5", "♯6"]
        },
        { name: "Yo", pattern: ["1", "♭3", "4", "5", "♭7"] }
      ]
    };
  },
  methods: {
    emit_steps(scale_name, root_note) {
      let index_of_scale = null;
      let index_of_root_note = null;

      // Get index of root note
      for (let i = 0; i < this.notes.length; i++) {
        if (this.notes[i] == root_note) {
          index_of_root_note = i;
        }
      }

      // Get index of scale
      for (let i = 0; i < this.scales.length; i++) {
        if (this.scales[i].name == scale_name) {
          index_of_scale = i;
        }
      }
      let note_pattern = this.scales[index_of_scale].pattern;

      // Convert note pattern to steps
      let note_steps = [];
      for (let i = 0; i < note_pattern.length; i++) {
        note_steps[i] = this.note_mapping[note_pattern[i]];
      }

      // Add steps to root note
      for (let i = 0; i < note_steps.length; i++) {
        note_steps[i] += index_of_root_note;
        note_steps[i] = note_steps[i] % 12;
      }
      this.$parent.$emit("note-pattern", note_steps);
    },
    setFretSlotStyle(are_circles) {
      if (are_circles == "true") {
        are_circles = true;
      } else if (are_circles == "false") {
        are_circles = false;
      }
      this.$parent.$emit("are-circles", are_circles);
    }
  }
};
</script>

<style scoped>
.menu {
  width: 100%;
  height: 100%;
  color: azure;
  background-color: rgb(41, 41, 41);
  padding: 30px;
  overflow-y: scroll;
  display: flex;
  row-gap: 15px;
}

.row {
  display: flex;
  flex-direction: row;
}

.column {
  display: flex;
  flex-direction: column;
}
</style>
