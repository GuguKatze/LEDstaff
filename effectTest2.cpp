#include "Globals.h"

void effectTest2 () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 5;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }
  ///////////
  // frame //
  ///////////
  //fill_solid( &secondary[0], NUM_LEDS_SECONDARY, CRGB::White );
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 8);
  for(int i=0;i<NUM_LEDS;i++){
    if(i % beatsin16(20, 3, 10) == beatsin16(20, 1, 9)){
      bufferBig[NUM_LEDS + i] = CHSV(gHue + 128 + i * 2, 255, 255);
    }else{
      //bufferBig[NUM_LEDS + i] = CRGB::Black;
    }
  }
  buffer2leds(NUM_LEDS, true);
  //if(fCount % 10 == 0){
    gHue++;
  //}
}
