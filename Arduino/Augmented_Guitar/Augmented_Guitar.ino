/*
  Hexaphonic Pickup Color Coding
  brown  :E string
  yellow :A string
  green  :D string
  red    :G string
  orange :B string
  white  :E string
*/

#include "src/debug/debug.h"
#include "src/teensy_touch/teensy_touch.h"
#include "src/mux/mux.h"
#include "src/MIDI/midi.h"
#include "src/device_specs/device_specs.h"
#include "src/GuitarStringClass/guitar_string.h"


const int string_input_pins[NUM_OF_STRINGS] = {34, 35, 36, 37, 38, 39};
const char open_string_notes[NUM_OF_STRINGS] = {'E', 'A', 'D', 'G', 'B', 'e'};
const int max_amplitudes[NUM_OF_STRINGS] = {540, 510, 560, 580, 620, 600};
const int min_thresholds[NUM_OF_STRINGS] = {15, 18, 20, 20, 15, 23};
const int max_wave_periods[NUM_OF_STRINGS] = {15, 10, 8, 6, 4, 3};

int mux_pins[NUM_OF_MUX_PINS] = {mux_1_pin, mux_2_pin};
int* ptr_mux_pins = mux_pins;
int touch_analog_values[NUM_OF_NOTES] = {0};
int* ptr_touch_analog_values = touch_analog_values;
int mux_ch = 0;
int* ptr_mux_ch = &mux_ch;
int touch_reference_analog_values[NUM_OF_NOTES] = {0};
int* ptr_touch_reference_analog_values = touch_reference_analog_values;

GuitarString guitar_strings[6];

#define DEBUG 1

void setup() {
#if DEBUG == 1
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial);
#endif

  /*_-_-Class setup_-_-*/
  for (int i = 0; i < 6; i++) {
    guitar_strings[i] = {i, string_input_pins[i], open_string_notes[i], max_amplitudes[i], min_thresholds[i], max_wave_periods[i], touch_analog_values, touch_reference_analog_values};
  }

  /*_-_-Left hand setup_-_-*/
  selectMuxChannel(0);
  teensyTouchInit(mux_pins[0]);
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }


  // Capacitance calibration
  while (touch_reference_analog_values[31] == 0) {
    teensyTouchRead(touch_reference_analog_values, 32, ptr_touch_reference_analog_values, mux_pins, 2, ptr_mux_pins, ptr_mux_ch);
  }

  /*_-_-Right hand setup_-_-*/
  for (int i = 0; i < 6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }

  /*_-_-Struct setup_-_-*/
  // for (int i = 0; i < 6; i++) {
  //   string_structs[i] = {max_wave_periods[i], false, 0, open_string_notes[i], string_input_pins[i],
  //                        0, 0, 0, 0, min_thresholds[i], max_amplitudes[i], 0, 0, 0
  //                       };
  // }
}



void loop() {

  teensyTouchRead(touch_analog_values, 32, ptr_touch_analog_values, mux_pins, 2, ptr_mux_pins, ptr_mux_ch);

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_strings[i].updateStringMIDIValue();
  }

  for (int i = 0; i < 6; i++) {
    Serial.print(guitar_strings[i].pressed_fret);
  }
  Serial.println("");

  // Detect peak and play MIDI for each string
  for (int i = 0; i < 6; i++) {

    guitar_strings[i].get_current_amplitude();

    guitar_strings[i].detect_note_off();
    guitar_strings[i].update_peak_value();
    guitar_strings[i].detect_note_on();
    guitar_strings[i].update_last_peak_value();

    
  }
}
