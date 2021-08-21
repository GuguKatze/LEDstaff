#include "Globals.h"

unsigned short bitsToUse = 8;
unsigned short bitWidth = 9;

//unsigned short bitsToUse = 12;
//unsigned short bitWidth = 6;

unsigned long nextBinaryCounter;

void binaryCounter() {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
  }
  //FastLED.clear();
  if(millis() > nextBinaryCounter){
    nextBinaryCounter = nextBinaryCounter + 1000;
    int long seconds = millis() / 1000;
    //seconds = random(255);
    for(int i = 0; i < bitsToUse; i++){
      unsigned int color = map(i, 0, bitsToUse, 224, 0);
      byte bit = bitRead(seconds, i);
      if(bit == 0){
        //fill_solid (&leds[i*bitWidth], bitWidth, CHSV(  0, 0, 0));
        fill_solid (&bufferBig[(NUM_LEDS/2) + (i*bitWidth)], bitWidth, CHSV(  0, 0, 0));
        bufferBig[(NUM_LEDS/2) + (i*bitWidth)] = CHSV( color, 255, 128);
        bufferBig[(NUM_LEDS/2) + (i*bitWidth) + bitWidth - 1] = CHSV( color, 255, 128);
      }else{
        //fill_solid (&leds[i*bitWidth], bitWidth, CHSV(  color, 255, 255));
        fill_solid (&bufferBig[(NUM_LEDS/2) + (i*bitWidth)], bitWidth, CHSV(  color, 255, 255));
      }
    }
    
    //flip();
    //mirror();
  }
  buffer2led(beatsin8(16, 0, (NUM_LEDS/2)));
  //memmove8(&leds[NUM_LEDS/2], &bufferBig[(NUM_LEDS/2)+beatsin8(32, 0, (NUM_LEDS/2))], (NUM_LEDS/2) * sizeof(CRGB));
  //for (uint8_t i=0; i<NUM_LEDS/2; i++){
  //  memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i + beatsin8(32, 0, (NUM_LEDS/2))], sizeof(CRGB));
  //}
  if(leds[0]){
    fill_solid (&secondary[0], NUM_LEDS_SECONDARY/2, leds[0]);
  }
  if(leds[NUM_LEDS/2]){
    fill_solid (&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, leds[NUM_LEDS/2]);
  }
}


//              ### ### # ### ## #### 

// 71    0 ===  72    143
