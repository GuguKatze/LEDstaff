#include "Globals.h"
unsigned long lastRotation = 0;
uint8_t rotation_ = 0;
void idleRotation() {
 if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);
 
  }
  
    fill_solid (&secondary[0], NUM_LEDS_SECONDARY, CRGB::Black);

    //secondary[(rotation_   ) % 3] = CRGB::White;
    
    secondary[(rotation_    ) % (NUM_LEDS_SECONDARY / 2)] = CHSV( gHue      , 255, 255);
    secondary[(rotation_ + 1) % (NUM_LEDS_SECONDARY / 2)] = CHSV( gHue +  32, 255, 255);
    //secondary[(rotation_ +2) % (NUM_LEDS_SECONDARY / 2)] = CHSV( gHue +  32, 255, 255);

    secondary[(rotation_    ) % (NUM_LEDS_SECONDARY / 2) + NUM_LEDS_SECONDARY / 2] = CHSV( gHue      , 255, 255);
    secondary[(rotation_ + 1) % (NUM_LEDS_SECONDARY / 2) + NUM_LEDS_SECONDARY / 2] = CHSV( gHue +  32, 255, 255);
    //secondary[(rotation_ + 2) % (NUM_LEDS_SECONDARY / 2) + NUM_LEDS_SECONDARY / 2] = CHSV( gHue +  32, 255, 255);

    if(fCount % 20 == 0){
      gHue++;
    }
 
  if(millis() - lastRotation > 250){
    lastRotation = millis();
    rotation_++;
    if(rotation_ > NUM_LEDS_SECONDARY / 2 - 2){
      rotation_ = 0;
    }
  }
}
