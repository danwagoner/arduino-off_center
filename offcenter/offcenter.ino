#include <FastLED.h>

#define LED_PIN             10
#define NUM_LEDS_TOTAL      300
#define NUM_LEDS            100
#define NUM_PLATES          5
#define NUM_LEDS_PER_PLATE  20
#define MAX_BRIGHTNESS      150
#define MIN_BRIGHTNESS      20

CRGB leds[NUM_LEDS_TOTAL];

int direction [NUM_PLATES];
int speed [NUM_PLATES];
int h [NUM_PLATES];
int s [NUM_PLATES];
int v [NUM_PLATES];
int count = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS_TOTAL);
  FastLED.clear();

  for (int p = 0; p <= NUM_PLATES - 1; p++){ //init plates
    direction[p] = 1;
    speed[p] = random(1,10);
    h[p] = 35;
    s[p] = 200;
    v[p] = 0;

    Serial.print ("Plate: ");
    Serial.print (p);
    Serial.print (" Direction: ");
    Serial.print (direction[p]);
    Serial.print (" Speed: ");
    Serial.println (speed[p]);
  }
  FastLED.show();
}

void loop() {
  count ++;
  if (count == 300){ // recalculate speed of a random plate every so often to create some more randomness
    int rp = random(0, NUM_PLATES - 1);
    speed[rp] = random(2,8);
    count = 0;
    Serial.print ("Plate: ");
    Serial.print (rp);
    Serial.print (" Speed: ");
    Serial.println (speed[rp]);
  }

  for (int p = 0; p <= NUM_PLATES - 1; p++) {
    if (direction[p] == 1) { // fade in
      v[p] = v[p] + speed[p];
      if ( v[p] >= MAX_BRIGHTNESS) {
        direction[p] = 0;
      }
    }
    else { // fade out
      v[p] = v[p] - speed[p];
      if ( v[p] <= MIN_BRIGHTNESS) {
        v[p] = MIN_BRIGHTNESS;
        direction[p] = 1;
      }
    }

    CRGB rgb;
    hsv2rgb_rainbow( CHSV(h[p], s[p], v[p]), rgb );
    writeLEDs();
    // Serial.print ("Plate: ");
    // Serial.print (p);
    // Serial.print (" Direction: ");
    // Serial.print (direction[p]);
    // Serial.print (" RGB: ");
    // Serial.print (rgb.r);
    // Serial.print (" ");
    // Serial.print (rgb.g);
    // Serial.print (" ");
    // Serial.print (rgb.b);
    // Serial.print (" HSV: ");
    // Serial.print (h[p]);
    // Serial.print (" ");
    // Serial.print (s[p]);
    // Serial.print (" ");
    // Serial.println (v[p]);
    // delay (500);
  }
}

void writeLEDs(){
  for (int i = 0; i < NUM_LEDS; i++) {
    int p = i/NUM_LEDS_PER_PLATE;
    leds[i] = (CHSV( h[p], s[p], v[p]));
  }
  FastLED.show();
}
