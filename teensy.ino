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

//#include <SPI.h>


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

/////////////
// packets //
/////////////
union effectPacket_            effectPacket;
union pitchPacket_              pitchPacket;
union vuPacket_                    vuPacket;
union pitchRemotePacket_  pitchRemotePacket;

bool useSerial = true;
bool ledsEnabled = true; // true;

uint8_t gHue = 0;
unsigned int msPerFrame =  10;
unsigned long lastFrame =   0;
bool boolRender = true;
unsigned long fCount = 0;

bool firstFrame = true; // set for the first frame of an effect

bool boolNewEffect = false;
bool boolIdle = true;
unsigned long lastEffectChange = 0;

bool vuChange = false;
bool vuSignal = false;

/*
bool boolImpulse = true;
uint8_t impulseCount = 0;
void s1Impulse(){ Serial.println("s1"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 1; };
void s2Impulse(){ Serial.println("s2"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 2; };
void s3Impulse(){ Serial.println("s3"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 3; };
void s4Impulse(){ Serial.println("s4"); lastImpulse = millis(); if(impulseCount <= 6){ impulseCount++; }; boolImpulse = true; state = 4; };
void vuImpulse(){ vuSignal = !digitalRead(5); vuChange = true; };
*/

bool boolVertical = false;
bool boolUp       = false;

void pinN3_T9() {                                Serial.println( "pinN3_T9"); };
void pinN4_T8() {                                Serial.println( "pinN4_T8"); };
void pinN5_T7() {                                Serial.println( "pinN5_T7"); };
void pinN6_T6() {                                Serial.println( "pinN6_T6"); };
void pinN7_T5() {                                Serial.println( "pinN7_T5"); };
void pinN8_T4() {                                Serial.println( "pinN8_T4"); };
void pinN9_T3() { boolVertical = digitalRead(3); Serial.println( "pinN9_T3"); };
void pinN10_T2(){ boolUp       = digitalRead(2); Serial.println("pinN10_T2"); };

unsigned long wireDuration = 0;

// vu data
float  filteredLeftFast[7];
float filteredRightFast[7];
float  filteredLeftSlow[7];
float filteredRightSlow[7];

//   //}else if(packetType == 3 && howMany == 1 + sizeof(vuPacket.bytes)){
void receiveEvent(int howMany)
{
  //uint8_t packetType = Wire.read();
  uint8_t packetType = Wire.peek();
  
  if(packetType == 1){ // effect
    //leds[20] = CRGB::Red;
    int i = 0; while(Wire.available()){ effectPacket.bytes[i] = Wire.read(); i++; }
    ////////////////
    /// ON EFFECT //
    boolNewEffect = true;
    boolIdle = false;
    lastEffectChange = millis();
    Serial.println(effectPacket.effect);
    /// ON EFFECT //
    ////////////////
  }else if(packetType == 2){ // pitch
    //leds[21] = CRGB::Green;
    int i = 0; while(Wire.available()){ pitchPacket.bytes[i] = Wire.read(); i++; }
  }else if(packetType == 3){ // vu
    //leds[22] = CRGB::Blue;
    int i = 0; while(Wire.available()){ vuPacket.bytes[i] = Wire.read(); i++; }
    vuFilter();
  }else if(packetType == 4){ // pitchRemote
    //leds[23] = CRGB::Green;
    int i = 0; while(Wire.available()){ pitchRemotePacket.bytes[i] = Wire.read(); i++; }
    int8_t pitchRemoteConstrained = constrain(pitchRemotePacket.pitch, -45, 45);
    uint8_t pitchRemoteLed = map(pitchRemoteConstrained, -45, 45, 143, 0);
    if(pitchRemoteLed <= 71){ pitchRemoteLed = 71 - pitchRemoteLed; }
    //leds[pitchRemoteLed] = CRGB::Orange;
  }
  // empty the buffer just to be safe?
  while(Wire.available() > 0) {
    Serial.println("Should not see this???");  
    Wire.read();
  }
}

void setup() {
  delay(1000); if(useSerial){ Serial.begin(115200); }; delay(1000);
  if(useSerial && !Serial){ delay(1000); if(!Serial){useSerial = false; }; }

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
 
  /*
  pinMode(5, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(5), vuImpulse, CHANGE);
  pinMode(6, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(6), s4Impulse, FALLING);
  pinMode(7, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(7), s3Impulse, FALLING);
  */
  //pinMode(4, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(4), pinN8_T4, CHANGE);
  pinMode(3, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(3), pinN9_T3, CHANGE);
  pinMode(2, INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(2), pinN10_T2, CHANGE);
  
  Serial.println("Setup finished ...");
}

uint8_t currentEffect = 0;
uint8_t gCurrentVerticalPatternNumber = 0;
uint8_t gCurrentSpinningPatternNumber = 0;
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
typedef void (*patternList[])();

// <-------------------------------------------------------------------------------------------------------------------------- PATTERN LIST ----------------------------------------------------------------------

// effectIce
//patternList patternListVertical = {effectFire, effectMatrix};
patternList patternListVertical = {effectMatrix};
//patternList patternListVertical = {effectBinaryCounter, effectPixels};
patternList patternListSpinning = {idleRotation, idlePulsating, idleFluorescentTube, neontube};
//                              0           1             2             3                    4          5
patternList effects = {effectNone, effectFire, effectMatrix, effectPixels, effectBinaryCounter, effectIce};

//////////
// loop //
//////////
void loop() {
  //if(vuChange){ Serial.println("--- VU CHANGE ---"); }
  //if(vuChange || (ARRAY_SIZE(gPatterns) > 1 && millis() - lastEffectChange > 1000 * 20)){ // change effect if vu signal starts/ends or every n minutes if there are more than 1 effects configured
  //if(millis() - lastEffectChange > 1000 * 20){
 
  if(!boolIdle && millis() - lastEffectChange > 1000 * effectPacket.duration){
    boolNewEffect = true;
    boolIdle = true;
  }
  if(boolNewEffect){
    boolNewEffect = false;
    Serial.println("Changing effect ...");    
    //ptrEffect = new Effect();
    //ptrEffect->init();

    firstFrame = true;
    fCount = 0;
    //if(vuChange){ vuChange = false; };
    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(MAX_POWER_VOLTS, MAX_POWER_MILLIAMPS);
  }
  if(boolRender){
    renderFrame();
    boolRender = false;
  }
  if(lastFrame + msPerFrame < millis()){

//leds[0] = CRGB::Green; 
//leds[NUM_LEDS - 1] = CRGB::Red; 

if(boolVertical){
  //leds[9] = CRGB::Green;
} else {
  //leds[9] = CRGB::Red;
}
    
    FastLED.show();
    lastFrame = millis();  
    fCount++;
    boolRender = true;
  }

}
void renderFrame(){
  ////////////////////////////////
  // THIS IS JUST THE BEGINNING //
  ////////////////////////////////

  //ptrEffect->runPattern();
  //ptrEffect->runPattern();
  //pixels2();

/*
  if(vuSignal){
    effectVumeter();
  }else if(impulseCount <= 4){
    gPatterns[gCurrentPatternNumber](); 
  }else{
    effectBinaryCounter();
  }
*/

  
  //if(boolVertical){
  //  patternListVertical[gCurrentVerticalPatternNumber]();
  //}else{
  //  patternListSpinning[gCurrentSpinningPatternNumber]();
  //}
  if(boolIdle){
    effectNone();
  }else{
    effects[effectPacket.effect]();
  }
  firstFrame = false;
  /////////////////////
  // THIS IS THE END //
  /////////////////////
}
