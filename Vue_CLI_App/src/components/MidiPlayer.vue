<template>
  <div class="menu column">
    <h4 class="menu-title">Music Menu:</h4>
    <div class="row menu-container">
      <button href="#" @click.prevent="startMusic()">Start Music</button>
      <button href="#" @click.prevent="stopMusic()">Stop Music</button>
      <button @click="goToPreviousPart()">Previous</button>
      <div id="part-counter-container">
        <p id="part-counter" :class="{ grow: nextLevelClass, shrink: !nextLevelClass }" @click="goToPreviousPart()">Part {{ selectedIndex }}</p>
      </div>
      <button @click="goToNextPart()">Next</button>
      <button @click="activateMidi()">Activate MIDI</button>
      <button @click="stopMidi()">Deactivate MIDI</button>

      <input type="number" v-model="selectedIndex" @input="validateIndex" style="width: 40px" />
    </div>

    <div class="feedback-items">
      <table v-if="enabledNoteTimeDifferences" class="time-differences-table">
        <thead>
          <tr>
            <th>Note</th>
            <th>Timing</th>
            <th></th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="(item, index) in timeDifferencesMelody" :key="index">
            <td>{{ index + 1 }}</td>
            <td>
              {{ item }}
              <span v-if="item !== ''">
                {{ item > 0 ? "(late)" : "(early)" }}
              </span>
            </td>
            <td class="grade-container">
              <span v-if="item !== '' && enabledShowMessages" :style="{ color: getColorForNoteGrade(item) }" class="grade-message">{{
                getTextForNoteGrade(item)
              }}</span>
            </td>
          </tr>
        </tbody>
      </table>

      <table v-if="enabledShowMessages && !enabledNoteTimeDifferences" class="time-differences-table-no-border">
        <tbody>
          <tr v-for="(item, index) in timeDifferencesMelody" :key="index">
            <td class="grade-container">
              <span v-if="item !== ''" :style="{ color: getColorForNoteGrade(item) }" class="grade-message">{{ getTextForNoteGrade(item) }}</span>
            </td>
          </tr>
        </tbody>
      </table>

      <div v-if="enabledPlayerBeginGreenLight && showingGreenLight">
        <div class="player-green-light"></div>
      </div>

      <div v-if="enabledCorrectNoteRatio" class="correct-note-ratio">
        <span v-html="accurateNoteRatio" :class="{ grow: animateNoteRatio, shrink: !animateNoteRatio }" class="correct-note-ratio-text"></span>
      </div>
    </div>

    <div class="ui-menu-parent">
      <div class="ui-menu">
        <!-- <button href="#" @click.prevent="startMusic()">Show Counter</button>
      <div v-if="showingCounter" class="player-feedback countdowner">
        <span class="circle-text">{{ countdown }}</span>
      </div> -->

        <button :class="{ activeButton: enabledSendFretColor, inactiveButton: !enabledSendFretColor }" @click="toggleEnabledFretColorGrading()">
          Fret Color Grading
        </button>

        <button
          href="#"
          :class="{ activeButton: enabledPlayerBeginGreenLight, inactiveButton: !enabledPlayerBeginGreenLight }"
          @click.prevent="toggleEnabledGreenLight()"
        >
          Green Light
        </button>

        <button :class="{ activeButton: enabledPlayUserVisuals, inactiveButton: !enabledPlayUserVisuals }" @click="toggleEnabledPlayUserVisuals()">
          User Part Visuals
        </button>

        <button :class="{ activeButton: enabledCorrectNoteRatio, inactiveButton: !enabledCorrectNoteRatio }" @click="toggleEnabledCorrectNoteRatio()">
          Correct Note Ratio
        </button>

        <button :class="{ activeButton: enabledShowMessages, inactiveButton: !enabledShowMessages }" @click="toggleEnabledGrades()">
          Note Messages
        </button>

        <button
          :class="{ activeButton: enabledNoteTimeDifferences, inactiveButton: !enabledNoteTimeDifferences }"
          @click="toggleEnabledNoteTimeDifferences()"
        >
          Time Differences Table
        </button>

        <button :class="{ activeButton: enabledAutoNextPart, inactiveButton: !enabledAutoNextPart }" @click="toggleEnabledAutoNextPart()">
          Auto Next Part
        </button>
      </div>
    </div>
  </div>
