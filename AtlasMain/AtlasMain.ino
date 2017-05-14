// include the library code:
#include "main.h"
#include <Wire.h> 
#include <LiquidCrystal.h> 
#include "Nunchuk.h"
#include "home.h"
#include "keyframe.h"
#include "frames.h"
#include "duration.h"
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
	//init states
	state = HOME_MENU;
	pStateFunc = homeMode;
	//init lcd
	lcd.createChar(0, doubleArrow);
	lcd.begin(20, 4);
	//ready nunchuk
	nc.begin();
	nc.read();
}

void loop(){
	nc.read(); //update input values
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