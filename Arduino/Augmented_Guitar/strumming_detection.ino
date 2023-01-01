void setupStrumming(){
  for (int i=0; i<6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }
}

int peak_detection(struct StringStruct& string) {
  if(string.current_amplitude > string.min_threshold){
//    Serial.println("anything");
    if (string.current_amplitude > string.previous_amplitude + hysteresis) {
      string.increasing = true;
    }
    if (string.current_amplitude < string.previous_amplitude - hysteresis && string.increasing && string.current_amplitude > string.peak_value) {
      string.increasing = false;
      int note_amplitude = string.previous_amplitude;
      string.peak_value = note_amplitude;
      return note_amplitude;
    } else{
      string.peak_value = 0;
      return 0;
    }
  }
}
