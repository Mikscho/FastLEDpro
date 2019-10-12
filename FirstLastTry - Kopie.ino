// set the board - 1: use it. 0: don't use it. This regulates which libs are loaded.
//#define ESP32 1
#define ESP8266 1

// WiFi and webserver
// replace the SSID and password with the credentials of your WiFi
const char* ssid = "PI-net";
const char* password = "blubblub";

#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  WebServer server(80);
#elif ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#endif

#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE


// FastLED
#define DATA_PIN    6
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS    36
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  120

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

/*
 * This serves a very rudimentary responsive webview. Visit this in browser.
 * The webview has a button. When tapped, a post request is made to this esp webserver.
 * The next animation will start. 
 */
void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html>  \
<head>  \
    <title>LED Controller</title>  \
    <link rel='stylesheet' type='text/css' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css'>  \
    <script>  \
        window.onload = function() { \
            var buttnext = document.getElementById('buttnext'); \
            buttnext.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/next', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'next'}));}, false);  \
      var buttlast = document.getElementById('buttlast'); \
      buttlast.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/last', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'last'}));}, false); \
      var buttconfetti = document.getElementById('buttconfetti'); \
      buttconfetti.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/confetti', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'confetti'}));}, false); \
      var buttrainbow = document.getElementById('buttrainbow'); \
      buttrainbow.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/rainbow', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'rainbow'}));}, false); \
      var buttcloudSlowBeatWave = document.getElementById('buttcloudSlowBeatWave'); \
      buttcloudSlowBeatWave.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/cloudSlowBeatWave', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'cloudSlowBeatWave'}));}, false); \
      var buttrainbowBT = document.getElementById('buttrainbowBT'); \
      buttrainbowBT.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/rainbowBT', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'rainbowBT'}));}, false); \
      var buttsinelon = document.getElementById('buttsinelon'); \
      buttsinelon.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/sinelon', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'sinelon'}));}, false); \
        } \
    </script> \
</head> \
  \
<body>  \
    <h1>LED Controller</h1> \
    <table><tr> \
    <td><button class='btn btn-primary' id='buttnext'>next</button></td><td><button class='btn btn-primary' id='buttlast'>last</button></td><td><button class='btn btn-primary' id='buttsinelon'>sinelon</button></td>  \
    </tr><tr><td><button class='btn btn-primary' id='buttconfetti'>confetti</button></td><td><button class='btn btn-primary' id='buttrainbow'>rainbow</button></td><td><button class='btn btn-primary' id='buttcloudSlowBeatWave'>cloudSlowBeatWave</button></td><td><button class='btn btn-primary' id='buttrainbowBT'>rainbowBlueTone</button>  \
    </tr></table> \
</body>");
}


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

  server.on("/", handleRoot);
  server.on("/confetti", HTTP_POST, [](){
    switchPattern(0);
    server.send(200, "text/plain", "OK");
  });
  server.on("/rainbow", HTTP_POST, [](){
    switchPattern(1);
    server.send(200, "text/plain", "OK");
  });
  server.on("/cloudSlowBeatWave", HTTP_POST, [](){
    switchPattern(2);
    server.send(200, "text/plain", "OK");
  });
  server.on("/rainbowBT", HTTP_POST, [](){
    switchPattern(3);
    server.send(200, "text/plain", "OK");
  });
  server.on("/sinelon", HTTP_POST, [](){
    switchPattern(4);
    server.send(200, "text/plain", "OK");
  });
  server.on("/next", HTTP_POST, [](){
    switchPattern(100);
    server.send(200, "text/plain", "OK");
  });
  server.on("/last", HTTP_POST, [](){
    switchPattern(101);
    server.send(200, "text/plain", "OK");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
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
}

typedef void (*Animations[])();
Animations animations = { confetti, rainbow, cloudSlowBeatWave, rainbowBlueTone, sinelon, juggle, rainbowRedTone, bpm };

void loop(void) {
  server.handleClient();

  animations[currentAnimation]();
  
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}


void switchPattern(int criteria){
  switch (criteria) {
    case 0:
    //confetti
      currentAnimation = (currentAnimation - currentAnimation) % ARRAY_SIZE(animations);
      break;
    case 1:
    //rainbow
      currentAnimation = (currentAnimation - currentAnimation + 1) % ARRAY_SIZE(animations);
      break;
    case 2:
    //cloudSlowBeatWave
      currentAnimation = (currentAnimation - currentAnimation + 2) % ARRAY_SIZE(animations);
      break;
    case 3:
    //rainbowBT
      currentAnimation = (currentAnimation - currentAnimation + 3) % ARRAY_SIZE(animations);
      break;
    case 4:
    //sinelon
      currentAnimation = (currentAnimation - currentAnimation + 4) % ARRAY_SIZE(animations);
      break;
    case 5:
    //juggle
      currentAnimation = (currentAnimation - currentAnimation + 5) % ARRAY_SIZE(animations);
      break;
    case 6:
    //rainbowRT
      currentAnimation = (currentAnimation - currentAnimation + 6) % ARRAY_SIZE(animations);
      break;
    case 7:
    //bpm
      currentAnimation = (currentAnimation - currentAnimation + 7) % ARRAY_SIZE(animations);
      break;
    case 100:
    //next
      currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
      break;
   case 101:
    //last
      currentAnimation = (currentAnimation - 1) % ARRAY_SIZE(animations);
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
