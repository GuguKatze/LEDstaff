#include "Globals.h"
void idleFluorescentTube() {
 if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);
  }
  FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.75, MAX_BRIGHTNESS));
  fadeToBlackBy(     leds,           NUM_LEDS,  8);
  fadeToBlackBy(secondary, NUM_LEDS_SECONDARY, 24);
  
  if(random(0,  beatsin8(8, 16, 256)) == 1){
    //fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( random(  56, 72), 176, 255));
    fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( random( 200, 220), 176, 255));
    //if(random(0, 10) == 1){ int rand5 = random(0, NUM_LEDS); leds[rand5] = CHSV( random(  56,  72), 176, 255); }
    if(random(0, 10) == 1){ int rand5 = random(0, NUM_LEDS); leds[rand5] = CHSV( random(  200,  220), 176, 255); }
  }
}
