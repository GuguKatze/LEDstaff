#include "Globals.h"
unsigned long nextStrobo = 0;
unsigned long nextClear = 0;
void strobo() {
  FastLED.setBrightness(MAX_BRIGHTNESS);
  if(millis() > nextStrobo){
    fill_solid(&leds[0],      NUM_LEDS,           CHSV( gHue, 255, 255));
    fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( gHue, 255, 255));
    nextStrobo = millis() + 250;
    nextClear = millis() + 20;
  }
  if(millis() > nextClear){
    FastLED.clear();
  }

}
