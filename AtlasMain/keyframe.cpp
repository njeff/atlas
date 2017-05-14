#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "keyframe.h"

static int32_t keyframes[4][3];

STATE keyframeMode(Nunchuk input, LiquidCrystal lcd){
	static uint8_t position = 0;
	static uint32_t lastUpdate = 0;

	for(int i = 0; i<=position; i++){
		lcd.setCursor(1, i);
		lcd.print("Keyframe ");
		lcd.print(i+1);
	}
	if(position < 3){
		lcd.setCursor(1,position+1);
		lcd.print("+ Keyframe");
	}

	//if(millis() - lastUpdate > 200){
		if(input.getJoyY() < -DEADZONE && input.yTime() == 1){
			if(position<3){
				position++;
			}
		}
		if(input.getChangeC() && input.cTime() == 1){

		}
		if(input.getJoyX() < -DEADZONE){
			return HOME_MENU;
		}
	//}

	return KEYFRAME_MENU;
}

int32_t getKeyframe(int frame, int axis){
	return keyframes[frame][axis];
}