</template>

<script>
import { Howl } from "howler";
import MIDIMessage from "../utils/MIDIMessage.js";
import MIDIccMessage from "../utils/MIDIccMessage.js";
import { MIDIMessageTypesStrings } from "../utils/MIDIMessageTypes.js";
import { noteToMIDI, MIDItoNote } from "../utils/MIDINotesMapping.js";
import acoustic_guitar_sprite from "../utils/sprites_json/guitar_acoustic_sprite.js";
import songParts from "../utils/songParts.js";

export default {
  data() {
    return {
      animateNoteRatio: false,
      gradeColor: "",
      accurateNoteRatio: "",
      nextLevelClass: false,
      accurateNotes: [],
      noteGrade: "",
      countdown: 1,
      audioContext: null,
      userPlayedNotesIDs: [],
      timestampedMelody: [],
      timeDifferencesMelody: [],

      // UI testing variables
      showingCounter: false,
      enabledPlayUserVisuals: false,
      enabledAutoNextPart: false,
      enabledSendFretColor: false,
      enabledPlayerBeginGreenLight: false,
      enabledShowMessages: false,
      enabledCorrectNoteRatio: false,
      enabledNoteTimeDifferences: false,
      timeDifferencesTable: "",
      showingResults: false,
      showingGreenLight: false,

      // Grading colors
      bestColor: "greenyellow",
      goodColor: "greenyellow",
      okColor: "orange",
      badColor: "red",

      perfectGrade: 40,
      goodGrade: 80,
      okGrade: 150,

      backingTrack: {
        howl: null,
        filePath: "./tracks/C_Jazz_2-5-1_BPM_120.aac",
        introEnd: 2701,
        sprite: {
          // Loaded programmatically in created() after songPartsAddStartEndTimes()
        },
        BPM: 120,
        barLength: null,
        timeSignature: 4,
      },
      isGuitarActive: true,
      isMusicPlaying: false,
      sampler: null,
      guitar: null,
      selectedIndex: 1,
      lastIndex: 1,
      introEnd: 2701,
      midi_message_app: null,
      midi_message_user: null,
      songParts: songParts,
    };
  },
  computed: {
    currentPart() {
      return this.songParts[this.selectedIndex];
    },
    partList() {
      return Object.keys(this.backingTrack.sprite);
    },
  },
  methods: {
    trackAddBarLength() {
      return new Promise((resolve, reject) => {
        this.backingTrack.barLength = this.backingTrack.timeSignature / (this.backingTrack.BPM / 60);
        if (this.backingTrack.barLength == 0) {
          reject("Wrong BPM");
        }
        resolve();
      });
    },
    songPartsAddStartEndTimes() {
      for (let i = 0; i < this.songParts.length; i++) {
        this.songParts[i].startTime = this.songParts[i - 1] ? this.songParts[i - 1].endTime + 1 : 0;
        let duration = this.songParts[i].bars * this.backingTrack.barLength * 1000;
        duration = i == 0 ? this.introEnd : duration;
        this.songParts[i].duration = duration;
        this.songParts[i].endTime = this.songParts[i - 1] ? this.songParts[i].startTime + duration : this.introEnd;
      }
    },
    toggleEnabledGreenLight() {
      this.enabledPlayerBeginGreenLight = !this.enabledPlayerBeginGreenLight;
    },
    toggleEnabledGrades() {
      this.enabledShowMessages = !this.enabledShowMessages;
    },
    toggleEnabledAutoNextPart() {
      this.enabledAutoNextPart = !this.enabledAutoNextPart;
    },
    toggleEnabledPlayUserVisuals() {
      this.enabledPlayUserVisuals = !this.enabledPlayUserVisuals;
    },
    toggleEnabledFretColorGrading() {
      this.enabledSendFretColor = !this.enabledSendFretColor;
    },
    toggleEnabledCorrectNoteRatio() {
      this.enabledCorrectNoteRatio = !this.enabledCorrectNoteRatio;
    },
    toggleEnabledNoteTimeDifferences() {
      this.enabledNoteTimeDifferences = !this.enabledNoteTimeDifferences;
    },
    play() {
      this.guitar.play("A2");
    },
    validateIndex() {
      if (this.selectedIndex < 1) {
        this.selectedIndex = 1;
      } else if (this.selectedIndex > this.songParts.length - 1) {
        this.selectedIndex = this.songParts.length - 1;
      }
    },
    getColorForNoteGrade(timeDifference) {
      timeDifference = Math.abs(timeDifference);
      switch (true) {
        case timeDifference < this.perfectGrade:
          return this.bestColor;
        case timeDifference < this.goodGrade:
          return this.goodColor;
        case timeDifference < this.okGrade:
          return this.okColor;
        case timeDifference > this.okGrade:
          return this.badColor;
        default:
          return "white";
      }
    },
    getTextForNoteGrade(timeDifference) {
      timeDifference = Math.abs(timeDifference);
      switch (true) {
        case timeDifference < this.perfectGrade:
          return "Perfect";
        case timeDifference < this.goodGrade:
          return "Good";
        case timeDifference < this.okGrade:
          return "OK";
        case timeDifference > this.okGrade:
          return "Meh";
        default:
          return "";
      }
    },
    barsToMilliseconds(bars) {
      return bars * this.backingTrack.barLength * 1000;
    },
    playMelody(midiMelody) {
      // Timestamp melody for user feedback
      let melodyStartTime = new Date().getTime();
      this.timestampedMelody = [];

      for (let i = 0; i < midiMelody.length; i++) {
        this.timestampedMelody.push({
          note: midiMelody[i].note,
          time: melodyStartTime + this.barsToMilliseconds(midiMelody[i].time) + this.songParts[this.selectedIndex].duration / 2,
        });
      }

      for (let i = 0; i < midiMelody.length; i++) {
        let startTime = this.barsToMilliseconds(midiMelody[i].time);
        let noteDuration = this.barsToMilliseconds(midiMelody[i].duration);
        let midiNote = noteToMIDI[midiMelody[i].note];
        let spriteId;
        // Set note ons
        setTimeout(() => {
          this.midi_message_app = new MIDIccMessage(midiMelody[i].string, MIDIMessageTypesStrings.cc, 0, midiNote);
          setTimeout(() => {
            this.midi_message_app = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_on, midiNote, 127);
          }, 0.1);
          spriteId = this.guitar.play(midiMelody[i].note);
        }, startTime);
        // Set note offs
        setTimeout(() => {
          this.guitar.stop(spriteId);
          this.midi_message_app = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_off, midiNote, 0);
        }, startTime + noteDuration);

        // Play melody visuals for user
        if (this.enabledPlayUserVisuals) {
          setTimeout(() => {
            setTimeout(() => {
              this.midi_message_user = new MIDIccMessage(midiMelody[i].string, MIDIMessageTypesStrings.cc, 0, midiNote);
              setTimeout(() => {
                this.midi_message_user = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_on, midiNote, 127);
              }, 0.1);
            }, startTime);
            setTimeout(() => {
              this.midi_message_user = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_off, midiNote, 0);
            }, startTime + noteDuration);
          }, this.songParts[this.selectedIndex].duration / 2);
        }

        // Countdown
        // let userPartBegin = this.songParts[this.selectedIndex].duration / 2;
        // let countdownMinusTime = this.backingTrack.barLength * 1000;
        // let firstNoteBegin = this.barsToMilliseconds(this.songParts[this.selectedIndex].midiMelody[0].time);
        // let countdownTime = userPartBegin - countdownMinusTime + firstNoteBegin;
        // setTimeout(() => {
        //   this.showingCounter = true;
        // }, countdownTime);

        // setTimeout(() => {
        //   this.countdown = 2;
        // }, countdownTime + this.barsToMilliseconds(1 / 4));
        // setTimeout(() => {
        //   this.countdown = 3;
        // }, countdownTime + this.barsToMilliseconds(2 / 4));
        // setTimeout(() => {
        //   this.countdown = 4;
        // }, countdownTime + this.barsToMilliseconds(3 / 4));
        // setTimeout(() => {
        //   this.showingCounter = false;
        //   this.countdown = 1;
        // }, countdownTime + this.barsToMilliseconds(4 / 4));
      }
    },
    handleSpriteEnd() {
      this.backingTrack.howl.play(this.partList[this.selectedIndex]);
      if (this.songParts[this.selectedIndex].midiMelody.length > 0) {
        this.playMelody(this.songParts[this.selectedIndex].midiMelody);
      }
    },
    checkAccurateNotes() {
      if (this.accurateNotes.length >= this.songParts[this.selectedIndex].midiMelody.length) {
        this.nextLevelClass = true;
        setTimeout(() => {
          this.nextLevelClass = false;
        }, 1000);
        if (this.enabledAutoNextPart) {
          this.goToNextPart();
        }
      }
    },
    handleSpriteStart() {
      this.showingGreenLight = false;
      let currentMelody = this.songParts[this.selectedIndex].midiMelody;
      if (this.songParts[this.selectedIndex].midiMelody.length > 0) {
        let lastNoteOfMelody = currentMelody.reduce((prev, curr) => {
          return curr.time > prev.time ? curr : prev;
        });
        let lastNoteEndTime = this.barsToMilliseconds(lastNoteOfMelody.time) + this.barsToMilliseconds(lastNoteOfMelody.duration);
        let userLastNoteEndTime = this.songParts[this.selectedIndex].duration / 2 + lastNoteEndTime;
        setTimeout(() => {
          let accurateNotesLength = this.accurateNotes.length > currentMelody.length ? currentMelody.length : this.accurateNotes.length;
          this.accurateNoteRatio = `${accurateNotesLength}/${currentMelody.length}`;
          this.checkAccurateNotes();
          this.animateNoteRatio = true;
          setTimeout(() => {
            this.animateNoteRatio = false;
            this.accurateNotes = [];
          }, 2000);
        }, userLastNoteEndTime + 100);
        setTimeout(() => {
          this.timeDifferencesMelody = Array(this.songParts[this.selectedIndex].midiMelody.length).fill("");
        }, this.songParts[this.selectedIndex].duration / 2 - 300);
        setTimeout(() => {
          this.showingGreenLight = true;
        }, this.songParts[this.selectedIndex].duration / 2 + this.barsToMilliseconds(this.songParts[this.selectedIndex].midiMelody[0].time));
      }
    },
    loadGuitar() {
      this.guitar = new Howl({
        src: ["./instruments/guitar_acoustic/guitar_acoustic.ogg"],
        sprite: acoustic_guitar_sprite,
      });
    },
    loadBackingTrack() {
      let handleSpriteEnd = this.handleSpriteEnd;
      let handleSpriteStart = this.handleSpriteStart;
      let sprite = this.backingTrack.sprite;
      this.backingTrack.howl = new Howl({
        src: ["./tracks/C_Jazz_2-5-1_BPM_120.aac"],
        sprite: sprite,
        loop: false,
        onend: handleSpriteEnd,
        onplay: handleSpriteStart,
      });
    },
    createTonePart(midiMelody) {
      let noteName = noteToMIDI[midiMelody.note];
      if (Array.isArray(midiMelody.note)) {
        noteName = [];
        for (let i = 0; i < midiMelody.length; i++) {
          noteName[i] = noteToMIDI[midiMelody.note[i]];
        }
      }
      this.midi_message_app = new MIDIccMessage(midiMelody.string, MIDIMessageTypesStrings.cc, 0, noteName);
      setTimeout(() => {
        this.midi_message_app = new MIDIMessage(midiMelody.string, MIDIMessageTypesStrings.note_on, noteName, 127);
      }, 1);
      setTimeout(() => {
        this.midi_message_app = new MIDIMessage(midiMelody.string, MIDIMessageTypesStrings.note_off, noteName, 0);
      }, midiMelody.duration);
    },
    activateMidi() {
      if (this.isGuitarActive) return;
      this.isGuitarActive = true;
      this.guitar.fade(0, 1, 5);
    },
    stopMidi() {
      if (!this.isGuitarActive) return;
      this.isGuitarActive = false;
      this.guitar.fade(1, 0, 1);
    },
    goToNextPart() {
      this.selectedIndex = +this.selectedIndex == this.songParts.length - 1 ? +this.selectedIndex : +this.selectedIndex + 1;
    },
    goToPreviousPart() {
      this.selectedIndex = +this.selectedIndex <= 1 ? +this.selectedIndex : +this.selectedIndex - 1;
    },
    stopMusic() {
      if (!this.isMusicPlaying) return;
      this.isMusicPlaying = false;
      this.backingTrack.howl.stop();
    },
    async startMusic() {
      if (this.isMusicPlaying) return;
      this.isMusicPlaying = true;
      this.backingTrack.howl.play("part0");
    },
  },
  watch: {
    midi_message_app(newValue) {
      this.$parent.$parent.$emit("MIDI-message-app", newValue);
    },
    midi_message_user(newValue) {
      this.$parent.$parent.$emit("MIDI-message-user", newValue);
    },
  },
  created() {
    this.trackAddBarLength().then(this.songPartsAddStartEndTimes());
    // this.backingTrack.sprite = {
    //   part0: [this.songParts[0].startTime, this.songParts[0].duration],
    //   part1: [this.songParts[1].startTime, this.songParts[1].duration],
    //   part2: [this.songParts[2].startTime, this.songParts[2].duration],
    //   part3: [this.songParts[3].startTime, this.songParts[3].duration],
    //   part4: [this.songParts[4].startTime, this.songParts[4].duration],
    //   part5: [this.songParts[5].startTime, this.songParts[5].duration],
    //   part6: [this.songParts[6].startTime, this.songParts[6].duration],
    //   part7: [this.songParts[7].startTime, this.songParts[7].duration],
    //   part8: [this.songParts[8].startTime, this.songParts[8].duration],
    //   part9: [this.songParts[9].startTime, this.songParts[9].duration],
    // };
    // crate for loop to populate this.backingTrack.sprite
    for (let i = 0; i < this.songParts.length; i++) {
      this.backingTrack.sprite[`part${i}`] = [this.songParts[i].startTime, this.songParts[i].duration];
    }
  },
  mounted() {
    this.loadGuitar();
    this.loadBackingTrack();
    // Play MIDI from user
    this.$parent.$on("MIDI-message-user", (data) => {
      if (data.message_type == "note_on") {
        let timestampUserPlayedNote = new Date().getTime();
        let playedNote = MIDItoNote[data.note];
        let noteID = this.guitar.play(playedNote);
        this.userPlayedNotesIDs.push({ note: playedNote, id: noteID });

        let matchingNotes = this.timestampedMelody.filter((storedNote) => storedNote.note == MIDItoNote[data.note]);

        if (matchingNotes.length > 0) {
          let closestNote = matchingNotes.reduce((prev, curr) => {
            return Math.abs(curr.time - timestampUserPlayedNote) < Math.abs(prev.time - timestampUserPlayedNote) ? curr : prev;
          });

          let indexOfClosestNote = this.timestampedMelody.indexOf(closestNote);

          let timeDifference = parseInt(timestampUserPlayedNote - closestNote.time);
          let absTimeDifference = Math.abs(timeDifference);

          switch (true) {
            case absTimeDifference < this.perfectGrade:
              if (this.enabledSendFretColor) {
                data.color = this.bestColor;
              }
              this.accurateNotes.push(playedNote);
              break;
            case absTimeDifference < this.goodGrade:
              if (this.enabledSendFretColor) {
                data.color = this.goodColor;
              }
              this.accurateNotes.push(playedNote);
              break;
            case absTimeDifference < this.okGrade:
              if (this.enabledSendFretColor) {
                data.color = this.okColor;
              }
              this.accurateNotes.push(playedNote);
              break;
            case absTimeDifference > this.okGrade && absTimeDifference < 1000:
              if (this.enabledSendFretColor) {
                data.color = this.badColor;
              }
              break;
            default:
              this.noteGrade = "";
          }

          if (absTimeDifference < 1000) {
            this.timeDifferencesMelody[indexOfClosestNote] = timeDifference;
          }

          this.showingResults = true;
          setTimeout(() => {
            this.showingResults = false;
          }, 700);
        }
      } else if (data.message_type == "note_off") {
        let note = MIDItoNote[data.note];
        let foundNoteID = this.userPlayedNotesIDs.find((obj) => obj.note == note).id;
        // this.guitar.onfade = () => {
        //   this.guitar.stop(foundNoteID);
        //   console.log("stopping");
        // };
        // this.guitar.fade(1, 0, 1, foundNoteID);
        // Stop method happens onfade
        this.guitar.stop(foundNoteID);

        // Delete note from this.userPlayedNotesIDs after note off
        this.userPlayedNotesIDs = this.userPlayedNotesIDs.filter((obj) => obj.note != note);
      }
      this.$parent.$parent.$emit("MIDI-message-user", data);
    });
  },
};
</script>

