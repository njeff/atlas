#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "duration.h"
#include "frames.h"

//index 0: duration, index 1: exposure
static int32_t times[2];
static uint8_t position = 0;

STATE durationMode(Nunchuk input, LiquidCrystal lcd){
	static int32_t timeBetweenFrames = 0;
	static uint32_t lastUpdate = 0;
	static uint8_t loopCycle = 125;
	static char buffer[5];

	lcd.setCursor(1,0);
	lcd.print("Timelapse duration:");
	lcd.setCursor(11,1);
	lcd.print("minutes");
	lcd.setCursor(1,2);
	lcd.print("Exposure:");
	lcd.setCursor(11,3);
	lcd.print("seconds");

	lcd.setCursor(0,position);
	lcd.print(">");

	int8_t y = input.getJoyY();
	if(millis() - lastUpdate > loopCycle){
		if(input.getButtonZ()){ //edit
			//draw arrow
			lcd.setCursor(19,position+1);
			lcd.write((uint8_t)0);
			if (abs(y) > 80){
				loopCycle = 75;
			} else {
				loopCycle = 125;
			}
			if (y > DEADZONE) {
				if(input.yTime() > 1000){
					times[position>>1]+=5;
				} else {
					times[position>>1]++;
				}
				lastUpdate = millis();
			} else if (y < -DEADZONE) {
				if(input.yTime() > 1000){
					times[position>>1]-=5;
				} else {
					times[position>>1]--;
				}
				if(times[position>>1] < 0){
					times[position>>1] = 0;
				}
				lastUpdate = millis();
			}
		} else { //scroll
			//clear arrow
			lcd.setCursor(19,position+1);
			lcd.write(" ");
			if(y < -DEADZONE){
				if(position == 0){
					lcd.setCursor(0,position);
					lcd.print(" ");
					position = 2;
				}
				lastUpdate = millis();
			} else if(y > DEADZONE){
				if(position == 2){
					lcd.setCursor(0,position);
					lcd.print(" ");
					position = 0;
				}
				lastUpdate = millis();
			} else if(input.getJoyX() < -DEADZONE){
				return HOME_MENU;
			}
		}

		if(getFrames() > 1){
			timeBetweenFrames = (times[0]*60)/(getFrames()-1);
			//don't allow exposure to exceed time between frames
			if(times[1] > timeBetweenFrames - 2 && timeBetweenFrames - 2 > 0){ //kinda jank
				times[1] = timeBetweenFrames - 2;
			}
		} else {
			timeBetweenFrames = 0;
		}
		
		//print duration
		sprintf(buffer, "%5lu", times[0]);
		lcd.setCursor(5, 1);
		lcd.print(buffer);

		//print exposure
		sprintf(buffer,"%5lu", times[1]);
		lcd.setCursor(5, 3);
		lcd.print(buffer);
	}

	return DURATION_MENU;
}

int32_t getDuration(){
	return times[0];
}

int32_t getExposure(){
	return times[1];
}