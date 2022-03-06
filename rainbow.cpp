#include "Globals.h"

void rainbow() {
  for(int i = 0; i < 8; i++){
    unsigned int color = map(i, 0, 8, 224, 0);
    //if(state == 1 || state == 4){
    if(true){
      fill_solid (&leds[ NUM_LEDS-1-(8*8)+(i*8)], 8, CHSV(  color, 255, 255));
    }else{
      fill_solid (&leds[ NUM_LEDS-1-(8*8)+(i*8)], 8, CHSV(  color, 255, 255));
    }
  }
  /*
  if(state == 1 || state == 4){
    fill_solid(&leds[ 0], NUM_LEDS, CHSV( 0, 0, 0));
  }else{
    fill_solid(&leds[71], NUM_LEDS, CHSV( 0, 0, 0));
  }
  */
}
