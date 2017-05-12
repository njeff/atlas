# Atlas

A prototype three-axis (linear, pan, and tilt) camera slider for timelapses (and maybe live video too).

### Hardware/Software
- Teensy 3.2
    - Programmed with Arduino's C/C++ language for ease of prototyping
    - Using a modified [nunchuck library](https://github.com/hughpyle/machinesalem-arduino-libs/tree/master/nunchuk) for input
- Three worm gear motors with encoders (may change to steppers)

This is a continuation of a camera slider I created two years ago that I originally developed on an ATMega328 with AVR Studio. It used a stepper motor with trapezoidal speed profiles and was designed to move between each shot of a timelapse. However, I had to disassemble it so the rail could be used on my CNC machine. All the old code/project files are zipped in AtlasMove.zip

Hopefully I will actually create a finished device this time.