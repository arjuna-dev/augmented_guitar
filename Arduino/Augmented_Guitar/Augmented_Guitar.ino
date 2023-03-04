/*
  Hexaphonic Pickup Color Coding
  white: 6th string
  orange: 5th string
  green : 3rd string
  red: 4th string
  yellow: 2nd string
  brown: 1st string
*/

#include "read_touch.h"

#define DEBUG 1

/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-_-Strumming detection variables and function prototypes_-_-_-*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
#define hysteresis 25
#define sine_wave_falling_edge 35

void peak_detection(struct StringStruct& string);
void setupStrumming();

int string_input_pins[6] = {34, 35, 36, 37, 38, 39};


/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-Finger position detection variables and function prototypes_-_-*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
#define mux_1_pin 30
#define mux_2_pin 23
int mux_pins[2] = {mux_1_pin, mux_2_pin};

void setupFingerPosition();
void updateTouchValues(int *touch_array);
void updateStringMIDIValue(struct StringStruct& string, int string_number);

//Mux control pins
const int controlPin[] = {12, 11, 10, 9};
const int muxChannel[16][4] = {
  {0, 0, 0, 0}, //channel 0
  {1, 0, 0, 0}, //channel 1
  {0, 1, 0, 0}, //channel 2
  {1, 1, 0, 0}, //channel 3
  {0, 0, 1, 0}, //channel 4
  {1, 0, 1, 0}, //channel 5
  {0, 1, 1, 0}, //channel 6
  {1, 1, 1, 0}, //channel 7
  {0, 0, 0, 1}, //channel 8
  {1, 0, 0, 1}, //channel 9
  {0, 1, 0, 1}, //channel 10
  {1, 1, 0, 1}, //channel 11
  {0, 0, 1, 1}, //channel 12
  {1, 0, 1, 1}, //channel 13
  {0, 1, 1, 1}, //channel 14
  {1, 1, 1, 1} //channel 15
};

// This array has maps the position of the array/fret with the order in which the frets are scanned.
// E.g. The first fret of the first string will be read on the 31st iteration (denoted by number 30)
const int value_positions[6][4] = {
  {30, 18, 6, 5},
  {28, 16, 4, 7},
  {26, 14, 2, 9},
  {24, 12, 0, 11},
  {22, 10, 1, 13},
  {20, 8, 3, 15},
};

const int MIDI_open_string_notes[6] = {40, 45, 50, 55, 59, 64};
const int capacitance_threshold = 5000;
//int touch_reference_analog_values[24] = {0};
//int touch_analog_values[24] = {0};

/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_Debug variables_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

#if DEBUG == 1
void printSineWaveValues(int string_number, int iteration, int number_of_iterations);
void printMIDIValues();

bool sine_wave_started = false;
int record_sine_wave_counter = 0;
int sine_wave_array[1000];
#endif

/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-Shared struct for finger position and strumming detection variables_-_-*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

struct StringStruct {
  // Strumming detection
  int input_pin;
  int current_amplitude;
  int previous_amplitude;
  int peak_value;
  int min_threshold;

  // Finger position detection
  int MIDI_value;
  int fret_nunmber_touched;
  bool both_frets_touched;
};


struct StringStruct string_structs[6];

/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-_-_-_-_-_-Non-blocking capcitive touch sensing variables_-_-_-_-_-_-_-_*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/


int* ptr_mux_pins = mux_pins;
int touch_analog_values[2] = {0};
int* ptr_touch_analog_values = touch_analog_values;
int* ptr_mux_ch;
int touch_reference_analog_values[32] = {0};
int* ptr_touch_reference_analog_values = touch_reference_analog_values;

// Array of touch inputs (pin numbers) to be read... (up to 12 pins):
//int mux_pins[2] = { mux_1_pin, mux_2_pin };
//int touchReadings[2];
//int touch_pin_index = 0;
//int channel_index = 0;
//int touch_array_index = 0;
//int firstPin = mux_pins[0];

void setup() {
#if DEBUG == 1
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
#endif

  /*_-_-Left hand setup_-_-*/
  selectMuxChannel(0);
  teensyTouchInit(mux_pins[0]);
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }
  setupFingerPosition();

  /*_-_-Right hand setup_-_-*/
  setupStrumming();

  /*_-_-Struct setup_-_-*/
  for (int i = 0; i < 6; i++) {
    string_structs[i] = {string_input_pins[i], 0, 0, 0, 120, MIDI_open_string_notes[i], false};
  }
}


void loop() {
  //  Serial.println("smthn");
  teensyTouchRead(touch_analog_values, 2, ptr_touch_analog_values, mux_pins, 2, ptr_mux_pins);


  //   if (teensyTouchDone()) {
  //     touch_analog_values[touch_array_index] = teensyTouchReturn();
  //     touch_array_index++;
  //     for (int i=0; i<32; i++){
  //       print(touch_analog_values[i]);
  //     }
  //     println("");
  //
  //     if (touch_array_index >= 32) {
  //       touch_array_index = 0;
  //     }
  //     touch_pin_index++;
  //     if (touch_pin_index >= 2) {
  //       touch_pin_index = 0;
  //       channel_index++;
  //       if (channel_index >= 16) {
  //         channel_index = 0;
  //       }
  //       selectMuxChannel(channel_index);
  //     }
  //     teensyTouchInit(touch_pins[touch_pin_index]);
  //   }

  // Update the MIDI values according to pressed frets
  // updateTouchValues(touch_analog_values);
  //  for (int i=0; i<6; i++){
  //    updateStringMIDIValue(string_structs[i], i);
  //  }

  //  printTouchedMIDIValues();

  // Detect peak and play MIDI for each string
  //  for (int i=0; i<6; i++) {
  //    string_structs[i].current_amplitude = analogRead(string_structs[i].input_pin);
  //
  //    peak_detection(string_structs[i]);
  //
  //    if (string_structs[i].peak_value){
  //      //Play MIDI
  //    }
  //    string_structs[i].previous_amplitude = string_structs[i].current_amplitude;
  //  }
}
