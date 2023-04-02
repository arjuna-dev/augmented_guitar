/*
  Hexaphonic Pickup Color Coding
  brown  :E string
  yellow :A string
  green  :D string
  red    :G string
  orange :B string
  white  :E string
*/

#include "src/right_hand/right_hand.h"
#include "src/left_hand/left_hand.h"
#include "src/teensy_touch/teensy_touch.h"
#include "src/mux/mux.h"
#include "src/MIDI/midi.h"
#include "src/StringStruct/string_struct.h"

void setup() {
#if DEBUG == 1
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial);
#endif

  /*_-_-Left hand setup_-_-*/
  selectMuxChannel(0);
  teensyTouchInit(mux_pins[0]);
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }
  setupLeftHand();

  setup_right_hand();

  /*_-_-Struct setup_-_-*/
  for (int i = 0; i < 6; i++) {
    string_structs[i] = {max_wave_periods[i], false, 0, open_string_notes[i], string_input_pins[i],
                         0, 0, 0, 0, min_thresholds[i], max_amplitudes[i], 0, 0, 0
                        };
  }
}

void detect_note_on(int i) {
  if (string_structs[i].peak_value > string_structs[i].last_peak_value + peak_diff_threshold && !string_structs[i].note_on) {
    string_structs[i].note_on = true;
    string_structs[i].note_on_timestamp = millis();
    MIDI_note_on(i, string_structs[i].peak_value, string_structs[i].pressed_fret);
    string_structs[i].last_sent_note_on_fret = string_structs[i].pressed_fret;
  }
}

void detect_note_off(int i) {
  if (string_structs[i].note_on) {
    // Threshold crossed, reset note_on_timestamp
    if (string_structs[i].current_amplitude > string_structs[i].min_threshold) {
      string_structs[i].note_on_timestamp = millis();
    }

    if (string_structs[i].note_on_timestamp + string_structs[i].max_wave_period < millis()) {
      MIDI_note_off(i, string_structs[i].last_sent_note_on_fret);
      string_structs[i].note_on = false;
    }
  }
}

void loop() {

  teensyTouchRead(touch_analog_values, 32, ptr_touch_analog_values, mux_pins, 2, ptr_mux_pins, ptr_mux_ch);

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    updateStringMIDIValue(string_structs[i], i);
  }

  // Detect peak and play MIDI for each string
  for (int i = 0; i < 6; i++) {

    string_structs[i].current_amplitude = analogRead(string_structs[i].input_pin);

    detect_note_off(i);
    update_peak_value(string_structs[i]);
    detect_note_on(i);

    if (string_structs[i].peak_value) {
      string_structs[i].last_peak_value = string_structs[i].peak_value;
    }
  }
}
