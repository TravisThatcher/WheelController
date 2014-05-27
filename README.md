WheelController
===============

An Arduino-based MIDI pitch and mod-wheel controller.

The WheelController utilizes the Arduino MIDI Library for basic MIDI IO. This library can be found at:
https://github.com/FortySevenEffects/arduino_midi_library

You must have this library in your path in the Arduino IDE in order to compile the WheelController.

Arduino Source Code
===============

The current source, WheelController1.1 is a work in progress, building a more formalized and modular version of 
WheelController1.0, also found in the src folder.  WheelController1.0 is the current fully functional version.

Schematic
===============

The schematics directory contains a png of the schematic, including the Arduino Uno R3 and the controls. There is also an Eagle schematic file. 

TODO
===============

*Designer an Arduino shield based around the schematic in Eagle to allow for easier construction. This would not be far off from the currently available proto-shields which have been used in previous builds.

*Finish modularization of the sensor code.

*Build in functionality to change the midi channel and cc numbers via runtime mode instead of hard-coded constants.

Contact
===============
Travis Thatcher - recompas@gmail.com
