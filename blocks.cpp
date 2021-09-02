#include "Globals.h"
void blocks () {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS/2);
    msPerFrame = 10;
  }
  fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  for(int i=0;i<7;i++){   
    uint8_t  color   = map( i, 0, 6, 224, 0);
    fill_solid(            &bufferBig[i * 10], 10 , CHSV( color, 255, 255));
  }
  b2l(0, 0,  NUM_LEDS/2, false);  //  0 ->  71
  b2l(0, 72, NUM_LEDS/2, false); // 72 -> 143
}
