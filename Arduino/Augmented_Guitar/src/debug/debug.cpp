#include "Arduino.h"
#include "debug.h"

void printCurrentMIDIValues(GuitarString guitar_strings[6]) {
  for (int i = 0; i < 6; i++) {
    Serial.print(guitar_strings[i].get_MIDI_value());
    Serial.print(" ");
  }
  Serial.println("");
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

int sine_wave_buffer[1000];
int sine_wave_counter = 0;
bool record_waveform_threshold_crossed = false;
int record_waveform_threshold = 80;

void collect_analog_values(const GuitarString& guitar_string, int iteration, int string_number, bool& record_waveform_threshold_crossed) {
  /**
   * Collects analog values from a guitar string and stores them in a buffer. (Printing on each iteration would slow down the code significantly ang give unreliable results)
   *
   * @param string_number The index of the guitar string to collect values from.
   * @param iteration the iteration number comping from a loop enclosing this function.
   * @param string_number The string we want to collect values from
   * 
   * Place in the detect note_on/note_off for loop.
   * e.g.: collect_analog_values(guitar_strings[i], i, 1, record_waveform_threshold_crossed);
   */
  if (iteration == string_number) {
    int current_amplitude = guitar_string.get_current_amplitude();
    sine_wave_buffer[sine_wave_counter] = current_amplitude;
    sine_wave_counter++;
    if (current_amplitude > record_waveform_threshold) {
      record_waveform_threshold_crossed = true;
    }
    if (sine_wave_counter >= 1000) {
      sine_wave_counter = 0;
    }
  }
};

void print_analog_values(bool& record_waveform_threshold_crossed) {
  /**
   * Prints analog values from a guitar string.
   *
   * Place after the detect note_on/note_off for loop.
   * e.g.: print_analog_values(record_waveform_threshold_crossed);
   */
  if (sine_wave_counter >= 1000-1) {
    if (record_waveform_threshold_crossed) {
      record_waveform_threshold_crossed = false;
      for (int i = 0; i < 1000; i++) {
        println(sine_wave_buffer[i]);
      }
    }
  }
};
