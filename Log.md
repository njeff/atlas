## Progress Log

10.1.14

- Migrated from AVR Studio 4 to 6

10.3.14

- Encoder not working
- Got encoder working after including interrupt.h
- Printing encoder count to LCD

10.4.14

- [16:45] Got transition table working, encoder now changes state; for some reason when changing directions the value continues in the previous direction once before going the other way
- [18:20] Now displaying the appropriate label for each menu state; strings in PROGMEM

10.5.14

- [13:41] Moved refreshState to interrupt vector
- [19:00] Wonky menu cursor added
- [19:25] I think I got cursor logic right
- [19:27] Menu not really working
- [19:58] Scrolling menu working :D
- [21:00] Added Settings and End Time to menu

10.22.14

- [22:42] Did a little to try to get buttons working
- [23:16] Button toggling with debounce works! (but some reason the button check logic still triggers when the encoder is turned; isn't a problem though but look into why)

12.7.14

- [20:47] Trying to find out what is going on with the buttons. They change correctly when the intermediate variable isn't changed
- [20:55] I think I solved the button issue by removing the "clear bit" from the button state handling. Weird. Begin value changing state (enter menu state)
- [21:42] Basic menu entry added

12.8.14

- [16:40] Working on menu edit system
- [17:37] Got cursor change/scroll system to work

12.23.14

- Determined to get this done.

Jan.-Feb. 2015

- Switched to working on timelapse dolly hardware
- Got gear motor and stepper motor

3.1.15-3.3.15

- Soldering up controller board

4.6.15-4.8.15

- [4.8.15, 22:27]Finished rev. 0 of the AtlasMove. Menu system is fully function and time lapses can be created. Will do text run ASAP. 
- Still unsure of how to create clamps for the belt on each end of the dolly. Thinking about something very similar to Dynamic Perceptions' screw clamps.
- Will need larger toothed belt for stepper moves and lighted base (Current wood one is extremely heavy).

4.9.15

- Created very simple wood clamps for each end of the dolly to hold belt
- Tested dolly: fully functional
    - Doesn't handle inclines well: over ~10 degrees the motor audibly strains, over 30 degrees doesn't move, between 30 and 45 it slips when it begins moving (doesn't slip when not moving though)
    - Steeper angles not attempted
---

5.7.17

- Restarting project for 3 axis control (and APCS final project). The plan is to create a complete system that isn't just a prototype.
- Bought V-bearings, REV 1" extrusion (hopefully it is stable enough), 3x worm gear motors with encoders, HTD5 timing belt and pulley
    - Not sure if the motors will come in time
- Need to order parts for the controller and case
- Currently debating whether gear motors or stepper motors are a better choice for the system. I wanted to use DC worm gear motors with encoders to get closed loop positioning and to get more familiar with control systems. My first dolly used a stepper motor, which has the advantage of being easy to drive at low and high speeds, while gear motors cog up a slow speeds if they are significantly geared down (and if they are geared down to be very slow, they can't run fast anymore). Also the worm gears in the DC motor gearbox prevent back driving when the power is not applied (saving power). I could attach a worm to stepper motors that drive a worm gear to get the precision of the steppers and power saving of worm gears, but that would require me to order another set of motors (costing more money). I think I will probably have to use this on the pan and tilt axes because the 30 rpm gear motors will likely have too much backlash for sub-degree positioning. For the linear motion I could use a threaded rod like a CNC for extreme precision, accuracy, and non-back driving characteristics. However, I can no longer get the 1/2-10 Acme rod for extremely cheap anymore.
- I've ported over parts of the menu system from before to Arduino to run on a Teensy 3.2 (which I've had lying around for a while doing nothing so I guess it will be the controller of this project), but I'll likely change it up to handle the two rotary axes and support more features like keyframe positioning.
    - I've essentially got the frame count, duration, and exposure length copied. The menu is controlled with a Wii nunchuk: the joystick changes the value and the z button advances the menu. The amount of time the joystick is tilted and how tilted the stick is affect how fast the increment goes.

5.9.17

- Now creating a new input handling system and menu system
- Proposed input system: method that returns an array of how long each button/joystick axis has been held down for and joystick magnitudes
- Menu system: I've been looking at the AVR butterfly menu system and been taking some ideas from how to implement something like it
    - It uses two tables: one that lists every menu state and input combination and the resulting action, and one that lists the text and function that is supposed to be associated with each state.
- I may reuse some parts of the scolling menu system I had before.
- Also looked into [interpolation methods](http://paulbourke.net/miscellaneous/interpolation/) for computing smooth movement between keyframes. Cubic interpolation looks like the easiest way; it just can cause slight overshoot in some cases. 

5.11.17
- Added hold times to nunchuk library
- Still need to create better menu system

5.13.17
- Created new menu system inspired by AVR Butterfly menu system
    - Created a state table that pairs states with state functions (each state must have a function)
    - State text and all state transitions are handled within the function (AVR butterfly has some menu states that are only text, which is why there is a need for a transition table). I wanted the screen to show multiple menu options at once that the user can visually scroll through, so I made each scrollable page a state (and not each menu option).
- Duration and frame count menus have been implemented
- Still need to add keyframe functionality and actual run mode
- Z enters menu and left joystick exits menu (may change this), up/down increments and decrements values

5.15.17
- Added keyframe edit mode
- Keyframes can now be deleted
- Changed how the cursor is drawn on the keyframe menu so it is handled in one area
- Added LOOPTIME constant

5.16.17
- Added z hold for entering edit mode for each keyframe

5.25.17
- Cut holes in the case for the screen

5.26.17
- Soldered together part of the circuit since the breadboard was getting a bit hectic

5.27.17
- Added keyframe motor movement capabilities (need to add other axes; may skip the tilt and only have linear and pan due to hardware time limitation)
- Working on the actual run mode now