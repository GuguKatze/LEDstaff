#include "Globals.h"

// Gradient palette "bhw1_07_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE( pal1 ) {
    0, 232, 65,  1,
  255, 229,227,  1};

// Gradient palette "bhw1_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( pal2 ) {
    0, 227,101,  3,
  117, 194, 18, 19,
  255,  92,  8,192};

// Gradient palette "bhw1_minty_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_minty.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( pal3 ) {
    0,  23,195,130,
   99, 121,255,125,
  121, 182,255,184,
  127, 255,255,255,
  134, 182,255,184,
  153, 121,255,125,
  255,  23,195,130};

// Gradient palette "Green_Yellow_White_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Green_Yellow_White.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Green_Yellow_White_gp ) {
    0,   0,255,  0,
   63,  42,255,  0,
  127, 255,255,  0,
  191, 255,255, 45,
  255, 255,255,255};


CRGBPalette16 test_ = Green_Yellow_White_gp;

uint8_t whichPalette = 0;
CRGBPalette16 currentPalette_ = pal1;
CRGBPalette16 targetPalette_ = pal2;

void lavalamp () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  }

  uint8_t paletteOffset = beatsin8(8, 0, 128);
  for (int i = 0; i < 8; i++) {
    leds[40 + i] = ColorFromPalette(test_, paletteOffset + i * 16 + random(0, 21) - 10);
  } 
  
  for (int i = 0; i < 8; i++) {
    leds[20 + i] = ColorFromPalette(currentPalette_, i * 30);
  } 
  nblendPaletteTowardPalette(currentPalette_, targetPalette_, 10 );
  switch (whichPalette) {
    case 0:
      targetPalette_ = pal2;
      break;
    case 1:
      targetPalette_ = pal3;
      break;
     case 2:
      targetPalette_ = pal1;
      break;
  }
  EVERY_N_SECONDS(5) {
    whichPalette++;
    if (whichPalette > 2){
      whichPalette = 0;
    }
  }
}
