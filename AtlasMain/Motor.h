// Motor class

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Encoder.h>

class Motor {
	private:
		Encoder *enc;
		uint8_t pwm, d1, d2;
		uint8_t direction;
		int64_t position;
	public:
		Motor(uint8_t _pwm, uint8_t _d1, uint8_t _d2, Encoder *_enc, uint8_t _direction);
		void setSpeed(int speed);
		int getPosition();
};

//add Motors
//is there a less messy way of access the motors across different menu sections
static Encoder linEnc(22,23);
static Motor linear(10,11,12,&linEnc,1);

#endif
