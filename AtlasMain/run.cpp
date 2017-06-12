#include <Arduino.h>
#include "Nunchuk.h"
#include <LiquidCrystal.h>
#include "main.h"
#include "keyframe.h"
#include "duration.h"
#include "Motor.h"
#include "run.h"
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
static bool ready = 0;

//may want to remove delays, but they are only used in the beginning and end of shoots

STATE runMode(Nunchuk input, LiquidCrystal lcd){
	static uint32_t seconds = 0;
	static char buffer[5];
	static char timeBuffer[8];
	
	if(getKeyframeCount() == 1 || getDuration() == 0 || getFrames() == 0){
		lcd.setCursor(6,1);
		lcd.print("Invalid");
		lcd.setCursor(6,2);
		lcd.print("settings.");
		delay(1000);
		return HOME_MENU;
	}

	if(ready == 0){
		//run to frame 1

		ready = 1;
	} else {
		//if we just entered
		if(ctime == 0){
			lcd.clear();
			digitalWrite(SHUTTER, LOW);
			digitalWrite(FOCUS, HIGH); //get first shot ready
			lcd.setCursor(7,0);
			lcd.print("Ready.");
			lcd.setCursor(3,2);
			lcd.print("You may remove");
			lcd.setCursor(4,3);
			lcd.print("the nunchuk.");
			delay(4000);
			lcd.clear();

			//LCD routines
			lcd.setCursor(0,0);
			lcd.print("Frames left:");
			lcd.setCursor(0,1);
			lcd.print("Time Left:");
			lcd.setCursor(0,2);
			lcd.print("Next Frame in:");
			lcd.setCursor(0,3);
			lcd.print("1");
			// for(uint8_t i = 1; i < getKeyframeCount(); i++){
			// 	lcd.print("     ");
			// 	lcd.print(i+1);
			// }
			//init
			moved = 1;
			focused = 1;
			tframes = getFrames();
			ctime = getDuration()*60*1000;
			endTime = millis() + ctime;
			timeBetweenFrames = ctime/(getFrames()-1);
			lastTime = millis() - timeBetweenFrames - 1;

			//run to frame 1
			//linear
			while(abs(linear.getPosition() - getKeyframe(0,0)) > 10){
				if(linear.getPosition() > getKeyframe(0,0)){
					linear.setSpeed(-800);
				} else {
					linear.setSpeed(800);
				}
			}
			linear.setSpeed(0);
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
			delay(10); //temporary
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
			double f = (double)(getKeyframeCount()-1) * (double)(getFrames() - tframes)/getFrames();
			int currentKeyframe = (int)f; //left frame
			double mu = f - currentKeyframe;
			//linear
			int ptLin[4]; //liner axis points
			ptLin[1] = getKeyframe(currentKeyframe,0);
			ptLin[2] = getKeyframe(currentKeyframe+1,0);
			if(currentKeyframe == 0){
				ptLin[0] = ptLin[1];
			} else {
				ptLin[0] = getKeyframe(currentKeyframe-1,0);
			}
			if(currentKeyframe == getKeyframeCount()-1){
				ptLin[3] = ptLin[2];
			} else {
				ptLin[3] = getKeyframe(currentKeyframe+2,0);
			}
			int linPos = (int)CubicInterpolate(ptLin[0],ptLin[1],ptLin[2],ptLin[3], mu);

			//thread-blocking move; temporary
			while(abs(linear.getPosition() - linPos) > 10){
				if(linear.getPosition() > linPos){
					linear.setSpeed(-800);
				} else {
					linear.setSpeed(800);
				}
			}
			linear.setSpeed(0);

			//show progress
			lcd.setCursor(0,3);
			sprintf(buffer,"%1.2f",f);
			lcd.print(buffer);
			lcd.print(" ");
			sprintf(buffer,"%1.2f",mu);
			lcd.print(buffer);
			lcd.print(" ");
			sprintf(buffer,"%5d", linPos);
			lcd.print(buffer);

			moved = 1;
		}

		//if done
		if(tframes == 0){
			ctime = 0;
			ready = 0;
			lcd.clear();
			lcd.setCursor(7,1);
			lcd.print("Done!");
			delay(1000);
			return HOME_MENU;
		}
	}

	//emergency exit
	if(input.isOk()){
		if(input.getButtonC() && input.getButtonZ()){
			tframes = 0;
			ctime = 0;
			ready = 0;
			return HOME_MENU;
		}
	} else {
		input.begin();
	}

	return RUNNING;
}

double CubicInterpolate(double y0, double y1, double y2, double y3, double mu){
	double a0,a1,a2,a3,mu2;
	mu2 = mu*mu;
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;
	return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}
