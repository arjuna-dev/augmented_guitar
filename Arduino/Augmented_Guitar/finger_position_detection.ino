void setupFingerPosition(){
  for (int i=0; i<4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);  
  }
  // Calibration
  updateTouchValues(touch_reference_analog_values);
}

int readMux(int channel, int signal_pin){
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  return touchRead(signal_pin);
}

void updateTouchValues(int *touch_array){
  for(int i = 0; i < 16; i ++){
    touch_array[i] = readMux(i, SIG_pin_1);
    if (i<8) {
      touch_array[i+16] = readMux(i, SIG_pin_2);  
    }
  }
}

void updateStringMIDIValue(struct StringStruct& string, int string_number){
    string.MIDI_value = MIDI_open_string_notes[string_number];
    string.both_frets_touched = false;
    bool last_fret_touched = true;
    bool fret_touched = false;
    for (int j=0; j<4; j++){
      if (touch_analog_values[value_positions[string_number][j]] - touch_reference_analog_values[value_positions[string_number][j]]>capacitance_threshold){
        fret_touched = true;
        string.fret_number_touched = j+1;
        string.MIDI_value = MIDI_open_string_notes[string_number]+j+1;
      } 
      if (last_fret_touched && fret_touched){
        string.both_frets_touched = true; 
      }
      last_fret_touched = fret_touched;
    }
}
