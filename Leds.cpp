#include "Globals.h"

// The snytax for memmove8 is:
// memmove8( &destination[start position], &source[start position], size of pixel data )
//
// The above is written as "size of pixel data" instead of "number
// of pixels" because in this case it would be the number of
// pixels x 3.  Three because our current CRGB has data for three
// color channels: RGB.
// In code we can write that as:  pixels * sizeof(CRGB).

// mirror 0->71 to 72->143
void mirror() {
  memmove8(     &leds[72],      &leds[0], 72 * sizeof(CRGB));
  memmove8(&secondary[ 3], &secondary[0],  3 * sizeof(CRGB));
}
// flip 0->71 to 71->0
void flip() {
  memmove8(&ledsTmpLow[0], &leds[0], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &ledsTmpLow[i], sizeof(CRGB));
  }
}

uint8_t iH(uint8_t index){ // indexHelper
 if(index <= 71){
   return 71 - index;
 }
 return index;
}

int findUnused() {
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (Pixels[i].used){ continue; }
    return i;
  }
  return -1;
}

void buffer2leds(unsigned int bufferOffset){
  memmove8(  &leds[NUM_LEDS/2], &bufferBig[(NUM_LEDS/2)+bufferOffset], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i + bufferOffset], sizeof(CRGB));
  }
}

void ledsTmp2leds(){
  memmove8(  &leds[NUM_LEDS/2], &ledsTmp[NUM_LEDS/2], NUM_LEDS/2 * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &ledsTmp[i], sizeof(CRGB));
  }
}
