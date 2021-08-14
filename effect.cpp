#include "Globals.h"
class Effect {
  uint8_t glowColor = 22;
  public:
  void init() {
    msPerFrame = 50;
  }
  void loop(){
    FastLED.setBrightness(random(MAX_BRIGHTNESS * 0.75, MAX_BRIGHTNESS * 0.90));
    fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( glowColor,       255, 255));
    fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( glowColor + 128, 255, 255));
    glowColor += 2;
  }
};
