/*
  WheelController Basic for Teensy 3.2
  July 2017 - Travis Thatcher recompas@gmail.com
*/

 
/************** EDIT HERE ****************/
static boolean test = false; // for debugging
boolean calibrateonstartup = false; // turn on calibration mode
long calibrateTime = 5000; // for 5 seconds after power on move both wheels to their extremes to calibrate
int startTime = 0;

// MIDI channel to send messages to
const int channelOut = 1;

// MIDI CC Number for 2nd wheel and pots
const int CONTROLWHEEL1 = 1;

/************ END EDIT HERE **************/

int lastWheel0 = 0;
int lastWheel1 = 0;

const int numReadings = 30;

int wheel0Min = 405;
int wheel1Min = 405;

int wheel0Max = 600;
int wheel1Max = 600;

boolean cMode = false;

int wheel0Raw = 0;
int wheel1Raw = 0;


int idx=0;
int readings0[numReadings];
int readings1[numReadings];

int wheel0Total = 0;
int wheel1Total = 0;

int wheel0Avg = 0;
int wheel1Avg = 0;

int WHEEL0 = A0;
int WHEEL1 = A2;

void setup() {

 startTime = millis();
 pinMode(8, INPUT);
 pinMode(13, OUTPUT);
  if(test==true){
   Serial.begin(9600);  
  }
  readings0[0]=0;
  readings1[0]=0;

  // turn on the lights
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);

  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);

}

void calibrate_wheels(){
    wheel0Raw = analogRead(WHEEL0);
    wheel1Raw = analogRead(WHEEL1);
   
    // record the maximum sensor value
    if (wheel0Raw > wheel0Max) {
      wheel0Max = wheel0Raw;
    }
    
    if (wheel1Raw > wheel1Max){
        wheel1Max = wheel1Raw;
    }
    
    // record the minimum sensor value
    if (wheel0Raw < wheel0Min) {
      wheel0Min = wheel0Raw;
    }
    if (wheel1Raw < wheel1Min) {
      wheel1Min = wheel1Raw;
    } 

}

void loop() {

  //int calibrate = digitalRead(8);
  
  if(calibrateonstartup){
    if(cMode==false){
      cMode=true;
      wheel0Min = 1500;
      wheel1Min = 1500;
      wheel0Max = 0;
      wheel1Max = 0;
      
    }

    if((millis()-startTime)>calibrateTime){
      calibrateonstartup=false;
    }
    calibrate_wheels(); 
    return;
    
  }else{
    cMode=false;
  }
  
  // average the readings!
  wheel0Total = wheel0Total - readings0[idx];
  wheel1Total = wheel1Total - readings1[idx];
  
  readings0[idx] = analogRead(WHEEL0);
  readings1[idx] = analogRead(WHEEL1);
  /*
  Serial.print("Pitch Bend ");
  Serial.println(analogRead(WHEEL0));
  Serial.print("Mod Wheel ");
  Serial.println(analogRead(WHEEL1));
  */

 
  wheel0Total = wheel0Total + readings0[idx];
  wheel1Total = wheel1Total + readings1[idx];
 
  idx = idx + 1;
  
  if(idx >= numReadings){
    idx=0;
  }
  
  wheel0Avg = wheel0Total / numReadings;
  wheel1Avg = wheel1Total / numReadings;
 
  // Process Mod cc
  if(wheel0Avg<wheel0Min){
    wheel0Avg=wheel0Min;
  }
  
  if(wheel0Avg>wheel0Max){
    wheel0Avg=wheel0Max;
  }
  int mappedWheel0 = map(wheel0Avg, wheel0Min, wheel0Max, 0, 16383);

  
  if(mappedWheel0>16000) mappedWheel0 = 16383;
  if(mappedWheel0<200) mappedWheel0 = 0;

    
    if(mappedWheel0<8800 && mappedWheel0>7500){
     mappedWheel0=8191; 
    }
   
    if(mappedWheel0>lastWheel0+150||mappedWheel0<lastWheel0-150){
  
      lastWheel0 = mappedWheel0;
      
       
      if(test==false){
        usbMIDI.sendPitchBend(mappedWheel0, channelOut);
      }else{
        Serial.println("pitch bend");
        Serial.println(mappedWheel0);
      }
    }
  
  // Process Volume cc

  int mappedWheel1 = map(wheel1Avg, wheel1Min, wheel1Max, 0, 127); //map value to 0-127

  if(mappedWheel1>124) mappedWheel1 = 127;
  if(mappedWheel1<4) mappedWheel1 = 0;

  if(mappedWheel1>lastWheel1+2||mappedWheel1<lastWheel1-2){
    lastWheel1=mappedWheel1;
      
    if(test==false){
      usbMIDI.sendControlChange(CONTROLWHEEL1, mappedWheel1, channelOut);
    }else{
      Serial.println("mod wheel");
      Serial.println(mappedWheel1);
    }
  }
    
}
