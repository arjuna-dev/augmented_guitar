const songParts = [
  {
    partNumber: 0,
    startTime: null,
    bars: null,
    duration: null,
    endTime: null,
    midiTriggered: false,
    midiMelody: [],
  },
  {
    partNumber: 1,
    startTime: null,
    bars: 4,
    duration: null,
    endTime: null,
    midiTriggered: false,
    midiMelody: [{ time: 0, note: "C4", string: 2, duration: 1 }],
  },
  {
    partNumber: 2,
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
    partNumber: 3,
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
    partNumber: 4,
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
    partNumber: 5,
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
    partNumber: 6,
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
    partNumber: 7,
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
    partNumber: 8,
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
  // chatGPT generated

  {
    partNumber: 9,
    startTime: null,
    endTime: null,
    bars: 8,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 },
      { time: 1, note: "E4", string: 1, duration: 1 },
      { time: 2, note: "G3", string: 3, duration: 1 },
      { time: 3, note: "C4", string: 2, duration: 1 },
    ],
  },

  {
    partNumber: 10,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "G3", string: 3, duration: 1 / 2 },
      { time: 1 / 2, note: "E4", string: 1, duration: 1 / 2 },
      { time: 1, note: "C4", string: 2, duration: 1 / 2 },
      { time: 3 / 2, note: "G4", string: 0, duration: 1 / 2 },
    ],
  },

  {
    partNumber: 11,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "B3", string: 2, duration: 1 / 4 },
      { time: 1 / 4, note: "G3", string: 3, duration: 1 / 4 },
      { time: 1 / 2, note: "E4", string: 1, duration: 1 / 4 },
      { time: 3 / 4, note: "C4", string: 2, duration: 1 / 4 },
    ],
  },
  {
    partNumber: 12,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "E4", string: 1, duration: 1 / 4 },
      { time: 1 / 4, note: "C4", string: 2, duration: 1 / 4 },
      { time: 1 / 2, note: "G4", string: 0, duration: 1 / 4 },
      { time: 3 / 4, note: "B3", string: 2, duration: 1 / 4 },
    ],
  },
  {
    partNumber: 13,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "E4", string: 1, duration: 1 / 4 },
      { time: 1 / 4, note: "C4", string: 2, duration: 1 / 4 },
      { time: 2 / 4, note: "G4", string: 0, duration: 1 / 4 },
      { time: 3 / 4, note: "B3", string: 2, duration: 1 / 4 },
      { time: 5 / 4, note: "C4", string: 2, duration: 1 / 4 },
    ],
  },
  {
    partNumber: 14,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "G3", string: 3, duration: 1 / 2 },
      { time: 1 / 2, note: "B3", string: 2, duration: 1 / 2 },
      { time: 1, note: "C4", string: 2, duration: 1 / 2 },
      { time: 3 / 2, note: "E4", string: 1, duration: 1 / 2 },
    ],
  },
  {
    partNumber: 15,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "G3", string: 3, duration: 1 / 2 },
      { time: 1 / 2, note: "B3", string: 2, duration: 1 / 2 },
      { time: 1, note: "C4", string: 2, duration: 1 / 2 },
      { time: 3 / 2, note: "G4", string: 0, duration: 1 / 2 },
    ],
  },
  {
    partNumber: 16,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "G3", string: 3, duration: 1 / 2 },
      { time: 1, note: "B3", string: 2, duration: 1 / 2 },
      { time: 3 / 2, note: "E4", string: 1, duration: 1 / 2 },
    ],
  },
  {
    partNumber: 17,
    startTime: null,
    endTime: null,
    bars: 8,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "E4", string: 1, duration: 1 },
      { time: 1, note: "C4", string: 2, duration: 1 },
      { time: 2, note: "G4", string: 0, duration: 1 },
      { time: 3, note: "B3", string: 2, duration: 1 },
    ],
  },
  {
    partNumber: 18,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "E4", string: 1, duration: 1 / 2 },
      { time: 1, note: "G4", string: 0, duration: 1 / 2 },
      { time: 3 / 2, note: "B3", string: 2, duration: 1 / 2 },
    ],
  },
  {
    partNumber: 19,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "E4", string: 1, duration: 1 / 2 },
      { time: 1, note: "G4", string: 0, duration: 1 / 2 },
      { time: 3 / 2, note: "A4", string: 0, duration: 1 / 2 },
    ],
  },

  // New notes added
  {
    partNumber: 20,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "E4", string: 1, duration: 1 / 2 },
      { time: 1, note: "G4", string: 0, duration: 1 / 2 },
      { time: 3 / 2, note: "C5", string: 0, duration: 1 / 2 },
    ],
  },
  {
    partNumber: 21,
    startTime: null,
    endTime: null,
    bars: 8,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "G3", string: 3, duration: 1 },
      { time: 1, note: "A3", string: 3, duration: 1 },
      { time: 2, note: "B3", string: 2, duration: 1 },
      { time: 3, note: "C4", string: 2, duration: 1 },
    ],
  },

  {
    partNumber: 22,
    startTime: null,
    endTime: null,
    bars: 8,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "D4", string: 2, duration: 1 },
      { time: 1, note: "B4", string: 0, duration: 1 },
      { time: 2, note: "C5", string: 0, duration: 1 },
    ],
  },

  {
    partNumber: 23,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "D4", string: 2, duration: 1 / 2 },
      { time: 1, note: "E4", string: 1, duration: 1 / 2 },
      { time: 3 / 2, note: "G4", string: 1, duration: 1 / 2 },
    ],
  },

  {
    partNumber: 24,
    startTime: null,
    endTime: null,
    bars: 8,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "D4", string: 2, duration: 1 / 2 },
      { time: 1, note: "E4", string: 1, duration: 1 / 2 },
      { time: 3 / 2, note: "G4", string: 1, duration: 1 / 2 },
      { time: 2, note: "C5", string: 0, duration: 1 / 2 },
    ],
  },

  {
    partNumber: 25,
    startTime: null,
    endTime: null,
    bars: 8,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "C4", string: 2, duration: 1 / 2 },
      { time: 1 / 2, note: "D4", string: 2, duration: 1 / 2 },
      { time: 1, note: "E4", string: 1, duration: 1 / 2 },
      { time: 3 / 2, note: "G4", string: 1, duration: 1 / 2 },
      { time: 2, note: "C5", string: 0, duration: 1 / 2 },
      { time: 2 + 1 / 4, note: "C5", string: 0, duration: 1 / 2 },
      { time: 2 + 3 / 4, note: "C5", string: 0, duration: 1 / 2 },
    ],
  },

  {
    partNumber: 26,
    startTime: null,
    endTime: null,
    bars: 4,
    duration: null,
    midiTriggered: false,
    midiMelody: [
      { time: 0, note: "E4", string: 1, duration: 1 / 3 },
      { time: 1 / 3, note: "G4", string: 0, duration: 1 / 3 },
      { time: 2 / 3, note: "B4", string: 0, duration: 1 / 3 },
      { time: 1, note: "C5", string: 0, duration: 1 / 3 },
      { time: 4 / 3, note: "D4", string: 2, duration: 1 / 3 },
    ],
  },

  {
    partNumber: 27,
    startTime: null,
    endTime: null,
    bars: 32,
    duration: null,
    midiTriggered: false,
    midiMelody: [],
  },
];

export default songParts;
