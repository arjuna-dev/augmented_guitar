<template>
  <div>
    <!-- <a href="#" @click.prevent="play('C6')">C4</a>
    <a href="#" @click.prevent="play('D4')">D4</a> -->
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
import * as Tone from "tone";
import MIDIMessage from "../utils/MIDIMessage.js";
import MIDIccMessage from "../utils/MIDIccMessage.js";
import { MIDIMessageTypesStrings } from "../utils/MIDIMessageTypes.js";
import { midiNotesMapping as mapMidi } from "../utils/midiNotesMapping.js";

export default {
  data() {
    return {
      audioContext: null,
      backingTrack: null,
      multiPlayer: null,
      isMidiTrackActive: true,
      isMusicPlaying: false,
      sampler: null,
      selectedIndex: 1,
      lastIndex: 1,
      tonePart: null,
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
  },
  methods: {
    play(note) {
      const synth = new Tone.Synth().toDestination();
      const now = Tone.now();
      synth.triggerAttack(note, now);
      synth.triggerRelease(now + 1);
    },
    refactorSongParts() {
      for (let i = 0; i < this.songParts.length; i++) {
        this.songParts[i].startTime = this.songParts[i - 1] ? this.songParts[i - 1].endTime : 0;
        const durationInSeconds = Tone.Time(this.songParts[i].duration).toSeconds();
        this.songParts[i].endTime = this.songParts[i - 1] ? this.songParts[i].startTime + durationInSeconds : this.introEnd;
      }
    },
    loadGuitar() {
      this.sampler = new Tone.Sampler({
        urls: {
          C4: "C4.mp3",
          "D#4": "Ds4.mp3",
          "F#4": "Fs4.mp3",
          A4: "A4.mp3",
        },
        release: 1,
        baseUrl: "/instruments/guitar_electric/",
      }).toDestination();
    },
    loadPlayers() {
      const multiPlayer = new Tone.Players({
        backingTrack: "/tracks/C_Jazz_2-5-1_BPM_120.wav",
      }).toDestination();
      this.backingTrack = multiPlayer.player("backingTrack");
    },
    initTone() {
      Tone.start();
      this.loadGuitar();
      this.loadPlayers();
    },
    createTonePart(midiMelody) {
      this.tonePart = new Tone.Part((time, note) => {
        this.sampler.triggerAttackRelease(note.note, note.duration, time);
        let noteName = mapMidi[note.note];
        if (Array.isArray(note.note)) {
          noteName = [];
          for (let i = 0; i < note.note.length; i++) {
            noteName[i] = mapMidi[note.note[i]];
          }
        }
        this.midi_message = new MIDIccMessage(note.string, MIDIMessageTypesStrings.cc, 0, noteName);
        setTimeout(() => {
          this.midi_message = new MIDIMessage(note.string, MIDIMessageTypesStrings.note_on, noteName, 127);
        }, 1);
        setTimeout(() => {
          this.midi_message = new MIDIMessage(note.string, MIDIMessageTypesStrings.note_off, noteName, 0);
        }, Tone.Time(note.duration).toMilliseconds());
      }, midiMelody);
      this.tonePart.humanize = true;
      this.tonePart.loop = true;
      this.tonePart.loopStart = 0;
      this.tonePart.loopEnd = Tone.Time(this.currentPart.duration).toSeconds();
    },
    clearTonePart() {
      this.tonePart.clear();
    },
    updateTonePart(songPart) {
      this.tonePart.loopEnd = songPart.duration;
      for (let i = 0; i < songPart.midiMelody.length; i++) {
        this.tonePart.add(songPart.midiMelody[i]);
      }
    },
    activateMidi() {
      if (this.isMidiTrackActive) return;
      this.tonePart.mute = false;
      this.isMidiTrackActive = true;
    },
    stopMidi() {
      if (!this.isMidiTrackActive) return;
      this.tonePart.mute = true;
      this.isMidiTrackActive = false;
    },
    goToNextPart() {
      this.selectedIndex = this.selectedIndex == this.songParts.length - 1 ? this.selectedIndex : this.selectedIndex + 1;
      this.$nextTick(() => {
        console.log("this.selectedIndex: ", this.selectedIndex);
        setTimeout(() => {}, 100);
      });
    },
    goToPreviousPart() {
      this.selectedIndex = this.selectedIndex <= 1 ? this.selectedIndex : this.selectedIndex - 1;
      this.$nextTick(() => {
        console.log("this.selectedIndex: ", this.selectedIndex);
      });
    },
    stopMusic() {
      if (this.tonePart) {
        this.tonePart.stop();
        this.tonePart.clear();
        this.tonePart.dispose();
      }
      this.backingTrack.stop();
      Tone.Transport.cancel();
      Tone.Transport.clear();
      Tone.Transport.stop();
      this.currentPart.midiTriggered = false;
      this.isMusicPlaying = false;
    },
    async startMusic() {
      if (this.isMusicPlaying) return;
      this.isMusicPlaying = true;
      this.initTone();
      await Tone.loaded();
      this.backingTrack.loop = true;
      this.backingTrack.loopStart = 0;
      this.backingTrack.loopEnd = this.songParts[1].endTime;
      this.backingTrack.setLoopPoints(0, this.songParts[1].endTime);

      this.createTonePart(this.songParts[1].midiMelody);

      // Go to next loop after intro
      Tone.Transport.scheduleOnce(() => {
        this.backingTrack.setLoopPoints(this.songParts[1].startTime, this.songParts[1].endTime);
      }, this.songParts[0].endTime - 0.01);

      // for (let i = 1; i < this.songParts.length; i++) {
      //   Tone.Transport.schedule(() => {
      //     this.$nextTick(() => {
      //       console.log("this.selectedIndex: ", this.selectedIndex);
      //       this.clearTonePart();
      //       this.updateTonePart(this.songParts[this.selectedIndex]);
      //       this.backingTrack.seek(this.songParts[this.selectedIndex].startTime);
      //       this.backingTrack.setLoopPoints(this.songParts[this.selectedIndex].startTime, this.songParts[this.selectedIndex].endTime);
      //     });
      //   }, this.songParts[i].endTime - 0.01);
      // }

      Tone.Transport.scheduleOnce(() => {
        this.tonePart.start(Tone.Transport.seconds);
      }, this.songParts[1].startTime - 0.01);

      // Start backingTrack at 0
      Tone.Transport.schedule(() => {
        this.backingTrack.start();
      }, 0);
      Tone.Transport.bpm.value = 120;
      Tone.Transport.start();
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
  mounted() {},
};
</script>
