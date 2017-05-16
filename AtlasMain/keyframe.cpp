#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "keyframe.h"

static int32_t keyframes[4][3];
static uint8_t fcount = 1; //number of frames
static uint8_t positionM = 0; //cursor pos. in frame list, 0 indexed
static uint8_t lastPosM = 1;

static uint8_t positionE = 2; //cursor position in edit menu

STATE keyframeMode(Nunchuk input, LiquidCrystal lcd){
	static uint32_t lastUpdate = 0;
	
	for(int i = 0; i<fcount; i++){
		lcd.setCursor(1, i);
		lcd.print("Keyframe ");
		lcd.print(i+1);
		lcd.print("       "); //to clear off "+ Frame text, c to del"
	}
	if(fcount < 4){
		lcd.setCursor(1,fcount);
		lcd.print("+ Frame, c to del");
	}

	if(millis() - lastUpdate > LOOPTIME){
		if(input.getJoyY() < -DEADZONE){
			if(positionM < 3){
				positionM++;
			}
			if(positionM > fcount-1){
				fcount++;
			}
			lastUpdate = millis();
		}
		if(input.getJoyY() > DEADZONE){
			if(positionM > 0){
				positionM--;
			}
			lastUpdate = millis();
		}

		if(input.getChangeC() && input.cTime() == 1){
			//only allow deleting of most recent frame
			if(positionM == fcount-1 && positionM != 0){
				if(positionM < 3){
					lcd.setCursor(0,positionM+1);
					lcd.print("                  ");
				}
				fcount--;
				positionM--;
			}
		}

		if(input.getJoyX() > DEADZONE && input.xTime() == 1){
			return KEYFRAME_EDIT;
		}
		if(input.getJoyX() < -DEADZONE && input.xTime() == 1){
			return HOME_MENU;
		}
	}

	//handle cursor
	if(positionM != lastPosM){
		lcd.setCursor(0,lastPosM);
		lcd.print(" ");
	}
	lcd.setCursor(0,positionM);
	lcd.print(">");

	lastPosM = positionM;

	return KEYFRAME_MENU;
}

uint8_t getFrameCount(){
	return fcount;
}

int32_t getKeyframe(int frame, int axis){
	return keyframes[frame][axis];
}

STATE keyframeEdit(Nunchuk input, LiquidCrystal lcd){
	static uint32_t lastUpdate = 0;
	lcd.setCursor(1,0);
	lcd.print("Editing keyframe ");
	lcd.print(positionM+1);
	lcd.setCursor(1,1);
	lcd.print("Hold z to edit axis");
	lcd.setCursor(1,2);
	lcd.print("Linear axis");
	lcd.setCursor(1,3);
	lcd.print("Pan/Tilt");

	lcd.setCursor(0,positionE);
	lcd.print(">");

	if(millis() - lastUpdate > LOOPTIME){
		if(input.getJoyY() < -DEADZONE){
			if(positionE < 3){
				lcd.setCursor(0,positionE);
				lcd.print(" ");
				positionE++;
			}
		}
		if(input.getJoyY() > DEADZONE){
			if(positionE > 2){
				lcd.setCursor(0,positionE);
				lcd.print(" ");
				positionE--;
			}
			lastUpdate = millis();
		}

		if(input.getJoyX() < -DEADZONE && input.xTime() == 1){
			return KEYFRAME_MENU;
		}
	}

	return KEYFRAME_EDIT;
}