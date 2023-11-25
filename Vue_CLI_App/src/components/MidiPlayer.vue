<template>
  <div>
    <a href="#" @click.prevent="play(C4)">C4</a>
    <a href="#" @click.prevent="play(D4)">D4</a>
    <a href="#" @click.prevent="startMusic()">Start Music</a>
    <button @click="goToPreviousPart()">Previous Part</button>
    <button @click="goToNextPart()">Next Part</button>
    <button @click="loadGuitar()">Play Guitar Chord</button>
  </div>
</template>

<script>
import { Howl } from "howler";
import * as Tone from "tone";

export default {
  data() {
    return {
      audioContext: null,
      player: null,
      sound: null,
      currentMidiMelody: null,
      partLength: 8000,
      startFirstPart: 2701,
      songParts: [
        { name: "part1", startingTime: 0, MidiSeekPoint: 100000000, midiTriggered: false },
        { name: "part2", startingTime: 2701, MidiSeekPoint: 3.05, midiTriggered: false },
        { name: "part3", startingTime: 2701 + 8000, MidiSeekPoint: 6.05, midiTriggered: false },
        { name: "part4", startingTime: 2701 + 8000 * 2, MidiSeekPoint: 9.05, midiTriggered: false },
        { name: "part5", startingTime: 2701 + 8000 * 3, MidiSeekPoint: 12.05, midiTriggered: false },
        { name: "part6", startingTime: 2701 + 8000 * 4, MidiSeekPoint: 15.05, midiTriggered: false },
        { name: "part7", startingTime: 2701 + 8000 * 5, MidiSeekPoint: 18.05, midiTriggered: false },
        { name: "part8", startingTime: 2701 + 8000 * 6, MidiSeekPoint: 21.05, midiTriggered: false },
      ],
      currentPart: { name: "part1", startingTime: 0, MidiSeekPoint: 1000000000, midiTriggered: false },
      C2: 0 + 12 * 2,
      c2: 1 + 12 * 2,
      D2: 2 + 12 * 2,
      d2: 3 + 12 * 2,
      E2: 4 + 12 * 2,
      F2: 5 + 12 * 2,
      f2: 6 + 12 * 2,
      G2: 7 + 12 * 2,
      g2: 8 + 12 * 2,
      A2: 9 + 12 * 2,
      a2: 10 + 12 * 2,
      B2: 11 + 12 * 2,
      C3: 0 + 12 * 3,
      c3: 1 + 12 * 3,
      D3: 2 + 12 * 3,
      d3: 3 + 12 * 3,
      E3: 4 + 12 * 3,
      F3: 5 + 12 * 3,
      f3: 6 + 12 * 3,
      G3: 7 + 12 * 3,
      g3: 8 + 12 * 3,
      A3: 9 + 12 * 3,
      a3: 10 + 12 * 3,
      B3: 11 + 12 * 3,
      C4: 0 + 12 * 4,
      c4: 1 + 12 * 4,
      D4: 2 + 12 * 4,
      d4: 3 + 12 * 4,
      E4: 4 + 12 * 4,
      F4: 5 + 12 * 4,
      f4: 6 + 12 * 4,
      G4: 7 + 12 * 4,
      g4: 8 + 12 * 4,
      A4: 9 + 12 * 4,
      a4: 10 + 12 * 4,
      B4: 11 + 12 * 4,
      C5: 0 + 12 * 5,
      c5: 1 + 12 * 5,
      D5: 2 + 12 * 5,
      d5: 3 + 12 * 5,
      E5: 4 + 12 * 5,
      F5: 5 + 12 * 5,
      f5: 6 + 12 * 5,
      G5: 7 + 12 * 5,
      g5: 8 + 12 * 5,
      A5: 9 + 12 * 5,
      a5: 10 + 12 * 5,
      B5: 11 + 12 * 5,
      C6: 0 + 12 * 6,
      c6: 1 + 12 * 6,
      D6: 2 + 12 * 6,
      d6: 3 + 12 * 6,
      E6: 4 + 12 * 6,
      F6: 5 + 12 * 6,
      f6: 6 + 12 * 6,
      G6: 7 + 12 * 6,
      g6: 8 + 12 * 6,
      A6: 9 + 12 * 6,
      a6: 10 + 12 * 6,
      B6: 11 + 12 * 6,
    };
  },
  computed: {
    partNames() {
      if (this.songParts) {
        return this.songParts.map((part) => part.name);
      } else {
        return [];
      }
    },
  },
  methods: {
    loadGuitar() {
      const sampler = new Tone.Sampler({
        urls: {
          C4: "C4.mp3",
          "D#4": "Ds4.mp3",
          "F#4": "Fs4.mp3",
          A4: "A4.mp3",
        },
        release: 1,
        baseUrl: "/instruments/guitar_electric/",
      }).toDestination();

      // Tone.loaded().then(() => {
      //   sampler.triggerAttackRelease(["Eb5", "G4", "B3"], 0.5);
      // });
      // eslint-disable-next-line
      const seq = new Tone.Sequence(
        (time, note) => {
          sampler.triggerAttackRelease(note, 0.1, time);
          // subdivisions are given as subarrays
        },
        ["C4", ["E4", "D4", "E4"], "G4", ["A4", "G4"]]
      ).start(0);
      Tone.Transport.start();
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
      this.sound = new Howl({
        // src: ['/tracks/Cmaj7_Jazzy.m4a']
        src: ["/tracks/C_Jazz_2-5-1.wav"],
        loop: true,
        sprite: {
          part1: [0, 2701],
          part2: [2701, 8000 - 6000],
          part3: [10701, 8000 - 6000],
          part4: [18701, 8000 - 6000],
          part5: [26701, 8000 - 6000],
          part6: [34701, 8000 - 6000],
          part7: [42701, 8000 - 6000],
          part8: [50701, 8000 - 6000],
        },
      });
    },
    startMusic() {
      this.checkInterval = setInterval(() => {
        const seek = this.sound.seek();
        console.log("seek: ", seek);
        console.log("this.currentPart.MidiSeekPoint: ", this.currentPart.MidiSeekPoint);
        console.log("seek >= this.currentPart.MidiSeekPoint: ", seek >= this.currentPart.MidiSeekPoint);
        console.log("this.currentPart.midiTriggered: ", this.currentPart.midiTriggered);

        if (seek >= this.currentPart.MidiSeekPoint && this.currentPart.midiTriggered == false) {
          console.log("will play MIDI ");
          this.play(this.C5);
          this.currentPart.midiTriggered = true;
        }
      }, 10);
      this.sound.loop(false);
      this.sound.play(this.songParts[0].name);
      // this.playPart(this.songParts[0]);
      this.sound.once("end", () => {
        console.log(this.currentPart.name);
        this.playPart(this.songParts[1]);
        console.log(this.currentPart.name);
      });
    },

    playPart(part) {
      console.log("playPart: ", part);
      this.sound.stop();
      this.currentPart = part;
      this.sound.loop(true);
      this.sound.play(part.name);
    },
    goToNextPart() {
      this.sound.once("end", () => {
        this.sound.loop(false);
        this.sound.stop();
        var currentIndex = this.songParts.indexOf(this.currentPart);
        var nextIndex = currentIndex + 1;
        if (nextIndex > this.partNames.length - 1) {
          nextIndex = currentIndex;
        }
        this.currentPart = this.songParts[nextIndex];
        this.currentPart.midiTriggered = false;
        this.playPart(this.currentPart);
      });
    },
    goToPreviousPart() {
      this.sound.once("end", () => {
        this.sound.loop(false);
        this.sound.stop();
        var currentIndex = this.songParts.indexOf(this.currentPart);
        var nextIndex = currentIndex - 1;
        if (nextIndex < 1) {
          nextIndex = currentIndex;
        }
        this.currentPart = this.songParts[nextIndex];
        this.currentPart.midiTriggered = false;
        this.playPart(this.currentPart);
      });
    },
  },
  mounted() {
    this.initHowl();
  },
};
</script>
