#include <FastLED.h>

#define LED_PIN     10
#define NUM_LEDS    300

CRGB leds[NUM_LEDS];
// CHSV yellow = (45, 200, 255);

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  for (int i = 0; i <= NUM_LEDS; i++) {
    // leds[i] = CRGB(245, 212, 20); //yellow
    leds[i] = CHSV (45, 200, 255);
  }
  FastLED.show();
  // int plate1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  // int plate2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  // int plate3[] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
  // int plate4[] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
  int plates[4][10] = {
                        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
                        { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
                        { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },
                        { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 }
                      };
}

void loop() {
  int fadespeed = random(1,4);
  int delayspeed = random(0,10);

  CHSV yellow(25, 120, 150);
  //fade in
    for (int b = 10; b < 150; b=b+fadespeed) {
      for (int i = 0; i <= NUM_LEDS; i++) {
        yellow.v = b;
        leds[i] = yellow; // update leds[0] with the HSV result
      }
      FastLED.show();
      delay(delayspeed);
    }

  //fade out
    for (int b = 150; b >= 10; b=b-fadespeed) {
      for (int i = 0; i <= NUM_LEDS; i++) {
        yellow.v = b;
        leds[i] = yellow; // update leds[0] with the HSV result
      }
      FastLED.show();
      delay(delayspeed);
    }
}