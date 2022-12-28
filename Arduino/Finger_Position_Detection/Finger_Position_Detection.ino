//Mux control pins
int controlPin[] = {12, 11, 10, 9};

//Mux in "SIG" pin
int SIG_pin_1 = 30;
int SIG_pin_2 = 23;

// This array has matches the position of the array/fret with the order in which the frets are scanned. 
// E.g. The first fret of the first string will be read on the 16th iteration (denoted by number 15)
int value_positions[6][4] = {
  {15, 9, 3, 18},
  {14, 8, 2, 19},
  {13, 7, 1, 20},
  {12, 6, 0, 21},
  {11, 5, 16, 22},
  {10, 4, 17, 23},
};

int MIDI_open_string_notes[6] = {40, 45, 50, 55, 59, 64};
int string_values[6] = {0};
int last_string_values[6] = {0};
bool both_frets_touched[6] = {0};

int touch_reference_analog_values[24] = {0};
int touch_analog_values[24] = {0};
bool touch_digital_values[24] = {0};
bool last_touch_digital_values[24] = {0};
int capacitance_threshold = 3000;

void setup(){
  Serial.begin(9600);
  
  for (int i=0; i<4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);  
  }
  // (Calibration)
  updateTouchValues(touch_reference_analog_values);
}

void updateTouchValues(int *touch_array){
  for(int i = 0; i < 16; i ++){
    touch_array[i] = readMux(i, SIG_pin_1);
    if (i<8) {
      touch_array[i+16] = readMux(i, SIG_pin_2);  
    }
  }
}


void updateStringValues(int *string_values){
  for (int i=0; i<6; i++){
    string_values[i] = 0;
    both_frets_touched[i] = false;
    int last_fret_touched = 0;
    for (int j=0; j<4; j++){
      if (touch_analog_values[value_positions[i][j]] - touch_reference_analog_values[value_positions[i][j]]>capacitance_threshold){
        last_fret_touched = string_values[i];
        string_values[i] = MIDI_open_string_notes[i]+j+1;
      } 
      if (last_fret_touched && string_values[i]){
        both_frets_touched[i] = true; 
      }
    }
  }
}

void playMIDI(){
  for (int i=0; i<6; i++){
    Serial.print(String(string_values[i]) + " ");
    if (both_frets_touched[i] && last_string_values[i] != string_values[i]) {
      usbMIDI.sendNoteOn(string_values[i], 100, 0);
    } else if (!string_values[i] && last_string_values[i]) {
      usbMIDI.sendNoteOff(last_string_values[i], 0, 0);
    }
    last_string_values[i] = string_values[i];
    if (!both_frets_touched[i]){
      last_string_values[i] = 0;
    }
  }
  Serial.println("");
}

void loop(){
  updateTouchValues(touch_analog_values);
  updateStringValues(string_values);
  playMIDI();
  delay(1);
}


int readMux(int channel, int signal_pin){
  int muxChannel[16][4]={
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
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  int val = touchRead(signal_pin);
  return val;
}
