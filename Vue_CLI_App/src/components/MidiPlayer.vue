<template>
  <div>
    <!-- <a href="#" @click.prevent="play()">A2</a> -->
    <button href="#" @click.prevent="startMusic()">Start Music</button>
    <button href="#" @click.prevent="stopMusic()">Stop Music</button>
    <button @click="goToPreviousPart()">Previous Part</button>
    <button @click="goToNextPart()">Next Part</button>
    <button @click="activateMidi()">Activate Solo</button>
    <button @click="stopMidi()">Deactivate Solo</button>
    <button @click="addToTonePart(songParts[2])">addToTonePart</button>
    <button @click="clearTonePart()">clearTonePart</button>
  </div>
</template>

<script>
import { Howl } from "howler";
import MIDIMessage from "../utils/MIDIMessage.js";
import MIDIccMessage from "../utils/MIDIccMessage.js";
import { MIDIMessageTypesStrings } from "../utils/MIDIMessageTypes.js";
import { midiNotesMapping as mapMidi } from "../utils/midiNotesMapping.js";

export default {
  data() {
    return {
      audioContext: null,
      backingTrack: {
        howl: null,
        filePath: "./tracks/C_Jazz_2-5-1_BPM_120.aac",
        sprite: {
          part0: [0, 2701],
          part1: [2702, 8000],
          part2: [2701 + 8000 * 2, 8000],
          part3: [2701 + 8000 * 3, 8000],
          part4: [2701 + 8000 * 4, 8000],
          part5: [2701 + 8000 * 5, 8000],
          part6: [2701 + 8000 * 6, 8000],
          part7: [2701 + 8000 * 7, 8000],
        },
        BPM: 120,
        barLength: 2,
      },
      isMidiTrackActive: true,
      isMusicPlaying: false,
      sampler: null,
      guitar: null,
      selectedIndex: 1,
      lastIndex: 1,
      introEnd: 2.701,
      midi_message: null,
      songParts: [
        {
          name: "part0",
          startTime: 0,
          duration: "4m",
          endTime: 2.701,
          midiTriggered: false,
          midiMelody: [],
        },
        {
          name: "part1",
          startTime: 2.701,
          duration: "4m",
          endTime: null,
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "E4", duration: "8n", string: 1 },
            { time: "2m", note: "E4", duration: "8n", string: 1 },
            { time: "3m", note: "A#3", duration: "8n", string: 2 },
          ],
        },
        {
          name: "part2",
          startTime: null,
          duration: "4m",
          endTime: null,
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "B3", duration: "1m", string: 2 },
            // { time: "2m", note: "B3", duration: "8n", string: 2 },
            // { time: "3m", note: "B3", duration: "8n", string: 2 },
          ],
        },
        {
          name: "part3",
          startTime: null,
          endTime: null,
          duration: "4m",
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "B3", duration: "1m", string: 2 },
            { time: "2m", note: "B3", duration: "8n", string: 2 },
            { time: "3m", note: "B3", duration: "8n", string: 2 },
          ],
        },
        {
          name: "part4",
          startTime: null,
          endTime: null,
          duration: "4m",
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "B3", duration: "1m", string: 2 },
            { time: "2m", note: "B3", duration: "8n", string: 2 },
            { time: "3m", note: "B3", duration: "8n", string: 2 },
          ],
        },
        {
          name: "part5",
          startTime: null,
          endTime: null,
          duration: "4m",
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "B3", duration: "1m", string: 2 },
            { time: "2m", note: "B3", duration: "8n", string: 2 },
            { time: "3m", note: "B3", duration: "8n", string: 2 },
          ],
        },
        {
          name: "part6",
          startTime: null,
          endTime: null,
          duration: "4m",
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "B3", duration: "1m", string: 2 },
            { time: "2m", note: "B3", duration: "8n", string: 2 },
            { time: "3m", note: "B3", duration: "8n", string: 2 },
          ],
        },
        {
          name: "part7",
          startTime: null,
          endTime: null,
          duration: "4m",
          midiTriggered: false,
          midiMelody: [
            { time: "0m", note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: "1m", note: "B3", duration: "1m", string: 2 },
            { time: "2m", note: "B3", duration: "8n", string: 2 },
            { time: "3m", note: "B3", duration: "8n", string: 2 },
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
    play() {
      this.guitar.play("A2");
    },
    handleSpriteEnd() {
      this.backingTrack.howl.play(this.partList[this.selectedIndex]);
    },
    refactorSongParts() {
      // for (let i = 0; i < this.songParts.length; i++) {
      //   this.songParts[i].startTime = this.songParts[i - 1] ? this.songParts[i - 1].endTime : 0;
      //   const durationInSeconds = Tone.Time(this.songParts[i].duration).toSeconds();
      //   this.songParts[i].endTime = this.songParts[i - 1] ? this.songParts[i].startTime + durationInSeconds : this.introEnd;
      // }
    },
    loadGuitar() {
      this.guitar = new Howl({
        src: ["./instruments/guitar_electric/guitar_electric_sprites.webm", "./instruments/guitar_electric/guitar_electric_sprites.mp3"],
        sprite: {
          A2: [0, 4284.081632653061],
          A3: [6000, 4284.081632653061],
          A4: [12000, 4284.081632653063],
          A5: [18000, 4284.081632653063],
          C3: [24000, 4284.081632653063],
          C4: [30000, 4284.081632653063],
          C5: [36000, 4284.081632653063],
          C6: [42000, 4284.081632653063],
          Cs2: [48000, 4284.081632653063],
          Ds3: [54000, 4284.081632653063],
          Ds4: [60000, 4284.081632653056],
          Ds5: [66000, 4284.081632653056],
          E2: [72000, 4284.081632653056],
          Fs2: [78000, 4284.081632653056],
          Fs3: [84000, 4284.081632653056],
          Fs4: [90000, 4284.081632653056],
          Fs5: [96000, 4284.081632653056],
        },
      });
    },
    loadBackingTrack() {
      let handleSpriteEnd = this.handleSpriteEnd;
      let sprite = this.backingTrack.sprite;
      this.backingTrack.howl = new Howl({
        src: ["./tracks/C_Jazz_2-5-1_BPM_120.aac"],
        sprite: sprite,
        loop: false,
        onend: function () {
          handleSpriteEnd();
        },
      });
    },
    createTonePart(midiMelody) {
      let noteName = mapMidi[midiMelody.note];
      if (Array.isArray(midiMelody.note)) {
        noteName = [];
        for (let i = 0; i < midiMelody.length; i++) {
          noteName[i] = mapMidi[midiMelody.note[i]];
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
    },
    stopMidi() {
      if (!this.isMidiTrackActive) return;
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
    this.refactorSongParts();
    console.log(this.songParts);
  },
  mounted() {
    this.loadGuitar();
    this.loadBackingTrack();
  },
};
</script>
