#define MENU_SIZE 5

typedef struct {
	STATE state;
	STATE (*pFunc)(Nunchuk input, LiquidCrystal lcd);
} MENU_STATE;

MENU_STATE menu_state[] = {
	{HOME_MENU, homeMode},
	{KEYFRAME_MENU, keyframeMode},
	{KEYFRAME_EDIT, keyframeEdit},
	{FRAME_MENU, framesMode},
	{DURATION_MENU, durationMode}
};