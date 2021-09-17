#include "Globals.h"
void idlePulsating() {
 if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);
  }
  FastLED.setBrightness(beatsin16(12, 16, MAX_BRIGHTNESS));
  fadeToBlackBy(leds, NUM_LEDS, 8);
  fill_solid (&secondary[0], NUM_LEDS_SECONDARY, CHSV( gHue, 255, 255));
  uint8_t led = beatsin16(12, 0, 143);
  leds[led] = CHSV( gHue, 255, 255);
  if(fCount % 20 == 0){
    gHue++;
  }
}
