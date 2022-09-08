<template>
  <div class="afterFretboardDisplay">
    <div @click="setFretSlotShape(true)">Circles</div>
    <div @click="setFretSlotShape(false)">Squares</div>
    <div v-for="(scale, index) in scales" :key="index">
      <h5 @click="setFretboard(scale.pattern)">{{ scale.name }}</h5>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      name: "AfterFretboardDisplay",
      noteMapping: {
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
      scales: [
        {
          name: "All Notes",
          pattern: [
            "1",
            "♯1",
            "2",
            "♯2",
            "3",
            "4",
            "♯4",
            "5",
            "♯5",
            "6",
            "♯6",
            "7"
          ]
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
          pattern: [
            "1",
            "♯1",
            "2",
            "♯2",
            "3",
            "4",
            "♯4",
            "5",
            "♯5",
            "6",
            "♯6",
            "7"
          ]
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
        { name: "In", pattern: ["1", "♭2", "4", "5", "♭6"] },
        { name: "Insen", pattern: ["1", "♭2", "4", "5", "♭7"] },
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
      ],
      notePattern: []
    };
  },
  methods: {
    setFretboard(notePattern) {
      let semitones = this.patternToSemitones(notePattern);
      this.$parent.$emit("note-pattern", semitones);
    },
    setFretSlotShape(areCircles) {
      this.$parent.$emit("are-circles", areCircles);
    },
    patternToSemitones(degreesArray) {
      let semitones = [];
      for (let i = 0; i < degreesArray.length; i++) {
        semitones[i] = this.noteMapping[degreesArray[i]];
      }
      return semitones;
    }
  }
};
</script>

<style scoped>
.afterFretboardDisplay {
  width: 100%;
  height: 100%;
  color: azure;
  background-color: rgb(41, 41, 41);
  padding: 30px;
  overflow-y: scroll;
}
</style>
