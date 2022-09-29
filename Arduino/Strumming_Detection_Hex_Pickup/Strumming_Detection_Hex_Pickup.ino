int string_values[6] = {0};
int strings_last_values[6] = {0};
int min_val = 10;
int max_val = 800;


void setup() 
{
  Serial.begin(9600);
}

int string1 = 0;
int string1_last = 0;
int string1_last_note_on = 0;
int hysteresis = 0;
bool note_on = false;
bool plam_mute = false;

void loop() {
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
}
