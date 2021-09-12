#include "Globals.h"
void idleFluorescentTube() {
 if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);
  }
  fadeToBlackBy(     leds,           NUM_LEDS,  4);
  fadeToBlackBy(secondary, NUM_LEDS_SECONDARY, 24);
  
  if(random(0,  beatsin8(16, 24, 256)) == 1){
    FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.75, MAX_BRIGHTNESS));
    //fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( random(  56, 72), 176, 255));
    //fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( random( 200, 220), 176, 255));
    fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( random( gHue, gHue + 32), 255, 255));
    //if(random(0, 10) == 1){ int rand5 = random(0, NUM_LEDS); leds[rand5] = CHSV( random(  56,  72), 176, 255); }
    //if(random(0, 10) == 1){ int rand5 = random(0, NUM_LEDS); leds[rand5] = CHSV( random(  200,  220), 176, 255); }
    
    //if(random(0, 10) == 1){
      uint8_t count = random(1, 25);
      for(uint8_t i = 0; i < count; i++){
        int index = random(0, NUM_LEDS);
        //leds[index] = CHSV( random(  gHue,  gHue + 64), 255, 255);
      }
    //}
  }
  if(fCount % 60 == 0){
    gHue++;
  }
}
