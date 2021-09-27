#include "Globals.h"

DEFINE_GRADIENT_PALETTE( fierce_ice_gp ) {
    0,   0,  0,  0,
   59,   0,  7, 45,
  119,   0, 33,255,
  149,   3, 82,255,
  180,  23,156,255,
  217, 100,203,255,
  255, 255,255,255};

// Gradient palette "Blue_Magenta_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Blue_Magenta_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Blue_Magenta_Yellow_gp ) {
    0,   0,  0,255,
   63,  42,  0,255,
  127, 255,  0,255,
  191, 255, 55, 45,
  255, 255,255,  0};



CRGBPalette16 lavaPalette = fierce_ice_gp;

uint16_t brightnessScale = 5;
uint16_t indexScale = 5;

void effectIce(){
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = inoise8(i * brightnessScale, millis() / 10);
    uint8_t index = inoise8(i * indexScale, millis() / 8);
    bufferBig[i] = ColorFromPalette(lavaPalette, index, brightness);
    //leds[i] = CHSV(0, 255, brightness);
  }
  buffer2leds(0, true);
  memmove8(&secondary[0]                     , &leds[0]           , NUM_LEDS_SECONDARY / 2 * sizeof(CRGB));
  memmove8(&secondary[NUM_LEDS_SECONDARY / 2], &leds[NUM_LEDS / 2], NUM_LEDS_SECONDARY / 2 * sizeof(CRGB));
  fill_solid (&secondary[0], NUM_LEDS_SECONDARY, CRGB::White);
}
