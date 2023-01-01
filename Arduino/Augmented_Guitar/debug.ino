void printMIDIValues(){
  for (int i=0; i<6; i++){
    Serial.print(String(string_structs[i].MIDI_value) + " ");
  }
  Serial.println("");
}
