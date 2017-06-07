// include the library code:
#include "main.h"
#include <Wire.h> 
#include <LiquidCrystal.h> 
#include <Encoder.h>

#include "Nunchuk.h"
#include "Motor.h"
#include "home.h"
#include "keyframe.h"
#include "frames.h"
#include "duration.h"
#include "run.h"
#include "menu.h" //add last

//Nunchuk for input
Nunchuk nc = Nunchuk(DEADZONE);

//Up-down arrow for menus
uint8_t doubleArrow[8] = {
	0b00100,
	0b01110,
	0b10101,
	0b00100,
	0b00100,
	0b10101,
	0b01110,
	0b00100
};

//initialize lcd with right pins
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

STATE state;
STATE nextstate;
STATE (*pStateFunc)(Nunchuk,LiquidCrystal);

void setup(){
	//pwm output to 10-bit
	analogWriteResolution(10);
	//init states
	state = HOME_MENU;
	pStateFunc = homeMode;
	//ready nunchuk
	nc.begin();
	nc.read();
	//init lcd
	lcd.createChar(0, doubleArrow);
	lcd.begin(20, 4);
	lcd.clear();
	lcd.setCursor(6,0);
	lcd.print("Atlas:");
	lcd.setCursor(2,1);
	lcd.print("Timelapse Motion");
	lcd.setCursor(7,3);
	lcd.print("Rev. 2");
	delay(1500);
	lcd.clear();

	//set the stand-by pin on the motor drivers
	pinMode(14, OUTPUT);
	digitalWrite(14, HIGH);
	//set camera pins
	pinMode(FOCUS, OUTPUT);
	pinMode(SHUTTER, OUTPUT);
	digitalWrite(FOCUS, LOW);
	digitalWrite(SHUTTER, LOW);
	//Serial.begin(115200);
}

void loop(){
	nc.read(); //update input values

	//if nunchuk not properly connected
	if(!nc.isOk() && state != RUNNING){
		lcd.clear();
		lcd.setCursor(2,1);
		lcd.print("Please reconnect");
		lcd.setCursor(4,2);
		lcd.print("the nunchuk.");
		while(!nc.isOk()){
			nc.begin();
			nc.read();
		}
		lcd.clear();
	}

	//run state function and get next state
	nextstate = pStateFunc(nc,lcd);

	//if there is a change of state
	if(nextstate != state){
		state = nextstate;
		lcd.clear();
		//look for next state function
		for(int i = 0; i<MENU_SIZE; i++){
			if(menu_state[i].state == state){
				pStateFunc = menu_state[i].pFunc;
				break;
			}
		}
	}
}
