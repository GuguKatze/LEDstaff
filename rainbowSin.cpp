#include "Globals.h"

void rainbowSin () {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS/2);
    msPerFrame = 10;
  }
  //fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 80);
  
  for(int i=0;i<8;i++){   
    uint8_t color   = map( i, 0, 8, 224, 0);
    bufferBig[beatsin16(16, 0, 143, 0, i * 2048)] =  CHSV(color ,255,255);
  }
  b2l( 0,  0, NUM_LEDS/2, true); //  0 ->  71
  b2l(72, 72, NUM_LEDS/2, false); // 72 -> 143
  bar2handle();
  gHue+=2;
}
