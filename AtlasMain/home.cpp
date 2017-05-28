#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "home.h"

STATE homeMode(Nunchuk input, LiquidCrystal lcd){
	static uint8_t position = 0;
	static uint32_t lastUpdate = 0;

	lcd.setCursor(1, 0);
	lcd.print("Set Keyframes");
	lcd.setCursor(1, 1);
	lcd.print("# of Frames");
	lcd.setCursor(1, 2);
	lcd.print("Duration");
	lcd.setCursor(1, 3);
	lcd.print("Start (hold z)");

	lcd.setCursor(0,position);
	lcd.print(">");

	if(millis() - lastUpdate > LOOPTIME){
		if(input.getJoyY() < -DEADZONE){
			lcd.setCursor(0,position);
			lcd.print(" ");
			position = (position+1)%4;
			lastUpdate = millis();
		} else if(input.getJoyY() > DEADZONE){
			lcd.setCursor(0,position);
			lcd.print(" ");
			if(position == 0){
				position = 3;
			} else {
				position--;
			}
			lastUpdate = millis();
			//if stick moved to right
		} else if(input.getJoyX() > DEADZONE && input.xTime() == 1){
			switch(position){
				case 0:
					return KEYFRAME_MENU;
				case 1:
					return FRAME_MENU;
				case 2:
					return DURATION_MENU;
				case 3:
					if(input.getButtonZ()){ //z must be held to start (prevent accidental start)
						return RUNNING;
					} else {
						break;
					}
				default:
					return HOME_MENU;
			}
			lastUpdate = millis();
		}
	}
	
	return HOME_MENU;
}