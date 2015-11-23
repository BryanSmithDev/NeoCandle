// candle for Adafruit NeoPixel
// 8 pixel version
// by Tim Bartlett, December 2013

#include <Adafruit_NeoPixel.h>
#define PIN 0

// color variables: mix RGB (0-255) for desired yellow
// use #define's for constants so the compiler can do its magic
#define redPx (255)
#define grnHigh (100)
#define bluePx (10)

// animation time variables, with recommendations
// how much green dips below grnHigh for normal burn
#define burnDepth (6)
// maximum dip for flutter
#define flutterDepth (20)
// duration of one dip in milliseconds
#define cycleTime (120)

// pay no attention to that man behind the curtain
int fDelay;
int fRep;
int flickerDepth;
int burnDelay;
int burnLow;
int flickDelay;
int flickLow;
int flutDelay;
int flutLow;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(27, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  flickerDepth = (burnDepth + flutterDepth) / 2.4;
  burnLow = grnHigh - burnDepth;
  burnDelay = (cycleTime / 2) / burnDepth;
  flickLow = grnHigh - flickerDepth;
  flickDelay = (cycleTime / 2) / flickerDepth;
  flutLow = grnHigh - flutterDepth;
  flutDelay = ((cycleTime / 2) / flutterDepth);
  
  strip.begin();
  strip.show();
}

// In loop, call CANDLE STATES, with duration in seconds
// 1. on() = solid yellow
// 2. burn() = candle is burning normally, flickering slightly
// 3. flicker() = candle flickers noticably
// 4. flutter() = the candle needs air!

void loop() {
  burn(10);
  flicker(5);
  burn(8);
  flutter(6);
  burn(3);
  on(10);
  burn(10);
  flicker(10);
}

void setGreenLevel(int grnPx)
{
  uint32_t c;
  int i;
  
  // warmer+dimmer at the top
  // led's 18-26 except 22 (center)
  c = strip.Color(redPx * 30 / 100, constrain(grnPx - 70, 5, 255), 0);
  for (i = 18; i < 27; i++) {
    if (i == 22)
      strip.setPixelColor(i, redPx * 50 / 100, constrain(grnPx - 50, 5, 255), bluePx - 5);
    else
      strip.setPixelColor(i, c);
  }

  // middle layer
  c = strip.Color(redPx * 75 / 100, constrain(grnPx - 35, 5, 255), bluePx - 2);  // 9-17 except 13
  for (i = 9; i < 18; i++) {
    if (i == 13)
      strip.setPixelColor(i, redPx, grnPx, bluePx);
    else
      strip.setPixelColor(i, c);
  }

  // bright at the bottom
  c = strip.Color(redPx, grnHigh, bluePx);  // 0-8 except 4
  for (i = 0; i < 9; i++) {
    if (i == 4)
      strip.setPixelColor(6, redPx, (grnHigh + grnPx) / 2, bluePx);  // 4
    else
      strip.setPixelColor(i, c);
  }
  strip.show();
}

// basic fire funciton - not called in main loop
void fire(int grnLow) {
  for (int grnPx = grnHigh; grnPx > grnLow; grnPx--) {
    setGreenLevel(grnPx);
    delay(fDelay);
  }  
  for (int grnPx = grnLow; grnPx < grnHigh; grnPx++) {
    setGreenLevel(grnPx);
    delay(fDelay);
  }
}

// fire animation
void on(int f) {
  fRep = f * 1000;
  int grnPx = grnHigh - 6;
  setGreenLevel(grnPx);
  delay(fRep);
}

void burn(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  for (int var = 0; var < fRep; var++) {
    fire(burnLow);
  }  
}

void flicker(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flickLow);
  }
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
  fire(burnLow);
}

void flutter(int f) {
  fRep = f * 8;  
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  fire(flickLow);
  fDelay = flutDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flutLow);
  }
  fDelay = flickDelay;
  fire(flickLow);
  fire(flickLow);
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
}

