#include "Globals.h"
void glow () {
  if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
  }
  if(gHue != 213){ gHue ++; }
  //FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.75, MAX_BRIGHTNESS * 0.90));
  FastLED.setBrightness(beatsin16(16, 128, 255));
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue + beatsin8(16, 0, 32) - 16, 255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + beatsin8(16, 0, 32, 255) - 16, 255, 255));
}