<style scoped>
.ui-menu-parent {
  display: flex;
  justify-content: center;
}

.ui-menu {
  position: fixed;
  bottom: 0px;
}

.activeButton {
  background-color: rgb(156, 255, 199);
  border: 1px solid gray;
  border-radius: 2px;
  box-shadow: none;
  outline: none;
}

.inactiveButton {
  background-color: lightgray;
  border: 1px solid gray;
  border-radius: 2px;
  box-shadow: none;
  outline: none;
}

#part-counter-container {
  position: relative;
  display: flex;
  flex-direction: column;
  justify-content: center;
  width: 100px;
  background-color: #ffffff;
}

#part-counter {
  position: absolute;
  display: flex;
  flex-direction: column;
  justify-content: flex-end;
  margin: 0px;
  padding: 0px 20px 0px 20px;
  font-style: italic;
  font-weight: bold;
  font-family: "Bungee", sans-serif;
  text-align: center;
  font-size: 1em;
  background-color: transparent;
  white-space: nowrap;
}

#part-counter.grow {
  font-size: 5em;
  color: rgb(255, 220, 0);
  text-shadow: -1px 0 rgb(62, 62, 62), 0 1px rgb(62, 62, 62), 1px 0 rgb(62, 62, 62), 0 -1px rgb(62, 62, 62);
  transition: font-size 0.25s ease-in-out, color 0.25s ease-in-out, text-shadow 0.25s ease-in-out;
}

