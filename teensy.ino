#include "Globals.h"
#include "Leds.h"
#include "Image.h"
#include "Effect.h"

#include <stdlib.h> // for malloc and free
//void* operator new(size_t size) { return malloc(size); }
//void operator delete(void* ptr) { free(ptr); } 

// https://github.com/Richard-Gemmell/teensy4_i2c
// git clone git@github.com:Richard-Gemmell/teensy4_i2c.git
// | Port | Pins               | imx_rt1060_i2c_driver.h | i2c_driver_wire.h |
// | ---- |--------------------| ------------------|-------|
// | 0    | SCL0(19), SDA0(18) | Master or Slave   | Wire  |
// | 1    | SCL1(16), SDA1(17) | Master1 or Slave1 | Wire1 |
// | 2    | SCL2(24), SDA2(25) | Master2 or Slave2 | Wire2 |
// The I2C protocol uses open drain pins to pull signal voltages low and pull up resistors to pull voltages back up again.
// This library enables the Teensy's 22 kΩ internal pull up resistor by default.
// Many sensors that support I2C often have internal pullups as well. These may provide enough resistance for your application.
// If you don't have any internal pullups or the internal resistors aren't big enough for your application then you'll need to use external pullups.
// I recommend 2.2 kΩ or 1 kΩ.
#include <i2c_driver.h>       // SCL 19 YELLOW
#include <i2c_driver_wire.h>  // SDA 18 GREEN

#include <SPI.h>


Effect* ptrEffect = NULL;

void receiveEvent(int howMany);

//#include <SD.h>
//#include <SPI.h>
//int chipSelect = BUILTIN_SDCARD;

CRGB        leds[NUM_LEDS];
CRGB   secondary[NUM_LEDS_SECONDARY];

CRGB     ledsTmp[NUM_LEDS];
CRGB  ledsTmpLow[NUM_LEDS / 2];
CRGB ledsTmpHigh[NUM_LEDS / 2];
CRGB   bufferBig[NUM_LEDS * 3];

Pixel Pixels[NUM_PIXELS];
bool blockingLookup[NUM_LEDS];

int8_t pitch;
union vu_ vu;
//union I2Cdata_ I2Cdata;

bool useSerial = true;
bool ledsEnabled = true; // true;

uint8_t gHue = 0;
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


unsigned long wireDuration = 0;

// vu data
float  filteredLeftFast[7];
float filteredRightFast[7];
float  filteredLeftSlow[7];
float filteredRightSlow[7];

void receiveEvent(int howMany)
{
  uint8_t packetType = Wire.read();
  if(packetType == 1 && howMany == 2){
     uint8_t effect = Wire.read();
     Serial.println("[PACKET] effect: " + String(effect)); 
  }else if(packetType == 2 && howMany == 2){
     pitch = Wire.read();
     Serial.println("[PACKET] pitch: " + String(pitch)); 
  }else if(packetType == 3 && howMany == 1 + sizeof(vu.bytes)){
    int i = 0;
    while(Wire.available()) {
      vu.bytes[i] = Wire.read();
      i++;
    }
    vuFilter();
    Serial.println("[PACKET] vu"); 
  }
  // empty the buffer just to be safe?
  while(Wire.available() > 0) {
    Wire.read();
  }
}

