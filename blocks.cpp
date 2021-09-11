#include "Globals.h"
//uint8_t i = 0;

unsigned short bitsToUse_ = 36;
unsigned short bitWidth_ = 2;

void blocks () {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS/2);
    msPerFrame = 10;
  }
  fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);

  uint8_t o = beatsin16(12, 0, 72);
  
  for(int i=0;i<bitsToUse_;i++){   
    //uint8_t color   = map( i, 0, bitsToUse_, 224, 0);
    uint8_t color   = map( i, 0, bitsToUse_, gHue, gHue + 128);
    unsigned int index = o + i * bitWidth_ + i * o * 0.2;
    if(index < 72){
      fill_solid(            &bufferBig[index], bitWidth_ , CHSV( color, 255, 255));
    }
      
    //fill_solid(            &bufferBig[i * 10], bitWidt_h , CHSV( color, 255, 255));
  }
  b2l(0, 0,  NUM_LEDS/2, true); //  0 ->  71
  b2l(0, 72, NUM_LEDS/2, true); // 72 -> 143
  bar2handle();
  gHue+=2;
}
