#include "Globals.h"
uint8_t ball = 1;
int8_t ballVelocity = 0;
CRGB ballSprite[3];

void level () {
  if(firstFrame){
    FastLED.setBrightness(BRIGHTNESS);
    msPerFrame = 25;
  }
  FastLED.clear();
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( gHue,       255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( gHue + 128, 255, 255));
  if(vu.pitch < -5 && ballVelocity > -10 && ball > 1){
    ballVelocity--;
  }else if(vu.pitch > 5 && ballVelocity < 10 && ball  < 142){
    ballVelocity++;
  }else if(vu.pitch <= 5 && vu.pitch >= -5){ // bring ball to standstill if vu.pitch is low
    if(ballVelocity < -1){
      ballVelocity++;
    }else if(ballVelocity > 1){
      ballVelocity--;
    }else{
      ballVelocity = 0;
    }
  }
  if(ballVelocity > 0 && ball < 142){
    ball++;
  }else if(ballVelocity < 0 && ball > 1){
    ball--;
  }
  
  Serial.println(String(vu.pitch) + " ---> " + String(ball));

  ballSprite[0] = CHSV( gHue, 255, 160);
  ballSprite[1] = CHSV( gHue, 255, 255);
  ballSprite[2] = CHSV( gHue, 255, 160);
  if(ball > 71){
    ballSprite[0] = CHSV( gHue + 128, 255, 160);
    ballSprite[1] = CHSV( gHue + 128, 255, 255);
    ballSprite[2] = CHSV( gHue + 128, 255, 160);
  }
  fill_solid (&ledsTmp[0], NUM_LEDS, CRGB::Black);
  //if(ball != 0){
    memmove8(&ledsTmp[ball-1], &ballSprite[0], 3 * sizeof(CRGB));
  //}
  memmove8(&leds[0], &ledsTmp[0], NUM_LEDS * sizeof(CRGB));
}
