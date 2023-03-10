/*
  Hexaphonic Pickup Color Coding
  brown  :E string
  yellow :A string
  green  :D string
  red    :G string
  orange :B string
  white  :E string
*/

#include "read_touch.h"
#include "mux.h"

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
  int fret_number_touched;
  bool both_frets_touched;
};


struct StringStruct string_structs[6];

/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
/*_-_-_-_-_-_-_-Non-blocking capcitive touch sensing variables_-_-_-_-_-_-_-_*/
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/


int* ptr_mux_pins = mux_pins;
int touch_analog_values[32] = {0};
int* ptr_touch_analog_values = touch_analog_values;
int mux_ch = 0;
int* ptr_mux_ch = &mux_ch;
int touch_reference_analog_values[32] = {0};
int* ptr_touch_reference_analog_values = touch_reference_analog_values;


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

  /*_-_-Right hand setup_-_-*/
  setupStrumming();

  /*_-_-Struct setup_-_-*/
  for (int i = 0; i < 6; i++) {
    string_structs[i] = {string_input_pins[i], 0, 0, 0, 120, MIDI_open_string_notes[i], false};
  }
}


void loop() {

  teensyTouchRead(touch_analog_values, 32, ptr_touch_analog_values, mux_pins, 2, ptr_mux_pins, ptr_mux_ch);

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    updateStringMIDIValue(string_structs[i], i);
  }

  printTouchedMIDIValues();

  // Detect peak and play MIDI for each string
   for (int i=0; i<6; i++) {
     string_structs[i].current_amplitude = analogRead(string_structs[i].input_pin);
  
     peak_detection(string_structs[i]);
  
     if (string_structs[i].peak_value){
       //Play MIDI
     }
     string_structs[i].previous_amplitude = string_structs[i].current_amplitude;
   }
}
