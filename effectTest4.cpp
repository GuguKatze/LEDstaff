#include "Globals.h"

void effectTest4 () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 20;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }
  ///////////
  // frame //
  ///////////
  fill_solid( &secondary[0], NUM_LEDS_SECONDARY, CHSV(gHue + random(0, 21), 255, 255) );
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 8);
  DrawPixels(random(0, 168) + random(0,1) / 10.0f, 4 ,  CHSV(gHue + random(0, 21), 255, 255));
  buffer2leds(0, true);
  //if(fCount % 10 == 0){
    gHue++;
  //}
}
