#include "Globals.h"
void blocks () {
  FastLED.setBrightness(32);
  int rand1 = random(  0,  50);
  int rand2 = random(  0, 255);
  int rand3 = random(  0, 143);
  int rand4 = random(  0, 143-rand3);
  int rand5 = random( 64, 255);
  int rand6 = random(  0, 255);
  //fadeToBlackBy( leds, NUM_LEDS, 3);
  if(rand1 == 0){
    //fill_solid(&leds[rand3], rand4, CHSV( rand2, rand5, rand6));
  }

  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 8, 0, NUM_LEDS-1 );
  if(pos < 72){
    pos = 71 - pos;
  }
  leds[pos] += CHSV( gHue, 255, 192);


  int pos2 = beatsin16( 6, 0, NUM_LEDS-1, 1 );
  if(pos2 < 72){
    pos2 = 71 - pos2;
  }
  leds[pos2] += CHSV( gHue + 128, 255, 192);
  
  fill_solid(&secondary[0], 5, CHSV( gHue, 255, 192));
  //flip();
}
