<template>
  <div>
    <div class="menu">
      <div v-if="showingCounter" class="playerFeedback countdowner">
        <span class="circle-text">{{ countdown }}</span>
      </div>
      <div :style="{ color: gradeColor }" v-if="showingGrade" class="playerFeedback grade-container">
        <span v-html="noteGrade" class="grade-text"></span>
      </div>
      <button href="#" @click.prevent="startMusic()">Start Music</button>
      <button href="#" @click.prevent="stopMusic()">Stop Music</button>
      <button @click="goToPreviousPart()">Previous</button>
      <div id="part-counter-container">
        <p id="part-counter" :class="{ grow: nextLevelClass, shrink: !nextLevelClass }" @click="goToPreviousPart()">Level {{ selectedIndex }}</p>
      </div>
      <button @click="goToNextPart()">Next</button>
      <button @click="activateMidi()">Activate Solo</button>
      <button @click="stopMidi()">Deactivate Solo</button>
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
      showingCounter: false,
      gradeColor: "",
      nextLevelClass: false,
      accurateNotes: [],
      noteGrade: "",
      showingGrade: false,
      countdown: 1,
      audioContext: null,
      userPlayedNotesIDs: [],
      timestampedMelody: [],
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
      isMidiTrackActive: true,
      isMusicPlaying: false,
      sampler: null,
      guitar: null,
      selectedIndex: 1,
      lastIndex: 1,
      introEnd: 2701,
      midi_message: null,
      songParts: [
        {
          name: "part0",
          startTime: null,
          bars: null,
          duration: null,
          endTime: null,
          midiTriggered: false,
          midiMelody: [],
        },
        {
          name: "part1",
          startTime: null,
          bars: 4,
          duration: null,
          endTime: null,
          midiTriggered: false,
          // prettier-ignore
          midiMelody: [
            { time: 0, note: "C4", string: 2, duration: 4},
          ],
        },
        {
          name: "part2",
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
          name: "part3",
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
          name: "part4",
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
          name: "part5",
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
          name: "part6",
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
          name: "part7",
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
          this.midi_message = new MIDIccMessage(midiMelody[i].string, MIDIMessageTypesStrings.cc, 0, midiNote);
          setTimeout(() => {
            this.midi_message = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_on, midiNote, 127);
          }, 0.1);
          spriteId = this.guitar.play(midiMelody[i].note);
        }, startTime);
        // Set note offs
        setTimeout(() => {
          this.guitar.stop(spriteId);
          this.midi_message = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_off, midiNote, 0);
        }, startTime + noteDuration);

        // Replay melody visuals for user
        setTimeout(() => {
          setTimeout(() => {
            this.midi_message = new MIDIccMessage(midiMelody[i].string, MIDIMessageTypesStrings.cc, 0, midiNote);
            setTimeout(() => {
              this.midi_message = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_on, midiNote, 127);
            }, 0.1);
          }, startTime);
          setTimeout(() => {
            this.midi_message = new MIDIMessage(midiMelody[i].string, MIDIMessageTypesStrings.note_off, midiNote, 0);
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
      if (this.accurateNotes.length == this.songParts[this.selectedIndex].midiMelody.length) {
        this.nextLevelClass = true;
        setTimeout(() => {
          this.nextLevelClass = false;
        }, 300);
        this.goToNextPart();
      }
      this.accurateNotes = [];
    },
    handleSpriteStart() {
      setTimeout(() => {
        this.checkAccurateNotes();
      }, this.songParts[this.selectedIndex].duration - 5);
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
      this.midi_message = new MIDIccMessage(midiMelody.string, MIDIMessageTypesStrings.cc, 0, noteName);
      setTimeout(() => {
        this.midi_message = new MIDIMessage(midiMelody.string, MIDIMessageTypesStrings.note_on, noteName, 127);
      }, 1);
      setTimeout(() => {
        this.midi_message = new MIDIMessage(midiMelody.string, MIDIMessageTypesStrings.note_off, noteName, 0);
      }, midiMelody.duration);
    },
    activateMidi() {
      if (this.isMidiTrackActive) return;
      this.isMidiTrackActive = true;
      this.guitar.fade(0, 1, 5);
    },
    stopMidi() {
      if (!this.isMidiTrackActive) return;
      this.isMidiTrackActive = false;
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
    midi_message(newValue) {
      this.$parent.$parent.$emit("MIDI-message", newValue);
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
    };
  },
  mounted() {
    this.loadGuitar();
    this.loadBackingTrack();
    // Play MIDI from user
    this.$parent.$on("MIDI-message", (data) => {
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

          let timeDifference = Math.abs(timestampUserPlayedNote - closestNote.time);

          // TODO: Eliminate magic numbers:
          switch (true) {
            case timeDifference < 40:
              this.noteGrade = "Perfect <br> timing";
              this.gradeColor = "greenyellow";
              this.accurateNotes.push(playedNote);
              break;
            case timeDifference < 80:
              this.noteGrade = "Good <br> timing";
              this.gradeColor = "greenyellow";
              this.accurateNotes.push(playedNote);
              break;
            case timeDifference < 300:
              this.noteGrade = "OK<br> timing";
              this.gradeColor = "orange";
              break;
            case timeDifference > 300 && timeDifference < 1000:
              this.noteGrade = "Meh<br> timing";
              this.gradeColor = "red";
              break;
            default:
              this.noteGrade = "";
          }

          this.showingGrade = true;
          setTimeout(() => {
            this.showingGrade = false;
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
.menu {
  position: relative;
  display: flex;
  flex-direction: row;
  height: 20px;
  width: 100%;
  padding-left: 30px;
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
  font-size: 5em; /* The size the text should grow to */
  color: rgb(255, 220, 0);
  transition: font-size 0.25s ease-in-out, color 0.25s ease-in-out; /* The transition effect */
}

#part-counter.shrink {
  font-size: 1em; /* The initial size of the text */
  color: rgb(0, 0, 0);
  transition: font-size 0.25s ease-in-out, color 0.25s ease-in-out; /* The transition effect */
}

.playerFeedback {
  position: absolute;
  left: 350px;
  bottom: 50px;
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
  transform: rotate(-10deg);
}

.grade-text {
  font-style: italic;
  font-weight: bold;
  font-family: "Bungee", sans-serif;
  transform: skewX(-20deg);
  font-size: 6em;
}
</style>
