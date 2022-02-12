#include "Globals.h"

CRGBPalette16 currentPalette__(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);

void fillnoise8() {
 
  #define scale 30                                                          // Don't change this programmatically or everything shakes.
  
  for(int i = 0; i < NUM_LEDS; i++) {                                       // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*scale, millis()/10+i*scale);                   // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette__, index, 255, LINEARBLEND);    // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
 
} // fillnoise8()

void effectTest () {
  if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
  }
  ///////////
  // frame //
  ///////////
  fillnoise8();  
  memmove8(&secondary[0]                   , &leds[0]           , (NUM_LEDS_SECONDARY/2) * sizeof(CRGB));
  memmove8(&secondary[NUM_LEDS_SECONDARY/2], &leds[NUM_LEDS / 2], (NUM_LEDS_SECONDARY/2) * sizeof(CRGB));
  
 
  EVERY_N_MILLIS(200) {
    nblendPaletteTowardPalette(currentPalette__, targetPalette, 128);          // Blend towards the target palette over 48 iterations.
  }
 
  EVERY_N_SECONDS(2) {                                                      // Change the target palette to a random one every 5 seconds.
    uint8_t baseC=random8();
    targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 255, random8(  0, 255)),   // Create palettes with similar colours.
                                  CHSV(baseC+random8(64), 255, random8(  0,  64)),
                                  CHSV(baseC+random8(96), 192, random8(  0,  64)),
                                  CHSV(baseC+random8(16), 255, random8(  0, 255)));
  }
}
