#include "Globals.h"


int maxLenLeft[7];
int maxLenRight[7];

unsigned long maxLenTimeLeft[7];
unsigned long maxLenTimeRight[7];
unsigned long maxReductionTime = 0;

bool extending = false;
uint8_t hiddenAmount = NUM_LEDS / 2;

void effectVumeter () {
  if(firstFrame){
    //readFromNano = true;
    extending = true;
    hiddenAmount = NUM_LEDS / 2;
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
  }
  fadeToBlackBy(bufferBig, NUM_LEDS * 3, 64);
  for(int i=0;i<7;i++){   
    uint8_t  color   = map( i, 0, 6, 224, 0);
    uint8_t lenLeft  = map(  constrain(  vuPacket.left[i] -  filteredLeftSlow[i] * 0.30, 0, 200), 0, 200, 0, 10);
    uint8_t lenRight = map(  constrain( vuPacket.right[i] - filteredRightSlow[i] * 0.30, 0, 200), 0, 200, 0, 10); 
    if(lenLeft  >  maxLenLeft[i]){ maxLenLeft[i] =  lenLeft;  maxLenTimeLeft[i] = millis(); };
    if(lenRight > maxLenRight[i]){ maxLenRight[i] = lenRight; maxLenTimeRight[i] = millis(); };
    uint8_t satLeft  = map(  filteredLeftFast[i], 0, 255, 16, 255);
    uint8_t satRight = map( filteredRightFast[i], 0, 255, 16, 255);
    fill_solid(            &bufferBig[NUM_LEDS     + i * 10], lenLeft , CHSV( color, 255, satLeft));
    if(maxLenLeft[i] > 1){  bufferBig[NUM_LEDS     + i * 10 + maxLenLeft[i]] = CHSV(color, 128, 64); };
    fill_solid(            &bufferBig[NUM_LEDS * 2 + i * 10], lenRight, CHSV( color, 255, satRight));
    if(maxLenRight[i] > 1){ bufferBig[NUM_LEDS * 2 + i * 10 + maxLenRight[i]] = CHSV(color, 128, 64); };
  }
  if(millis() - maxReductionTime > 150){
    maxReductionTime = millis();
    for(int i=0;i<7;i++){
      if( maxLenLeft[i] > 0 && millis() -  maxLenTimeLeft[i] > 150){  maxLenLeft[i] =  maxLenLeft[i] - 1; };
      if(maxLenRight[i] > 0 && millis() - maxLenTimeRight[i] > 150){ maxLenRight[i] = maxLenRight[i] - 1; };
    }
  }
  uint8_t index = 0;
  //b2l(NUM_LEDS + index             ,            0, NUM_LEDS/2, false);  //  0 ->  71
  //b2l(NUM_LEDS + index + NUM_LEDS/2,   NUM_LEDS/2, NUM_LEDS/2, false); // 72 -> 143

  hiddenAmount = beatsin16(16, 0, NUM_LEDS / 2 - 32);
  hiddenAmount = 0;
  b2l(NUM_LEDS     + hiddenAmount,          0, NUM_LEDS/2, false);  //  0 ->  71
  b2l(NUM_LEDS * 2 + hiddenAmount, NUM_LEDS/2, NUM_LEDS/2, false); // 72 -> 143
  bar2handle();
}
