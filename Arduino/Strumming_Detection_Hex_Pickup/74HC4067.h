int s3 = 36;
int s2 = 35;
int s1 = 34;
int s0 = 33;
int SIG_pin_mux67 = 30;

void setup_mux67(){
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
}

int select_mux67_pin(int channel){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[6][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
//    {0,1,1,0}, //channel 6
//    {1,1,1,0}, //channel 7
//    {0,0,0,1}, //channel 8
//    {1,0,0,1}, //channel 9
//    {0,1,0,1}, //channel 10
//    {1,1,0,1}, //channel 11
//    {0,0,1,1}, //channel 12
//    {1,0,1,1}, //channel 13
//    {0,1,1,1}, //channel 14
//    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
}
