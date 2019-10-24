// Testing the CRGBSet array function and using it in an array of CRGBSET arrays
 
#include "FastLED.h"
#define NUM_LEDS 36
 
#define Data_Pin 6
 
CRGB rawleds[NUM_LEDS];
CRGBSet leds(rawleds, NUM_LEDS);
CRGBSet leds1(leds(0,11));
CRGBSet leds2(leds(12,23));
CRGBSet leds3(leds(24,35));
CRGBSet all(leds(0,35));
 
struct CRGB * ledsarry[] ={leds1, leds2, leds3};
 
 
void setup() {
 delay(2000); // power-up safety delay
 FastLED.addLeds<NEOPIXEL, Data_Pin>(leds, NUM_LEDS);
 FastLED.setBrightness(50);
 
}
void loop() {
 
  
     for (uint8_t y=0; y<12; y++){
       if (y<6){
        ledsarry[0][y]=CRGB::Red;
        ledsarry[1][y]=CRGB::Red;
        ledsarry[2][y]=CRGB::Red;
       }
       
      else
      {
        ledsarry[0][y]=CRGB::Blue;
        ledsarry[1][y]=CRGB::Blue;
        ledsarry[2][y]=CRGB::Blue;
      }
     FastLED.show();
      delay(200);
      ledsarry[0][y]=CRGB::Black;
      ledsarry[1][y]=CRGB::Black;
      ledsarry[2][y]=CRGB::Black;
     FastLED.show();
     }  
     
}
