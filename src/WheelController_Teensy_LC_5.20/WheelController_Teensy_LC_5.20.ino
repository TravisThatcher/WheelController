/*
  Wheel 0 is reserved to send pitch bend information but the other
  controls can be configured to send data on any MIDI CC.
*/

//#include "ControlPot.h"

/************** EDIT HERE ****************/

// MIDI channel to send messages to
const int channelOut = 1;

// MIDI CC Number for 2nd wheel and pots
const int CONTROLWHEEL1 = 1;
const int CONTROLPOT0 = 20;
const int CONTROLPOT1 = 21;
const int CONTROLPOT2 = 22;
const int CONTROLPOT3 = 23;

/************ END EDIT HERE **************/

static boolean test=false;

int lastWheel0 = 100000;
int lastWheel1 = 100000;

int lastPot0 = 100000;
int lastPot1 = 100000;
int lastPot2 = 100000;
int lastPot3 = 100000;

const int numReadings = 30;

int wheel0Min = 350;
int wheel1Min = 350;
int pot0Min = 0;
int pot1Min = 0;
int pot2Min = 0;
int pot3Min = 4;

int wheel0Max = 690;
int wheel1Max = 650;
int pot0Max = 1024;
int pot1Max = 1024;
int pot2Max = 1024;
int pot3Max = 1020;

boolean cMode = true;

int wheel0Raw = 0;
int wheel1Raw = 0;

int pot0Raw = 0;
int pot1Raw = 0;
int pot2Raw = 0;
int pot3Raw = 0;

int idx=0;
int readings0[numReadings];
int readings1[numReadings];
int readingsP0[numReadings];
int readingsP1[numReadings];
int readingsP2[numReadings];
int readingsP3[numReadings];

int wheel0Total = 0;
int wheel1Total = 0;
int pot0Total = 0;
int pot1Total = 0;
int pot2Total = 0;
int pot3Total = 0;

int wheel0Avg = 0;
int wheel1Avg = 0;
int pot0Avg = 0;
int pot1Avg = 0;
int pot2Avg = 0;
int pot3Avg = 0;

boolean FIRSTRUN = true;


int WHEEL0 = A0;
int WHEEL1 = A1;

int POT0 = A2;
int POT1 = A3;
int POT2 = A4;
int POT3 = A5;

void setup() {
  //startTime = millis();
  if(test==true){
    Serial.begin(9600);  
  }
  readings0[0]=0;
  readings1[0]=0;

  readingsP0[0]=0;
  readingsP1[0]=0;
  readingsP2[0]=0;
  readingsP3[0]=0;
  //calibrate! 

}

void calibrate_wheels(){
    wheel0Raw = analogRead(WHEEL0);
    wheel1Raw = analogRead(WHEEL1);
    pot0Raw = analogRead(POT0);
    pot1Raw = analogRead(POT1);
    pot2Raw = analogRead(POT2);
    pot3Raw = analogRead(POT3);
    // record the maximum sensor value
    if (wheel0Raw > wheel0Max) {
      wheel0Max = wheel0Raw;
    }
    
    if (wheel1Raw > wheel1Max){
        wheel1Max = wheel1Raw;
    }
    
    if (pot0Raw > pot0Max){
        pot0Max = pot0Raw;
    }
    if (pot1Raw > pot1Max){
        pot1Max = pot1Raw;
    }
    if (pot2Raw > pot2Max){
        pot2Max = pot2Raw;
    }
    if (pot3Raw > pot3Max){
        pot3Max = pot3Raw;
    }

    // record the minimum sensor value
    if (wheel0Raw < wheel0Min) {
      wheel0Min = wheel0Raw;
    }
    if (wheel1Raw < wheel1Min) {
      wheel1Min = wheel1Raw;
    } 
   
   if (pot0Raw < pot0Min){
        pot0Min = pot0Raw;
    }
    if (pot1Raw < pot1Min){
        pot1Min = pot1Raw;
    }
    if (pot2Raw < pot2Min){
        pot2Min = pot2Raw;
    }
    if (pot3Raw < pot3Min){
        pot3Min = pot3Raw;
    } 
    
}

