#include "Globals.h"
void snowflakes () {
  if(firstFrame){
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
    // initialize Pixels
    for(int i = 0; i < NUM_PIXELS; i++){
      Pixels[i].used = false;
      Pixels[i].ledPos = 72;
      Pixels[i].gravity = 0.9;
      Pixels[i].velocity = 0;
      Pixels[i].startTime = 0;
      Pixels[i].pixelData = CRGB::Black;
    }
    fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CRGB::MediumTurquoise);
    fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CRGB::MediumTurquoise);
  }

 fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
 if(random(0, beatsin16(12, 64, 512)) == 0){
    int unused = findUnused();
    if(unused != -1){
      Serial.println("Using unused index: " + String(unused));
      Pixels[unused].used = true;
      Pixels[unused].ledPos = 143;
      Pixels[unused].gravity = random(85, 95) / 1000.0;
      Pixels[unused].velocity = random(0, 10) - 5;
      Pixels[unused].startTime = millis();
      //Pixels[unused].pixelData = CRGB::White;
      Pixels[unused].pixelData = CHSV(random(140, 180), 64, 255);
    }
  }
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (Pixels[i].used){
      Serial.println("Drawing index: " + String(i));
      int ledPos = (int) Pixels[i].ledPos;
      bufferBig[ledPos] = Pixels[i].pixelData;
      Pixels[i].pixelData.fadeLightBy(6);
      if(random(0, 2048) == 0){
        Pixels[i].pixelData.maximizeBrightness();
      }
      Pixels[i].ledPos += Pixels[i].velocity / 100;
      Pixels[i].gravity = (random(10, 20) - beatsin16(6, 0, 20)) / 1000.0;
      Pixels[i].velocity -= Pixels[i].gravity;
      if(random(0, 64) == 0){
        Pixels[i].velocity += random(20, 30) / 100.0;
      }
      //Pixels[i].velocity -= 0.9;
      if(ledPos > 215 || ledPos == 0){
        Pixels[i].used = false;
      }
    }
  }
  memmove8(&leds[NUM_LEDS/2], &bufferBig[NUM_LEDS/2], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i], sizeof(CRGB));
  }
}
