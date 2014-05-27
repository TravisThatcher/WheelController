#include "Arduino.h"
#include "ControlPot.h"

ControlPot::ControlPot(int ccNum, int pinAssign)
{
  _avgReading = 0;
  _minReading = 1023;
  _maxReading = 0;
  _rawReading = 0;
  _bufferSize = 30;
  _readingsPointer = 0;
  _pinAssign = pinAssign; 
  _ccNum = ccNum;
}

int ControlPot::getAvgReading(){
  return _avgReading;
}

void ControlPot::readValue()
{
  _readings[_readingsPointer] = analogRead(_pinAssign);
  
  _readingsPointer ++;
  if(_readingsPointer >= _bufferSize){
     _readingsPointer = 0; 
  }
  
  

}

