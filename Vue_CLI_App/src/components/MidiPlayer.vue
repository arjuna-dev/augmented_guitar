<template>
  <div>
    <a href="#" @click.prevent="play(C4)">C4</a>
    <a href="#" @click.prevent="play(D4)">D4</a>
    <a href="#" @click.prevent="playIntro()">Start Music</a>
    <button @click="goToPreviousPart()">Previous Part</button>
    <button @click="goToNextPart()">Next Part</button>
  </div>
</template>

<script>
/* global WebAudioFontPlayer */
import { Howl } from "howler";

export default {
  data() {
    return {
      audioContext: null,
      player: null,
      sound: null,
      currentPart: "part1",
      songParts: ["part1", "part2", "part3"],
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
  methods: {
    loadSoundFont() {
      const script = document.createElement("script");
      script.src =
        "https://surikov.github.io/webaudiofontdata/sound/0250_SoundBlasterOld_sf2.js";
      script.onload = () => {
        console.log("SoundFont script loaded");
      };
      script.onerror = (e) => {
        console.error("Error loading the SoundFont script", e);
      };
      document.head.appendChild(script);
    },
    loadWebAudioFontPlayer() {
      const script = document.createElement("script");
      script.src =
        "https://surikov.github.io/webaudiofont/npm/dist/WebAudioFontPlayer.js";
      script.onload = () => {
        console.log("WebAudioFontPlayer script loaded");
        this.player = new WebAudioFontPlayer();
      };
      script.onerror = (e) => {
        console.error("Error loading the WebAudioFontPlayer script", e);
      };
      document.head.appendChild(script);
    },
    play(note) {
      if (!this.audioContext) {
        this.audioContext = new (window.AudioContext ||
          window.webkitAudioContext)();
      }
      if (window._tone_0250_SoundBlasterOld_sf2) {
        this.player.loader.decodeAfterLoading(
          this.audioContext,
          "_tone_0250_SoundBlasterOld_sf2"
        );
        this.player.queueWaveTable(
          this.audioContext,
          this.audioContext.destination,
          window._tone_0250_SoundBlasterOld_sf2,
          0,
          note,
          2
        );
        return false;
      }
    },
    initHowler() {
      this.sound = new Howl({
        // src: ['/mp3/Cmaj7_Jazzy.m4a']
        src: ["/mp3/C_Jazz_2-5-1.wav"],
        loop: true,
        sprite: {
          part1: [0, 10701 - 6000],
          part2: [2701, 8000 - 6000],
          part3: [10701, 8000 - 6000],
          part4: [18701, 8000 - 6000],
          part5: [26701, 8000 - 6000],
          part6: [34701, 8000 - 6000],
          part7: [42701, 8000 - 6000],
          part8: [50701, 8000 - 6000],
        },
      });
      this.checkInterval = setInterval(() => {
        const seek = this.sound.seek();
        console.log("seek", seek);
        if (3.05 < seek <= 3) {
          console.log("play C5");
          this.play(this.C5);
        }
      }, 10);
    },
    playIntro() {
      this.initHowler();
      this.playPart("part1");
      this.sound.once("end", () => {
        this.playPart("part2");
      });
    },

    playPart(partName) {
      this.sound.stop();
      this.currentPart = partName;
      this.sound.loop(true);
      this.sound.play(partName);
    },
    goToNextPart() {
      this.sound.once("end", () => {
        this.sound.loop(false);
        this.sound.stop();
        var currentIndex = this.songParts.indexOf(this.currentPart);
        var nextIndex = (currentIndex + 1) % this.songParts.length;
        if (nextIndex < this.songParts.length - 1) {
          nextIndex = currentIndex;
        }
        this.playPart(this.songParts[nextIndex]);
      });
    },
    goToPreviousPart() {
      this.sound.once("end", () => {
        this.sound.loop(false);
        this.sound.stop();
        var currentIndex = this.songParts.indexOf(this.currentPart);
        var nextIndex = (currentIndex - 1) % this.songParts.length;
        if (nextIndex < 1) {
          nextIndex = currentIndex;
        }
        this.playPart(this.songParts[nextIndex]);
      });
    },
  },
  mounted() {
    this.loadSoundFont();
    this.loadWebAudioFontPlayer();
  },
};
</script>
