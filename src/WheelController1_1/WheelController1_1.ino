/*
  WheelController1_1 - This is the latest work in progress version of the Wheel Controller.
  It utilizes 2 wheel assemblies as found in the DIY section from http://www.doepfer.de
  as well as providing for an additional 4 potentiometer conrols.  All analog inputs
  generate MIDI data. Wheel 0 is reserved to send pitch bend information but the other
  controls can be configured to send data on any MIDI CC.
*/

#include <MIDI.h>
#include "ControlPot.h"

/************** EDIT HERE ****************/

// MIDI channel to send messages to
const int channelOut = 1;

// MIDI CC Number for 2nd wheel and pots
const int CONTROLWHEEL2 = 1;
const int CONTROLPOT0 = 7;
const int CONTROLPOT1 = 8;
const int CONTROLPOT2 = 9;
const int CONTROLPOT3 = 10;

/************ END EDIT HERE **************/

int lastWheel0 = 100000;
int lastWheel1 = 100000;

int lastPot0 = 100000;
int lastPot1 = 100000;
int lastPot2 = 100000;
int lastPot3 = 100000;

const int numReadings = 30;

int wheel0Min = 1023;
int wheel1Min = 1023;
int pot0Min = 1023;
int pot1Min = 1023;
int pot2Min = 1023;
int pot3Min = 1023;

int wheel0Max = 0;
int wheel1Max = 0;
int pot0Max = 0;
int pot1Max = 0;
int pot2Max = 0;
int pot3Max = 0;

boolean cMode = false;

int wheel0Raw = 0;
int wheel1Raw = 0;

int pot0Raw = 0;
int pot1Raw = 0;
int pot2Raw = 0;
int pot3Raw = 0;

int index=0;
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
static boolean test=false;

int WHEEL1 = A0;
int WHEEL2 = A1;

int POT0 = A2;
int POT1 = A3;
int POT2 = A4;
int POT3 = A5;

void setup() {
 if(test==false){
   MIDI.begin(1); 
   MIDI.turnThruOff();
  }
 pinMode(8, INPUT);
 pinMode(13, OUTPUT);
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
    wheel0Raw = analogRead(WHEEL1);
    wheel1Raw = analogRead(WHEEL2);
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
  
  
  
  // average the readings!
  wheel0Total = wheel0Total - readings0[index];
  wheel1Total = wheel1Total - readings1[index];
  pot0Total = pot0Total - readingsP0[index];
  pot1Total = pot1Total - readingsP1[index];
  pot2Total = pot2Total - readingsP2[index];
  pot3Total = pot3Total - readingsP3[index];
  
  readings0[index] = analogRead(WHEEL1);
  readings1[index] = analogRead(WHEEL2);
  readingsP0[index] = analogRead(POT0);
  readingsP1[index] = analogRead(POT1);
  readingsP2[index] = analogRead(POT2);
  readingsP3[index] = analogRead(POT3);
if(test==true){
  Serial.println(readings0[index]);
}
  wheel0Total = wheel0Total + readings0[index];
  wheel1Total = wheel1Total + readings1[index];
  pot0Total = pot0Total + readingsP0[index];
  pot1Total = pot1Total + readingsP1[index];
  pot2Total = pot2Total + readingsP2[index];
  pot3Total = pot3Total + readingsP3[index];
  index = index + 1;
  
  if(index >= numReadings){
    index=0;
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
  if(mappedWheel0<-7500) mappedWheel0 = -8192;

    
    if(mappedWheel0<1200&&mappedWheel0>-200){
     mappedWheel0=0; 
    }
   
    if(mappedWheel0>lastWheel0+150||mappedWheel0<lastWheel0-150){
  
      lastWheel0 = mappedWheel0;
      
       //Serial.println(mappedWheel0);
    if(test==false){
      MIDI.sendPitchBend(mappedWheel0, channelOut);
    }
      //Serial.println(mappedWheel0);
    }else{
      
    }
  
  // Process Volume cc

  int mappedWheel1 = map(wheel1Avg, wheel1Min, wheel1Max, 0, 127); //map value to 0-127

  if(mappedWheel1>124) mappedWheel1 = 127;
  if(mappedWheel1<5) mappedWheel1 = 0;

    if(mappedWheel1>lastWheel1+2||mappedWheel1<lastWheel1-2){
      lastWheel1=mappedWheel1;
      //
        if(test==false){
          MIDI.sendControlChange(CONTROLWHEEL2, mappedWheel1, channelOut);
        }
     // digitalWrite(13, HIGH);      
    }else{

    }
    
  // Process Control0 cc

  int mappedPot0 = map(pot0Avg, pot0Min, pot0Max, 0, 127); //map value to 0-127

  if(mappedPot0>125) mappedPot0 = 127;
  if(mappedPot0<2) mappedPot0 = 0;

  if(mappedPot0>lastPot0+2||mappedPot0<lastPot0-2){
    lastPot0=mappedPot0;
    //
      if(test==false){
        MIDI.sendControlChange(CONTROLPOT0, mappedPot0, channelOut);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  
  // Process Control1 cc

  int mappedPot1 = map(pot1Avg, pot1Min, pot1Max, 0, 127); //map value to 0-127

  if(mappedPot1>125) mappedPot1 = 127;
  if(mappedPot1<2) mappedPot1 = 0;

  if(mappedPot1>lastPot1+2||mappedPot1<lastPot1-2){
    lastPot1=mappedPot1;
    //
      if(test==false){
        MIDI.sendControlChange(CONTROLPOT1, mappedPot1, channelOut);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  
  // Process Control0 cc

  int mappedPot2 = map(pot2Avg, pot2Min, pot2Max, 0, 127); //map value to 0-127

  if(mappedPot2>125) mappedPot2 = 127;
  if(mappedPot2<2) mappedPot2 = 0;

  if(mappedPot2>lastPot2+2||mappedPot2<lastPot2-2){
    lastPot2=mappedPot2;
    //
      if(test==false){
        MIDI.sendControlChange(CONTROLPOT2, mappedPot2, channelOut);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  
  // Process Control0 cc

  int mappedPot3 = map(pot3Avg, pot3Min, pot3Max, 0, 127); //map value to 0-127

  if(mappedPot3>125) mappedPot3 = 127;
  if(mappedPot3<2) mappedPot3 = 0;

  if(mappedPot3>lastPot3+2||mappedPot3<lastPot3-2){
    lastPot3=mappedPot3;
    //
      if(test==false){
        MIDI.sendControlChange(CONTROLPOT3, mappedPot3, channelOut);
      }
   // digitalWrite(13, HIGH);      
  }else{

  }
  

}
