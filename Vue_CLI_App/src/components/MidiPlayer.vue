<template>
  <div>
    <a href="#" @click.prevent="play(C4)">C4</a>
    <a href="#" @click.prevent="play(D4)">D4</a>
    <a href="#" @click.prevent="startMusic()">Start Music</a>
    <button @click="goToPreviousPart()">Previous Part</button>
    <button @click="goToNextPart()">Next Part</button>
    <button @click="loadGuitar()">Play Guitar Chord</button>
    <button @click="startMidi()">startMidi</button>
    <button @click="stopMidi()">stopMidi</button>
  </div>
</template>

<script>
import { Howl } from "howler";
import * as Tone from "tone";
import ChordSequence from "../utils/chordSequence";

export default {
  data() {
    return {
      audioContext: null,
      player: null,
      musicTrack: null,
      currentMidiMelody: null,
      partLength: 8000,
      startFirstPart: 2701,
      sampler: null,
      selectedIndex: 1,
      midiSeq: null,
      songParts: [
        {
          name: "part1",
          startingTime: 0,
          MidiSeekPoint: 100000000,
          midiTriggered: false,
        },
        {
          name: "part2",
          startingTime: 2701,
          MidiSeekPoint: 3.05,
          midiTriggered: false,
          midiMelody: ["A3", [["C4", "E4", "G4"]], [["E4", "G4", "B4"]], [["E4", "G4", "B4"]]],
        },
        {
          name: "part3",
          startingTime: 2701 + 8000,
          MidiSeekPoint: 6 + 8,
          midiTriggered: false,
          midiMelody: ["A3", [["C4", "E4", "G4"]], [["E4", "G4", "B4"]], [["D4", "E4", "A5"]]],
        },
        { name: "part4", startingTime: 2701 + 8000 * 2, MidiSeekPoint: 6 + 8 * 1, midiTriggered: false },
        { name: "part5", startingTime: 2701 + 8000 * 3, MidiSeekPoint: 6 + 8 * 2, midiTriggered: false },
        { name: "part6", startingTime: 2701 + 8000 * 4, MidiSeekPoint: 6 + 8 * 3, midiTriggered: false },
        { name: "part7", startingTime: 2701 + 8000 * 5, MidiSeekPoint: 6 + 8 * 4, midiTriggered: false },
        { name: "part8", startingTime: 2701 + 8000 * 6, MidiSeekPoint: 6 + 8 * 5, midiTriggered: false },
      ],
    };
  },
  computed: {
    currentPart() {
      return this.songParts[this.selectedIndex];
    },
  },
  methods: {
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
      this.musicTrack = new Howl({
        src: ["/tracks/C_Jazz_2-5-1.wav"],
        loop: true,
        sprite: {
          part1: [0, 2701],
          part2: [2701, 8000],
          part3: [10701, 8000],
          part4: [18701, 8000],
          part5: [26701, 8000],
          part6: [34701, 8000],
          part7: [42701, 8000],
          part8: [50701, 8000],
        },
      });
    },
    initTone() {
      Tone.start();
      this.loadGuitar();
    },
    async playMidiMelody(midiMelody) {
      await Tone.loaded();
      this.midiSeq = new ChordSequence(
        (time, note) => {
          this.sampler.triggerAttackRelease(note, 0.1, time);
        },
        midiMelody,
        "4n"
      );
      this.midiSeq.humanize = true;
      this.midiSeq.loop = true;
      this.midiSeq.start(0);
      Tone.Transport.start();
    },
    startMidi() {
      this.initTone();
      this.playMidiMelody(["A3", [["C4", "E4", "G4"]], [["E4", "G4", "B4"]], [["E4", "G4", "B4"]], [["D4", "E4", "A5"]]]);
      this.midiSeq.start(0);
    },
    stopMidi() {
      this.midiSeq.stop();
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
    startMusic() {
      this.initHowl();
      this.initTone();
      this.checkInterval = setInterval(() => {
        if (this.midiSeq) {
          if (this.midiSeq.progress > 0.98) {
            this.midiSeq.stop();
            this.midiSeq.clear();
            this.midiSeq.dispose();
            Tone.Transport.stop();
          }
        }
        const seek = this.musicTrack.seek();
        if (seek >= this.currentPart.MidiSeekPoint && !this.currentPart.midiTriggered) {
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
        this.currentPart.midiTriggered = false;
      });
    },
  },
  mounted() {},
};
</script>
