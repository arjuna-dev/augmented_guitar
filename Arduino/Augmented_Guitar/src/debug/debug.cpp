#include "Arduino.h"
#include "debug.h"
#include "../MIDI/midi.h"

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
