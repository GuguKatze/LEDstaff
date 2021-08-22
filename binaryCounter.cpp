#include "Globals.h"

bool up = true;
uint8_t offset = 0;

//unsigned short bitsToUse = 8;
//unsigned short bitWidth = 9;

unsigned short bitsToUse = 12;
unsigned short bitWidth = 6;

unsigned long nextBinaryCounter;

void binaryCounter() {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
  }
  //FastLED.clear();
  if(millis() > nextBinaryCounter){
    nextBinaryCounter = nextBinaryCounter + 1000;
    fill_solid (&bufferBig[0], NUM_LEDS*3, CHSV(  0, 0, 0));
    int long seconds = millis() / 1000;
    //seconds = random(255);
    for(int i = 0; i < bitsToUse; i++){
      unsigned int color = map(i, 0, bitsToUse, 224, 0);
      byte bit = bitRead(seconds, i);
      if(bit == 0){
        bufferBig[(NUM_LEDS/2) + (i*bitWidth)] = CHSV( color, 255, 128);
        bufferBig[(NUM_LEDS/2) + (i*bitWidth) + bitWidth - 1] = CHSV( color, 255, 128);
      }else{
        fill_solid (&bufferBig[(NUM_LEDS/2) + (i*bitWidth)], bitWidth, CHSV(  color, 255, 255));
      }
    }
    buffer2leds((NUM_LEDS/2) - offset);
    if(up){
      offset++;
    }else{
      offset--;
    }
    if(offset >= 72 || offset <= 0){ up = !up; }
  }
  //buffer2led(beatsin8(16, 0, (NUM_LEDS/2)));
  if(leds[0]){
    fill_solid (&secondary[0], NUM_LEDS_SECONDARY/2, leds[0]);
  }
  if(leds[NUM_LEDS/2]){
    fill_solid (&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, leds[NUM_LEDS/2]);
  }
}


//              ### ### # ### ## #### 

// 71    0 ===  72    143
