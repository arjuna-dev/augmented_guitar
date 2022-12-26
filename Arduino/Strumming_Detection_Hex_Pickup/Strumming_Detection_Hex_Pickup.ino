/*
  Hexaphonic Pickup Color Coding
  white: 6th string
  orange: 5th string
  green : 3rd string
  red: 4th string
  yellow: 2nd string
  brown: 1st string
*/

struct StringStruct{
  int input_pin;
  int current_amplitude;
  int previous_amplitude;
  int peak_value;
  int min_threshold;
  bool increasing;
};

struct StringStruct string_structs[6];
int string_input_pins[6] = {34, 35, 36, 37, 38, 39};
int hysteresis = 25;

void setup() {
  Serial.begin(9600);
  for (int i=0; i<6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }
  for (int i=0; i<6; i++) {
    string_structs[i] = {string_input_pins[i], 0, 0, 0, 120, false};
  }
}

void peak_detection(struct StringStruct string) {
  if(string.current_amplitude > string.min_threshold){
    if (string.current_amplitude > string.previous_amplitude + hysteresis) {
      string.increasing = true;
    }
    if (string.current_amplitude < string.previous_amplitude - hysteresis && string.increasing && string.current_amplitude > string.peak_value) {
      string.increasing = false;
      int note_amplitude = string.previous_amplitude;
      string.peak_value = note_amplitude;
      Serial.println(note_amplitude);
    }   
  }
}

void loop() {
  for (int i=0; i<6; i++) {
    
    string_structs[i].current_amplitude = analogRead(string_structs[i].input_pin);
    peak_detection(string_structs[i]);
    string_structs[i].previous_amplitude = string_structs[i].current_amplitude;
  }
}
