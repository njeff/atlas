#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "keyframe.h"
#include "duration.h"
#include "frames.h"

static int32_t tframes = 0;
//all milliseconds
static uint32_t ctime = 0;
static uint32_t timeBetweenFrames = 0;
static uint32_t lastTime;
static uint32_t currentTime;
static uint32_t endTime;
static bool focused = 0;
static bool moved = 0;

//may want to remove delays, but they are only used in the beginning and end of shoots

STATE runMode(Nunchuk input, LiquidCrystal lcd){
	static uint32_t seconds = 0;
	static char buffer[5];
	static char timeBuffer[8];
	
	if(getFrameCount() == 1 || getDuration() == 0 || getFrames() == 0){
		lcd.setCursor(6,1);
		lcd.print("Invalid");
		lcd.setCursor(6,2);
		lcd.print("settings.");
		delay(1000);
		return HOME_MENU;
	}

	//LCD routines
	lcd.setCursor(0,0);
	lcd.print("Frames left:");
	lcd.setCursor(0,1);
	lcd.print("Time Left:");
	lcd.setCursor(0,2);
	lcd.print("Next Frame in:");
	lcd.setCursor(0,3);
	lcd.print("1");
	for(uint8_t i = 1; i < getFrameCount(); i++){
		lcd.print("     ");
		lcd.print(i+1);
	}

	//if we just entered
	if(ctime == 0){
		lcd.clear();
		digitalWrite(FOCUS, HIGH); //get first shot ready
		lcd.setCursor(7,0);
		lcd.print("Ready.");
		lcd.setCursor(3,2);
		lcd.print("You may remove");
		lcd.setCursor(4,3);
		lcd.print("the nunchuk.");
		delay(4000);
		lcd.clear();
		//init
		moved = 1;
		focused = 1;
		tframes = getFrames();
		ctime = getDuration()*60*1000;
		endTime = millis() + ctime;
		timeBetweenFrames = ctime/(getFrames()-1);
		lastTime = millis() - timeBetweenFrames - 1;
	}

	sprintf(buffer,"%5lu",tframes);
	lcd.setCursor(15, 0);
	lcd.print(buffer);

	seconds = (endTime - currentTime)/1000;
	sprintf(timeBuffer,"%02lu:%02lu:%02lu",seconds/3600,(seconds/60)%60,seconds%60);
	lcd.setCursor(12,1);
	lcd.print(timeBuffer);

	sprintf(buffer, "%5lu", (timeBetweenFrames-(currentTime-lastTime))/1000);
	lcd.setCursor(15,2);
	lcd.print(buffer);

	currentTime = millis();

	//take photo
	if(currentTime-lastTime > timeBetweenFrames){
		digitalWrite(SHUTTER, HIGH);
		delay(5); //temporary
		digitalWrite(SHUTTER, LOW);
		digitalWrite(FOCUS, LOW);
		focused = 0;
		moved = 0;
		lastTime = currentTime;
		tframes--;
	}

	//focus a second before capture
	if((currentTime-lastTime) > (timeBetweenFrames-1000) && focused == 0){
		digitalWrite(FOCUS, HIGH);
		focused = 1;
	}

	//move
	if(currentTime > (lastTime + getExposure() + 500) && moved == 0){
		moved = 1;

	}

	//if done
	if(tframes == 0){
		ctime = 0;
		lcd.clear();
		lcd.setCursor(7,1);
		lcd.print("Done!");
		delay(1000);
		return HOME_MENU;
	}

	//emergency exit
	if(input.isOk()){
		if(input.getButtonC() && input.getButtonZ()){
			tframes = 0;
			ctime = 0;
			return HOME_MENU;
		}
	} else {
		input.begin();
	}

	return RUNNING;
}