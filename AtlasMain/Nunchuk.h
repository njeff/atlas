/*
  Nunchuk.h
  
  Arduino library for interface to the Wiimote Nunchuk (hard-wired, not wireless)
  Based on Chad Phillips' work at http://www.windmeadow.com/node/42
  and the non-OEM initialization by crimony, http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1264805255
  
  To use:
  - Your sketch will need to #include <Wire.h> before you #include <Nunchuk.h>
  - Wiring:
      - GREEN data pin to A4
      - YELLOW clock pin to A5
      - RED power pin to arduino 3.3v
      - WHITE ground pin to arduino ground
  - Call read(), check whether it isOk(), then use the current results.
  
  2012-07-27 @machinesalem,  (cc) https://creativecommons.org/licenses/by/3.0/
*/

#ifndef NUNCHUK_h
#define NUNCHUK_h

#include "Arduino.h"

#define NUNCHUK_TWI_DEVICE_ADDRESS 0x52
#define NUNCHUK_TWI_CMD_IDENT      0xFA
#define NUNCHUK_TWI_CMD_INIT       0x40
#define NUNCHUK_TWI_CMD_ZERO       0x00
#define NUNCHUK_TWI_BUFFER_SIZE    6
#define NUNCHUK_TWI_DELAY_MICROSEC 25


class Nunchuk
{
  private:
    uint8_t _ok;
    int _ax, _ay, _az;
    long _ax2, _ay2, _az2;
    uint8_t _buf[NUNCHUK_TWI_BUFFER_SIZE];
    uint8_t _decode_byte(uint8_t x);
    uint32_t starty;
    uint32_t startx;
    uint32_t startc;
    uint32_t startz;

    uint32_t yt;
    uint32_t xt;
    uint32_t ct;
    uint32_t zt;

    uint8_t lastz;
    uint8_t lastc;

    uint8_t changez;
    uint8_t changec;
    uint8_t deadzone;

  public:
    Nunchuk(uint8_t dz);
    void begin();
    void read();       /* Read the current data */
    bool isOk();          /* Did the data read ok? */
    bool getButtonZ();    /* 1 if pressed, 0 if not */
    bool getButtonC();    /* 1 if pressed, 0 if not */
    
    int  getJoyX();       /* -127 to 128, in practice -100 to 100 */
    int  getJoyY();       /* -127 to 128, in practice -100 to 100 */

    uint32_t yTime();
    uint32_t xTime();
    uint32_t cTime();
    uint32_t zTime();

    bool getChangeZ();
    bool getChangeC();
    
    int  getAccelX();     /* side-side  acceleration, -511 to 512 */
    int  getAccelY();     /* front-back acceleration, -511 to 512 */
    int  getAccelZ();     /* top-bottom acceleration, -511 to 512 */
    
    float getAccel();     /* Modulus of acceleration.  1G ~= 200 */
    
    float getTiltX();     /* degrees, +/- 90 */
    float getTiltY();     /* degrees, +/- 90 */
    float getTiltZ();     /* degrees, +/- 90 */
};

#endif
