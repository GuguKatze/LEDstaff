#pragma once
#include "Leds.h"
//#include <Wire.h>
//#define SCL A3
//#define SDA A2

extern CRGB leds[NUM_LEDS];
extern CRGB ledsTmpLow[NUM_LEDS/2];
extern CRGB ledsTmpHigh[NUM_LEDS/2];
extern CRGB secondary[NUM_LEDS_SECONDARY];

extern bool useSerial;
extern bool ledsEnabled;

//extern bool mirror;
extern uint8_t gHue;
extern unsigned int msPerFrame;

extern unsigned long pixelNextUpdateTime;

extern unsigned int state;

void mirror();
void flip();

union vu_ {
  struct __attribute__((packed)){
    //uint16_t left[7];
    uint8_t left[7];
    //uint16_t right[7];
    uint8_t right[7];
  };
  //uint8_t bytes[28];
  uint8_t bytes[14];
};
extern union vu_ vu;