export default class fretSlot {
  constructor(MIDI_note, string, fret, color, text, is_circle, border, text_color, z_index) {
    this.MIDI_note = MIDI_note;
    this.string = string;
    this.fret = fret;
    this.color = color;
    this.text = text;
    this.is_circle = is_circle;
    this.border = border;
    this.text_color = text_color;
    this.z_index = z_index;
  }
}
