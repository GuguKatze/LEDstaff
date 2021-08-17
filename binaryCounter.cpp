#include "Globals.h"

unsigned short bitsToUse = 8;
unsigned short bitWidth = 9;
unsigned long nextBinaryCounter;

void binaryCounter() {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
  }
  //FastLED.clear();
  if(millis() > nextBinaryCounter){
    nextBinaryCounter = nextBinaryCounter + 250;
    int long seconds = millis() / 250;
    //seconds = random(255);
    for(int i = 0; i < bitsToUse; i++){
      unsigned int color = map(i, 0, bitsToUse, 224, 0);
      byte bit = bitRead(seconds, i);
      if(bit == 0){
        leds[73-bitsToUse+i] = CRGB::Black;
        fill_solid (&leds[ 71-(bitsToUse*bitWidth)+(i*bitWidth)], bitWidth, CHSV(  0, 0, 0));
        //fill_solid (&leds[143-(bitsToUse*bitWidth)+(i*bitWidth)], bitWidth, CHSV(  0, 0, 0));
      }else{
        //leds[73-bitsToUse+i] = CHSV(color, 255, 255);
        fill_solid (&leds[ 71-(bitsToUse*bitWidth)+(i*bitWidth)], bitWidth, CHSV(  color, 255, 255));
        //fill_solid (&leds[143-(bitsToUse*bitWidth)+(i*bitWidth)], bitWidth, CHSV(  color, 255, 255));
      }
    }
    //flip();
    mirror();
  }
}
