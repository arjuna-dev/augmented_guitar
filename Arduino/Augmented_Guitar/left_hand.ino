void setupLeftHand() {
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }
  // Calibration
  while (touch_reference_analog_values[31] == 0) {
    teensyTouchRead(touch_reference_analog_values, 32, ptr_touch_reference_analog_values, mux_pins, 2, ptr_mux_pins, ptr_mux_ch);
  }
}

void updateStringMIDIValue(struct StringStruct& string, int string_number) {
  bool last_fret_touched = true;
  bool fret_touched = false;
  bool any_fret_touched = false;
  for (int j = 0; j < 4; j++) {
    if (touch_analog_values[value_positions[string_number][j]] - touch_reference_analog_values[value_positions[string_number][j]] > capacitance_threshold) {
      fret_touched = true;
      any_fret_touched = true;
    } else {
      fret_touched = false;
    }
    if (last_fret_touched && fret_touched && !string.note_on) {
      string.fret = j + 1;
      string.MIDI_value = MIDI_open_string_notes[string_number] + j + 1;
    }
    last_fret_touched = fret_touched;
  }
  if (!any_fret_touched && !string.note_on) {
    string.fret = 0;
    string.MIDI_value = MIDI_open_string_notes[string_number];
  }
  if (any_fret_touched && string.fret_last != string.fret) {
    MIDI_press_fret(string.MIDI_value, string.fret);
  }
  string.fret_last = string.fret;
}
