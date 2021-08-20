#include "Globals.h"
#include "Leds.h"
#include "Image.h"

// https://github.com/Richard-Gemmell/teensy4_i2c
// | Port | Pins               | imx_rt1060_i2c_driver.h | i2c_driver_wire.h |
// | ---- |--------------------| ------------------|-------|
// | 0    | SCL0(19), SDA0(18) | Master or Slave   | Wire  |
// | 1    | SCL1(16), SDA1(17) | Master1 or Slave1 | Wire1 |
// | 2    | SCL2(24), SDA2(25) | Master2 or Slave2 | Wire2 |
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

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

//CRGB ledsTmpLow[NUM_LEDS/2];
//CRGB ledsTmpHigh[NUM_LEDS/2];

CRGB secondary[NUM_LEDS_SECONDARY];

CRGB bufferBig[NUM_LEDS * 3];
CRGB bufferLow[NUM_LEDS * 3];
CRGB bufferHigh[NUM_LEDS * 3];

Pixel Pixels[NUM_PIXELS];
bool blockingLookup[NUM_LEDS];

union vu_ vu;
int8_t pitch;

bool useSerial = true;
bool ledsEnabled = true; // true;
uint8_t gHue = 0;
bool firstFrame = true; // set for the first frame of an effect
bool readFromNano = false;

unsigned int state = 0;
unsigned long lastImpulse = 0;
unsigned long lastEffectChange = 0;
unsigned long lastImpulseDecrease = 0;

bool boolImpulse = true;
uint8_t impulseCount = 0;
void s1Impulse(){ Serial.println("s1"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 1; };
void s2Impulse(){ Serial.println("s2"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 2; };
void s3Impulse(){ Serial.println("s3"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 3; };
void s4Impulse(){ Serial.println("s4"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 4; };

bool boolClear = false;

unsigned long wireDuration = 0;
unsigned long lastFrame =   0;
unsigned int msPerFrame =  10;

void frame();

void receiveEvent(int howMany)
{
  while(Wire.available() > 0) {
  char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

// The snytax for memmove8 is:
// memmove8( &destination[start position], &source[start position], size of pixel data )
//
// The above is written as "size of pixel data" instead of "number
// of pixels" because in this case it would be the number of
// pixels x 3.  Three because our current CRGB has data for three
// color channels: RGB.
// In code we can write that as:  pixels * sizeof(CRGB).

// mirror 0->71 to 72->143
void mirror() {
  memmove8(     &leds[72],      &leds[0], 72 * sizeof(CRGB));
  memmove8(&secondary[ 3], &secondary[0],  3 * sizeof(CRGB));
}
// flip 0->71 to 71->0
void flip() {
  memmove8(&ledsTmpLow[0], &leds[0], (NUM_LEDS/2) * sizeof(CRGB));
  for (uint8_t i=0; i<NUM_LEDS/2; i++){
    memmove8(&leds[(NUM_LEDS/2)-1-i], &ledsTmpLow[i], sizeof(CRGB));
  }
}

uint8_t iH(uint8_t index){ // indexHelper
 if(index <= 71){
   return 71 - index;
 }
 return index;
}

int findUnused() {
  for (uint8_t i = 0; i < NUM_PIXELS; i++){
    if (Pixels[i].used){ continue; }
    return i;
  }
  return -1;
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
  //FastLED.setMaxPowerInVoltsAndMilliamps(MAX_POWER_VOLTS, MAX_POWER_MILLIAMPS);
  FastLED.setMaxRefreshRate(0);
  FastLED.clear();
  FastLED.show();

  Wire.begin(); 

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
  pinMode(5, INPUT_PULLUP); // reserved
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
//SimplePatternList gPatterns = {neontube, blocks, binaryCounter, glow};
//SimplePatternList gPatterns = {glow};
//SimplePatternList gPatterns = {fire};
//SimplePatternList gPatterns = {waterdrops};
//SimplePatternList gPatterns = {level};
//SimplePatternList gPatterns = {pixels2};
//SimplePatternList gPatterns = {snowflakes};
//SimplePatternList gPatterns = {snowflakes, pixels2};
SimplePatternList gPatterns = {binaryCounter};

//////////
// loop //
//////////
void loop() {
  if(millis() - lastEffectChange > 1000 * 60 * 10){
    lastEffectChange = millis();
    Serial.println("Changing effect ...");
    firstFrame = true;
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  }
  if(millis() - lastImpulseDecrease > 1000){
    lastImpulseDecrease = millis();
    if(impulseCount > 0){
      impulseCount--;
    }
  }
  if(readFromNano){
    EVERY_N_MILLISECONDS(250){
      unsigned long wireStartTime = millis();
      Wire.requestFrom(0x40, sizeof(vu.bytes));
      //    Serial.print((char)Wire.peek());
      if (Wire.available()) {
        int i = 0; while(Wire.available()) { vu.bytes[i] = Wire.read(); i++; }
        //Serial.print("<");
      }
      wireDuration = millis() - wireStartTime;
    }
  }
  //pitch = vu.pitch * -1;
  if(lastFrame + msPerFrame < millis()){
    lastFrame = millis();  
    frame();
  }
}
void frame(){
  // THIS IS JUST THE BEGINNING
  /////////////////////////////
  //gPatterns[gCurrentPatternNumber]();
  if(impulseCount <= 4){
     gPatterns[gCurrentPatternNumber](); 
  }else{
    //vumeter();
    binaryCounter();
  }
  firstFrame = false;
  //////////////////
  // THIS IS THE END
  if(!ledsEnabled){
    FastLED.clear();
  }
  //leds[  2] = CRGB::Green;
  //leds[141] = CRGB::Red;
  
  // debug
  unsigned long frameDuration = millis() - lastFrame;
  if(frameDuration > 2){ leds[frameDuration] = CRGB::Green;  }
  if(wireDuration  > 6){ leds[wireDuration]  = CRGB::Yellow; }
  if(impulseCount  > 0){ leds[impulseCount]  = CRGB::Blue;   }
  FastLED.show();
}
