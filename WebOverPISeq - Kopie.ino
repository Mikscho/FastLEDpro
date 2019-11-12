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
CRGBSet leds1(defleds(0,(NUM_LEDS / 3)-1));
CRGBSet leds2(defleds((NUM_LEDS / 3),(NUM_LEDS / 3)*2-1));
CRGBSet leds3(defleds((NUM_LEDS / 3)*2,(NUM_LEDS / 3)*3-1));
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
struct CRGB * ledsarry[] ={leds1, leds2, leds3};

#define BRIGHTNESS         30
#define FRAMES_PER_SECOND  120

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;
int counter = 0;
int fader = 0;

CRGBPalette16 currentPalette;
ColorTemperature Temperature = UncorrectedTemperature;

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
Animations animations = {confetti, rainbow, cloudSlowBeatWave, fire, sinelon, juggle, CylonBounce, bpm, Strobe, meteorRain, RGBLoop, FadeInOutR, FadeInOutG, FadeInOutB, FadeInOutW, twinkle, TwinkleRandom, black };

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
    case 3:    //Fire
      currentAnimation = criteria;
      break;
    case 4:    //sinelon
      currentAnimation = criteria;
      break;
    case 5:    //juggle
      currentAnimation = criteria;
      break;
    case 6:    //CylonBounce
      currentAnimation = criteria;
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
    case 15:  //Twinkle
      currentAnimation = criteria;
      break;
    case 16:  //TwinkleRandom
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
      Temperature = UncorrectedTemperature;
      break;
    case 501:
      Temperature = Candle;
      break;
    case 502:
      Temperature = Tungsten40W;
      break;
    case 503:
      Temperature = Tungsten100W;
      break;
    case 504:
      Temperature = Halogen;
      break;
    case 505:
      Temperature = CarbonArc;
      break;
    case 506:
      Temperature = HighNoonSun;
      break;
    case 507:
      Temperature = DirectSunlight;
      break;
    case 508:
      Temperature = OvercastSky;
      break;
    case 509:
      Temperature = ClearBlueSky;
      break;
    case 510:
      Temperature = WarmFluorescent;
      break;
    case 511:
      Temperature = StandardFluorescent;
      break;
    case 512:
      Temperature = CoolWhiteFluorescent;
      break;
    case 513:
      Temperature = FullSpectrumFluorescent;
      break;
    case 514:
      Temperature = GrowLightFluorescent;
      break;
    case 515:
      Temperature = BlackLightFluorescent;
      break;
    case 516:
      Temperature = MercuryVapor;
      break;
    case 517:
      Temperature = SodiumVapor;
      break;
    case 518:
      Temperature = MetalHalide;
      break;
    case 519:
      Temperature = HighPressureSodium;
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
  FastLED.setTemperature(Temperature);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  static uint8_t hue = 0;
  leds[pos] += CHSV(--hue + random8(64), 200, 255);
  FastLED.setTemperature(Temperature);
}

void sinelon() {
  for(int i=0; i < 3; i++){
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(ledsarry[i], 100, 20);
    int pos = beatsin16(13, 0, 99 );
    static uint8_t hue = 0;
    ledsarry[i][pos] += CHSV(++hue, 255, 192);
  }
  FastLED.setTemperature(Temperature);
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
  FastLED.setTemperature(Temperature);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for(int i = 0; i < 8; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.setTemperature(Temperature);
}

void cloudSlowBeatWave() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int colorIndex = beat8(30  + i % 40) + ease8InOutQuad(i + NUM_LEDS);
    leds[i] = ColorFromPalette(CloudColors_p, colorIndex, NUM_LEDS, LINEARBLEND);
  }
  FastLED.setTemperature(Temperature);
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
 FastLED.setTemperature(Temperature);
 delay(EndPause);
}

void black(){
  setAll(0,0,0);
}

void fire() {
  int Cooling = 55;
  int Sparking = 120;
  int SpeedDelay = 15;
  static byte heat[NUM_LEDS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  FastLED.setTemperature(Temperature);
  showStrip();
  delay(SpeedDelay);
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
  FastLED.setTemperature(UncorrectedTemperature);
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

void CylonBounce(){
  byte red = 0xff; byte green = 0xff; byte blue = 0xff; int EyeSize = 4; int SpeedDelay = 10; int ReturnDelay = 50;
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  FastLED.setTemperature(Temperature);
  delay(ReturnDelay);
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
  FastLED.setTemperature(Temperature);
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
  FastLED.setTemperature(Temperature);
  showStrip();
  fader += 1;
}

void FadeOut(int red, int green, int blue){
  float r, g, b;
  r = ((fader-255)/256.0)*red;
  g = ((fader-255)/256.0)*green;
  b = ((fader-255)/256.0)*blue;
  setAll(r,g,b);
  FastLED.setTemperature(Temperature);
  showStrip();
  fader -= 2;
}

void twinkle() {
  byte red = 0xff; byte green = 0xff; byte blue = 0xff; int Count = 10; int SpeedDelay = 100; boolean OnlyOne = false;
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  FastLED.setTemperature(Temperature);
  delay(SpeedDelay);
}

void TwinkleRandom() {
  int Count = 20; int SpeedDelay = 100; boolean OnlyOne = false;
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  FastLED.setTemperature(Temperature);
  delay(SpeedDelay);
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

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}
