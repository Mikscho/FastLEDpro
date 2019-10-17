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
#define NUM_LEDS    36
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         30
#define FRAMES_PER_SECOND  120

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;

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
  Serial.println("\nServer listening on port 80");
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
Animations animations = { confetti, rainbow, cloudSlowBeatWave, rainbowBlueTone, sinelon, juggle, rainbowRedTone, bpm, Strobe, meteorRain };

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
    if(packetBuffer != "favicon.ico"){
      String packet = String(packetBuffer);
  	  switchPattern((packet).toInt());
    }
  }

  animations[currentAnimation]();
  
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
      currentAnimation = criteria;
      break;
    case 4:    //sinelon
      currentAnimation = criteria;
      break;
    case 5:    //juggle
      currentAnimation = criteria;
      break;
    case 6:    //rainbowRT
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
    default:
      break;
  }
}

void rainbow() {
  // cycle forward with a rainbow
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, ++hue, 5);
  FastLED.setTemperature(UncorrectedTemperature);
}

void rainbowBlueTone() {
  // cycle backward with a rainbow of rather short color slices and blue tone
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, --hue, 23);
  FastLED.setTemperature(ClearBlueSky);
}

void rainbowRedTone() {
  // cycle backward with a rainbow of rather short color slices and red tone
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, --hue, 18);
  FastLED.setTemperature(Tungsten100W);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  static uint8_t hue = 0;
  leds[pos] += CHSV(--hue + random8(64), 200, 255);
  FastLED.setTemperature(UncorrectedTemperature);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-1 );
  static uint8_t hue = 0;
  leds[pos] += CHSV(++hue, 255, 192);
  FastLED.setTemperature(UncorrectedTemperature);
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
  FastLED.setTemperature(UncorrectedTemperature);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for(int i = 0; i < 8; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.setTemperature(UncorrectedTemperature);
}

void cloudSlowBeatWave() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int colorIndex = beat8(30  + i % 40) + ease8InOutQuad(i + NUM_LEDS);
    leds[i] = ColorFromPalette(CloudColors_p, colorIndex, NUM_LEDS, LINEARBLEND);
  }
  FastLED.setTemperature(UncorrectedTemperature);
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
