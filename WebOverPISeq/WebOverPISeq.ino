// replace the SSID and password with the credentials of your WiFi
const char* ssid = "PI-net";
const char* password = "blubblub";

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE


// FastLED
#define DATA_PIN    6
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS    300

CRGB leds[NUM_LEDS];
CRGBSet defleds(leds, NUM_LEDS);
CRGBSet leds1(defleds(0,(NUM_LEDS / 5)-1));
CRGBSet leds2(defleds((NUM_LEDS / 5),(NUM_LEDS / 5)*2-1));
CRGBSet leds3(defleds((NUM_LEDS / 5)*2,(NUM_LEDS / 5)*3-1));
CRGBSet leds4(defleds((NUM_LEDS / 5)*3,(NUM_LEDS / 5)*3-1));
CRGBSet leds5(defleds((NUM_LEDS / 5)*4,(NUM_LEDS -1)));
CRGBSet all(defleds(0,(NUM_LEDS -1)));


/* int LEDperSEQ = NUM_LEDS / Seqs;
 * int Seqs = 5;
 * 
 * CRGB leds[NUM_LEDS];
 * CRGBSet defleds(leds, NUM_LEDS);
 * struct CRGB * ledsarry[] = new CRGBSet[5];
 * for(int i = 0; i < Seqs; i++){
 *    CRGBSet[i] = (defleds(i * LEDperSEQ, (i+1)*LEDperSEQ -1))
 * }
 */
struct CRGB * ledsarry[] ={leds1, leds2, leds3, leds4, leds5};

#define BRIGHTNESS         30
#define FRAMES_PER_SECOND  120

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;
int counter = 0;
int fader = 0;
#define TEMPERATURE UncorrectedTemperature

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

//UDP Connection
unsigned int localPort = 8888;      // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet

WiFiUDP Udp;

void startWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  Serial.print("Starting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Success! My Subnet IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("\nServer listening on port 8888");
}

void setup(void) {
  Serial.begin(115200);

  // start wifi
  //  xTaskCreatePinnedToCore(startWifi, "start_WiFi", 10000, NULL, 0, NULL, 0);
  startWifi();

  // fastled
  delay(2000);
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(BRIGHTNESS);
  Udp.begin(localPort);
}

typedef void (*Animations[])();
Animations animations = {confetti, rainbow, cloudSlowBeatWave, rainbow, sinelon, juggle, rainbow, bpm, Strobe, meteorRain, RGBLoop, FadeInOutR, FadeInOutG, FadeInOutB, FadeInOutW, black };

void loop(void) {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    String packet = String(packetBuffer);
  	switchPattern((packet).toInt());
  }
  
  //Alle +-5 Minuten wird die Animation gewechselt
  animations[currentAnimation]();
  counter += 1;
  if(counter >= 30000){
    currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
    counter = 0;
  }
  if(fader == 255){
    fader += 255;
  }
  if(fader == 256){
    fader -= 255;
  }
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}


void switchPattern(int criteria){
  switch (criteria) {
    case 0:    //confetti
      currentAnimation = criteria;
      break;
    case 1:    //rainbow
      currentAnimation = criteria;
      break;
    case 2:    //cloudSlowBeatWave
      currentAnimation = criteria;
      break;
    case 3:    //rainbowBT
      //currentAnimation = criteria;
      break;
    case 4:    //sinelon
      currentAnimation = criteria;
      break;
    case 5:    //juggle
      currentAnimation = criteria;
      break;
    case 6:    //rainbowRT
      //currentAnimation = criteria;
      break;
    case 7:    //bpm
      currentAnimation = criteria;
      break;
    case 8:    //Strobe
      currentAnimation = criteria;
      break;
    case 9:    //meteorRain
      currentAnimation = criteria;
      break;
    case 10:  //RGBLoop
      currentAnimation = criteria;
      break;
    case 11:  //FadeInOutR
      currentAnimation = criteria;
      break;
    case 12:  //FadeInOutG
      currentAnimation = criteria;
      break;
    case 13:  //FadeInOutB
      currentAnimation = criteria;
      break;
    case 14:  //FadeInOutW
      currentAnimation = criteria;
      break;
    case 100:    //next
      currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
      break;
    case 101:    //last
      currentAnimation = (currentAnimation - 1) % ARRAY_SIZE(animations);
      break;
    case 201:    //Brightness 8
      FastLED.setBrightness(8);
      break;
    case 202:    //Brightness 16
      FastLED.setBrightness(16);
      break;
    case 203:    //Brightness 32
      FastLED.setBrightness(32);
      break;
    case 204:    //Brightness 64
      FastLED.setBrightness(64);
      break;
    case 205:    //Brightness 100
      FastLED.setBrightness(100);
      break;
    case 500:
      #define TEMPERATURE UncorrectedTemperature
      break;
    case 501:
      #define TEMPERATURE Candle
      break;
    case 502:
      #define TEMPERATURE Tungsten40W
      break;
    case 503:
      #define TEMPERATURE Tungsten100W
      break;
    case 504:
      #define TEMPERATURE Halogen
      break;
    case 505:
      #define TEMPERATURE CarbonArc
      break;
    case 506:
      #define TEMPERATURE HighNoonSun
      break;
    case 507:
      #define TEMPERATURE DirectSunlight
      break;
    case 508:
      #define TEMPERATURE OvercastSky
      break;
    case 509:
      #define TEMPERATURE ClearBlueSky
      break;
    case 510:
      #define TEMPERATURE WarmFluorescent
      break;
    case 511:
      #define TEMPERATURE StandardFluorescent
      break;
    case 512:
      #define TEMPERATURE CoolWhiteFluorescent
      break;
    case 513:
      #define TEMPERATURE FullSpectrumFluorescent
      break;
    case 514:
      #define TEMPERATURE GrowLightFluorescent
      break;
    case 515:
      #define TEMPERATURE BlackLightFluorescent
      break;
    case 516:
      #define TEMPERATURE MercuryVapor
      break;
    case 517:
      #define TEMPERATURE SodiumVapor
      break;
    case 518:
      #define TEMPERATURE MetalHalide
      break;
    case 519:
      #define TEMPERATURE HighPressureSodium
      break;
    case 1000:  //Black, nothing
      currentAnimation = ARRAY_SIZE(animations) -1;
      break;
    default:
      break;
  }
  counter = 0;
}

