#include "Globals.h"
int fLeft[7];
int fRight[7];

int maxLenLeft[7];
int maxLenRight[7];
unsigned long maxLenTimeLeft[7];
unsigned long maxLenTimeRight[7];
unsigned long maxReductionTime = 0;

void vumeter2 () {
  if(firstFrame){
    readFromNano = true;
    FastLED.setBrightness(MAX_BRIGHTNESS);
    msPerFrame = 10;
  }
  //fill_solid (&bufferBig[0], NUM_LEDS * 3, CRGB::Black);
  //fadeToBlackBy(bufferBig, NUM_LEDS * 3, 80);
  
  fadeToBlackBy(leds, NUM_LEDS, 64);
  
  for(int i=0;i<7;i++){
    fLeft[i]  = 0.75 *  fLeft[i] + 0.25 *  vu.left[i];
    fRight[i] = 0.75 * fRight[i] + 0.25 * vu.right[i];
    
    uint8_t  color   = map( i, 0, 6, 224, 0);
    
    uint8_t lenLeft  = map(  fLeft[i], 0, 255, 0, 10);
    uint8_t lenRight = map( fRight[i], 0, 255, 0, 10);

    if(lenLeft  >  maxLenLeft[i]){ maxLenLeft[i] =  lenLeft;  maxLenTimeLeft[i] = millis(); };
    if(lenRight > maxLenRight[i]){ maxLenRight[i] = lenRight; maxLenTimeRight[i] = millis(); };
    
    uint8_t satLeft  = map(  fLeft[i], 0, 255, 16, 255);
    uint8_t satRight = map( fRight[i], 0, 255, 16, 255);

    //leds[72 + lenLeft] = CHSV( color, 255, 255);
    //leds[lenRight]     = CHSV( color, 255, 255); 
     
     fill_solid(&leds[72 + i * 10], lenLeft , CHSV( color, 255, satLeft));
     if(maxLenLeft[i] > 1){ leds[72 + i * 10 + maxLenLeft[i]] = CHSV(color, 128, 64); };
     
     fill_solid(&leds[     i * 10], lenRight, CHSV( color, 255, satRight));
     if(maxLenRight[i] > 1){ leds[     i * 10 + maxLenRight[i]] = CHSV(color, 128, 64); };
     
    //bufferBig[72 + lenLeft] = CHSV( color, 255, 255);
    //bufferBig[lenRight]     = CHSV( color, 255, 255);
  }
  if(millis() - maxReductionTime > 150){
    maxReductionTime = millis();
    for(int i=0;i<7;i++){
      if( maxLenLeft[i] > 0 && millis() -  maxLenTimeLeft[i] > 150){  maxLenLeft[i] =  maxLenLeft[i] - 1; };
      if(maxLenRight[i] > 0 && millis() - maxLenTimeRight[i] > 150){ maxLenRight[i] = maxLenRight[i] - 1; };
    }
  }
  bar2handle();
  //buffer2leds(0, false);
}