void loop() {
  usbMIDI.read();
/*
  int calibrate = digitalRead(8);
  
  if(calibrate==HIGH){
    if(cMode==false){
      cMode=true;
      wheel0Min = 1500;
      wheel1Min = 1500;
      wheel0Max = 0;
      wheel1Max = 0;
      
      pot0Min = 1500;
      pot1Min = 1500;
      pot2Min = 1500;
      pot3Min = 1500;
      
      pot0Max = 0;
      pot1Max = 0;
      pot2Max = 0;
      pot3Max = 0;
      
    }
    
     digitalWrite(13, HIGH);
     calibrate_wheels(); 

     return;
  }else{
   digitalWrite(13, LOW); 
   cMode=false;
  }
  
  */
  
  // average the readings!
  wheel0Total = wheel0Total - readings0[idx];
  wheel1Total = wheel1Total - readings1[idx];
  pot0Total = pot0Total - readingsP0[idx];
  pot1Total = pot1Total - readingsP1[idx];
  pot2Total = pot2Total - readingsP2[idx];
  pot3Total = pot3Total - readingsP3[idx];
  
  readings0[idx] = analogRead(WHEEL0);
  readings1[idx] = analogRead(WHEEL1);
  readingsP0[idx] = analogRead(POT0);
  readingsP1[idx] = analogRead(POT1);
  readingsP2[idx] = analogRead(POT2);
  readingsP3[idx] = analogRead(POT3);
if(test==true){
  //Serial.println(readings0[idx]);
}
  wheel0Total = wheel0Total + readings0[idx];
  wheel1Total = wheel1Total + readings1[idx];
  pot0Total = pot0Total + readingsP0[idx];
  pot1Total = pot1Total + readingsP1[idx];
  pot2Total = pot2Total + readingsP2[idx];
  pot3Total = pot3Total + readingsP3[idx];
  idx = idx + 1;
  
  if(idx >= numReadings){
    idx=0;
  }
  
  wheel0Avg = wheel0Total / numReadings;
  wheel1Avg = wheel1Total / numReadings;
  pot0Avg = pot0Total / numReadings;
  pot1Avg = pot1Total / numReadings;
  pot2Avg = pot2Total / numReadings;
  pot3Avg = pot3Total / numReadings;

  // Process Mod cc
  if(wheel0Avg<wheel0Min){
    wheel0Avg=wheel0Min;
  }
  
  if(wheel0Avg>wheel0Max){
    wheel0Avg=wheel0Max;
  }
  int mappedWheel0 = map(wheel0Avg, wheel0Min, wheel0Max, -8192, 8191);
  if(mappedWheel0>8000) mappedWheel0 = 8191;
  if(mappedWheel0<-8000) mappedWheel0 = -8192;

  if(mappedWheel0<20 && mappedWheel0>-400){
     mappedWheel0=0; 
  }
  
  /*if(test){
    Serial.print("pitch is ");
    Serial.println(mappedWheel0);
  }*/
   
    if(mappedWheel0>lastWheel0+50||mappedWheel0<lastWheel0-50){
  
      lastWheel0 = mappedWheel0;
      
       //Serial.println(mappedWheel0);
      if(test==false){
        usbMIDI.sendPitchBend(mappedWheel0, channelOut);
      }else{
        Serial.print("control 0 ");
        Serial.println(mappedWheel0);
      }
    }else{
      
    }
  
  // Process Volume cc

  int mappedWheel1 = map(wheel1Avg, wheel1Min, wheel1Max, 0, 127); //map value to 0-127
  
  if(mappedWheel1>124) mappedWheel1 = 127;
  if(mappedWheel1<5) mappedWheel1 = 0;

    if(mappedWheel1>lastWheel1+1||mappedWheel1<lastWheel1-1){
      lastWheel1=mappedWheel1;
      //
        if(test==false){
          usbMIDI.sendControlChange(CONTROLWHEEL1, mappedWheel1, channelOut);
        }else{
          Serial.print("Mod ");
          Serial.println(mappedWheel1);
        }
     // digitalWrite(13, HIGH);      
    }else{

    }
    
  // Process Control0 cc

  int mappedPot0 = map(pot0Avg, pot0Min, pot0Max, 0, 127); //map value to 0-127
  //mappedPot0 = abs(mappedPot0-127);
  if(mappedPot0>125) mappedPot0 = 127;
  if(mappedPot0<2) mappedPot0 = 0;

  if(mappedPot0>lastPot0+0||mappedPot0<lastPot0-0){
    lastPot0=mappedPot0;
    //
      if(test==false){
        usbMIDI.sendControlChange(CONTROLPOT0, mappedPot0, channelOut);
      }else{
        Serial.print("pot 1 ");
        Serial.println(mappedPot0);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  
  // Process Control1 cc

  int mappedPot1 = map(pot1Avg, pot1Min, pot1Max, 0, 127); //map value to 0-127
  //mappedPot1 = abs(mappedPot1-127);
  if(mappedPot1>125) mappedPot1 = 127;
  if(mappedPot1<2) mappedPot1 = 0;

  if(mappedPot1>lastPot1+0||mappedPot1<lastPot1-0){
    lastPot1=mappedPot1;
    //
      if(test==false){
        usbMIDI.sendControlChange(CONTROLPOT1, mappedPot1, channelOut);
      }else{
        Serial.print("pot 2 ");
        Serial.println(mappedPot1);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  
  // Process Control0 cc

  int mappedPot2 = map(pot2Avg, pot2Min, pot2Max, 0, 127); //map value to 0-127
  //mappedPot2 = abs(mappedPot2-127);
  if(mappedPot2>125) mappedPot2 = 127;
  if(mappedPot2<2) mappedPot2 = 0;

  if(mappedPot2>lastPot2+0||mappedPot2<lastPot2-0){
    lastPot2=mappedPot2;
    //
      if(test==false){
        usbMIDI.sendControlChange(CONTROLPOT2, mappedPot2, channelOut);
      }else{
        Serial.print("pot 3 ");
        Serial.println(mappedPot2);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  
  // Process Control0 cc

  int mappedPot3 = map(pot3Avg, pot3Min, pot3Max, 0, 127); //map value to 0-127
  //mappedPot3 = abs(mappedPot3-127);
  if(mappedPot3>125) mappedPot3 = 127;
  if(mappedPot3<2) mappedPot3 = 0;

  
  if(mappedPot3>lastPot3+0||mappedPot3<lastPot3-0){
    lastPot3=mappedPot3;
    //
      if(test==false){
        usbMIDI.sendControlChange(CONTROLPOT3, mappedPot3, channelOut);
      }else{
        Serial.print("pot 4 ");
        Serial.println(mappedPot3);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  

}
