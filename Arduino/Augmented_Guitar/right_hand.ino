void setupStrumming() {
  for (int i = 0; i < 6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }
}

void update_peak_value(struct StringStruct& string) {
  if (string.current_amplitude > string.min_threshold) {
    if (string.current_amplitude < string.previous_amplitude - sine_wave_falling_edge) {
      string.peak_value = string.previous_amplitude;
      string.peak_value = map(string.peak_value, 0, string.max_amplitude, 0, 127);
      if (string.peak_value > 127) {
        string.peak_value = 127;
      }
    } else {
      string.peak_value = 0;
    }
  } else {
    string.peak_value = 0;
  }
  string.previous_amplitude = string.current_amplitude;
}