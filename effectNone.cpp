#include "Globals.h"
void effectNone () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 20;
    fill_solid (&bufferBig[0], NUM_LEDS * 3,       CRGB::Black);
    fill_solid(&secondary[0],  NUM_LEDS_SECONDARY, CRGB::Black);
    fill_solid(&leds[0],       NUM_LEDS,           CRGB::Black);
  }
  ///////////
  // frame //
  ///////////
  fadeToBlackBy(secondary, NUM_LEDS_SECONDARY, 16);
  fadeToBlackBy(leds,      NUM_LEDS,           16);
}
