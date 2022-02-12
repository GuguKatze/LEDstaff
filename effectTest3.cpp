#include "Globals.h"

float pos = 0;
float pos2 = 0;
void effectTest3 () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 5;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }
  ///////////
  // frame //
  ///////////
  //fill_solid( &secondary[0], NUM_LEDS_SECONDARY, CRGB::White );
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 16);


  pos = 72 +  (beatsin16(6, 0, 750)         - 400) / 10.0f;
  pos      += (beatsin16(12, 0, 750, 0, 128) - 400) / 10.0f;

  pos2 = 72 +  (beatsin16(4, 0, 750)          - 400) / 10.0f;
  pos2      += (beatsin16(8, 0, 750, 0, 128) - 400) / 10.0f;
  
  //void DrawPixels(float fPos, float count, CRGB color)
  DrawPixels( pos, 10,  CHSV(gHue, 255, 255));
  DrawPixels(pos2, 10,  CHSV(gHue + 128, 255, 255));
  
  
  
  buffer2leds(0, true);
  if(fCount % 10 == 0){
    gHue++;
  }
}