void rainbow() {
  // cycle forward with a rainbow
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, ++hue, 5);
  FastLED.setTemperature(TEMPERATURE);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  static uint8_t hue = 0;
  leds[pos] += CHSV(--hue + random8(64), 200, 255);
  FastLED.setTemperature(TEMPERATURE);
}

void sinelon() {
  for(int i=0; i < 5; i++){
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(ledsarry[i], 60, 20);
    int pos = beatsin16(13, 0, 59 );
    static uint8_t hue = 0;
    ledsarry[i][pos] += CHSV(++hue, 255, 192);
  }
  FastLED.setTemperature(TEMPERATURE);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 25;
  CRGBPalette16 palette = OceanColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  static uint8_t hue = 0;
  hue--;
  for (int i = 0; i < NUM_LEDS; i++) {
    int x = beat-hue+i*10;
    if (x < 0) x = NUM_LEDS - x;
    leds[i] = ColorFromPalette(palette, hue+i*2 % NUM_LEDS, x);
  }
  FastLED.setTemperature(TEMPERATURE);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for(int i = 0; i < 8; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.setTemperature(TEMPERATURE);
}

void cloudSlowBeatWave() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int colorIndex = beat8(30  + i % 40) + ease8InOutQuad(i + NUM_LEDS);
    leds[i] = ColorFromPalette(CloudColors_p, colorIndex, NUM_LEDS, LINEARBLEND);
  }
  FastLED.setTemperature(TEMPERATURE);
}

void Strobe(){
  byte red = 0xff; byte green = 0xff; byte blue = 0xff; int StrobeCount = 10; int FlashDelay = 50; int EndPause = 1000;
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void black(){
  setAll(0,0,0);
}

void meteorRain() {
  byte red = 0xff; byte green = 0x00; byte blue = 0x12; byte meteorSize = 10; byte meteorTrailDecay = 64; boolean meteorRandomDecay = true; int SpeedDelay = 30;
  setAll(0,0,0);
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      } 
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

// used by meteorrain
void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void RGBLoop(){
  for(int j = 0; j < 3; j++ ) { 
    // Fade IN
    for(int k = 0; k < 256; k++) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
  }
}

void FadeInOutR(){
  if(fader < 255){
    FadeIn(255, 0, 0);
  }else{
    FadeOut(255, 0, 0);
  }
}

void FadeInOutW(){
  if(fader < 255){
    FadeIn(255, 255, 255);
  }else{
    FadeOut(255, 255, 255);
  }
}

void FadeInOutB(){
  if(fader < 255){
    FadeIn(0, 0, 255);
  }else{
    FadeOut(0, 0, 255);
  }
}

void FadeInOutG(){
  if(fader < 255){
    FadeIn(0, 255, 0);
  }else{
    FadeOut(0, 255, 0);
  }
}

void FadeIn(int red, int green, int blue){
  float r, g, b;
  r = (fader/256.0)*red;
  g = (fader/256.0)*green;
  b = (fader/256.0)*blue;
  setAll(r,g,b);
  showStrip();
  fader += 1;
}

void FadeOut(int red, int green, int blue){
  float r, g, b;
  r = ((fader-255)/256.0)*red;
  g = ((fader-255)/256.0)*green;
  b = ((fader-255)/256.0)*blue;
  setAll(r,g,b);
  showStrip();
  fader -= 2;
}

// ***************************************
// ** FastLed/NeoPixel Common Functions **
// ***************************************

// Apply LED color changes
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

// Set a LED color (not yet visible)
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
