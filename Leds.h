#pragma once
#include <Arduino.h>
#define NUM_LEDS             144
#define NUM_LEDS_SECONDARY     6

#define BRIGHTNESS            56
#define MAX_BRIGHTNESS       216 // absolute PWM

#define MAX_POWER_MILLIAMPS 1500
#define MAX_POWER_VOLTS        6
//#define FRAMES_PER_SECOND 20

//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERNAL
#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
FASTLED_USING_NAMESPACE

// effects
void blendwave();
void beatwave();
void beatwaveSetup();
void blur();
void dot_beat();
void lightning();
void plasma();
void plasmaWrapper();

void secondaryL();


void binaryCounter();
void blendWave();
void rainbow();

void neontube();
void strobo();
void pixels();
void hearts();
void blocks();
void police();
void vumeter();
void fire();
