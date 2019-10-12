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
    <link href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' rel='stylesheet' media='screen'>  \
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
      var buttjuggle = document.getElementById('buttjuggle'); \
      buttjuggle.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/juggle', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'juggle'}));}, false); \
      var buttrainbowRT = document.getElementById('buttrainbowRT'); \
      buttrainbowRT.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/rainbowRT', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'rainbowRT'}));}, false); \
      var buttbpm = document.getElementById('buttbpm'); \
      buttbpm.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/bpm', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'bpm'}));}, false); \
        } \
    </script> \
</head> \
  \
<body>  \
    <h1>LED Controller</h1>\
<div class='panel panel-default'>\
    <div class='panel-heading'>Animation</div>\
    <div class='panel-body'>\
            <div class='row'>\
                <div class='col-sm-4 btn-group-vertical' role='group'>\
                    <button class='btn btn-primary' id='buttnext'>Next</button>\
                    <button class='btn btn-primary' id='buttlast'>Last</button>\
                    <button class='btn btn-primary' id='buttconfetti'>Confetti</button>\
                    <button class='btn btn-primary' id='buttrainbow'>Rainbow</button>\
                    <button class='btn btn-primary' id='buttcloudSlowBeatWave'>CloudSlowBeatWave</button>\
                    <button class='btn btn-primary' id='buttrainbowBT'>RainbowBlueTone</button>\
                    <button class='btn btn-primary' id='buttsinelon'>Sinelon</button>\
                    <button class='btn btn-primary' id='buttjuggle'>Juggle</button>\
                    <button class='btn btn-primary' id='buttrainbowRT'>RainbowRedTone</button>\
                    <button class='btn btn-primary' id='buttbpm'>BPM</button>\
                </div>\
                <div class='col-sm-4 btn-group-vertical'>\
                    <button class='btn btn-primary' name='203' type='submit'>Pixel Bounce</button>\
                    <button class='btn btn-primary' name='211' type='submit'>Pixel Smooth Shift Right</button>\
                    <button class='btn btn-primary' name='212' type='submit'>Pixel Smooth Shift Left</button>\
                    <button class='btn btn-primary' name='213' type='submit'>Pixel Smooth Bounce</button>\
                    <button class='btn btn-primary' name='221' type='submit'>Comet</button>\
                    <button class='btn btn-primary' name='222' type='submit'>Comet (colored)</button>\
                    <button class='btn btn-primary' name='241' type='submit'>Moving Bars</button>\
                    <button class='btn btn-primary' name='242' type='submit'>Moving Gradient</button>\
                    <button class='btn btn-primary' name='251' type='submit'>Larson Scanner</button>\
                    <button class='btn btn-primary' name='252' type='submit'>Larson Scanner 2</button>\
                </div>\
                <div class='col-sm-4 btn-group-vertical'>\
                    <button class='btn btn-primary' name='301' type='submit'>FadeIn</button>\
                    <button class='btn btn-primary' name='302' type='submit'>FadeOut</button>\
                    <button class='btn btn-primary' name='303' type='submit'>FadeInOut</button>\
                    <button class='btn btn-primary' name='304' type='submit'>Glow</button>\
                    <button class='btn btn-primary' name='351' type='submit'>Fade Colors</button>\
                    <button class='btn btn-primary' name='352' type='submit'>Fade Colors (loop)</button>\
                    <button class='btn btn-primary' name='353' type='submit'>Pixels Fade</button>\
                    <button class='btn btn-primary' name='501' type='submit'>Fire</button>\
                    <button class='btn btn-primary' name='502' type='submit'>Bouncing Balls</button>\
                    <button class='btn btn-primary' name='503' type='submit'>Larson Scanner 2</button>\
                </div>\
            </div>\
    </div>\
</div>\
\
\
<div class='row'>\
<div class='col-sm-4'>\
    <div class='panel panel-default'>\
        <div class='panel-heading'>Brightness</div>\
        <div class='panel-body' class='btn-group' role='group'>\
            <form action='/brightness/' method='post'>\
                <button class='btn btn-primary' name='8' type='submit'>20%</button>\
                <button class='btn btn-primary' name='16' type='submit'>40%</button>\
                <button class='btn btn-primary' name='32' type='submit'>60%</button>\
                <button class='btn btn-primary' name='64' type='submit'>80%</button>\
                <button class='btn btn-primary' name='100' type='submit'>100%</button>\
            </form>\
        </div>\
    </div>\
</div>\
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
  server.on("/juggle", HTTP_POST, [](){
    switchPattern(5);
    server.send(200, "text/plain", "OK");
  });
  server.on("/rainbowRT", HTTP_POST, [](){
    switchPattern(6);
    server.send(200, "text/plain", "OK");
  });
  server.on("/bpm", HTTP_POST, [](){
    switchPattern(7);
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
    case 100:    //next
      currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
      break;
   case 101:    //last
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
