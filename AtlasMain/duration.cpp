#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "duration.h"
#include "frames.h"

static uint32_t duration = 0;

STATE durationMode(Nunchuk input, LiquidCrystal lcd){
	static uint32_t timeBetweenFrames = 0;
	static uint32_t lastUpdate = 0;
	static uint8_t loopCycle = 125;
	static char buffer[5];

	lcd.setCursor(0,0);
	lcd.print("Duration of the");
	lcd.setCursor(0,1);
	lcd.print("timelapse:");
	lcd.setCursor(11,2);
	lcd.print("minutes ");
	lcd.write((uint8_t)0);
	lcd.setCursor(0,3);
	lcd.print("Seconds/frame:");

	int8_t y = input.getJoyY();
	if(millis() - lastUpdate > loopCycle){
		if (abs(y) > 80){
			loopCycle = 75;
		} else {
			loopCycle = 125;
		}
		if (y > DEADZONE) {
			if(input.yTime() > 1000){
				duration+=5;
			} else {
				duration++;
			}
			lastUpdate = millis();
		} else if (y < -DEADZONE) {
			if(input.yTime() > 1000){
				duration-=5;
			} else {
				duration--;
			}
			if(duration < 0){
				duration = 0;
			}
			lastUpdate = millis();
		}	
		sprintf(buffer, "%5lu", duration);
		lcd.setCursor(5, 2);
		lcd.print(buffer);

		//print time between frames
		if(getFrames() > 1){
			timeBetweenFrames = (duration*60)/(getFrames()-1);
		} else {
			timeBetweenFrames = 0;
		}
		sprintf(buffer,"%5lu",timeBetweenFrames);
		lcd.setCursor(15,3);
		lcd.print(buffer);

		if(input.getJoyX() < -DEADZONE){
			return HOME_MENU;
		}
	}

	return DURATION_MENU;
}

int32_t getDuration(){
	return duration;
}