#include "74HC4067.h"
#include "74HC4051.h"
int string_values[6] = {0};
int strings_last_values[6] = {0};
int min_val = 10;
int max_val = 800;


void setup() 
{
  Serial.begin(9600);
  setup_mux67();
  setup_mux51();
  
  select_pin_mux51(2);
}

void calibrate(){
    for (int i=0;i<6;i++) {
        int max_value = 0;
        int min_value = 0;
        for (int j=0; j<20; j++) {
            int read_value = analogRead(zInput);
            min_value = min(min_value, read_value);
            max_value = max(max_value, read_value);
        }
//        strings_calibrated_values[i] = max_value;
    }
}

int string1 = 0;
int string1_last = 0;
int string1_last_note_on = 0;
int hysteresis = 0;
bool note_on = false;
bool plam_mute = false;

void loop() {
  // Loop through all eight pins.
//    int i = 0;
//    for (byte pin=2; pin<=7; pin++) {
//        select_pin_mux51(pin); // Select one at a time
//        if (i==0) {

            string1 = analogRead(zInput);
            delay(1); 
            hysteresis = string1*0.2;

             //re-Pluck detection
            if (string1 > string1_last_note_on){
                string1_last_note_on = string1;
                note_on = true;
                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string1) + ", MIDI: " + map(string1, 0, 700, 0, 127));
//                Serial.println("re-Pluck");
            }
            //Note_ON detection (value is higher than last)
            else if (string1 > 50 && string1 > string1_last && plam_mute==false){
                string1_last_note_on = string1;
                note_on = true;
                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string1) + ", MIDI: " + map(string1, 0, 700, 0, 127));
            }
            //Palm Mute detection (value is lower than last)
            else if ((string1_last_note_on-string1) > hysteresis && note_on == true){
                note_on = false;
                plam_mute = true;
                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string1) + ", MIDI: " + map(string1, 0, 700, 0, 127));
                Serial.println("PALM MUTE");
            }
            if (string1 < 20 && note_on == true){
                note_on = false;
                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string1) + ", MIDI: " + map(string1, 0, 700, 0, 127));
                Serial.println("NOTE OFF");
            }
            if (string1 > string1_last+hysteresis && plam_mute == true){
                plam_mute = false;
                Serial.println("UNMUTE");
            }
            
            string1_last = string1;
            //            Serial.println("last string: " + String(string1_last) + ", hysteresis: " + String(hysteresis) + ", substraction: " + String(string1_last-hysteresis) + ", string1: " + String(string1));
//        }

//        string_values[i] = analogRead(zInput);
//        Serial.print(", string " + String(i) + ": ");
//        Serial.print(string_values[i]);
//        if (read_value > strings_calibrated_values[i]) {
//          Serial.print("playing string " + String(i));
//        }
//        i++;
//  }
//  Serial.println();
}
