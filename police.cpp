#include "Globals.h"
void police() {
  msPerFrame = 0;
  FastLED.setBrightness(MAX_BRIGHTNESS);
 
  fill_solid(&leds[NUM_LEDS/2],                NUM_LEDS/2,           CRGB(18, 0, 224));
  fill_solid(&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CRGB::Blue);
  FastLED.show();
  delay(10);
  fill_solid(&leds[NUM_LEDS/2],                NUM_LEDS/2,           CRGB::Black);
  fill_solid(&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CRGB::Blue);
  FastLED.show();
  delay(120);
  
  fill_solid(&leds[NUM_LEDS/2],                NUM_LEDS/2,           CRGB(37, 41, 225));
  fill_solid(&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CRGB::Blue);
  FastLED.show();
  delay(10);
  fill_solid(&leds[NUM_LEDS/2],                NUM_LEDS/2,           CRGB::Black);
  fill_solid(&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CRGB::Blue);
  FastLED.show();
  delay(120);
  

  fill_solid(&leds[NUM_LEDS/2],                NUM_LEDS/2,           CRGB(246, 248, 247));
  fill_solid(&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CRGB::Blue);
  FastLED.show();
  delay(10);
  fill_solid(&leds[NUM_LEDS/2],                NUM_LEDS/2,           CRGB::Black);
  fill_solid(&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CRGB::Black);
  FastLED.show();
  delay(120);
  
  fill_solid(&leds[0],      NUM_LEDS/2,           CRGB(246, 248, 247));
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY/2, CRGB::Red);
  FastLED.show();
  delay(10);
  fill_solid(&leds[0],      NUM_LEDS/2,           CRGB::Black);
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY/2, CRGB::Red);
  FastLED.show();
  delay(120);
   
  fill_solid(&leds[0],      NUM_LEDS/2,           CRGB(255, 48, 56));
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY/2, CRGB::Red);
  FastLED.show();
  delay(10);
  fill_solid(&leds[0],      NUM_LEDS/2,           CRGB::Black);
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY/2, CRGB::Red);
  FastLED.show();
  delay(120);
  
  fill_solid(&leds[0],      NUM_LEDS/2,           CRGB(227, 15, 25));
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY/2, CRGB::Red);
  FastLED.show();
  delay(10);
  fill_solid(&leds[0],      NUM_LEDS/2,           CRGB::Black);
  fill_solid(&secondary[0], NUM_LEDS_SECONDARY/2, CRGB::Black);
  FastLED.show();
  delay(120);
  
}
