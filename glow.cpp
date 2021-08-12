#include "Globals.h"
uint8_t glowColor = 22;
bool upwards = true;
void glow () {
  msPerFrame = 60;
  FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.75, MAX_BRIGHTNESS * 0.90));
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( glowColor,       255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( glowColor + 128, 255, 255));
  //if(glowColor < 74 && upwards == true){
    //glowColor ++;
    glowColor += 2;
  //}else if(glowColor > 22 && upwards == false){
    //glowColor --;
  //}
  if(glowColor == 74 && upwards == true){
    upwards = false;
  }else if(glowColor == 22 && upwards == false){
    upwards = true;
  }
  // low
  memmove8(&ledsTmpLow[1], &ledsTmpLow[0], (NUM_LEDS/2-1) * sizeof(CRGB));
  ledsTmpLow[0] = CRGB::Black;
  int rand1 = random(  0,  6);
  if(rand1 == 0){
    ledsTmpLow[0] = CHSV( glowColor, 255, 64);
  }
  memmove8(&leds[0], &ledsTmpLow[0], (NUM_LEDS/2)   * sizeof(CRGB));
  // high
  memmove8(&ledsTmpHigh[1], &ledsTmpHigh[0], (NUM_LEDS/2-1) * sizeof(CRGB));
  ledsTmpHigh[0] = CRGB::Black;
  rand1 = random(  0,  6);
  if(rand1 == 0){
    ledsTmpHigh[0] = CHSV( glowColor + 128, 255, 64);
  }
  memmove8(&leds[NUM_LEDS/2], &ledsTmpHigh[0], (NUM_LEDS/2)   * sizeof(CRGB));
}
