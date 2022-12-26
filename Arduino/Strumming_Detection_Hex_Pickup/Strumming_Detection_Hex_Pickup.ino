/*
  Hexaphonic Pickup Color Coding
  white: 6th string
  orange: 5th string
  green : 3rd string
  red: 4th string
  yellow: 2nd string
  brown: 1st string
*/

int string_input_pins[6] = {34, 35, 36, 37, 38, 39};
int string_current_values[6] = {0};
int string_previous_values[6] = {0};
int hysteresis = 25;
int min_threshold = 120;

int peak_values[6] = {0};

bool increasing[6] = {false};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i > 6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }
}

void peak_detection(int current_value, int previous_value, int string_number) {
  if(current_value > min_threshold){
    if (current_value > previous_value + hysteresis) {
      increasing[string_number] = true;
    }
    if (current_value < previous_value - hysteresis && increasing[string_number] && current_value > peak_values[string_number]) {
      increasing[string_number] = false;
      int note_amplitude = previous_value;
      peak_values[string_number] = note_amplitude;
      Serial.println(note_amplitude);
    }   
  }
}

void loop() {
  for (int i=0; i<6; i++) {
    string_current_values[i] = analogRead(string_input_pins[i]);
    peak_detection(string_current_values[i], string_previous_values[i], i);
    string_previous_values[i] = string_current_values[i];    
  }
}
