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
  </div>
</template>

<script>
import { Howl } from "howler";
import * as Tone from "tone";
import MIDIMessage from "../utils/MIDIMessage.js";
import MIDIccMessage from "../utils/MIDIccMessage.js";
import { MIDIMessageTypesStrings } from "../utils/MIDIMessageTypes.js";
import { midiNotesMapping as mapMidi } from "../utils/midiNotesMapping.js";

export default {
  data() {
    return {
      audioContext: null,
      player: null,
      musicTrack: null,
      isMidiTrackActive: true,
      currentMidiMelody: null,
      isMusicPlaying: false,
      partLength: 8000,
      introEnd: 2701,
      sampler: null,
      selectedIndex: 1,
      midiSeq: null,
      midi_message: null,
      songParts: [
        {
          name: "part1",
          startingTime: 0,
          duration: 0,
          MidiSeekPoint: 0,
          midiTriggered: false,
          midiMelody: [],
        },
        {
          name: "part2",
          startingTime: null,
          duration: 8000,
          MidiSeekPoint: 2.702,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: ["A#3", "D4"], duration: "1m", string: [2, 1] },
            { time: 1, note: "E4", duration: "8n", string: [2, 1] },
            { time: 2, note: "E4", duration: "8n", string: [2, 1] },
            { time: 3, note: "A#3", duration: "8n", string: [2, 1] },
          ],
        },
        {
          name: "part3",
          startingTime: null,
          duration: 8000,
          MidiSeekPoint: 6 + 8,
          midiTriggered: false,
          midiMelody: [
            { time: 0, note: ["C4", "E4", "F4"], duration: "8n" },
            { time: "0:2", note: "E4", duration: "4n" },
            { time: "0:4", note: "F4", duration: "8n" },
          ],
        },
        { name: "part4", startingTime: null, duration: 8000, MidiSeekPoint: 6 + 8 * 1, midiTriggered: false },
        { name: "part5", startingTime: null, duration: 8000, MidiSeekPoint: 6 + 8 * 2, midiTriggered: false },
        { name: "part6", startingTime: null, duration: 8000, MidiSeekPoint: 6 + 8 * 3, midiTriggered: false },
        { name: "part7", startingTime: null, duration: 8000, MidiSeekPoint: 6 + 8 * 4, midiTriggered: false },
        { name: "part8", startingTime: null, duration: 8000, MidiSeekPoint: 6 + 8 * 5, midiTriggered: false },
      ],
    };
  },
  computed: {
    currentPart() {
      return this.songParts[this.selectedIndex];
    },
  },
  methods: {
    refactorSongParts() {
      for (let i = 0; i < this.songParts.length; i++) {
        this.songParts[i].startingTime = this.songParts[i - 1] ? this.songParts[i - 1].startingTime + this.songParts[i - 1].duration : 0;
        this.songParts[i].duration = this.songParts[i - 1] ? this.songParts[i].duration : this.introEnd;
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

      //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
      //Try yo create my own scheduler. Record midi with time stamp and note. Then play it back with Tone.js feeding timestamp to the time (3rd)parameter of triggerAttackRelease
      //_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
    },
    play(note) {
      const synth = new Tone.Synth().toDestination();
      const now = Tone.now();
      // trigger the attack immediately
      synth.triggerAttack(note, now);
      // wait one second before triggering the release
      synth.triggerRelease(now + 1);
    },
    initHowl() {
      // const intro_time = 2701;
      this.musicTrack = new Howl({
        src: ["/tracks/C_Jazz_2-5-1_BPM_120.wav"],
        loop: true,
        sprite: {
          part1: [0, this.introEnd],
          part2: [this.introEnd, this.songParts[1].duration],
          part3: [10701, this.songParts[2].duration],
          part4: [18701, this.songParts[3].duration],
          part5: [26701, this.songParts[4].duration],
          part6: [34701, this.songParts[5].duration],
          part7: [42701, this.songParts[6].duration],
          part8: [50701, this.songParts[7].duration],
        },
      });
    },
    initTone() {
      Tone.start();
      this.loadGuitar();
    },
    async playMidiMelody(midiMelody) {
      if (!midiMelody.length) return;
      this.midiSeq = new Tone.Part(
        (time, note) => {
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
          }, 400);
        },
        midiMelody,
        "4n"
      );
      this.midiSeq.humanize = true;
      this.midiSeq.loop = false;
      this.midiSeq.start(0);
      Tone.Transport.bpm.value = 120;
      Tone.Transport.start();
    },
    activateMidi() {
      this.isMidiTrackActive = true;
    },
    stopMidi() {
      this.midiSeq.stop();
      this.midiSeq.clear();
      this.midiSeq.dispose();
      this.isMidiTrackActive = false;
    },
    playPart(part) {
      this.musicTrack.stop();
      this.musicTrack.loop(true);
      this.musicTrack.play(part.name);
    },
    goToNextPart() {
      this.musicTrack.once("end", () => {
        this.musicTrack.loop(false);
        this.musicTrack.stop();
        this.selectedIndex = this.selectedIndex > this.songParts.length - 1 ? this.selectedIndex : this.selectedIndex + 1;
        this.currentPart.midiTriggered = false;
        this.playPart(this.currentPart);
      });
    },
    goToPreviousPart() {
      this.musicTrack.once("end", () => {
        this.musicTrack.loop(false);
        this.musicTrack.stop();
        this.selectedIndex = this.selectedIndex == 1 ? this.selectedIndex : this.selectedIndex - 1;
        this.currentPart.midiTriggered = false;
        this.playPart(this.currentPart);
      });
    },
    stopMusic() {
      this.musicTrack.stop();
      this.midiSeq.stop();
      this.midiSeq.clear();
      this.midiSeq.dispose();
      Tone.Transport.stop();
      this.currentPart.midiTriggered = false;
      this.isMusicPlaying = false;
    },
    startMusic() {
      if (this.isMusicPlaying) return;
      this.initHowl();
      this.initTone();
      this.checkInterval = setInterval(() => {
        const seek = this.musicTrack.seek();
        if (seek >= this.currentPart.MidiSeekPoint && !this.currentPart.midiTriggered) {
          console.log("seek: ", seek);
          this.currentPart.midiTriggered = true;
          this.playMidiMelody(this.currentPart.midiMelody);
        }
      }, 10);
      this.musicTrack.loop(false);
      this.musicTrack.play(this.songParts[0].name);
      this.musicTrack.once("end", () => {
        this.playPart(this.songParts[1]);
      });
      this.musicTrack.on("end", () => {
        this.midiSeq.stop();
        this.midiSeq.clear();
        this.midiSeq.dispose();
        Tone.Transport.stop();
        this.currentPart.midiTriggered = false;
      });
      this.isMusicPlaying = true;
    },
  },
  watch: {
    midi_message(newValue) {
      this.$parent.$parent.$emit("MIDI-message", newValue);
    },
  },
  created() {
    this.refactorSongParts();
  },
  mounted() {},
};
</script>
