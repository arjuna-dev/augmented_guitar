#ifndef DEBUG_H
#define DEBUG_H

void printSineWaveValues(int string_number, int iteration, int number_of_iterations);
void printMIDIValues();

constexpr int NUM_OF_ARR_VALUES = 1000;

extern bool sine_wave_started;
extern int record_sine_wave_counter;
extern int sine_wave_array[NUM_OF_ARR_VALUES];

void printTouchedMIDIValues();
void printSineWaveValues(int string_number, int iteration, int number_of_iterations);
void println(String str1);
void println(int var);
void println(String str1, int var1);
void print(String str1);
void print(int var1);
void print(String str1, int var1);

#endif