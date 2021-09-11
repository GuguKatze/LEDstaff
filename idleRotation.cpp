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
    
    secondary[(rotation_   ) % 3] = CHSV( gHue      , 255, 255);
    secondary[(rotation_ +1) % 3] = CHSV( gHue +  85, 255, 255);
    secondary[(rotation_ +2) % 3] = CHSV( gHue + 170, 255, 255);
    //secondary[(rotation_ +2) % 3] = CRGB::White;
    
    //secondary[(rotation_    ) % 3 + 3] = CRGB::White;
    //secondary[(rotation_ + 1) % 3 + 3] = CRGB::Fuchsia;
    //secondary[(rotation_ + 2) % 3 + 3] = CRGB::Purple;

    secondary[(rotation_    ) % 3 + 3] = CHSV( gHue      , 255, 255);
    secondary[(rotation_ + 1) % 3 + 3] = CHSV( gHue +  85, 255, 255);
    secondary[(rotation_ + 2) % 3 + 3] = CHSV( gHue + 170, 255, 255);
    //secondary[(rotation_ + 2) % 3 + 3] = CRGB::White;

    if(fCount % 2 == 0){
      gHue++;
    }
 
  if(millis() - lastRotation > 1000){
    lastRotation = millis();
    rotation_++;
    if(rotation_ > 2){
      rotation_ = 0;
    }
  }
}
