//Nunchuck joystick deadzone
#define DEADZONE 30
#define LOOPTIME 200

//camera release pins
#define SHUTTER 13
#define FOCUS 13

//all states
typedef enum STATE {
	HOME_MENU,
	KEYFRAME_MENU,
	KEYFRAME_EDIT,
	FRAME_MENU,
	DURATION_MENU,
	RUNNING
} STATE;