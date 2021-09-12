#include "Globals.h"
#include "Leds.h"
#include "Image.h"
#include "Effect.h"

#include <stdlib.h> // for malloc and free
//void* operator new(size_t size) { return malloc(size); }
//void operator delete(void* ptr) { free(ptr); } 

// https://github.com/Richard-Gemmell/teensy4_i2c
// | Port | Pins               | imx_rt1060_i2c_driver.h | i2c_driver_wire.h |
// | ---- |--------------------| ------------------|-------|
// | 0    | SCL0(19), SDA0(18) | Master or Slave   | Wire  |
// | 1    | SCL1(16), SDA1(17) | Master1 or Slave1 | Wire1 |
// | 2    | SCL2(24), SDA2(25) | Master2 or Slave2 | Wire2 |
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

Effect* ptrEffect = NULL;

void receiveEvent(int howMany);

//#define FASTLED_INTERNAL
#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
//FASTLED_USING_NAMESPACE

//#include <SD.h>
//#include <SPI.h>
//int chipSelect = BUILTIN_SDCARD;

CRGB    leds[NUM_LEDS];
CRGB ledsTmp[NUM_LEDS];
CRGB ledsTmpLow[NUM_LEDS/2];
CRGB ledsTmpHigh[NUM_LEDS/2];
CRGB bufferBig[NUM_LEDS * 3];

CRGB secondary[NUM_LEDS_SECONDARY];


//CRGB bufferLow[NUM_LEDS * 3];
//CRGB bufferHigh[NUM_LEDS * 3];

Pixel Pixels[NUM_PIXELS];
bool blockingLookup[NUM_LEDS];

union vu_ vu;
union I2Cdata_ I2Cdata;

bool useSerial = true;
bool ledsEnabled = true; // true;

uint8_t gHue = 0;
//unsigned long lastHue   =   0;
//uint8_t hueIncrement = 1;
//uint8_t msPerHue = 10;

unsigned int msPerFrame =  10;
unsigned long lastFrame =   0;
unsigned long fCount = 0;

bool firstFrame = true; // set for the first frame of an effect
bool readFromNano = false;

unsigned int state = 0;
unsigned long lastImpulse = 0;
unsigned long lastEffectChange = 0;
unsigned long lastImpulseDecrease = 0;
unsigned long lastWireTime = 0;
bool vuChange = false;
bool vuSignal = false;

