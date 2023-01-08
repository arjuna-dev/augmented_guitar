/*
  Hexaphonic Pickup Color Coding
  white: 6th string
  orange: 5th string
  green : 3rd string
  red: 4th string
  yellow: 2nd string
  brown: 1st string
*/

#define DEBUG 1

#if DEBUG == 1
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x)
#endif

#define hysteresis 25
void peak_detection(struct StringStruct& string);
void setupStrumming();
int string_input_pins[6] = {34, 35, 36, 37, 38, 39};
int hysteresis = 25;


#define SIG_pin_1 30
#define SIG_pin_2 23

void setupFingerPosition();
void updateTouchValues(int *touch_array);
void updateStringMIDIValue(struct StringStruct& string, int string_number);

//Mux control pins
const int controlPin[] = {12, 11, 10, 9};
const int muxChannel[16][4]={
  {0,0,0,0}, //channel 0
  {1,0,0,0}, //channel 1
  {0,1,0,0}, //channel 2
  {1,1,0,0}, //channel 3
  {0,0,1,0}, //channel 4
  {1,0,1,0}, //channel 5
  {0,1,1,0}, //channel 6
  {1,1,1,0}, //channel 7
  {0,0,0,1}, //channel 8
  {1,0,0,1}, //channel 9
  {0,1,0,1}, //channel 10
  {1,1,0,1}, //channel 11
  {0,0,1,1}, //channel 12
  {1,0,1,1}, //channel 13
  {0,1,1,1}, //channel 14
  {1,1,1,1}  //channel 15
};

// This array has matches the position of the array/fret with the order in which the frets are scanned. 
// E.g. The first fret of the first string will be read on the 16th iteration (denoted by number 15)
const int value_positions[6][4] = {
  {15, 9, 3, 18},
  {14, 8, 2, 19},
  {13, 7, 1, 20},
  {12, 6, 0, 21},
  {11, 5, 16, 22},
  {10, 4, 17, 23},
};

const int MIDI_open_string_notes[6] = {40, 45, 50, 55, 59, 64};
const int capacitance_threshold = 20000;
int touch_reference_analog_values[24] = {0};
int touch_analog_values[24] = {0};
int capacitance_threshold = 20000;



/* Shared struct for finger position and strumming detection variables */
struct StringStruct{
  // Strumming detection
  int input_pin;
  int current_amplitude;
  int previous_amplitude;
  int peak_value;
  int min_threshold;
  bool increasing;
    
  // Finger position detection
  int MIDI_value;
  bool both_frets_touched;
};

struct StringStruct string_structs[6];

void setup(){
  /* TODO: Add "calibration" to each string to detect the minimum 
  threshold for each string instead of having a hard-coded 120. 
  Alternatively solve with hardware electronics components */
  #if DEBUG == 1
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
  #endif

  setupFingerPosition();
  setupStrumming();
  for (int i=0; i<6; i++) {
    string_structs[i] = {string_input_pins[i], 0, 0, 0, 120, false, MIDI_open_string_notes[i], false};
  }
}

void loop(){
  // Update the MIDI values according to pressed frets
  updateTouchValues(touch_analog_values);
  for (int i=0; i<6; i++){
    updateStringMIDIValue(string_structs[i], i);  
  }
//  printMIDIValues();

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