const MIDIMessageTypes = {
  1000: "note_off",
  1001: "note_on",
  1011: "cc",
  1110: "pitch_bend",
  1010: "polyaftertouch",
  1101: "channel_pressure",
};
const MIDIMessageTypesStrings = {
  note_off: "note_off",
  note_on: "note_on",
  cc: "cc",
  pitch_bend: "pitch_bend",
  polyaftertouch: "polyaftertouch",
  channel_pressure: "channel_pressure",
};

export { MIDIMessageTypes, MIDIMessageTypesStrings };
