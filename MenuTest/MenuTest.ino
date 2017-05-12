// include the library code:
#include <Wire.h> 
#include <LiquidCrystal.h> 
#include "Nunchuk.h"

#define DEADZONE 20

// Nunchuck for input
Nunchuk nc = Nunchuk(DEADZONE);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

boolean lastZ = 0;
char buffer[5];

void setup(){
	lcd.begin(20, 4);
	lcd.setCursor(1, 0);
	lcd.print("End Start Positions");
	lcd.setCursor(1, 1);
	lcd.print("# of Frames       0");
	lcd.setCursor(1, 2);
	lcd.print("Duration (min)    0");
	lcd.setCursor(1, 3);
	lcd.print("sec/frame         0");

	//ready nunchuck
	nc.begin();
	nc.read();
}

void loop(){
	lcd.setCursor(0, 2);
	lcd.print(" ");
	lcd.setCursor(0, 1);
	lcd.print(">");

	uint32_t lastUpdate = 0;
	uint8_t loopCycle = 125; //how fast to increment value

	int frames = 0;
	while(1){
		nc.read();
		int x = nc.getJoyX();
		if(millis()-lastUpdate > loopCycle){
			if (abs(x) > 80){
				loopCycle = 75;
			} else {
				loopCycle = 125;
			}
			if (x > DEADZONE) {
				if(nc.xTime() > 1000){
					frames+=5;
				} else {
					frames++;
				}
				lastUpdate = millis();
			} else if (x < -DEADZONE) {
				if(nc.xTime() > 1000){
					frames-=5;
				} else {
					frames--;
				}
				if(frames < 0){
					frames = 0;
				}
				lastUpdate = millis();
			}
		}
		sprintf(buffer, "%5d", frames);
		lcd.setCursor(15, 1);
		lcd.print(buffer);
		if(nc.getChangeZ() && nc.zTime() == 1){
			break;
		}
	}

	lcd.setCursor(0, 1);
	lcd.print(" ");
	lcd.setCursor(0, 2);
	lcd.print(">");

	int minutes = 0;
	uint32_t timeBetweenFrames = 0;
	uint32_t totalTime = 0;
	while(1){
		nc.read();
		int x = nc.getJoyX();
		if(millis()-lastUpdate > loopCycle){
			if (abs(x) > 80){
			  loopCycle = 75;
			} else {
			  loopCycle = 125;
			}
			if (x > DEADZONE) {
				if(nc.xTime() > 1000){
					minutes+=5;
				} else {
					minutes++;
				}
				lastUpdate = millis();
			} else if (x < -DEADZONE) {
				if(nc.xTime() > 1000){
					minutes-=5;
				} else {
					minutes--;
				}
				if(minutes < 0){
					minutes = 0;
				}
				lastUpdate = millis();
			}
		}
		sprintf(buffer, "%5d", minutes);
		lcd.setCursor(15, 2);
		lcd.print(buffer);
		totalTime = minutes*60*1000; //convert to millis
		timeBetweenFrames = totalTime/(frames-1);
		sprintf(buffer,"%5lu",timeBetweenFrames/1000);
		lcd.setCursor(15,3);
		lcd.print(buffer);
		if(nc.getChangeZ() && nc.zTime() == 1){
			break;
		}
	}

	lcd.setCursor(0, 2);
	lcd.print(" ");
	lcd.setCursor(0, 3);
	lcd.print(">Exposure (sec)    0");
	
	int exposure = 0;
	while(1){
		nc.read();
		int x = nc.getJoyX();
		if(millis()-lastUpdate > loopCycle){
			if (abs(x) > 80){
			  loopCycle = 75;
			} else {
			  loopCycle = 125;
			}
			if (x > DEADZONE) {
				if(nc.xTime() > 1000){
					exposure+=5;
				} else {
					exposure++;
				}
				if(exposure > (int)(timeBetweenFrames/1000 - 2)){ //maximum exposure
					exposure = timeBetweenFrames/1000 - 2;
				}
				lastUpdate = millis();
			} else if (x < -DEADZONE) {
				if(nc.xTime() > 1000){
					exposure-=5;
				} else {
					exposure--;
				}
				if(exposure< 0){
					exposure = 0;
				}
				lastUpdate = millis();
			}
		}
		sprintf(buffer, "%5d", exposure);
		lcd.setCursor(15, 3);
		lcd.print(buffer);
		if(nc.getChangeZ() && nc.zTime() == 1){
			break;
		}
	}

	// enter main loop
}