#include "Globals.h"

DEFINE_GRADIENT_PALETTE( BlacK_Green_gp ) {
    0,   0,  0,  0,
  127,   0, 55,  0,
  255,   0,255,  0};


CRGBPalette16 metaBalls_p = BlacK_Green_gp;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void effectMetaballs() {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 20;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }
  ///////////
  // frame //
  ///////////
  FastLED.setBrightness(beatsin8(2, 0.5 * MAX_BRIGHTNESS, 0.75 * MAX_BRIGHTNESS));
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( beatsin8(2, 64, 96), 255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( beatsin8(2, 64, 96), 255, 255));
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, beatsin16(12, 8, 8));
  //fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  /*
  //uint8_t b1 = beatsin8(2, 0, NUM_LEDS-1, 0, 0);
  //uint8_t b2 = beatsin8(4, 0, NUM_LEDS-1, 0, 32);
  //uint8_t b3 = beatsin8(6, 0, NUM_LEDS-1, 0, 64);
  int16_t b1 = beatsin8(2, 0, NUM_LEDS-1, 0, 0);
  int16_t b2 = beatsin8(4, 0, NUM_LEDS-1, 0, 32);
  int16_t b3 = beatsin8(6, 0, NUM_LEDS-1, 0, 64);

  //for(uint8_t i = 0; i < NUM_LEDS; i ++){
  for(int16_t i = 0; i < NUM_LEDS; i ++){
    byte  sum = abs(i - b1);
    sum = sum + abs(i - b2);
    //sum = sum + abs(i - b3);
    bufferBig[i] =  ColorFromPalette(HeatColors_p, constrain(sum * 8 + 200, 0, 255), 255);
  }
  */
   float speed = 1;

  uint16_t t = millis();
  float pos = 0;

  pos = inoise16(t, t * 2, t); // These functions are scaled to return 0-65535 
  pos = constrain(pos, 13000, 51000);
  pos = mapfloat(pos, 13000, 51000, 0, NUM_LEDS - 1);
  uint8_t x1 = pos;

  pos = inoise16(t + 10000, 3 * t + 10000, t * 2 + 10000); // These functions are scaled to return 0-65535 
  pos = constrain(pos, 13000, 51000);
  pos = mapfloat(pos, 13000, 51000, 0, NUM_LEDS - 1);
  uint8_t x2 = pos;
  
  pos = inoise16(t + 20000, 4 * t + 20000, t + 20000); // These functions are scaled to return 0-65535 
  pos = constrain(pos, 13000, 51000);
  pos = mapfloat(pos, 13000, 51000, 0, NUM_LEDS - 1);
  uint8_t x3 = pos;

  //pos = inoise16(t + 30000, t * 4 + 30000, t + 30000); // These functions are scaled to return 0-65535 
  //pos = constrain(pos, 13000, 51000);
  //pos = mapfloat(pos, 13000, 51000, 0, NUM_LEDS - 1);
  //uint8_t x4 = pos;

  uint8_t x4 = beatsin8(1, 3, 140) - beatsin8(1, 0, 8) + 4;
  

    for (uint8_t x = 0; x < NUM_LEDS; x++) {
      float sum = 0;
      // calculate distances of the 3 points from actual pixel
      // and add them together with weightening
      uint16_t dist = 0;
      dist = max(1, abs(x - x1));
      sum += NUM_LEDS * 1 / dist;
      dist = max(1, abs(x - x2));
      sum += NUM_LEDS * 2 / dist;
      dist = max(1, abs(x - x3));
      sum += NUM_LEDS * 3 / dist;
      dist = max(1, abs(x - x4));
      sum += NUM_LEDS * 1 / dist;
      //sum = sqrt(sum);
      sum + 50;
     

      //sum *= ;
      // inverse result
      uint8_t color     = constrain(sum, 64, 96);
      uint8_t brightness = constrain(sum, 32, 255);
      
      if(color <= 64){
        bufferBig[x] = CRGB::Black;
      }else{
        //bufferBig[x] = CHSV(color + random(0, 8), 255 - random(0, 8), brightness - random(0, 8));
        bufferBig[x] = CHSV(color - random(0, 8) + 4, 255 - random(0, 8), brightness);
      }
     
        // show the 3 points, too
        //bufferBig[x1] = CRGB(255, 255,255);
        //bufferBig[x2] = CRGB(255, 255,255);
        //bufferBig[x3] = CRGB(255, 255,255);
 
  }
  blur1d(bufferBig, NUM_LEDS * 3, 32);
  blur1d(bufferBig, NUM_LEDS * 3, 32);
  buffer2leds(0, true);
  
}
