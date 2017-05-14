#define MENU_SIZE 4

typedef struct {
	STATE state;
    STATE (*pFunc)(Nunchuk input, LiquidCrystal lcd);
} MENU_STATE;

MENU_STATE menu_state[] = {
	{HOME_MENU, homeMode},
	{KEYFRAME_MENU, keyframeMode},
	{FRAME_MENU, framesMode},
	{DURATION_MENU, durationMode}
};