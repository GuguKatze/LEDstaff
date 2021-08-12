#include "Globals.h"
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  90
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

bool gReverseDirection = false;

CRGB foob( uint8_t temperature)
{
    CRGB heatcolor;

    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 191);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        // we're in the hottest third
        heatcolor.r = 0; // full red
        heatcolor.g = 255; // full green
        heatcolor.b = heatramp; // ramp up blue

    } else if( t192 & 0x40 ) {
        // we're in the middle third
        heatcolor.r = heatramp; // full red
        heatcolor.g = 0; // ramp up green
        heatcolor.b = 255; // no blue

    } else {
        // we're in the coolest third
        heatcolor.r = 0; // ramp up red
        heatcolor.g = 0; // no green
        heatcolor.b = heatramp; // no blue
    }

    return heatcolor;
}


void fire () {
  msPerFrame = 25;
  FastLED.setBrightness(24);
  // Array of temperature readings at each simulation cell
  static uint8_t heatLow[(NUM_LEDS / 2)];
  static uint8_t heatHigh[(NUM_LEDS / 2)];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < (NUM_LEDS / 2); i++) {
      heatLow[i] = qsub8( heatLow[i],  random8(0, ((COOLING * 10) / (NUM_LEDS / 2)) + 2));
      heatHigh[i] = qsub8( heatHigh[i],  random8(0, ((COOLING * 10) / (NUM_LEDS / 2)) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= (NUM_LEDS / 2) - 1; k >= 2; k--) {
      heatLow[k] = (heatLow[k - 1] + heatLow[k - 2] + heatLow[k - 2] ) / 3;
      heatHigh[k] = (heatHigh[k - 1] + heatHigh[k - 2] + heatHigh[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heatLow[y] = qadd8( heatLow[y], random8(160,255) );
    }
    if( random8() < SPARKING ) {
      int y = random8(7);
      heatHigh[y] = qadd8( heatHigh[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < (NUM_LEDS / 2); j++) {
      //CRGB colorLow = HeatColor( heatLow[j]);
      CRGB colorLow = foob( heatLow[j]);
      //CHSV colorLow = CHSV( heatLow[j], 255, 255);
      CRGB colorHigh = HeatColor( heatHigh[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = ((NUM_LEDS / 2) -1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = colorLow;
      leds[pixelnumber + (NUM_LEDS / 2)] = colorHigh;
    }
}