#part-counter.shrink {
  font-size: 1em;
  color: rgb(0, 0, 0);
  transition: font-size 0.25s ease-in-out, color 0.25s ease-in-out;
}

.feedback-items > * {
  display: inline-block;
}

.countdowner {
  width: 100px;
  height: 100px;
  background-color: #ffffff;
  border-radius: 50%;
  border: 1em solid #7fdbff;
  margin: 1em;
  display: flex;
  align-items: center;
  justify-content: center;
}

.circle-text {
  font-family: "Playbill", sans-serif;
  font-size: 4em;
}

.grade-container {
  width: auto;
  height: auto;
  transform: rotate(-3deg);
}

.grade-message {
  font-style: italic;
  font-weight: bold;
  font-family: "Bungee", sans-serif;
  transform: skewX(-20deg);
  font-size: 1em;
}

.player-green-light {
  width: 30px;
  height: 30px;
  border-radius: 50%;
  background-color: yellowgreen; /* adjust color as needed */
}

.correct-note-ratio {
  /* width: 15px;
  height: 15px; */
  position: relative;
  top: -60px;
  margin-left: 30px;
  transform: rotate(-10deg);
}

.correct-note-ratio-text {
  font-style: italic;
  font-weight: bold;
  color: rgb(1, 255, 112);
  font-family: "Bungee", sans-serif;
  font-size: 0em;
  transform: skewX(-10deg);
}

.correct-note-ratio-text.grow {
  font-size: 3em;
  color: rgb(1, 255, 112);
  transition: font-size 0.25s ease-in-out;
}

.correct-note-ratio-text.shrink {
  font-size: 0em;
  color: rgb(1, 255, 112);
  transition: font-size 0.25s ease-in-out;
}

.time-differences-table {
  border-collapse: collapse;
  color: white;
}

.time-differences-table-no-border {
  border: none;
}

.time-differences-table td,
.time-differences-table th {
  border: 1px solid white;
  padding: 5px;
  text-align: center;
}

.time-differences-table th:nth-child(3),
.time-differences-table td:nth-child(3) {
  border: none;
}
</style>
