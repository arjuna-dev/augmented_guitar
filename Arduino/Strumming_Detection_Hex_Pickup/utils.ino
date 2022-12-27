
void plot(int pin_1, int pin_2, int pin_3, int pin_4, int pin_5, int pin_6){
  Serial.print(analogRead(pin_1));
  Serial.print(", ");
  Serial.print(analogRead(pin_2));
  Serial.print(", ");
  Serial.print(analogRead(pin_3));
  Serial.print(", ");
  Serial.print(analogRead(pin_4));
  Serial.print(", ");
  Serial.print(analogRead(pin_5));
  Serial.print(", ");
  Serial.print(analogRead(pin_6));
  Serial.println(" ");
}
