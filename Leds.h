#pragma once
#include <Arduino.h>
#define NUM_LEDS             144
//#define NUM_LEDS_SECONDARY     6
#define NUM_LEDS_SECONDARY    14

#define BRIGHTNESS            64
#define MAX_BRIGHTNESS       255 // absolute PWM

#define MAX_POWER_MILLIAMPS    750
#define MAX_POWER_VOLTS          5
#define MAX_POWER_MILLIWATTS  2000
//#define FRAMES_PER_SECOND 20

//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERNAL

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33
//   Teensy 4.0:  1, 8, 14, 17, 20, 24, 29, 39
//   Teensy 4.1:  1, 8, 14, 17, 20, 24, 29, 35, 47, 53

void effectNone();

void effectTest();
void effectTest2();
void effectTest3();
void effectTest4();
void effectNoise();
void effectMetaballs();
void effectHeat();

// idle
void ledTest();
void idleRotation();
void idleFluorescentTube();
void idlePulsating();

// effects
void effectFire();
void effectPixels();
void effectIce();
void effectBinaryCounter();
void effectPalette();
void effectLevel();
void effectVumeter();
void effectMatrix();

// rotation

void effectAirplane();
void blendwave();
void beatwave();
void beatwaveSetup();
void blur();
void dot_beat();
void lightning();
void plasma();
void plasmaWrapper();

void secondaryL();

void rainbowSin();

void blendWave();
void rainbow();


void neontube();
void strobo();
void hearts();
void blocks();
void police();


void fire();
void waterdrops();



void glow();
//void pixels();
void snowflakes();
void lavalamp();
