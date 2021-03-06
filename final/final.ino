// replace the SSID and password with the credentials of your WiFi
const char* ssid = "PI-net";
const char* password = "blubblub";

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define DATA_PIN    6
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS    212

CRGB leds[NUM_LEDS];
CRGBSet defleds(leds, NUM_LEDS);
CRGBSet ledBL(defleds(0, 41));
CRGBSet ledAOL(defleds(42, 57));
CRGBSet ledAUL(defleds(58, 73));
CRGBSet ledVL(defleds(74, 105));
CRGBSet ledVR(defleds(106, 137));
CRGBSet ledAOR(defleds(138, 153));
CRGBSet ledAUR(defleds(154, 169));
CRGBSet ledBR(defleds(170, 211));

struct CRGB * ledsarry[] ={ledBL, ledAOL, ledAUL, ledVL, ledVR, ledAOR, ledAUR, ledBR};

#define BRIGHTNESS         50
#define FRAMES_PER_SECOND  120

uint8_t currentAnimation = 0;
int counter = 0;
int fader = 0;
boolean faderup = true;
CRGB currcolor = CRGB::White;

CRGBPalette16 currentPalette = RainbowColors_p;
ColorTemperature Temperature = UncorrectedTemperature;

//For Function FadeToMiddle
const int barCount = 8; 
int bars[barCount];

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
  startWifi();

  // fastled
  delay(2000);
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(BRIGHTNESS);
  Udp.begin(localPort);

  //For Function FadeToMiddle
  for (int i = 0; i < barCount; i++) {bars[i] = i * ((NUM_LEDS * 16) / barCount);}
}

typedef void (*Animations[])();
Animations animations = {confetti, rainbow, cloudSlowBeatWave, fire, sinelon, juggle, CylonBounce, bpm, Strobe, meteorRain, RGBLoop, FadeInOut, simpleColor, sunriseset, american, twinkle, TwinkleRandom, setsimple, stroberandom, plasma, PoliceSiren, FadeToMiddle, Lightning, black };

int recPack(){
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
    if(packet){return (packet).toInt();}
  }
  return 1000;
}

void loop(void) {
  recPack();
  //Alle +-5 Minuten wird die Animation gewechselt
  animations[currentAnimation]();
  counter += 1;
  if(counter >= 30000){
    currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
    counter = 0;
  }
  if(fader >= 255){
    fader = 0;
    if(faderup = true){faderup = false;}
    if(faderup = false){faderup = true;}
  }
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}


void switchPattern(int criteria){
  if(criteria < 99){
    currentAnimation = criteria;
    black();
  }
  switch (criteria) {
    case 100:    //next
      currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
      break;
    case 101:    //last
      currentAnimation = (currentAnimation - 1) % ARRAY_SIZE(animations);
      break;
    case 201:    //Brightness 20%
      FastLED.setBrightness(20);
      break;
    case 202:    //Brightness 40%
      FastLED.setBrightness(40);
      break;
    case 203:    //Brightness 60%
      FastLED.setBrightness(60);
      break;
    case 204:    //Brightness 80%
      FastLED.setBrightness(80);
      break;
    case 205:    //Brightness 100%
      FastLED.setBrightness(100);
      break;
    case 301:
    //https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv
      currcolor = CRGB::White;
      break;
    case 302:
      currcolor = CRGB::Amethyst;
      break;
    case 303:
      currcolor = CRGB::Aqua;
      break;
    case 304:
      currcolor = CRGB::Aquamarine;
      break;
    case 305:
      currcolor = CRGB::Beige;
      break;
    case 306:
      currcolor = CRGB::Blue;
      break;
    case 307:
      currcolor = CRGB::BlueViolet;
      break;
    case 308:
      currcolor = CRGB::Brown;
      break;
    case 309:
      currcolor = CRGB::CadetBlue;
      break;
    case 310:
      currcolor = CRGB::Coral;
      break;
    case 311:
      currcolor = CRGB::Crimson;
      break;
    case 312:
      currcolor = CRGB::DarkGreen;
      break;
    case 313:
      currcolor = CRGB::DarkRed;
      break;
    case 314:
      currcolor = CRGB::DeepPink;
      break;
    case 315:
      currcolor = CRGB::Gold;
      break;
    case 316:
      currcolor = CRGB::Green;
      break;
    case 317:
      currcolor = CRGB::GreenYellow;
      break;
    case 318:
      currcolor = CRGB::Navy;
      break;
    case 319:
      currcolor = CRGB::Tomato;
      break;
    case 320:
      currcolor = CRGB::Yellow;
      break;
    case 401:
      currentPalette = CloudColors_p;
      break;
    case 402:
      currentPalette = LavaColors_p;
      break;
    case 403:
      currentPalette = OceanColors_p;
      break;
    case 404:
      currentPalette = ForestColors_p;
      break;
    case 405:
      currentPalette = PartyColors_p;
      break;
    case 406:
      currentPalette = HeatColors_p;
      break;
    case 407:
      currentPalette = RainbowColors_p;
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

/*void sinelon() {
  int arraysize = ARRAY_SIZE(ledsarry);
  for(int i=0; i < arraysize; i++){
    // a colored dot sweeping back and forth, with fading trails
    int striplength = ARRAY_SIZE(ledsarry[i]);
    Callsinelon(ledsarry[i], striplength);
  }
  FastLED.setTemperature(Temperature);
}*/

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-1 );
  static uint8_t hue = 0;
  leds[pos] += CHSV(++hue, 255, 192);
  FastLED.setTemperature(Temperature);
}

