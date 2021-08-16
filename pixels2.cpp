#include "Globals.h"

typedef struct Pixel {
  bool used;
  float ledPos;
  float velocity;
  unsigned long startTime;
  CRGB pixelData;
} Pixel;

#define NUM_PIXELS 50
Pixel Pixels[NUM_PIXELS];

int findUnused() {
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    Serial.println("Testing index: " + String(i));
    if (!Pixels[i].used){
      return i;
    }
  }
  return -1;
}

void pixels2 () {
  if(firstFrame){
    msPerFrame = 10;
    // initialize Pixels
    for(int i = 0; i < NUM_PIXELS; i++){
      Pixels[i].used = false;
      Pixels[i].ledPos = 72;
      Pixels[i].velocity = 0;
      Pixels[i].startTime = 0;
      Pixels[i].pixelData = CRGB::Black;
    }
  }
  FastLED.setBrightness(beatsin16(20, 32, 64));
  EVERY_N_MILLISECONDS(40){ gHue++; }
  
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue,       255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + 128, 255, 255));
  //glowColor += 2;

  fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  if(random(0, beatsin8(12, 64, 1024)) == 0){
    int unused = findUnused();
    if(unused != -1){
      Serial.println("Using unused index: " + String(unused));
      Pixels[unused].used = true;
      Pixels[unused].ledPos = 72;
      Pixels[unused].velocity = random(24, 136);
      Pixels[unused].startTime = millis();
      Pixels[unused].pixelData = CHSV(gHue, 255, 255);
    }
  }
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (Pixels[i].used){
      Serial.println("Drawing index: " + String(i));
      bufferBig[(int) Pixels[i].ledPos] = Pixels[i].pixelData;
      //int8_t increment = (int8_t)(Pixels[i].velocity / 10);
      //Serial.println("increment: " + String(increment));
      //Pixels[i].ledPos += increment;
      //Pixels[i].velocity -= 0.98; // :P
      //Pixels[i].ledPos
      Pixels[i].ledPos += Pixels[i].velocity / 100;
      Pixels[i].velocity -= 0.9;
      if(Pixels[i].ledPos > 215 || Pixels[i].ledPos < 1){
        Pixels[i].used = false;
      }
    }
  }
  memmove8(&leds[NUM_LEDS/2], &bufferBig[NUM_LEDS/2], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i], sizeof(CRGB));
  }
}
