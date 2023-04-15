#include "Arduino.h"
#include "debug.h"
#include "../StringStruct/string_struct.h"
#include "../MIDI/midi.h"

#define hysteresis 25

bool sine_wave_started = false;
int record_sine_wave_counter = 0;
int sine_wave_array[NUM_OF_ARR_VALUES];

void printTouchedMIDIValues() {
  for (int i = 0; i < 6; i++) {
    Serial.print(MIDI_open_string_notes[string_structs[i].pressed_fret]);
    Serial.print("");
  }
  Serial.println("");
}

void printSineWaveValues(int string_number, int iteration, int number_of_iterations) {
  if (string_number == iteration) {
    if (sine_wave_started == false && string_structs[string_number].current_amplitude > string_structs[string_number].min_threshold && string_structs[string_number].current_amplitude > string_structs[string_number].previous_amplitude + hysteresis) {
      sine_wave_started = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (sine_wave_started && record_sine_wave_counter < number_of_iterations) {
      sine_wave_array[record_sine_wave_counter] = string_structs[string_number].current_amplitude;
      record_sine_wave_counter++;
    }
    if (record_sine_wave_counter == number_of_iterations - 1) {
      for (int j = 0; j < number_of_iterations - 1; j++) {
        Serial.println(sine_wave_array[j]);
      }
    }
  }
}

void println(String str1) {
  Serial.println(str1);
}

void println(int var) {
  Serial.println(var);
}

void println(String str1, int var1) {
  Serial.println(str1 + " " + String(var1) + " ");
}

void print(String str1) {
  Serial.print(str1 + " ");
}

void print(int var1) {
  Serial.print(String(var1) + " ");
}

void print(String str1, int var1) {
  Serial.print(str1 + " " + String(var1) + " ");
}