void setsimple(){
  for(int i = 0; i < ARRAY_SIZE(ledsarry); i++){
      ledsarry[i][0] = CRGB::Red;
      ledsarry[i][5] = CRGB::Blue;
      ledsarry[i][10] = CRGB::Aqua;
      ledsarry[i][15] = CRGB::Green;
      ledsarry[i][20] = CRGB::Yellow;
  }
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 25;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  static uint8_t hue = 0;
  hue--;
  for (int i = 0; i < NUM_LEDS; i++) {
    int x = beat-hue+i*10;
    if (x < 0) x = NUM_LEDS - x;
    leds[i] = ColorFromPalette(currentPalette, hue+i*2 % NUM_LEDS, x);
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
    leds[i] = ColorFromPalette(currentPalette, colorIndex, NUM_LEDS, LINEARBLEND);
  }
  FastLED.setTemperature(Temperature);
}

void Strobe(){
  int StrobeCount = 10; int FlashDelay = 50; int EndPause = 500;
  for(int j = 0; j < StrobeCount; j++) {
    strobeani(currcolor.r,currcolor.g,currcolor.b);
    if(recPack() < 200){break;}
  }
 FastLED.setTemperature(Temperature);
 delay(EndPause);
}

void stroberandom(){
  int StrobeCount = 10; int FlashDelay = 50; int EndPause = 500;
  for(int j = 0; j < StrobeCount; j++) {
    strobeani(random(0,255),random(0,255),random(0,255));
    if(recPack() < 200){break;}
  }
 FastLED.setTemperature(Temperature);
 delay(EndPause);
}

void strobeani(byte red, byte green, byte blue){
    setAll(red, green, blue);
    showStrip();
    delay(50);
    black();
    showStrip();
    delay(50);
}

void black(){
  setAll(0,0,0);
}

void simpleColor(){
  setAll(currcolor);
  FastLED.setTemperature(Temperature);
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

  if (recPack() < 200){return;}
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
    if(recPack()<200){return;}
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
  byte red = currcolor.r; byte green = currcolor.g; byte blue = currcolor.b; int EyeSize = 4; int SpeedDelay = 10; int ReturnDelay = 50;
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
    if(recPack()<200){return;}
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
    if(recPack()<200){return;}
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
      if(recPack()<200){return;}
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
      if(recPack()<200){return;}
      delay(3);
    }
  }
  FastLED.setTemperature(Temperature);
}

void FadeInOut(){
  byte red = currcolor.r; byte green = currcolor.g; byte blue = currcolor.b;
  recPack();
  if(faderup){
    Fade(red, green, blue);
    fader += 2;
  }else{
    Fade(red, green, blue);
    fader -= 2;
  }
}

void Fade(int red, int green, int blue){
  float r, g, b;
  int scale = 0;
  if(faderup){scale = fader;}else{scale = 256-fader;}
  r = (scale/256.0)*red;
  g = (scale/256.0)*green;
  b = (scale/256.0)*blue;
  setAll(r,g,b);
  FastLED.setTemperature(Temperature);
}

void sunriseset() {
  CRGB color = ColorFromPalette(HeatColors_p, fader);
  fader++;
  // fill the entire strip with the current color
  fill_solid(leds, NUM_LEDS, color);  
}

void american(){
  fill_stripes();
  add_glitter();
  showStrip();
}
//for animation american
void fill_stripes()
{
  CRGBPalette16 gPalette ( 
    CRGB::Black, CRGB::Black, 
    CRGB::Red,   CRGB::Red,  CRGB::Red,  CRGB::Red, 
    CRGB::Gray,  CRGB::Gray, CRGB::Gray, CRGB::Gray,
    CRGB::Blue,  CRGB::Blue, CRGB::Blue, CRGB::Blue,
    CRGB::Black, CRGB::Black
  );
  static uint8_t startValue = 0;
  startValue = startValue + 2;
  
  uint8_t value = startValue;  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( gPalette, triwave8( value), 128, LINEARBLEND);
    value += 7;
  }
}
//for animation american
void add_glitter()
{
  int chance_of_glitter =  5; // percent of the time that we add glitter
  int number_of_glitters = 3; // number of glitter sparkles to add
  
  int r = random8(100);
  if( r < chance_of_glitter ) {
    for( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White; // very bright glitter
    }
  }
}

