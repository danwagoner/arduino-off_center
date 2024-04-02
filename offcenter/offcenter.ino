#include <FastLED.h>

#define LED_PIN             10
#define NUM_LEDS_TOTAL      300
#define NUM_LEDS            100
#define NUM_PLATES          5
#define NUM_LEDS_PER_PLATE  20
#define MAX_BRIGHTNESS      196
#define MIN_BRIGHTNESS      10

CRGB leds[NUM_LEDS_TOTAL];

struct plate{
   int direction, speed, h, s, v;
} plates[NUM_PLATES];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS_TOTAL);
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB(245, 212, 20); //yellow
  }
  FastLED.clear();
  FastLED.show();

  for (int p = 0; p < NUM_PLATES; p++){ //init plates
    plates[p].direction = 1;
    plates[p].speed = 5;
    plates[p].h = 64;
    plates[p].s = 200;
    plates[p].v = 0;
  }
}

void loop() {

  for (int p = 0; p <= NUM_PLATES; p++) {
    // CHSV _targetHSV = rgb2hsv_approximate( CRGB( plates[p].r, plates[p].g, plates[p].b ) );

    if (plates[p].direction == 1) { // fade in
      plates[p].v + plates[p].speed;
      plates[p].v = plates[p].v + plates[p].speed;
      if ( plates[p].v >= 150) {
        plates[p].direction = 0;
      }
    }
    else { // fade out
      plates[p].v = plates[p].v - plates[p].speed;
      if ( plates[p].v <= 0) {
        plates[p].direction = 1;
      }
    }

    CRGB rgb;
    hsv2rgb_rainbow( CHSV(plates[p].h, plates[p].s, plates[p].v), rgb );

    Serial.print ("Plate: ");
    Serial.print (p);
    Serial.print (" Direction: ");
    Serial.print (plates[p].direction);
    Serial.print (" RGB: ");
    Serial.print (rgb.r);
    Serial.print (" ");
    Serial.print (rgb.g);
    Serial.print (" ");
    Serial.print (rgb.b);
    Serial.print (" HSV: ");
    Serial.print (plates[p].h);
    Serial.print (" ");
    Serial.print (plates[p].s);
    Serial.print (" ");
    Serial.println (plates[p].v);
    delay (500);
    // writeLEDs();
  }
}

// void writeLEDs(){
//   for (int i = 0; i < NUM_LEDS; i++) {
//     int p = i/NUM_LEDS_PER_PLATE;
//     leds[i] = (CRGB( plates[p].r, plates[p].g, plates[p].b ));
//   }
//   FastLED.show();
// }
