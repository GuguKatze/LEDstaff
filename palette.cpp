#include "Globals.h"
CRGBPalette256 currentPalette;
CHSV hsv;
CRGB rgb;

void newGradientPalette(){
  uint8_t xyz[16];  // Needs to be 4 times however many colors are being used.
                    // 3 colors = 12, 4 colors = 16, etc.

  hsv = CHSV(random8(), 255, 255);  // pick random hue
  hsv2rgb_rainbow( hsv, rgb);  // convert to R,G,B
  
  xyz[0] = 0;  // anchor of first color - must be zero
  xyz[1] = rgb.r;
  xyz[2] = rgb.g;
  xyz[3] = rgb.b;

  xyz[4] = random8(48,118);  // random anchor point for second color
  xyz[5] = 255;
  xyz[6] = 0;
  xyz[7] = 0;
  
  xyz[8] = random8(138,225);  // random anchor point for third color
  xyz[9] = 0,
  xyz[10] = 255;
  xyz[11] = 0;

  hsv = CHSV(random8(), 255, 255);  // pick random hue
  hsv2rgb_rainbow( hsv, rgb);  // convert to R,G,B

  xyz[12] = 255;  // anchor of last color - must be 255
  xyz[13] = rgb.r;
  xyz[14] = rgb.g;
  xyz[15] = rgb.b;

  currentPalette = xyz;

  //for (int p = 0; p < NUM_LEDS; p++) {
  //  leds[p] = ColorFromPalette(currentPalette, p*255/(NUM_LEDS-1), BRIGHTNESS, NOBLEND);
  //}
  fill_palette(ledsTmp, NUM_LEDS, 0, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  ledsTmp2leds();
}

void palette() {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 1000;
  }
  newGradientPalette();
}