void setup() {
  delay(1000); if(useSerial){ Serial.begin(115200); }; delay(1000);
  if(useSerial && !Serial){ delay(1000); if(!Serial){useSerial = false; }; }
  if(useSerial){ Serial.println("Starting up ..."); };
  // DATA_RATE_MHZ(24)++
  FastLED.addLeds<SK9822,       11, 13, BGR>(     leds,                      0, NUM_LEDS / 2          ).setCorrection(TypicalLEDStrip); // DATA 11 GREEN, CLOCK 13 YELLOW
  FastLED.addLeds<WS2812SERIAL, 14,     BRG>(secondary,                      0, NUM_LEDS_SECONDARY / 2).setCorrection(TypicalLEDStrip); // DATA 14 WHITE
  
  FastLED.addLeds<SK9822,       26, 27, BGR>(     leds,           NUM_LEDS / 2, NUM_LEDS / 2          ).setCorrection(TypicalLEDStrip); // DATA 26 GREEN, CLOCK 27 YELLOW
  FastLED.addLeds<WS2812SERIAL, 17,     BRG>(secondary, NUM_LEDS_SECONDARY / 2, NUM_LEDS_SECONDARY / 2).setCorrection(TypicalLEDStrip); // DATA 17 WHITE
  
  
  FastLED.setBrightness(MAX_BRIGHTNESS); // PWM duty cycles
  //FastLED.setMaxPowerInVoltsAndMilliamps(MAX_POWER_VOLTS, MAX_POWER_MILLIAMPS);
  FastLED.setMaxPowerInMilliWatts(MAX_POWER_MILLIWATTS);
  //set_max_power_indicator_LED(LED_BUILTIN);
  //set_max_power_indicator_LED(13);
  FastLED.setMaxRefreshRate(0);
  FastLED.clear();
  FastLED.show();

  // SPI
  //pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  //SPI.begin(); 

  // I2C
  /*
  Wire.begin(); 
  Wire.setClock(1000000);
  */
  Wire.begin(0x40);                // join i2c bus with address #4
  Wire.setClock(1000000);
  Wire.onReceive(receiveEvent); // register event
  //Wire.onRequest(requestEvent);
 
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
//SimplePatternList gPatterns = {ledTest};
//SimplePatternList gPatterns = {effectTest};
//SimplePatternList gPatterns = {effectTest2};
//SimplePatternList gPatterns = {effectTest3};
//SimplePatternList gPatterns = {effectTest4};
//SimplePatternList gPatterns = {effectNoise};
//SimplePatternList gPatterns = {effectMetaballs};
//SimplePatternList gPatterns = {effectHeat};

//SimplePatternList gPatterns = {effectLevel};
//SimplePatternList gPatterns = {effectBinaryCounter};

//SimplePatternList gPatterns = {effectFire};
//SimplePatternList gPatterns = {effectPixels};
//SimplePatternList gPatterns = {effectIce};
//SimplePatternList gPatterns = {effectMatrix};

//SimplePatternList gPatterns = {effectFire, effectPixels, effectIce, effectMatrix};
//SimplePatternList gPatterns = {effectAirplane};


SimplePatternList gPatterns = {idleFluorescentTube, neontube};
//SimplePatternList gPatterns = {idleRotation};
//SimplePatternList gPatterns = {idlePulsating};


//SimplePatternList gPatterns = {glow, pixels2, vumeter2, snowflakes, binaryCounter};

//SimplePatternList gPatterns = {lavalamp};
//SimplePatternList gPatterns = {blocks};
//SimplePatternList gPatterns = {rainbowSin};
//SimplePatternList gPatterns = {idleFluorescentTube, idleRotation};




//SimplePatternList gPatterns = {effectPalette};

//////////
// loop //
//////////
void loop() {
  //if(vuChange){ Serial.println("--- VU CHANGE ---"); }
  if(vuChange || (ARRAY_SIZE(gPatterns) > 1 && millis() - lastEffectChange > 1000 * 20)){ // change effect if vu signal starts/ends or every n minutes if there are more than 1 effects configured
    Serial.println("Changing effect ...");    
    //ptrEffect = new Effect();
    //ptrEffect->init();

    lastEffectChange = millis();
    firstFrame = true;
    fCount = 0;
    if(vuChange){ vuChange = false; };

    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(MAX_POWER_VOLTS, MAX_POWER_MILLIAMPS);

    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  }
  if(millis() - lastImpulseDecrease > 1000){
    lastImpulseDecrease = millis();
    if(impulseCount > 0){
      impulseCount--;
    }
  }
  /*
  if(readFromNano && millis() - lastWireTime > 50){
    lastWireTime = millis();
    unsigned long wireStartTime = millis();
    //Wire.requestFrom(0x40, sizeof(vu.bytes));
    //    Serial.print((char)Wire.peek());

    uint8_t packetType = 0;
    Wire.requestFrom(0x40, 1);
    while(Wire.available()){
      packetType = Wire.read();    // receive a byte as character
    }
    switch (packetType) {
      case 0:
        break;
      case 1:
        Serial.println("[packet] effect");
        Wire.requestFrom(0x40, 1);
        while(Wire.available()){
          Wire.read();
        }
      case 3:
        Serial.println("[packet] pitch");
        Wire.requestFrom(0x40, 1);
        while(Wire.available()){
          Wire.read();
        }
        break;
      default:
        Serial.println("[packet] ???"); 
        break;
    }
*/
/*
    uint8_t packetType = (uint8_t)Wire.peek();
    if(packetType == 3){
      Serial.println("[packet] pitch");  
    }else{
      Serial.println("[packet] " + String(packetType));  
    }
*/
    /*
    Wire.requestFrom(0x40, sizeof(I2Cdata.bytes));
    if (Wire.available()) {
      int i = 0; while(Wire.available()) { I2Cdata.bytes[i] = Wire.read(); i++; }
    }
    memcpy8(&vu.bytes[0], &I2Cdata.bytes[0], sizeof vu.bytes); // copy vu data from the I2C union over to the vu union
    vuFilter();
    */

    
    //wireDuration = millis() - wireStartTime;
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

  if(vuSignal){
    effectVumeter();
  }else if(impulseCount <= 4){
    gPatterns[gCurrentPatternNumber](); 
  }else{
    effectBinaryCounter();
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
  /////////////////////if(readFromNano){ leds[19] = CRGB::Green; } else { leds[19] = CRGB::Red; }
  //if(vuSignal){ leds[20] = CRGB::Blue; } else { leds[20] = CRGB::Red; }
  //leds[71]  = CHSV( 64, 255, 64); // bottom indicator
  FastLED.show();
}
