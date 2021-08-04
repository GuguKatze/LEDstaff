#pragma once
#include "Globals.h"
#include <Arduino.h>

#define FASTLED_INTERNAL
#include <FastLED.h>
FASTLED_USING_NAMESPACE

typedef struct {
  unsigned int cols = 0;
  unsigned int rows = 0;
  CRGB imagedata[90][72] = {};
}Image;

Image loadImage();

extern void showImage();


// CRGB secondary[14];
// Copy ten led colors from leds[src .. src+9] to leds[dest .. dest+9]
// memmove( &leds[dest], &leds[src], 10 * sizeof( CRGB) );
