<template>
  <div class="menu column">
    <h4 class="menu-title">Fretboard Menu:</h4>

    <div class="row menu-container">
      <div class="menu-item">
        <div>Root Note:</div>
        <select v-model="selected_root_note" v-on:change="emit_steps(selected_scale, selected_root_note)">
          <option v-for="(note, index) in notes" :value="note" :key="index">
            {{ note }}
          </option>
        </select>
      </div>

      <div class="menu-item">
        <div>Scale:</div>
        <select v-model="selected_scale" v-on:change="emit_steps(selected_scale, selected_root_note)">
          <option v-for="(scale, index) in scales" :value="scale.name" :key="index">
            {{ scale.name }}
          </option>
        </select>
      </div>

      <div class="menu-item">
        <div>Style:</div>
        <select v-model="selected_style" v-on:change="setFretSlotStyle(selected_style)">
          <option value="false" selected>Full Color</option>
          <option value="true">Circles</option>
        </select>
      </div>
      <div class="menu-item">
        <div>Text:</div>
        <select v-model="selected_name" v-on:change="setIsMidiNote(selected_name)">
          <option value="false" selected>Notes</option>
          <option value="true">MIDI Note</option>
        </select>
      </div>
      <div class="menu-item">
        <div>Capo:</div>
        <button @click="move_capo_up">+</button>
        <div>{{ capo }}</div>
        <button @click="move_capo_down">-</button>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      selected_style: "false",
      selected_name: "MIDI",
      selected_scale: "Chromatic",
      selected_root_note: "C",
      name: "Menu",
      capo: 3,
      note_mapping: {
        1: 0,
        "♯1": 1,
        "♭2": 1,
        2: 2,
        "♯2": 3,
        "♭3": 3,
        3: 4,
        4: 5,
        "♯4": 6,
        "♭5": 6,
        5: 7,
        "♯5": 8,
        "♭6": 8,
        6: 9,
        "♯6": 10,
        "♭7": 10,
        7: 11,
      },
      notes: ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"],
      scales: [
        {
          name: "Chromatic",
          pattern: ["1", "♯1", "2", "♯2", "3", "4", "♯4", "5", "♯5", "6", "♯6", "7"],
          description: "All Notes.",
        },
        {
          name: "Acoustic",
          pattern: ["1", "2", "3", "♯4", "5", "6", "♭7"],
          description: "Major scale with a natural seventh.",
        },
        {
          name: "Aeolian Mode",
          pattern: ["1", "2", "♭3", "4", "5", "♭6", "♭7"],
          description: "Natural minor scale.",
        },
        {
          name: "Algerian",
          pattern: ["1", "2", "♭3", "♯4", "5", "♭6", "7"],
          description: "Arabian scale with a raised fourth and seventh.",
        },
        {
          name: "Altered",
          pattern: ["1", "♭2", "♭3", "♭4", "♭5", "♭6", "♭7"],
          description: "Super Locrian scale, used in jazz.",
        },
        {
          name: "Augmented",
          pattern: ["1", "♭3", "3", "5", "♯5", "7"],
          description: "Major scale with a raised fifth.",
        },
        {
          name: "Bebop Dominant",
          pattern: ["1", "2", "3", "4", "5", "6", "♭7", "7"],
          description: "Mixolydian scale with an added chromatic passing tone.",
        },
        {
          name: "Blues",
          pattern: ["1", "♭3", "4", "♭5", "5", "♭7"],
          description: "Blues scale (1, b3, 4, b5, 5, b7).",
        },
        {
          name: "Dorian Mode",
          pattern: ["1", "2", "♭3", "4", "5", "6", "♭7"],
          description: "Natural minor scale with a raised sixth.",
        },
        {
          name: "Double harmonic",
          pattern: ["1", "♭2", "3", "4", "5", "♭6", "7"],
          description: "Byzantine scale with augmented steps.",
        },
        {
          name: "Enigmatic",
          pattern: ["1", "♭2", "3", "♯4", "♯5", "♯6", "7"],
          description: "Octatonic scale with an added note.",
        },
        {
          name: "Flamenco Mode",
          pattern: ["1", "♭2", "3", "4", "5", "♭6", "7"],
          description: "Phrygian scale with a raised third.",
        },
        {
          name: "Gypsy",
          pattern: ["1", "2", "♭3", "♯4", "5", "♭6", "♭7"],
          description: "Hungarian Minor scale.",
        },
        {
          name: "Half Diminished",
          pattern: ["1", "2", "♭3", "4", "♭5", "♭6", "♭7"],
          description: "Locrian scale with a natural fifth.",
        },
        {
          name: "Harmonic Major",
          pattern: ["1", "2", "3", "4", "5", "♭6", "7"],
          description: "Major scale with a lowered sixth.",
        },
        {
          name: "Harmonic Minor",
          pattern: ["1", "2", "♭3", "4", "5", "♭6", "7"],
          description: "Natural minor scale with a raised seventh.",
        },
        {
          name: "Hirajoshi",
          pattern: ["1", "3", "♯4", "5", "7"],
          description: "Pentatonic scale from traditional Japanese music.",
        },
        {
          name: "Hungarian Gypsy",
          pattern: ["1", "2", "♭3", "♯4", "5", "♭6", "7"],
          description: "Hungarian Minor scale.",
        },
        {
          name: "Hungarian Major",
          pattern: ["1", "♯2", "3", "♯4", "5", "6", "♭7"],
          description: "Major scale with a raised fourth and lowered seventh.",
        },
        {
          name: "In",
          pattern: ["1", "♭2", "4", "5", "♭6"],
          description: "Japanese scale used in traditional court music.",
        },
        {
          name: "Insen",
          pattern: ["1", "♭2", "4", "5", "♭7"],
          description: "Pentatonic scale from traditional Japanese music.",
        },
        {
          name: "Major",
          pattern: ["1", "2", "3", "4", "5", "6", "7"],
          description: "Ionian scale.",
        },
        {
          name: "Istrian",
          pattern: ["1", "♭2", "♭3", "♭4", "♭5", "5"],
          description: "Istrian scale with augmented steps.",
        },
        {
          name: "Iwato",
          pattern: ["1", "♭2", "4", "♭5", "♭7"],
          description: "Pentatonic scale from traditional Japanese music.",
        },
        {
          name: "Locrian Mode",
          description: "Diminished scale starting on the seventh degree.",
          pattern: ["1", "♭2", "♭3", "4", "♭5", "♭6", "♭7"],
        },
        {
          name: "Lydian Augmented",
          description: "Lydian scale with a raised fifth.",
          pattern: ["1", "2", "3", "♯4", "♯5", "6", "7"],
        },
        {
          name: "Lydian Mode",
          pattern: ["1", "2", "3", "♯4", "5", "6", "7"],
          description: "Major scale with a raised fourth.",
        },
        {
          name: "Major Bebop",
          pattern: ["1", "2", "3", "4", "5", "♭6", "6", "7"],
          description: "Major scale with an added chromatic passing tone.",
        },
        {
          name: "Major Locrian",
          pattern: ["1", "2", "3", "4", "♭5", "♭6", "♭7"],
          description: "Locrian scale with a raised third.",
        },
        {
          name: "Major Pentatonic",
          pattern: ["1", "2", "3", "5", "6"],
          description: "Pentatonic scale with a major third.",
        },
        {
          name: "Melodic Minor",
          pattern: ["1", "2", "♭3", "4", "5", "6", "7"],
          description: "Minor scale with a raised sixth and seventh (ascending).",
        },
        {
          name: "Minor Pentatonic",
          pattern: ["1", "♭3", "4", "5", "♭7"],
          description: "Pentatonic scale with a minor third.",
        },
        {
          name: "Mixolydian Mode",
          pattern: ["1", "2", "3", "4", "5", "6", "♭7"],
          description: "Major scale with a lowered seventh.",
        },
        {
          name: "Neapolitan Major",
          pattern: ["1", "♭2", "♭3", "4", "5", "6", "7"],
          description: "Major scale with a lowered second.",
        },
        {
          name: "Neapolitan Minor",
          pattern: ["1", "♭2", "♭3", "4", "5", "♭6", "7"],
          description: "Natural minor scale with a lowered second.",
        },
        {
          name: "Octatonic 1",
          pattern: ["1", "2", "♭3", "4", "♭5", "♭6", "6", "7"],
          description: "Diminished scale.",
        },
        {
          name: "Octatonic 2",
          pattern: ["1", "♭2", "♭3", "3", "♯4", "5", "6", "♭7"],
          description: "Another form of the diminished scale.",
        },
        {
          name: "Persian",
          pattern: ["1", "♭2", "3", "4", "♭5", "♭6", "7"],
          description: "Persian scale with augmented steps.",
        },
        {
          name: "Phrygian Dominant",
          pattern: ["1", "♭2", "3", "4", "5", "♭6", "♭7"],
          description: "Phrygian scale with a raised third.",
        },
        {
          name: "Phrygian Mode",
          pattern: ["1", "♭2", "♭3", "4", "5", "♭6", "♭7"],
          description: "Natural minor scale with a lowered second.",
        },
        {
          name: "Prometheus",
          pattern: ["1", "2", "3", "♯4", "6", "♭7"],
          description: "Octatonic scale with an added note.",
        },
        {
          name: "Scale of Harmonics",
          pattern: ["1", "♭3", "3", "4", "5", "6"],
          description: "Harmonic series-based scale.",
        },
        {
          name: "Tritone",
          pattern: ["1", "♭2", "3", "♭5", "5", "♭7"],
          description: "Diminished scale.",
        },
        {
          name: "Two-semitone Tritone",
          pattern: ["1", "♭2", "2", "♯4", "5", "♭6"],
          description: "Diminished scale with two semitones.",
        },
        {
          name: "Ukrainian Dorian",
          pattern: ["1", "2", "♭3", "♯4", "5", "6", "♭7"],
          description: "Dorian scale with a raised fourth.",
        },
        {
          name: "Whole Tone",
          pattern: ["1", "2", "3", "♯4", "♯5", "♯6"],
          description: "All whole steps.",
        },
        {
          name: "Yo",
          pattern: ["1", "♭3", "4", "5", "♭7"],
          description: "Pentatonic scale from traditional Japanese music.",
        },
      ],
    };
  },
  methods: {
    move_capo_up() {
      this.capo++;
      this.$parent.$emit("capo-fret", this.capo);
    },
    move_capo_down() {
      if (this.capo > 0) {
        this.capo--;
        this.$parent.$emit("capo-fret", this.capo);
      }
    },
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
    },
    setIsMidiNote(is_midi_note) {
      if (is_midi_note == "true") {
        is_midi_note = true;
      } else if (is_midi_note == "false") {
        is_midi_note = false;
      }
      this.$parent.$emit("is-midi-note", is_midi_note);
    },
  },
};
</script>

<style scoped></style>
