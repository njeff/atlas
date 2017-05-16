#ifndef KEYFRAME_H
#define KEYFRAME_H
STATE keyframeMode(Nunchuk input, LiquidCrystal lcd);
uint8_t getFrameCount();
int32_t getKeyframe(int frame, int axis);
STATE keyframeEdit(Nunchuk input, LiquidCrystal lcd);

#endif