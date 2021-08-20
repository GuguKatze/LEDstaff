#include "Globals.h"

void pixels2 () {
  if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
    
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
  //FastLED.setBrightness(beatsin16(20, 32, 64));
  EVERY_N_MILLISECONDS(120){ gHue++; }
  
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue,       255, 255));
  //fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + 128, 255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue, 255, 255));

  fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  if(random(0, beatsin16(12, 24, 256)) == 0){
    int unused = findUnused();
    if(unused != -1){
      Serial.println("Using unused index: " + String(unused));
      Pixels[unused].used = true;
      Pixels[unused].ledPos = 72;
      Pixels[unused].gravity = random(800, 900) / 1000.0;
      //Pixels[unused].velocity = random(40, 120);
      Pixels[unused].velocity = beatsin16(8, 50, 110) + random(0, 11) - 5;
      Pixels[unused].startTime = millis();
      Pixels[unused].pixelData = CHSV(gHue + random(0, 61) - 30, 255, 255);
    }
  }
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (!Pixels[i].used){ continue; }
    Serial.println("Drawing index: " + String(i));
    int ledPos = (int) Pixels[i].ledPos;
    bufferBig[ledPos] = Pixels[i].pixelData;
    //if(millis() - Pixels[i].startTime > 1000){
    if(Pixels[i].velocity < -80){
      Pixels[i].pixelData.fadeToBlackBy(4);
    }else if(Pixels[i].velocity < 5){
      Pixels[i].pixelData.fadeLightBy(4);
    }
    Pixels[i].ledPos += Pixels[i].velocity / 100;
    Pixels[i].velocity -= Pixels[i].gravity;
    //Pixels[i].velocity -= 0.9;
    if(ledPos > 215 || ledPos == 0){
      Pixels[i].used = false;
    }
  }
  memmove8(&leds[NUM_LEDS/2], &bufferBig[NUM_LEDS/2], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i], sizeof(CRGB));
  }
}
