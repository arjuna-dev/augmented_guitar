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
      <button @click="activateMidi()">Activate Solo</button>
      <button @click="stopMidi()">Deactivate Solo</button>
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
            <td>
              <span v-if="item !== ''" :style="{ color: item > 0 ? 'red' : 'greenyellow' }" class="player-green-light">oh yeah</span>
            </td>
          </tr>
        </tbody>
      </table>
      <div v-if="enabledPlayerBeginGreenLight">
        <span class="player-green-light"></span>
      </div>
      <div :style="{ color: gradeColor }" v-if="showingResults && enabledShowGrades" class="grade-container">
        <span v-html="noteGrade" class="grade-text"></span>
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
        <button
          href="#"
          :class="{ activeButton: enabledPlayerBeginGreenLight, inactiveButton: !enabledPlayerBeginGreenLight }"
          @click.prevent="toggleEnabledGreenLight()"
        >
          Show Green Light
        </button>

        <button :class="{ activeButton: enabledShowGrades, inactiveButton: !enabledShowGrades }" @click="toggleEnabledGrades()">
          Show Note Messages
        </button>

        <button :class="{ activeButton: enabledCorrectNoteRatio, inactiveButton: !enabledCorrectNoteRatio }" @click="toggleEnabledCorrectNoteRatio()">
          Show Correct Note Ratio
        </button>

        <button
          :class="{ activeButton: enabledNoteTimeDifferences, inactiveButton: !enabledNoteTimeDifferences }"
          @click="toggleEnabledNoteTimeDifferences()"
        >
          Show Note Time Differences
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
      enabledPlayerBeginGreenLight: false,
      enabledShowGrades: false,
      enabledCorrectNoteRatio: false,
      enabledNoteTimeDifferences: false,
      timeDifferencesTable: "",
      showingResults: false,

      backingTrack: {
        howl: null,
        filePath: "./tracks/C_Jazz_2-5-1_BPM_120.aac",
        introEnd: 2701,
        sprite: {
          // Loaded programmatically in created() after refactoring songParts
          part0: null,
          part1: null,
          part2: null,
          part3: null,
          part4: null,
          part5: null,
          part6: null,
          part7: null,
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
      songParts: [
        {
          startTime: null,
          bars: null,
          duration: null,
          endTime: null,
          midiTriggered: false,
          midiMelody: [],
        },
        {
          startTime: null,
          bars: 4,
          duration: null,
          endTime: null,
          midiTriggered: false,
          midiMelody: [{ time: 0, note: "C4", string: 2, duration: 1 }],
        },
        {
          startTime: null,
          bars: 4,
          duration: null,
          endTime: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: "G3", string: 3, duration: 1 },
            { time: 1 / 4, note: "C4", string: 2, duration: 1 },
          ],
        },
        {
          startTime: null,
          endTime: null,
          bars: 4,
          duration: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: "G3", string: 3, duration: 1 },
            { time: 1 / 4, note: "C4", string: 2, duration: 1 },
            { time: 2 / 4, note: "E4", string: 1, duration: 1 },
          ],
        },
        {
          startTime: null,
          endTime: null,
          bars: 4,
          duration: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: "C4", string: 2, duration: 1 },
            { time: 0, note: "G3", string: 3, duration: 1 },
            { time: 0, note: "E4", string: 1, duration: 1 },
          ],
        },
        {
          startTime: null,
          endTime: null,
          bars: 4,
          duration: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: "B3", string: 2, duration: 1 / 4 },
            { time: 1 / 4, note: "C4", string: 2, duration: 1 / 4 },
          ],
        },
        {
          startTime: null,
          endTime: null,
          bars: 4,
          duration: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: "B3", string: 2, duration: 1 / 4 },
            { time: 1 / 4, note: "C4", string: 2, duration: 1 / 4 },
            { time: 4 / 4, note: "B3", string: 2, duration: 1 / 4 },
            { time: 4 / 4 + 1 / 8, note: "C4", string: 2, duration: 1 / 4 },
            { time: 5 / 4 + 1 / 8, note: "C4", string: 2, duration: 1 / 4 },
          ],
        },
        {
          startTime: null,
          endTime: null,
          bars: 4,
          duration: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: "B3", string: 2, duration: 1 / 4 },
            { time: 1 / 4, note: "C4", string: 2, duration: 1 / 4 },
            { time: 4 / 4, note: "B3", string: 2, duration: 1 / 4 },
            { time: 4 / 4 + 1 / 8, note: "C4", string: 2, duration: 1 / 4 },
            { time: 5 / 4 + 1 / 8, note: "C4", string: 2, duration: 1 / 4 },
          ],
        },
        {
          startTime: null,
          endTime: null,
          bars: 4,
          duration: null,
          midiTriggered: false,
          midiMelody: [
            { time: 0 / 4, note: "C4", string: 2, duration: 1 / 4 },
            { time: 1 / 4, note: "E4", string: 1, duration: 1 / 4 },
            { time: 1 / 4 + 1 / 7, note: "C4", string: 2, duration: 1 / 4 },
            { time: 2 / 4, note: "G3", string: 3, duration: 1 / 4 },
            { time: 3 / 4, note: "E4", string: 1, duration: 1 / 4 },
            { time: 4 / 4, note: "C4", string: 2, duration: 1 / 4 },
            { time: 5 / 4, note: "E4", string: 1, duration: 1 / 4 },
          ],
        },
      ],
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
    refactorTrack() {
      return new Promise((resolve, reject) => {
        this.backingTrack.barLength = this.backingTrack.timeSignature / (this.backingTrack.BPM / 60);
        if (this.backingTrack.barLength == 0) {
          reject("Wrong BPM");
        }
        resolve();
      });
    },
    refactorSongParts() {
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
      this.enabledShowGrades = !this.enabledShowGrades;
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
    barsToMilliseconds(bars) {
      return bars * this.backingTrack.barLength * 1000;
    },
    playMelody(midiMelody) {
      // Timestamp melody for user feedback
      let melodyStartTime = new Date().getTime();
      this.timestampedMelody = [];
      this.timeDifferencesMelody = [];
      for (let i = 0; i < midiMelody.length; i++) {
        this.timeDifferencesMelody[i] = "";
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

        // Replay melody visuals for user
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
      this.playMelody(this.songParts[this.selectedIndex].midiMelody);
    },
    checkAccurateNotes() {
      if (this.accurateNotes.length >= this.songParts[this.selectedIndex].midiMelody.length) {
        this.nextLevelClass = true;
        setTimeout(() => {
          this.nextLevelClass = false;
        }, 1000);
        this.goToNextPart();
      }
    },
    handleSpriteStart() {
      let currentMelody = this.songParts[this.selectedIndex].midiMelody;
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
      this.selectedIndex = this.selectedIndex == this.songParts.length - 1 ? this.selectedIndex : this.selectedIndex + 1;
    },
    goToPreviousPart() {
      this.selectedIndex = this.selectedIndex <= 1 ? this.selectedIndex : this.selectedIndex - 1;
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
    this.refactorTrack().then(this.refactorSongParts());
    this.backingTrack.sprite = {
      part0: [this.songParts[0].startTime, this.songParts[0].duration],
      part1: [this.songParts[1].startTime, this.songParts[1].duration],
      part2: [this.songParts[2].startTime, this.songParts[2].duration],
      part3: [this.songParts[3].startTime, this.songParts[3].duration],
      part4: [this.songParts[4].startTime, this.songParts[4].duration],
      part5: [this.songParts[5].startTime, this.songParts[5].duration],
      part6: [this.songParts[6].startTime, this.songParts[6].duration],
      part7: [this.songParts[7].startTime, this.songParts[7].duration],
      part8: [this.songParts[8].startTime, this.songParts[8].duration],
    };
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

          let timeDifference = timestampUserPlayedNote - closestNote.time;
          let absTimeDifference = Math.abs(timeDifference);

          // TODO: Eliminate magic numbers:
          switch (true) {
            case absTimeDifference < 40:
              this.noteGrade = "Perfect <br> timing";
              this.gradeColor = "greenyellow";
              this.accurateNotes.push(playedNote);
              break;
            case absTimeDifference < 80:
              this.noteGrade = "Good <br> timing";
              this.gradeColor = "greenyellow";
              this.accurateNotes.push(playedNote);
              break;
            case absTimeDifference < 150:
              this.noteGrade = "OK<br> timing";
              this.gradeColor = "orange";
              this.accurateNotes.push(playedNote);
              break;
            case absTimeDifference > 150 && absTimeDifference < 1000:
              this.noteGrade = "Meh<br> timing";
              this.gradeColor = "red";
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
    });
  },
};
</script>

<style scoped>
/* .menu {
  position: relative;
  display: flex;
  flex-direction: row;
  height: 20px;
  width: 100%;
  padding-left: 30px;
  margin-bottom: 30px;
} */

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
  margin-left: 30px;
  transform: rotate(-10deg);
}

.grade-text {
  font-style: italic;
  font-weight: bold;
  font-family: "Bungee", sans-serif;
  transform: skewX(-20deg);
  font-size: 3.5em;
}

.correct-note-ratio {
  width: 150px;
  height: 150px;
  margin-left: 30px;
  /* display: flex;
  align-items: center;
  justify-content: center; */
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
