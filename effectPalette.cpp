#include "Globals.h"
CRGBPalette256 currentPalette;
CHSV hsv;
CRGB rgb;
unsigned long lastPalette = 0;
// https://github.com/marmilicious/FastLED_examples/blob/master/palette_example2.ino

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
  //fill_palette(ledsTmp, NUM_LEDS, 0, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  //ledsTmp2leds();
}

void effectPalette() {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }
  if(millis() - lastPalette > 2000){
    lastPalette = millis();
    newGradientPalette();
  }
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 8);
  uint8_t index = beatsin16(12, 0, NUM_LEDS - 1);
  uint8_t color = map(index, 0, 143, 0, 255);
  bufferBig[index] = ColorFromPalette(currentPalette, color, 255);
  b2l(         0,          0, NUM_LEDS/2, true); //  0 ->  71
  b2l(NUM_LEDS/2, NUM_LEDS/2, NUM_LEDS/2, false); // 72 -> 143
}