void twinkle() {
  byte red = currcolor.r; byte green = currcolor.g; byte blue = currcolor.b; int Count = 10; int SpeedDelay = 100;
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     showStrip();
     delay(SpeedDelay);
   }
  FastLED.setTemperature(Temperature);
  delay(SpeedDelay);
}

void TwinkleRandom() {
  int Count = 20; int SpeedDelay = 100;
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
   }
  FastLED.setTemperature(Temperature);
  delay(SpeedDelay);
}

#define qsuba(x, b)  ((x>b)?x-b:0)
void plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.
  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

    leds[k] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, LINEARBLEND);  // Let's now add the foreground colour.
  }
  FastLED.setTemperature(Temperature);
  delay(50);
}

void PoliceSiren(){
  for(int i = 0; i < NUM_LEDS; i++){
    if(counter % 2 == 0 && i %2 == 0){
      setPixel(i, 255, 0, 0);
    }else if(counter % 2 == 0 && i % 2 == 1){
      setPixel(i, 0, 0, 0);
    }else if(counter % 2 == 1 && i % 2 == 0){
      setPixel(i, 0, 0, 0);
    }else if(counter % 2 == 1 && i % 2 == 1){
      setPixel(i, 0, 0, 255);
    }
  }
  delay(500);
}

void FadeToMiddle(){
  memset8(leds, 0, NUM_LEDS * sizeof(CRGB));
  int Width = 4; int F16delta = 1; uint16_t Fhue16 = 0;
    for (int i = 0; i < barCount; i++) {
        int bar = bars[i];

        // Update the "Fraction Bar" by 1/16th pixel every time
        if (i % 2 == 0)
            bar += F16delta;
        else
            bar -= F16delta;

        // wrap around at end
        // remember that F16pos contains position in "16ths of a pixel"
        // so the 'end of the strip' is (NUM_LEDS * 16)
        if (bar >= (NUM_LEDS * 16))
            bar -= NUM_LEDS * 16;
        else if (bar < 0)
            bar = (NUM_LEDS * 16) - 1;

        // draw the Fractional Bar, length=4px
        drawFractionalBar(bar, Width, 0, 0);

        bars[i] = bar;
    }
  
}

void drawFractionalBar(int pos16, int width, uint8_t hue, uint8_t sat)
{
    int i = pos16 / 16; // convert from pos to raw pixel number
    uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
    uint8_t firstpixelbrightness = 255 - (frac * 16);
    uint8_t lastpixelbrightness = 255 - firstpixelbrightness;
    uint8_t bright;
    for (int n = 0; n <= width; n++) {
        if (n == 0) {
            // first pixel in the bar
            bright = firstpixelbrightness;
        }
        else if (n == width) {
            // last pixel in the bar
            bright = lastpixelbrightness;
        }
        else {
            // middle pixels
            bright = 255;
        }

        leds[i] += CHSV(hue, sat, bright);
        i++;
        if (i == NUM_LEDS) i = 0; // wrap around
    }
}

void Lightning(){
  uint8_t frequency = 50;
  uint8_t flashes = 8;                                          //the upper limit of flashes per strike
  unsigned int dimmer = 1;

  uint8_t ledstart;                                             // Starting location of a flash
  uint8_t ledlen;
  ledstart = random8(NUM_LEDS);                               // Determine starting location of flash
  ledlen = random8(NUM_LEDS-ledstart);                        // Determine length of flash (not to go beyond NUM_LEDS-1)
  
  for (int flashCounter = 0; flashCounter < random8(3,flashes); flashCounter++) {
    if(flashCounter == 0) dimmer = 5;                         // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1,3);                               // return strokes are brighter than the leader
    
    fill_solid(leds+ledstart,ledlen,CHSV(255, 0, 255/dimmer));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4,10));                                     // each flash only lasts 4-10 milliseconds
    fill_solid(leds+ledstart,ledlen,CHSV(255,0,0));           // Clear the section of LED's
    FastLED.show();
    if(recPack()<200){return;}
    if (flashCounter == 0) delay (150);                       // longer delay until next flash after the leader
    
    delay(50+random8(100));                                   // shorter delay between strokes  
  } // for()
  
  delay(random8(frequency)*100);
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

void setAll(CRGB color){
  byte red = color.r; byte green = color.g; byte blue = color.b;
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
