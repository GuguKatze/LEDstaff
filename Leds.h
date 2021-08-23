#pragma once
#include <Arduino.h>
#define NUM_LEDS             144
#define NUM_LEDS_SECONDARY     6

#define BRIGHTNESS            40
#define MAX_BRIGHTNESS       240 // absolute PWM

#define MAX_POWER_MILLIAMPS  500
#define MAX_POWER_VOLTS        6
//#define FRAMES_PER_SECOND 20

//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERNAL

#include <WS2812Serial.h>
#define USE_WS2812SERIAL
// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33
//   Teensy 4.0:  1, 8, 14, 17, 20, 24, 29, 39
//   Teensy 4.1:  1, 8, 14, 17, 20, 24, 29, 35, 47, 53

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
void hearts();
void blocks();
void police();
void vumeter();
void vumeter2();
void fire();
void waterdrops();
void level();
void palette();

void glow();
//void pixels();
void pixels2();
void snowflakes();
