#include "Globals.h"

unsigned short tmp;
//int threshold[7] = {600, 600, 600, 600, 600, 600, 600};
int maxLeft[7];
int maxRight[7];
int maxLenLeft[7];
int maxLenRight[7];
int filteredLeft[7];
int filteredRight[7];

int lastLeft[7];
int lastRight[7];
int lastLeft2[7];
int lastRight2[7];

unsigned long maxLenTimeLeft[7];
unsigned long maxLenTimeRight[7];
unsigned long lastPulseTimeLeft[7];
unsigned long lastPulseTimeRight[7];

int col1 = 128;
int col2 = 210;

int hue = 0;

void vumeter() {
  
  //fadeToBlackBy( leds, NUM_LEDS, 16);

  if(millis() - lastPulseTimeLeft[0] > 40 && millis() - lastPulseTimeRight[0] > 40){
    FastLED.clear();
  }else if(millis() - lastPulseTimeLeft[0] > 30 && millis() - lastPulseTimeRight[0] > 30){
    fadeToBlackBy( leds, NUM_LEDS, 8);
  };
    
    /*
    fadeToBlackBy( secondary, NUM_LEDS_SECONDARY, 128);
    fadeToBlackBy( leds, NUM_LEDS, 128);
    */
    
    //fadeToBlackBy( secondary, NUM_LEDS_SECONDARY, 64);
    //fadeToBlackBy( leds, NUM_LEDS, 64);
    //fill_solid (&secondary[0], NUM_LEDS_SECONDARY/2, CHSV( 210, 255, 0));

  //FastLED.clear();
  tmp = tmp + 1;
  if(tmp == 2){tmp = 0;}
  unsigned int sumLeft  = 0;
  unsigned int sumRight = 0;
  unsigned int bassLeft  = 0;
  unsigned int bassRight = 0;

  EVERY_N_MILLISECONDS( 100 ) { 
    //int tmp = col1;
    //col1 = col2;
    //col2 = tmp;  
  }
  
  //////////////
  // 7 BAND LOOP
  for(int i=0;i<7;i++){
   filteredLeft[i]  = 0.92 *  filteredLeft[i] + 0.08 *  vu.left[i];
   filteredRight[i] = 0.92 * filteredRight[i] + 0.08 * vu.right[i];
////////////////////////////////////////////////////////////////////

    int filteredLenLeft = map(filteredLeft[i], 0, 255, 0, 9);
    int     limitedLeft =  vu.left[i];
    int    limitedRight = vu.right[i];
    
    int  lenLeft = map(  filteredLeft[i], 0, 255,   0, 61);
    int lenRight = map( filteredRight[i], 0, 255,   0, 71);

    //////
    if(lenLeft  >  maxLenLeft[i]){
     if(i == 0 && millis() - lastPulseTimeLeft[i] > 300 && lenLeft > 30){
        lastPulseTimeLeft[i] = millis();
        fill_solid (&leds[0], NUM_LEDS/2, CHSV( col1, 0, 64));
       
      }
      maxLenLeft[i] =  lenLeft;  maxLenTimeLeft[i] = millis();
    };
    
    if(lenRight > maxLenRight[i]){
      if(i == 0 && millis() - lastPulseTimeRight[i] > 300 && lenRight > 30){
        lastPulseTimeRight[i] = millis();
        fill_solid (&leds[NUM_LEDS/2], NUM_LEDS/2, CHSV( col2, 0, 64));
        hue = hue + 5;
      }
      maxLenRight[i] = lenRight; maxLenTimeRight[i] = millis();
    };
    //////
    unsigned int color     = map(i, 0, 6, 224, 0);

    if( maxLeft[i] > 100){  maxLeft[i] =  maxLeft[i] - 1; };
    if(maxRight[i] > 100){ maxRight[i] = maxRight[i] - 1; };
    if( vu.left[i] >  maxLeft[i]){  maxLeft[i] =  vu.left[i]; };
    if(vu.right[i] > maxRight[i]){ maxRight[i] = vu.right[i]; };

//////////////////////
    if(i == 0){
      int briLeft          = map(       lenLeft, 0, 61, 40, 152);
      int briLeftPeak      = map( maxLenLeft[i], 0, 61, 40, 152);
      int briLeftSecondary = map(max (0, filteredLeft[i] - 144), 0, 40, 112, 255);
      //lastPulseTimeLeft[i] = millis();    
      if(lenLeft >= 5){
        fill_solid (&leds[max(0,       lenLeft - 10)], 5 + min(0,       lenLeft-5), CHSV( col1, 255, briLeft));
      };
      if(maxLenLeft[i] >= 5){
        fill_solid (&leds[max(0, maxLenLeft[i] -  5)], 5 + min(0, maxLenLeft[i]-5), CHSV( col1, 255, briLeftPeak));
      };
      fill_solid (&secondary[0], NUM_LEDS_SECONDARY/2, CHSV( col1, 255, briLeftSecondary));
    }
    if(i == 0){
      int briRight          = map(       lenRight, 0, 61, 40, 152);
      int briRightPeak      = map( maxLenRight[i], 0, 61, 40, 152);
      int briRightSecondary = map(max (0, filteredRight[i] - 144), 0, 40, 112, 255);
      if(lenRight >= 5){
        fill_solid (&leds[NUM_LEDS/2 + max(0,       lenRight - 10)], 5 + min(0,       lenRight -5), CHSV( col2, 255, briRight));
      };
      if(maxLenRight[i] >= 5){
        fill_solid (&leds[NUM_LEDS/2 + max(0, maxLenRight[i] -  5)], 5 + min(0, maxLenRight[i] -5), CHSV( col2, 255, briRightPeak));
      };
      if(millis() - lastPulseTimeRight[i] > 200 && lenRight > 20 && lenRight == maxLenRight[i]){
        //fill_solid (&leds[NUM_LEDS/2], NUM_LEDS/2, CHSV( 255, 0, 128));
        //delay(20);
        //lastPulseTimeRight[i] = millis();
      }
      fill_solid (&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CHSV( col2, 255, briRightSecondary));
    }
  
    if(i > 0 && i < 6){
     sumLeft  =  sumLeft +  filteredLeft[i];
     sumRight = sumRight + filteredRight[i];
    }
    if(i >= 0 && i <= 1){
      bassLeft  =  sumLeft +  filteredLeft[i];
      bassRight = sumRight + filteredRight[i];
    }
///////////////////////////////////
    if( vu.left[i] >  maxLeft[i] &&  vu.left[i] > 50){  maxLeft[i] =  vu.left[i]; };
    if(vu.right[i] > maxRight[i] && vu.right[i] > 50){ maxRight[i] = vu.right[i]; };
    if(tmp == 0){
      if( maxLenLeft[i] > 0 && millis() -  maxLenTimeLeft[i] > 200){  maxLenLeft[i] =  maxLenLeft[i] - 1; };
      if(maxLenRight[i] > 0 && millis() - maxLenTimeRight[i] > 200){ maxLenRight[i] = maxLenRight[i] - 1; };
    }
    lastLeft2[i] = lastLeft[i];
    lastRight2[i] = lastRight[i];
    lastLeft[i] =  vu.left[i];
    lastRight[i] = vu.right[i];
  }
  
  int  masterLeft = map(  sumLeft, 0, 1275, -1, 71);
  int masterRight = map( sumRight, 0, 1275, -1, 71);
  if(masterLeft > -1){
    leds[masterLeft]             = CHSV( col1, 128, 128);
  }
  if(masterRight > -1){
    leds[NUM_LEDS/2+masterRight] = CHSV( col2, 128, 128);
  }
  int  masterLeftBri = map(  masterLeft, -1, 71, 0, 255);
  int masterRightBri = map( masterRight, -1, 71, 0, 255);
  fill_solid (&secondary[0],                    NUM_LEDS_SECONDARY/2, CHSV( col1, 255, masterLeftBri));
  fill_solid (&secondary[NUM_LEDS_SECONDARY/2], NUM_LEDS_SECONDARY/2, CHSV( col2, 255, masterRightBri));
 
///////////////////////////////////

   //mirror();
}
