#ifndef RUN_H
#define RUN_H
STATE runMode(Nunchuk input, LiquidCrystal lcd);
double CubicInterpolate(double y0, double y1, double y2, double y3, double mu);

#endif