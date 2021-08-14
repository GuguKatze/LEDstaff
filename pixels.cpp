#include "Globals.h"
uint8_t glowColor = 22;

int8_t ball = 0;
int8_t ballVelocity = 0;

void pixels () {
  if(firstFrame){
    msPerFrame = 25;
  }
  //msPerFrame = beatsin16(10, 10, 50);
  FastLED.setBrightness(beatsin16(20, 32, 224));
  fill_solid(&secondary[0],                      NUM_LEDS_SECONDARY / 2, CHSV( glowColor,       255, 255));
  fill_solid(&secondary[NUM_LEDS_SECONDARY / 2], NUM_LEDS_SECONDARY / 2, CHSV( glowColor + 128, 255, 255));
  glowColor += 2;

  // low
  memmove8(&ledsTmpLow[1], &ledsTmpLow[0], (NUM_LEDS/2-1) * sizeof(CRGB));
  ledsTmpLow[0] = CRGB::Black;
  int rand1 = random(  0,  6);
  if(rand1 == 0){
    //ledsTmpLow[0] = CHSV( glowColor, 255, 255);
  }
  memmove8(&leds[0], &ledsTmpLow[0], (NUM_LEDS/2)   * sizeof(CRGB));
  // high
  memmove8(&ledsTmpHigh[1], &ledsTmpHigh[0], (NUM_LEDS/2-1) * sizeof(CRGB));
  ledsTmpHigh[0] = CRGB::Black;
  rand1 = random(  0,  6);
  if(rand1 == 0){
    //ledsTmpHigh[0] = CHSV( glowColor + 128, 255, 255);
  }
  memmove8(&leds[NUM_LEDS/2], &ledsTmpHigh[0], (NUM_LEDS/2)   * sizeof(CRGB));

  if(vu.pitch < -5 && ballVelocity > -10 && ball != -72){
    //ball--;
    ballVelocity--;
  }else if(vu.pitch > 5 && ballVelocity < 10 && ball != 72){
    //ball++;
    ballVelocity++;
  }else if(vu.pitch <= 5 && vu.pitch >= -5){
    if(ballVelocity > 0){
      //ball--;
      ballVelocity--;
    }else if(ballVelocity < 0){
      //ball++;
      ballVelocity++;
    }
  }
  if(ballVelocity > 0 && ball < 72){
    ball++;
  }else if(ballVelocity < 0 && ball > -72){
    ball--;
  }
  if(ball == -72 || ball == 72){
    ballVelocity = 0;
  }
  
  Serial.println(String(vu.pitch) + " ---> " + String(ball));
  if(ball != 0){
    uint8_t ballLed = 71 + ball;
    if(ball < 0){
      ballLed = (ball * -1) - 1;
    }
    leds[ballLed] =    CHSV( glowColor,       255, 255);
    if(ballLed > 71){
      leds[ballLed] =  CHSV( glowColor + 128, 255, 255);
    }
  }
  
}
