#ifndef DEBUG_H
#define DEBUG_H

#include "../GuitarStringClass/guitar_string.h"

void printCurrentMIDIValues(GuitarString guitar_strings[6]);
void println(String str1);
void println(int var);
void println(String str1, int var1);
void print(String str1);
void print(int var1);
void print(String str1, int var1);
void collect_analog_values(const GuitarString& guitar_string, int iteration, int string_number);
void print_analog_values();


#endif