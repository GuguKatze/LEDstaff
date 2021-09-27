#include "Globals.h"
#define COOLING   70
#define SPARKING  55
//CRGBPalette16 gPal = HeatColors_p;
bool gReverseDirection = false;

// Gradient palette "pink_feathers_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/faun/tn/pink-feathers.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( pink_feathers_gp ) {
    0,   0,  0,  0,
   16,   1,  0,  1,
   32,   7,  0,  1,
   51,  20,  0,  3,
   70,  40,  0,  7,
   96,  82,  0, 15,
  122, 142,  0, 25,
  188, 194, 55,103,
  255, 255,255,255};

// Gradient palette "fire_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/fire.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( fire_gp ) {
    0,   1,  1,  0,
   76,  32,  5,  0,
  146, 192, 24,  0,
  197, 220,105,  5,
  240, 252,255, 31,
  250, 252,255,111,
  255, 255,255,255};


CRGBPalette16 gPal = fire_gp;

void effectFire(){
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 20;
    fill_solid (&leds[0], NUM_LEDS, CRGB::Black);

    // initialize Pixels
    for(int i = 0; i < NUM_PIXELS; i++){
      Pixels[i].used = false;
      Pixels[i].ledPos = 72;
      Pixels[i].gravity = 0.9;
      Pixels[i].velocity = 0;
      Pixels[i].startTime = 0;
      Pixels[i].pixelData = CRGB::Black;
    }
  }
  FastLED.setBrightness(random(MAX_BRIGHTNESS * .5, MAX_BRIGHTNESS * .75));
  random16_add_entropy( random());

  //CRGB darkcolor  = CHSV(gHue,255,192); // pure hue, three-quarters brightness
  //CRGB lightcolor = CHSV(gHue,128,255); // half 'whitened', full brightness
  //gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
  //gHue++;
  
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS / 2];

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS / 2; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / (NUM_LEDS / 2)) + 2));
  } 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS / 2 - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160,255) );
  }
  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS / 2; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    uint8_t colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if( gReverseDirection ) {
      pixelnumber = (NUM_LEDS/2-1) - j;
    } else {
      pixelnumber = j;
    }
    //leds[pixelnumber     ] = color;
     bufferBig[pixelnumber + 72] = color;
  }
  /////
  if(random(0, beatsin16(12, 128, 512)) == 0){
    int unused = findUnused();
    if(unused != -1){
      Pixels[unused].used = true;
      Pixels[unused].ledPos = 71;
      Pixels[unused].gravity = random(1600, 1800) / 1000.0;
      Pixels[unused].velocity = beatsin16(8, 10, 20) - 20;
      Pixels[unused].startTime = millis();
      Pixels[unused].pixelData = CHSV(gHue + random(8, 32), 255, 255);
    }
  }
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (!Pixels[i].used){ continue; }
    int ledPos = (int) Pixels[i].ledPos;
    bufferBig[ledPos] = Pixels[i].pixelData;
    Pixels[i].pixelData.fadeToBlackBy(10); 
    Pixels[i].ledPos += Pixels[i].velocity / 100;
    Pixels[i].velocity -= Pixels[i].gravity;
    if(ledPos > 215 || ledPos <= 0){
      Pixels[i].used = false;
    }
  }
  /////
  fadeToBlackBy(bufferBig, 72, 128);
  buffer2leds(0, true);
  memmove8(&secondary[NUM_LEDS_SECONDARY/2], &leds[NUM_LEDS / 2], (NUM_LEDS_SECONDARY/2) * sizeof(CRGB));
  memmove8(&secondary[0]                   , &leds[NUM_LEDS / 2], (NUM_LEDS_SECONDARY/2) * sizeof(CRGB)); // leds upper -> secondary lower!!!
}
