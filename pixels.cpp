#include "Globals.h"
void pixels () {
  if(firstFrame){
    msPerFrame = 25;
  }
  FastLED.setBrightness(beatsin16(20, 32, 64));
  //FastLED.setBrightness(beatsin16(16, 32, 240));
  
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue,       255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + 128, 255, 255));
  //glowColor += 2;


  // low
  memmove8(&bufferLow[1], &bufferLow[0], ((NUM_LEDS*3)-1) * sizeof(CRGB));
  bufferLow[0] = CRGB::Black;
  int rand1 = random(  0,  24);
  if(rand1 == 0){
    bufferLow[0] = CHSV( gHue, 255, 255);
  }
  int8_t offsetLow = beatsin8(16, 0, NUM_LEDS * 2);
  memmove8(&leds[0], &bufferLow[offsetLow], (NUM_LEDS/2)   * sizeof(CRGB));
  
  // high
  memmove8(&bufferHigh[1], &bufferHigh[0], ((NUM_LEDS*3)-1) * sizeof(CRGB));
  bufferHigh[0] = CRGB::Black;
  rand1 = random(  0,  24);
  if(rand1 == 0){
    bufferHigh[0] = CHSV( gHue + 128, 255, 255);
  }
  int8_t offsetHigh = beatsin8(16, 0, NUM_LEDS * 2);
  memmove8(&leds[NUM_LEDS/2], &bufferHigh[offsetHigh], (NUM_LEDS/2)   * sizeof(CRGB));
}
