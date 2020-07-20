/*
  ControlPot.h
  Library for Control Potentiometer Sensor
*/


#include "Arduino.h"

class ControlPot{
 public:
   ControlPot(int ccNum, int pinAssign);
   int getAvgReading();
   void readValue();
   
 private:
    int _avgReading;
    int _minReading;
    int _maxReading;
    int _rawReading;
    int _bufferSize;
    int _readings[30];
    int _readingsPointer;
    int _pinAssign;
    int _ccNum;
 };
