#include "Globals.h"

bool up = true;
uint8_t offset = 0;

unsigned short bitsToUse = 8;
unsigned short bitWidth = 9;

//unsigned short bitsToUse = 12;
//unsigned short bitWidth = 6;

unsigned long lastBinaryCounter = 0;

void effectBinaryCounter() {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
    lastBinaryCounter = 0;
  }
  //FastLED.clear();
  if(millis() - lastBinaryCounter > 50){
    lastBinaryCounter = millis();
    fill_solid (&bufferBig[0], NUM_LEDS*3, CHSV(  0, 0, 0));
    int long seconds = millis() / 1000;
    //seconds = random(255);
    for(int i = 0; i < bitsToUse; i++){
      unsigned int color = map(i, 0, bitsToUse, 224, 0);
      //unsigned int color = map(i, 0, bitsToUse, gHue, gHue+64);
      byte bit = bitRead(seconds, i);
      if(bit == 0){
        bufferBig[NUM_LEDS + (i*bitWidth)] = CHSV( color, 255, 128);
        bufferBig[NUM_LEDS + (i*bitWidth) + bitWidth - 1] = CHSV( color, 255, 128);
      }else{
        fill_solid (&bufferBig[NUM_LEDS + (i*bitWidth)], bitWidth, CHSV(  color, 255, 255));
      }
    }
    /*
    buffer2leds((NUM_LEDS/2) - offset);
    if(up){
      offset++;
    }else{
      offset--;
    }
    if(offset >= 72 || offset <= 0){ up = !up; }
    */
  }
  uint8_t overshoot = 36;

  //uint8_t index = beatsin16(6, NUM_LEDS/2, NUM_LEDS);
  //b2l(index             ,          0, NUM_LEDS/2, true);  //  0 ->  71
  //b2l(index + NUM_LEDS/2, NUM_LEDS/2, NUM_LEDS/2, false); // 72 -> 143

  uint8_t index = beatsin16(1,       NUM_LEDS / 4, NUM_LEDS);
  b2l(index           ,            0, NUM_LEDS / 2, true);  //  0 ->  71
  b2l(index + NUM_LEDS * 0.75, NUM_LEDS / 2, NUM_LEDS / 2, false); // 72 -> 143
  
  bar2handle();
  gHue+=1;
}


//              ### ### # ### ## #### 

// 71    0 ===  72    143
