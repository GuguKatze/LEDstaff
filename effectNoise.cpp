#include "Globals.h"

float mapfloat__(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void effectNoise () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }
 
  ///////////
  // frame //
  ///////////
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( beatsin8(2, 64, 96), 255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( beatsin8(2, 64, 96), 255, 255));
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 24);
 
  uint16_t x = 0;
  uint16_t t = millis();
  int scale = 10;
  //for(uint8_t i = 0; i < NUM_LEDS ; i ++){
  //  uint8_t pos = inoise8(i * scale + x, t);
  //  pos = map(pos, 0, 255, 0, 143);
  //  leds[pos] = CRGB::Orange;
  //}

  //uint16_t pos = inoise16(0, t);
  for(uint8_t i = 0; i < 3; i ++){
    float pos = inoise16(i * 10000, t, t); // These functions are scaled to return 0-65535 
    pos = constrain(pos, 13000, 51000);
    pos = mapfloat__(pos, 13000, 51000, 0, NUM_LEDS - 1);
    Serial.println(String(pos));
    DrawPixels(NUM_LEDS + pos, 3.0f, CHSV( beatsin8(2, 64, 96), 255, 255));
  }
  buffer2leds(NUM_LEDS, true);
}
