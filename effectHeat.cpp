#include "Globals.h"

void effectHeat() {
 if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 10;
    fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
    // initialize Pixels
    for(int i = 0; i < NUM_PIXELS; i++){
      Pixels[i].ledPos = random(3, 30);
      Pixels[i].gravity = 0.9;
      Pixels[i].velocity = random(0, 10) - 5;
      Pixels[i].startTime = 0;
      Pixels[i].heat = 40.0f - 10 * i;
      Pixels[i].pixelData = CRGB::Green;
    }
  }
  ///////////
  // frame //
  ///////////
  FastLED.setBrightness(beatsin8(2, 0.6 * MAX_BRIGHTNESS, 0.8 * MAX_BRIGHTNESS));
  
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( 92, 255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( 92, 255, 255));
  
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, beatsin16(12, 8, 8));
  //fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);

 
  //for (uint8_t i = 0; i < NUM_PIXELS; i++){
  for (uint8_t i = 0; i < 6; i++){
      Pixels[i].startTime = millis();
      if(Pixels[i].ledPos < 20){
        uint8_t heat = map(Pixels[i].ledPos, 0, 19, 100, 0);
        Pixels[i].heat += random( i*3, heat) / 1000.0f;
      }else if(Pixels[i].ledPos > 120){
        uint8_t heat = map(Pixels[i].ledPos, 100, 143, 0, 50);
        Pixels[i].heat -= random( i*3, heat) / 1000.0f;
      }
      
      //Pixels[i].velocity = (Pixels[i].heat - 30) / 10;
      Pixels[i].velocity = (Pixels[i].heat - 30) / 10;
      
      if(Pixels[i].velocity > -1 && Pixels[i].velocity < 1){
        if(Pixels[i].ledPos > 10 && Pixels[i].ledPos < 130){
          Pixels[i].heat -= 1;
        }
      }
      
      if(random(0, 1000) == 0){
        Pixels[i].heat += random(1, 10) - 4;
      }
      
      /*
      if(Pixels[i].heat > 40){
        Pixels[i].velocity += random( 1,  2) / 100.0f;
      }else if(Pixels[i].heat < 30){
        Pixels[i].velocity -= random( 2,  4) / 100.0f;
      }else {
        Pixels[i].pixelData = CRGB::Orange;
      }
      */
      //Pixels[i].heat = constrain(Pixels[i].heat, 20, 60);
      uint8_t color = constrain(Pixels[i].heat, 20, 60);
      //color = map(color, 20, 60, 96, 0);
      color = map(color, 20, 60, 96, 80);
      Pixels[i].pixelData = CHSV(color , 255, 255);
      
      //int ledPos = (int) Pixels[i].ledPos;
      //bufferBig[ledPos] = Pixels[i].pixelData;
      Pixels[i].ledPos += Pixels[i].velocity / 100;
      if(Pixels[i].ledPos > 140){ Pixels[i].ledPos = 140; }
      if(Pixels[i].ledPos <   2){ Pixels[i].ledPos =   2; }
  }

  for (uint8_t x = 0; x < NUM_LEDS; x++) {
    float sum = 0;
    uint16_t dist = 0;
    for (uint8_t i = 0; i < 6; i++){
     
      dist = max(1, abs(x - Pixels[i].ledPos));
      sum += (NUM_LEDS + beatsin8(2, 0, 4, 0, i * 32) * 10) / (dist * 1.5);
    }   
    uint8_t color      = constrain(sum, 72, 96);
    uint8_t brightness = constrain(sum, 72, 255);
      
    if(color <= 72){
      bufferBig[x] = CRGB::Black;
    }else{
      //bufferBig[x] = CHSV(color + random(0, 8), 255 - random(0, 8), brightness - random(0, 8));
      bufferBig[x] = CHSV(color - random(0, 8) + 4, 255 - random(0, 8), brightness);
    }
  }
  for (uint8_t i = 0; i < 6; i++){
    int ledPos = (int) Pixels[i].ledPos;
    //bufferBig[ledPos] = CRGB::Purple;
  }
  
  blur1d(bufferBig, NUM_LEDS * 3, 128);
  //blur1d(bufferBig, NUM_LEDS * 3, 128);
  buffer2leds(0, true);
}
  
