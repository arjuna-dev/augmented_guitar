/*
Hexaphonic Pickup Color Coding
white: 6th string
red: 4th string
*/


int string_values[6] = {0};
int strings_last_values[6] = {0};
int strings_last_note_on_values[6] = {0};
int min_val = 10;
int max_val = 800;
int string_4_input = 22;
int string_6_input = 21;
int string_input_pins[2] = {21, 22};

void setup() {
  Serial.begin(9600);
  pinMode(string_4_input, INPUT);
  pinMode(string_6_input, INPUT);
}

int string_4 = 0;
int string_6 = 0;

int string_4_last = 0;
int string_4_last_note_on = 0;
int hysteresis = 0;
bool note_on = false;
bool plam_mute = false;


void note_detection(int string_value, int string_value_last, int string_number) {

         //re-Pluck detection
        if (string_value > strings_last_note_on_values[string_number]){
            strings_last_note_on_values[string_number] = string_value;
            note_on = true;
            Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_value) + ", MIDI: " + map(string_value, 0, 700, 0, 127));
            Serial.println("re-Pluck"); 
        }
        //Note_ON detection (value is higher than last)
        else if (string_value > 50 && string_value > string_value_last && plam_mute==false){
            strings_last_note_on_values[string_number] = string_value;
            note_on = true;
            Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_value) + ", MIDI: " + map(string_value, 0, 700, 0, 127));
        }
        //Palm Mute detection (value is lower than last)
        else if ((strings_last_note_on_values[string_number]-string_value) > hysteresis && note_on == true){
            note_on = false;
            plam_mute = true;
            Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_value) + ", MIDI: " + map(string_value, 0, 700, 0, 127));
            Serial.println("PALM MUTE");
        }
        if (string_value < 20 && note_on == true){
            note_on = false;
            Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_value) + ", MIDI: " + map(string_value, 0, 700, 0, 127));
            Serial.println("NOTE OFF");
        }
        if (string_value > string_value_last+hysteresis && plam_mute == true){
            plam_mute = false;
            Serial.println("UNMUTE");
        }

}

void loop() {
        for (int i=0; i<2; i++) {
          string_values[i] = analogRead(string_input_pins[i]);
        }
//            string_4 = analogRead(string_4_input);
//            Serial.print("string_4: ");
//            Serial.print(string_4);
//            Serial.print(" ");
//            string_6 = analogRead(string_6_input);
//            Serial.print("string_6: ");
//            Serial.println(string_6);
//            Serial.print(" ");
            
        delay(1);
        hysteresis = string_4*0.2;
        
        for (int i=0; i<2; i++) {
          note_detection(string_values[i], strings_last_values[i], i);
        }

        for (int i=0; i<2; i++) {
          strings_last_values[i] = string_values[i];
        }
}



//             //re-Pluck detection
//            if (string_4 > string_4_last_note_on){
//                string_4_last_note_on = string_4;
//                note_on = true;
//                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_4) + ", MIDI: " + map(string_4, 0, 700, 0, 127));
//                Serial.println("re-Pluck"); 
//            }
//            //Note_ON detection (value is higher than last)
//            else if (string_4 > 50 && string_4 > string_4_last && plam_mute==false){
//                string_4_last_note_on = string_4;
//                note_on = true;
//                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_4) + ", MIDI: " + map(string_4, 0, 700, 0, 127));
//            }
//            //Palm Mute detection (value is lower than last)
//            else if ((string_4_last_note_on-string_4) > hysteresis && note_on == true){
//                note_on = false;
//                plam_mute = true;
//                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_4) + ", MIDI: " + map(string_4, 0, 700, 0, 127));
//                Serial.println("PALM MUTE");
//            }
//            if (string_4 < 20 && note_on == true){
//                note_on = false;
//                Serial.println("Note_ON: " + String(note_on) + " v:"  + String(string_4) + ", MIDI: " + map(string_4, 0, 700, 0, 127));
//                Serial.println("NOTE OFF");
//            }
//            if (string_4 > string_4_last+hysteresis && plam_mute == true){
//                plam_mute = false;
//                Serial.println("UNMUTE");
//            }
//            
//            string_4_last = string_4;
//}
