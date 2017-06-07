#include "Motor.h"

Motor::Motor(uint8_t _pwm, uint8_t _d1, uint8_t _d2, Encoder *_enc, uint8_t _direction){
	pwm = _pwm;
	pinMode(pwm, OUTPUT);
	d1 = _d1;
	d2 = _d2;
	pinMode(d1, OUTPUT);
	pinMode(d2, OUTPUT);
	direction = _direction; //1 or -1 for reverse
	enc = _enc;
}

void Motor::setSpeed(int speed){
	//clip
	if(abs(speed) > 1024){
		if(speed > 1024) {
			speed = 1024;
		} else {
			speed = -1024;
		}
	}
	if(speed == 0){
		digitalWrite(d1, HIGH);
		digitalWrite(d2, HIGH);
		analogWrite(pwm, 0);
	} else if(speed*direction < 0){
		digitalWrite(d1, LOW);
		digitalWrite(d2, HIGH);
		analogWrite(pwm, -speed*direction);
	} else {
		digitalWrite(d1, HIGH);
		digitalWrite(d2, LOW);
		analogWrite(pwm, speed*direction);
	}
}

int Motor::getPosition(){
	return enc->read();
}
