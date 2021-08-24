#include "Globals.h"
unsigned long nextStroboLow  = 0;
unsigned long nextStroboHigh = 0;
unsigned long nextClearLow  = 0;
unsigned long nextClearHigh = 0;

void strobo() {
  if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
  }
  gHue++;
  if(millis() > nextStroboLow){
    fill_solid(&leds[0],                           NUM_LEDS / 2,           CHSV( gHue,       255, 255));
    fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue,       255, 255));
    nextStroboLow = millis()  +  beatsin16( 20, 100, 200);
    nextClearLow  =  millis() +  30;
  }
  if(millis() > nextStroboHigh){
    fill_solid(&leds[NUM_LEDS / 2],                NUM_LEDS / 2,           CHSV( gHue + 128, 255, 255));
    fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + 128, 255, 255));
    nextStroboHigh = millis()  +  beatsin16(40, 100, 200);
    nextClearHigh  =  millis() +  30;
  }
  if(millis() > nextClearLow){
    //FastLED.clear();
    fill_solid(&leds[0],                           NUM_LEDS / 2,           CRGB::Black);
    fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CRGB::Black);
  }
  if(millis() > nextClearHigh){
    //FastLED.clear();
    fill_solid(&leds[NUM_LEDS / 2],                NUM_LEDS / 2,           CRGB::Black);
    fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CRGB::Black);
  }
}
