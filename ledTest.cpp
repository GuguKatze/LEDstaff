#include "Globals.h"
float scroll_ = 72.0f;

void ledTest() {
  if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);
  }
  fadeToBlackBy(leds, NUM_LEDS / 2, 8);
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue,       255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + 128, 255, 255));
  uint8_t i = beatsin16(4, 0, 71);
  leds[i] =  CHSV( gHue, 255, 255);
  
  
  //leds[i + 72] =  CHSV( gHue + 128, 255, 255);

  //Serial.println(String(f));
  fill_solid (&leds[NUM_LEDS / 2], NUM_LEDS / 2, CRGB::Black);
  DrawPixels(scroll_, 3.0f, CHSV( gHue + 128, 255, 255));
  scroll_ += 0.1f;
  if (scroll_ > 138.0f){ scroll_ = 72.0f; }
  
  
  if(fCount % 10 == 0){
    gHue++;
  }
}
