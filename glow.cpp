#include "Globals.h"
uint8_t glowColor = 22;
bool upwards = true;
void glow () {
  msPerFrame = 250;
  FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.75, MAX_BRIGHTNESS));
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY, CHSV( glowColor, 255, 255));
 if(glowColor < 74 && upwards == true){
    glowColor ++;
  }else if(glowColor > 22 && upwards == false){
    glowColor --;
  }
  if(glowColor == 74 && upwards == true){
    upwards = false;
  }else if(glowColor == 22 && upwards == false){
    upwards = true;
  }
  //FastLED.setBrightness(10); 
  //leds[20] = CHSV( 120, 255, 255);
}
