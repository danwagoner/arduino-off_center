#include <FastLED.h>

#define LED_PIN             10
#define NUM_LEDS_TOTAL      300
#define NUM_LEDS            100
#define NUM_PLATES          5
#define NUM_LEDS_PER_PLATE  20
#define MAX_BRIGHTNESS      150
#define MIN_BRIGHTNESS      10

CRGB leds[NUM_LEDS_TOTAL];

struct plate{
   int direction, speed;
} plates[NUM_PLATES];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS_TOTAL);
  FastLED.clear();
  // for (int i = 0; i <= NUM_LEDS; i++) {
  //   // leds[i] = CRGB(245, 212, 20); //yellow
  //   leds[i] = CHSV (45, 200, 255);
  // }
  FastLED.show();

  for (int p = 0; p < NUM_PLATES; p++){ //init plates
    plates[p].direction = 1;
    plates[p].speed = random(1,4);
    // plates[p].hsv = (45, 200, 255);

    // Serial.print("Plate: ");
    // Serial.print (p);
    // Serial.print(" Direction: ");
    // Serial.print (plates[p].direction);
    // Serial.print(" Speed: ");
    // Serial.println (plates[p].speed);
  }

}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int p = i/NUM_LEDS_PER_PLATE;
    CHSV _targetHSV = rgb2hsv_approximate(leds[i]);

    if (plates[p].direction == 1) { // fade in
      _targetHSV.v = _targetHSV.v + plates[p].speed;
      if ( _targetHSV.v >= 150) {
        plates[p].direction = 0;
        // Serial.print("Plate: ");
        // Serial.print (p);
        // Serial.print(" Change Direction: ");
        // Serial.println (plates[p].direction);
      }
    }
    else { // fade out
      _targetHSV.v = _targetHSV.v - plates[p].speed;
      if ( _targetHSV.v <= 0) {
      }
    }
    leds[i] = _targetHSV;
    // Serial.print ("Plate: ");
    // Serial.print (p);
    // Serial.print (" Led: ");
    // Serial.print (i);
    // Serial.print (" Direction: ");
    // Serial.print (plates[p].direction);
    // Serial.print (" Value: ");
    // Serial.println (_targetHSV.v);
  }
}