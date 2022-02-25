#pragma once
#include <Arduino.h>

// https://forum.pjrc.com/threads/64645-Teensy-4-1-WS2812B-LED-Problem
// https://github.com/PaulStoffregen/WS2812Serial
// git clone git@github.com:PaulStoffregen/WS2812Serial.git
#include <WS2812Serial.h>
#define USE_WS2812SERIAL

// FASTLED_USING_NAMESPACE
#define FASTLED_INTERNAL // Pragma messages should not be getting marked as warnings or errors - that's a bug you should file with the IDE. In the meantime, you can disable them by including #define FASTLED_INTERNAL before #include <FastLED.h>.
#include <FastLED.h>
#include "Leds.h"

extern CRGB leds[NUM_LEDS];
extern CRGB ledsTmp[NUM_LEDS];
extern CRGB ledsTmpLow[NUM_LEDS/2];
extern CRGB ledsTmpHigh[NUM_LEDS/2];
extern CRGB bufferBig[NUM_LEDS * 3];

extern CRGB secondary[NUM_LEDS_SECONDARY];

extern bool blockingLookup[NUM_LEDS];

extern int8_t pitch;

extern bool useSerial;
extern bool ledsEnabled;
extern bool firstFrame;
extern bool readFromNano;

extern bool boolSpinning;
extern bool boolVertical;
extern bool boolUp;

//extern bool mirror;
extern uint8_t gHue;
extern unsigned int msPerFrame;
extern unsigned long fCount;

//extern uint8_t msPerHue;
//extern uint8_t hueIncrement;
//extern unsigned long 
//extern unsigned long pixelNextUpdateTime;

extern unsigned int state;

//extern unsigned long frameCount;
void vuFilter();
int findUnused();
void DrawPixels(float fPos, float count, CRGB color);
void mirror();
void flip();
void buffer2leds(unsigned int bufferOffset, bool flip);
void buffer2gap(unsigned int bufferOffset, unsigned int gap);

void b2l(unsigned int bufferIndex, unsigned int ledsIndex, unsigned int numberOfLeds, bool flip);

void bar2handle();
void ledsTmp2leds();

/*
union I2Cdata_ {
  struct __attribute__((packed)){
    uint8_t left[7];
    uint8_t right[7];
    int8_t pitch;
  };
  uint8_t bytes[15];
};
extern union I2Cdata_ I2Cdata;
*/

typedef struct Pixel {
  bool used;
  float ledPos;
  float gravity;
  float velocity;
  float heat;
  unsigned long startTime;
  CRGB pixelData;
  //CHSV pixelData;
  bool blocking;
} Pixel;

#define NUM_PIXELS 32
extern Pixel Pixels[NUM_PIXELS];

extern float filteredLeftFast[7];
extern float filteredRightFast[7];
extern float filteredLeftSlow[7];
extern float filteredRightSlow[7];


/*
union vu_ {
  struct __attribute__((packed)){
    uint8_t left[7];
    uint8_t right[7];
  };
  uint8_t bytes[14];
};
extern union vu_ vu;
*/

////////////
// effect //
////////////
union effectPacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 1;
    uint8_t effect;
  };
  uint8_t bytes[2];
};
extern union effectPacket_ effectPacket;
///////////
// pitch //
///////////
union pitchPacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 2;
    int8_t pitch;
  };
  uint8_t bytes[2];
};
extern union pitchPacket_ pitchPacket;
////////
// vu //
////////
union vuPacket_ {
  struct __attribute__((packed)){
    uint8_t packetType = 3;
    uint8_t left[7];
    uint8_t right[7];
  };
  uint8_t bytes[15];
};
extern union vuPacket_ vuPacket;
