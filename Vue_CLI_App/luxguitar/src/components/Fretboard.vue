<template>
  <div>
    <div class="the-grid" :style="fretboardGridStyle" @click="debug">
      <div
        v-bind:style="{ display: 'grid', placeItems: 'center' }"
        v-for="(fretSlot, index) in fretboardSlots(notePattern)"
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
      numberOfStrings: 6,
      numberOfFrets: 20,
      stringLength: 647.7,
      notes: [
        "C",
        "C♯/D♭",
        "D",
        "D♯/E♭",
        "E",
        "F",
        "F♯/G♭",
        "G",
        "G♯/A♭",
        "A",
        "A♯/B♭",
        "B"
      ],
      colors: [
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
        "#0074D9"
      ],
      tuning: ["E", "A", "D", "G", "B", "E"],
      areCircles: true,
      notePattern: []
    };
  },
  computed: {
    fretboardSlotsAmount: function() {
      return this.numberOfStrings * this.numberOfFrets;
    },
    fretDistances() {
      let fretDist = [];
      // Adjustment for screen resolution distance discrepancies
      let newLength = this.stringLength * 0.867469097;
      // let newLength = this.stringLength * 0.85981;
      // let newLength = this.stringLength * 0.85981551561;
      for (let i = 0; i < this.numberOfFrets; i++) {
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
        gridTemplateRows: "repeat(" + this.numberOfStrings + ", 1fr )",
        width: this.fretboardLength + "mm"
      };
    }
  },
  methods: {
    debug() {
      console.log(this.fretboardSlots);
    },
    fretboardSlots(selectedNotes) {
      let fretboardSlots = [];
      for (let i = this.tuning.length - 1; i >= 0; i--) {
        let firstNoteOfString = this.tuning[i];
        for (let j = 0; j < this.numberOfFrets; j++) {
          let noteIndex = (j + this.notes.indexOf(firstNoteOfString) + 1) % 12;
          let note = this.notes[noteIndex];
          let color = "";
          if (selectedNotes.includes(noteIndex)) {
            color = this.colors[noteIndex];
            fretboardSlots.push(
              new fretSlot(note, i, j, color, note, this.areCircles)
            );
          } else if (selectedNotes.length > 0) {
            color = "black";
            fretboardSlots.push(
              new fretSlot(
                note,
                i,
                j,
                color,
                "",
                this.areCircles,
                "1px solid white"
              )
            );
          } else {
            color = this.colors[noteIndex];
            fretboardSlots.push(
              new fretSlot(note, i, j, color, note, this.areCircles)
            );
          }
        }
      }
      return fretboardSlots;
    },
    fretStyle(fretSlot) {
      if (fretSlot.isCircle == true) {
        return {
          display: "grid",
          placeItems: "center",
          backgroundColor: fretSlot.color,
          width: "20px",
          height: "20px",
          borderRadius: "50%",
          border: fretSlot.border
        };
      } else {
        return {
          display: "grid",
          placeItems: "center",
          backgroundColor: fretSlot.color,
          width: "100%",
          height: "100%"
        };
      }
    }
  },
  mounted() {
    this.$parent.$on("note-pattern", data => {
      this.notePattern = data;
    });
    this.$parent.$on("are-circles", data => {
      console.log(data);
      this.areCircles = data;
    });
  }
};
</script>

<style scoped>
.the-grid {
  display: grid;
  height: 100%;
  background-color: black;
}
</style>
