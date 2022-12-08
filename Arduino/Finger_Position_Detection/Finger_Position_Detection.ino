//Mux control pins
int s0 = 12;
int s1 = 11;
int s2 = 10;
int s3 = 9;

//Mux in "SIG" pin
int SIG_pin_1 = 30;
int SIG_pin_2 = 23;

int value_positions[6][4] = {
{15, 9, 3, 18},
{14, 8, 2, 19},
{13, 7, 1, 20},
{12, 6, 0, 21},
{11, 5, 16, 22},
{10, 4, 17, 23},
};

int touchpoint_reference_values[24] = {0};
int touchpoint_values[24] = {0};
bool touchpoint_high_low[24] = {0};

void setup(){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Serial.begin(9600);

  // (Calibration)
  getTouchpointValues(touchpoint_reference_values);
}

int getTouchpointValues(int *touchpoint_array){
  for(int i = 0; i < 16; i ++){
    touchpoint_array[i] = readMux(i, SIG_pin_1);
    if (i<8) {
      touchpoint_array[i+16] = readMux(i, SIG_pin_2);  
    }
  }
}

void loop(){

  int touchpoint_values[24] = {0};
  getTouchpointValues(touchpoint_values);
  delay(100);
  for (int i=0; i<6; i++){
    for (int j=0; j<4; j++){
      if (touchpoint_values[value_positions[i][j]] - touchpoint_reference_values[value_positions[i][j]]>100){
        touchpoint_high_low[value_positions[i][j]] = 1;
      } else {
        touchpoint_high_low[value_positions[i][j]] = 0;
      }
      Serial.print(String(touchpoint_high_low[value_positions[i][j]]) + " ");
    }
  }
  Serial.println(" ");
}


int readMux(int channel, int signal_pin){
  int controlPin[] = {s0, s1, s2, s3};
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
