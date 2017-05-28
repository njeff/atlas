#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "frames.h"

static int32_t frames = 0;

STATE framesMode(Nunchuk input, LiquidCrystal lcd){
	static uint32_t lastUpdate = 0;
	static uint8_t loopCycle = 125;
	static char buffer[5];

	lcd.setCursor(0,0);
	lcd.print("Number of frames in");
	lcd.setCursor(0,1);
	lcd.print("the timelapse:");
	lcd.setCursor(12,2);
	lcd.print("frames ");
	lcd.write((uint8_t)0);

	int8_t y = input.getJoyY();
	if(millis() - lastUpdate > loopCycle){
		if (abs(y) > 80){
			loopCycle = 75;
		} else {
			loopCycle = 125;
		}
		if (y > DEADZONE) {
			if(input.yTime() > 1000){
				frames+=5;
			} else {
				frames++;
			}
			lastUpdate = millis();
		} else if (y < -DEADZONE) {
			if(input.yTime() > 1000){
				frames-=5;
			} else {
				frames--;
			}
			if(frames < 0){
				frames = 0;
			}
			lastUpdate = millis();
		} else if(input.getJoyX() < -DEADZONE){
			return HOME_MENU;
		}
		
		sprintf(buffer, "%5lu", frames);
		lcd.setCursor(6, 2);
		lcd.print(buffer);
	}

	return FRAME_MENU;
}

int32_t getFrames(){
	return frames;
}