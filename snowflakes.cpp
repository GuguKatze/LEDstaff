#include "Globals.h"


int findHighestBlocker() {
  int highestBlockerPos = -1;
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
     if (!Pixels[i].used || !Pixels[i].blocking){ continue; }
     int ledPos = (int) Pixels[i].ledPos;
    if (ledPos > highestBlockerPos){
      highestBlockerPos = ledPos;
    }
  }
  return highestBlockerPos;
}
int findLowestBlocker() {
  int lowestBlockerPos = 9000;
  for (uint8_t i = 0; i < NUM_PIXELS - 1; i++){
    if (!Pixels[i].used || !Pixels[i].blocking){ continue; }
    int ledPos = (int) Pixels[i].ledPos;
    if (ledPos < lowestBlockerPos){
      lowestBlockerPos = ledPos;
    }
  }
  return lowestBlockerPos;
}

void populateBlockingLookup(){
  for (uint8_t i = 0; i < NUM_LEDS; i++){
     blockingLookup[i] = false;
  }
  uint8_t blockingCount = 0;
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (!Pixels[i].used || !Pixels[i].blocking){ continue; }
    int ledPos = (int) Pixels[i].ledPos;
    blockingLookup[ledPos] = true;
    blockingCount++;
  }
  //Serial.println("Blocking count: " + String(blockingCount));
}

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
      Pixels[i].blocking = false;
    }
    fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CRGB::MediumTurquoise);
    fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CRGB::MediumTurquoise);
  }

 fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
 //int highestBlockerPos = findHighestBlocker();
 //int lowestBlockerPos = findLowestBlocker();
 //Serial.println("Highest/lowest blocking  pos: " + String(highestBlockerPos) + "/" + String(lowestBlockerPos));
 populateBlockingLookup();
 if(random(0, beatsin16(9, 25, 129)) == 0){
    int unused = findUnused();
    if(unused != -1){
      //Serial.println("Using unused index: " + String(unused));
      Pixels[unused].used = true;
      Pixels[unused].ledPos = 143;
      Pixels[unused].gravity = random(85, 95) / 1000.0;
      Pixels[unused].velocity = random(0, 10) - 5;
      Pixels[unused].startTime = millis();
      //Pixels[unused].pixelData = CRGB::White;
      Pixels[unused].pixelData = CHSV(random(140, 180), 64, 192);
      Pixels[unused].blocking = false;
    }
  }
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (!Pixels[i].used){ continue; }
    
    // draw pixel as is
    int ledPos = (int) Pixels[i].ledPos;
    if(ledPos > NUM_LEDS * 3 || ledPos < 0){
      Pixels[i].used = false;
      continue;
    }
    bufferBig[ledPos] = Pixels[i].pixelData;
    
    // alter pixel state
    Pixels[i].pixelData.fadeLightBy(6);
    if(random(0, 2048) == 0){ Pixels[i].pixelData.maximizeBrightness(); }
    //if(random(0, 2048) == 0){ Pixels[i].pixelData += 8; }
    //if(ledPos - 1 <= highestBlockerPos && !(lowestBlockerPos > 0 && ledPos == lowestBlockerPos)){
    if(ledPos == 0 || blockingLookup[ledPos-1]){
      Pixels[i].velocity = -30;
      Pixels[i].blocking = true;
    }else{
      Pixels[i].gravity = (random(25, 30) - beatsin16(6, 0, 20)) / 1000.0;
      Pixels[i].velocity -= Pixels[i].gravity;
      if(random(0, 64) == 0 && Pixels[i].velocity < 0){ Pixels[i].velocity += random(60, 70) / 100.0; }
      Pixels[i].ledPos += Pixels[i].velocity / 100;
    }
    if(Pixels[i].blocking && random(0, 17) == 0){
      Pixels[i].pixelData += CRGB(0, 0, 17);
      Pixels[i].pixelData.fadeLightBy(9);
    }

    // melt snow flakes
    if(millis() - Pixels[i].startTime > 30000){
      Pixels[i].used = false;
    }
  }
  memmove8(&leds[NUM_LEDS/2], &bufferBig[NUM_LEDS/2], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i], sizeof(CRGB));
  }
}
