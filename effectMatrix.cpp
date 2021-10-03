#include "Globals.h"

void matrixPixel(unsigned int i){
 uint8_t r = random(0, 41);
 if(r < 12){
    bufferBig[i] = CRGB::Green; 
  }else{
    bufferBig[i] = CRGB::Black; 
  } 
  if(r == 0){
    bufferBig[i] = CRGB::White; 
  }
}

void effectMatrix () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 20;
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
  ///////////
  // frame //
  ///////////
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( 85, 255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( 85, 255, 255));

  //fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  //fadeToBlackBy(bufferBig, NUM_LEDS * 3, 24);
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, beatsin16(12, 8, 24));
  
  if(random(0, beatsin16(8, 6, 16)) == 0){ // chance to launch
    int unused = findUnused();
    if(unused != -1){
      //Serial.println("Using unused index: " + String(unused));
      Pixels[unused].used = true;
      Pixels[unused].ledPos = random(32, NUM_LEDS * 3);
      
      Pixels[unused].gravity = random(800, 900) / 1000.0;
      //Pixels[unused].velocity = random(40, 120);
      Pixels[unused].velocity = 0; // speed
      Pixels[unused].startTime = millis();
      Pixels[unused].pixelData = CHSV(random(80, 91), 255, 255);
    }
  }


  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (!Pixels[i].used){ continue; }
    //Serial.println("Drawing index: " + String(i));
    int ledPos = (int) Pixels[i].ledPos;
    bufferBig[ledPos] = Pixels[i].pixelData;
    if(Pixels[i].velocity > -64){
      //bufferBig[ledPos-1] = CRGB::White;
    }
    bufferBig[ledPos-1] = CHSV(85,0, 255 + Pixels[i].velocity * 2);
    //if(millis() - Pixels[i].startTime > 1000){
    /*
    if(Pixels[i].velocity < -80){
      Pixels[i].pixelData.fadeToBlackBy(4);
    }else if(Pixels[i].velocity < 5){
      Pixels[i].pixelData.fadeLightBy(4);
    }
    */
    Pixels[i].ledPos += Pixels[i].velocity / 100;
    Pixels[i].velocity -= Pixels[i].gravity;
    //Pixels[i].velocity -= 0.9;
    if(ledPos > 215 || ledPos <= 0){
      Pixels[i].used = false;
    }
  }
  buffer2leds(0, true);
  //memmove8(&leds[NUM_LEDS/2], &bufferBig[NUM_LEDS/2], (NUM_LEDS/2) * sizeof(CRGB));
  //for (uint8_t i=0; i<NUM_LEDS/2; i++){
  //  memmove8(&leds[(NUM_LEDS/2)-1-i], &bufferBig[i], sizeof(CRGB));
  //}

/*
  if(random(0, 4) == 0){
    uint8_t s = random(0, 18);
    for(uint8_t o = 0; o <8; o++){
      matrixPixel(NUM_LEDS + s * 8 + o);
    }
  }
  matrixPixel(NUM_LEDS * 2);

  memmove8(&bufferBig[NUM_LEDS], &bufferBig[NUM_LEDS + 1], NUM_LEDS * sizeof(CRGB));
  buffer2leds(NUM_LEDS, true);

  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    //if(bufferBig[NUM_LEDS + i].b > 0) bufferBig[NUM_LEDS + i].nscale8(32);
    //if(bufferBig[NUM_LEDS + i].b > 0) {
      fade_video(&bufferBig[NUM_LEDS + i], 1, 16);
    //}
    //if(bufferBig[NUM_LEDS + i].b > 0) bufferBig[NUM_LEDS + i].fadeLightBy(-1);
  }
*/

}
