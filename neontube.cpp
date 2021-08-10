#include "Globals.h"
void neontube() {
  
  FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.25, MAX_BRIGHTNESS * 0.75));
  if(random(  0,  50) == 1){ fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV(                   0,   0,   0)); }
  if(random(  0,  20) == 1){ fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( random(  200,  220), 255, 255)); }
  fadeToBlackBy( leds, NUM_LEDS, 3);
  if(random(  0, 100) == 1){ int rand5 = random(0, NUM_LEDS); leds[rand5] = CHSV( random(  200,  220), 255, 255); }
}
