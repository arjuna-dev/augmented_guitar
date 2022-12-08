/*
  Hexaphonic Pickup Color Coding
  white: 6th string
  orange: 5th string
  green : 3rd string
  red: 4th string
  yellow: 2nd string
  brown: 1st string
*/

int string_values[6] = {0};
int strings_last_values[6] = {0};
int strings_last_note_on_values[6] = {0};
int min_val = 10;
int max_val = 800;
int string_input_pins[2] = {21, 22};
bool note_ons[6] = {0};
bool plam_mutes[6] = {0};
int hysteresis = 0;

void setup() {
    Serial.begin(9600);
    for (int i = 0; i > 6; i++) {
      pinMode(string_input_pins[i], INPUT);
    }
}

void note_detection(int string) {
    //re-Pluck detection
    if (string_values[string] > strings_last_note_on_values[string]) {
      strings_last_note_on_values[string] = string_values[string];
      note_ons[string] = true;
      Serial.println("Note_ON: " + String(note_ons[string]) + " v:"  + String(string_values[string]) + ", MIDI: " + map(string_values[string], 0, 700, 0, 127));
      Serial.println("re-Pluck");
    }
    //Note_ON detection (value is higher than last)
    else if (string_values[string] > 50 && string_values[string] > strings_last_values[string] && plam_mutes[string] == false) {
      strings_last_note_on_values[string] = string_values[string];
      note_ons[string] = true;
      Serial.println("Note_ON: " + String(note_ons[string]) + " v:"  + String(string_values[string]) + ", MIDI: " + map(string_values[string], 0, 700, 0, 127));
    }
    //Palm Mute detection (value is lower than last)
    else if ((strings_last_note_on_values[string] - string_values[string]) > hysteresis && note_ons[string] == true) {
      note_ons[string] = false;
      plam_mutes[string] = true;
      Serial.println("Note_ON: " + String(note_ons[string]) + " v:"  + String(string_values[string]) + ", MIDI: " + map(string_values[string], 0, 700, 0, 127));
      Serial.println("PALM MUTE");
    }
    if (string_values[string] < 20 && note_ons[string] == true) {
      note_ons[string] = false;
      Serial.println("Note_ON: " + String(note_ons[string]) + " v:"  + String(string_values[string]) + ", MIDI: " + map(string_values[string], 0, 700, 0, 127));
      Serial.println("NOTE OFF");
    }
    if (string_values[string] > strings_last_values[string] + hysteresis && plam_mutes[string] == true) {
      plam_mutes[string] = false;
      Serial.println("UNMUTE");
    }

    strings_last_values[string] = string_values[string];
}

void loop() {
  for (int i = 0; i < 2; i++) {
    string_values[i] = analogRead(string_input_pins[i]);
    delay(1);
    hysteresis = string_values[i] * 0.2;
    note_detection(i);
  }
}
