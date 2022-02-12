#include "Globals.h"
void effectAirplane() {
 if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
  }
  fadeToBlackBy(leds, NUM_LEDS, 48);
  if(fCount % 400 == 0 || fCount % 400 == 10){
    fill_solid (&leds[NUM_LEDS/2-10], 10, CHSV(  0, 255, 255));
  }
  if(fCount % 400 == 60 || fCount % 400 == 70){
    fill_solid (&leds[NUM_LEDS-10]  , 10, CHSV( 96, 255, 255));
  }
  
}
