#pragma once
#include "Leds.h"
//#include <Wire.h>
//#define SCL A3
//#define SDA A2

extern CRGB leds[NUM_LEDS];
extern CRGB ledsTmp[NUM_LEDS];
extern CRGB ledsTmpLow[NUM_LEDS/2];
extern CRGB ledsTmpHigh[NUM_LEDS/2];
extern CRGB bufferBig[NUM_LEDS * 3];

extern CRGB secondary[NUM_LEDS_SECONDARY];

extern bool blockingLookup[NUM_LEDS];

extern bool useSerial;
extern bool ledsEnabled;
extern bool firstFrame;
extern bool readFromNano;

//extern bool mirror;
extern uint8_t gHue;
extern unsigned int msPerFrame;

//extern unsigned long pixelNextUpdateTime;

extern unsigned int state;

//extern unsigned long frameCount;

int findUnused();
void mirror();
void flip();
void buffer2leds(unsigned int bufferOffset, bool flip);
void bar2handle();
void ledsTmp2leds();

uint8_t iH(uint8_t);

union I2Cdata_ {
  struct __attribute__((packed)){
    uint8_t left[7];
    uint8_t right[7];
    int8_t pitch;
  };
  uint8_t bytes[15];
};
extern union I2Cdata_ I2Cdata;

union vu_ {
  struct __attribute__((packed)){
    uint8_t left[7];
    uint8_t right[7];
  };
  uint8_t bytes[14];
};
extern union vu_ vu;


typedef struct Pixel {
  bool used;
  float ledPos;
  float gravity;
  float velocity;
  unsigned long startTime;
  CRGB pixelData;
  //CHSV pixelData;
  bool blocking;
} Pixel;

#define NUM_PIXELS 100
extern Pixel Pixels[NUM_PIXELS];