bool boolImpulse = true;
uint8_t impulseCount = 0;
void s1Impulse(){ Serial.println("s1"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 1; };
void s2Impulse(){ Serial.println("s2"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 2; };
void s3Impulse(){ Serial.println("s3"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 3; };
void s4Impulse(){ Serial.println("s4"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 4; };
void vuImpulse(){ vuSignal = !digitalRead(5); vuChange = true; };

bool boolClear = false;
//unsigned long frameCount = 0;
unsigned long wireDuration = 0;



//void frame();

// vu data
float  filteredLeftFast[7];
float filteredRightFast[7];
float  filteredLeftSlow[7];
float filteredRightSlow[7];

//Obj effect;



void receiveEvent(int howMany)
{
  while(Wire.available() > 0) {
  char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void setup() {
  delay(1000); if(useSerial){ Serial.begin(115200); }; delay(1000);
  if(useSerial && !Serial){ delay(1000); if(!Serial){useSerial = false; }; }
  if(useSerial){ Serial.println("Starting up ..."); };
  // DATA_RATE_MHZ(24)++
  FastLED.addLeds<SK9822,       11, 13, BGR>(leds,        0, 72).setCorrection(TypicalLEDStrip); // DATA 11 GREEN, CLOCK 13 YELLOW
  FastLED.addLeds<SK9822,       26, 27, BGR>(leds,       72, 72).setCorrection(TypicalLEDStrip); // DATA 26 GREEN, CLOCK 27 YELLOW
  FastLED.addLeds<WS2812SERIAL, 14,     BRG>(secondary,   0,  3).setCorrection(TypicalLEDStrip); // DATA 14 WHITE
  FastLED.addLeds<WS2812SERIAL, 17,     BRG>(secondary,   3,  3).setCorrection(TypicalLEDStrip); // DATA 17 WHITE
  FastLED.setBrightness(MAX_BRIGHTNESS); // PWM duty cycles
  FastLED.setMaxPowerInVoltsAndMilliamps(MAX_POWER_VOLTS, MAX_POWER_MILLIAMPS);
  FastLED.setMaxRefreshRate(0);
  FastLED.clear();
  FastLED.show();

  Wire.begin(); 
  Wire.setClock(1000000);
 
  //if(!SD.begin()){
  //  if(useSerial){ Serial.println("SD failed ..."); };
  //} else {
  //  if(useSerial){ Serial.println("SD working ..."); };
  //}
  //Wire.begin(9);                // 9
  // Teensy4.1 Pin2 -> Pin9 --> Arduino NANO 33 BLE Sense Pin10 --> Pin3
  // Teensy4.1  2 -> 10
  // Teensy4.1  3 ->  9
  // Teensy4.1  4 ->  8
  // Teensy4.1  5 ->  7
  // Teensy4.1  6 ->  6
  // Teensy4.1  7 ->  5
  // Teensy4.1  8 ->  4
  // Teensy4.1  9 ->  3
  //Wire.onReceive(receiveEvent);
  pinMode(2, INPUT_PULLUP); // reserved
  pinMode(3, INPUT_PULLUP); // reserved
  pinMode(4, INPUT_PULLUP); // reserved
  pinMode(5, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(5), vuImpulse, CHANGE);
  pinMode(6, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(6), s4Impulse, FALLING);
  pinMode(7, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(7), s3Impulse, FALLING);
  pinMode(8, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(8), s2Impulse, FALLING);
  pinMode(9, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(9), s1Impulse, FALLING);
  Serial.println("Setup finished ...");
}

uint8_t gCurrentPatternNumber = 0;
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
typedef void (*SimplePatternList[])();

// <-------------------------------------------------------------------------------------------------------------------------- PATTERN LIST ----------------------------------------------------------------------
//SimplePatternList gPatterns = {glow, pixels2, vumeter2, snowflakes, binaryCounter};

//SimplePatternList gPatterns = {effectPixels};
//SimplePatternList gPatterns = {binaryCounter};
//SimplePatternList gPatterns = {lavalamp};
//SimplePatternList gPatterns = {blocks};
//SimplePatternList gPatterns = {rainbowSin};
//SimplePatternList gPatterns = {idleFluorescentTube, idleRotation};
//SimplePatternList gPatterns = {idleFluorescentTube};
SimplePatternList gPatterns = {effectFire};

//////////
// loop //
//////////
void loop() {
  //if(vuChange){ Serial.println("--- VU CHANGE ---"); }
  if(vuChange || (ARRAY_SIZE(gPatterns) > 1 && millis() - lastEffectChange > 1000 * 60 * 0.5)){ // change effect if vu signal starts/ends or every n minutes if there are more than 1 effects configured
    Serial.println("Changing effect ...");    
    ptrEffect = new Effect();
    ptrEffect->init();

    lastEffectChange = millis();
    firstFrame = true;
    fCount = 0;
    if(vuChange){ vuChange = false; };

    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(MAX_POWER_VOLTS, MAX_POWER_MILLIAMPS);
   
    //frameCount = 0;
    if(vuChange && vuSignal){
      readFromNano = true;
    }else{
      readFromNano = false;
    }
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  }
  if(millis() - lastImpulseDecrease > 1000){
    lastImpulseDecrease = millis();
    if(impulseCount > 0){
      impulseCount--;
    }
  }
  if(readFromNano && millis() - lastWireTime > 50){
    lastWireTime = millis();
    unsigned long wireStartTime = millis();
    //Wire.requestFrom(0x40, sizeof(vu.bytes));
    Wire.requestFrom(0x40, sizeof(I2Cdata.bytes));
    //    Serial.print((char)Wire.peek());
    Serial.print("?");
    //leds[19] = CRGB::Orange;
    if (Wire.available()) {
      int i = 0; while(Wire.available()) { I2Cdata.bytes[i] = Wire.read(); i++; }
      //Serial.println("<");
    }
    memcpy8(&vu.bytes[0], &I2Cdata.bytes[0], sizeof vu.bytes); // copy vu data from the I2C union over to the vu union
    vuFilter();
    wireDuration = millis() - wireStartTime;
  }
  //if(lastHue + msPerHue < millis()){
  //  lastHue = millis();
  //  gHue += hueIncrement;
  //}
  if(lastFrame + msPerFrame < millis()){
    lastFrame = millis();  
    frame();
    fCount++;
  }
}
void frame(){
  
  ////////////////////////////////
  // THIS IS JUST THE BEGINNING //
  ////////////////////////////////

  //ptrEffect->runPattern();
  //ptrEffect->runPattern();
  //pixels2();

  if(vuSignal && 1==2){
    vumeter2();
  }else if(impulseCount <= 4){
     gPatterns[gCurrentPatternNumber](); 
  }else{
    binaryCounter();
  }
 
  firstFrame = false; 
  /////////////////////
  // THIS IS THE END //
  /////////////////////
  
  if(!ledsEnabled){
    FastLED.clear();
  }
  //leds[  2] = CRGB::Green;
  //leds[141] = CRGB::Red;
  
  // debug
  unsigned long frameDuration = millis() - lastFrame;
  
  if(frameDuration > 2) { if(frameDuration > 60){ frameDuration = 60; }; leds[frameDuration] = CRGB::Green;  }
  if(wireDuration  > 16){ if(wireDuration  > 60){ wireDuration  = 60; };  leds[wireDuration] = CRGB::Yellow; }
  //if(impulseCount  > 0){ leds[impulseCount]  = CRGB::Blue;   }
  //if(readFromNano){ leds[19] = CRGB::Green; } else { leds[19] = CRGB::Red; }
  //if(vuSignal){ leds[20] = CRGB::Green; } else { leds[20] = CRGB::Red; }
  //leds[71]  = CHSV( 64, 255, 64);
  FastLED.show();
}
