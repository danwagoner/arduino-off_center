#include <FastLED.h>

#define LED_PIN                 10
#define NUM_LEDS_TOTAL          300
#define NUM_LEDS                130
#define NUM_PLATES              5
#define NUM_LEDS_PER_PLATE      26
#define MAX_BRIGHTNESS          250
#define MIN_BRIGHTNESS          50
#define MAX_SPEED_THRESHOLD     8
#define RECALC_RANDOM_INTERVAL  300

CRGB leds[NUM_LEDS_TOTAL];

int direction [NUM_PLATES];
int speed [NUM_PLATES];
int h [NUM_PLATES];
int s [NUM_PLATES];
int v [NUM_PLATES];
int freeze [NUM_PLATES];
int count = 0;
int min_speed = 2;
int max_speed = 8;
int speed_direction = 0;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS_TOTAL);
  FastLED.clear();

  
  for (int i = (NUM_PLATES * NUM_LEDS_PER_PLATE); i < NUM_LEDS_TOTAL; i++) { // set top light
      leds[i] = (CHSV( 45, 200, 200));
  }

  for (int p = 0; p <= NUM_PLATES - 1; p++){ //init plates
    direction[p] = 1;
    freeze[p] = 0;
    speed[p] = random(min_speed,max_speed);
    h[p] = 35;
    s[p] = 200;
    v[p] = 0;
  }
  FastLED.show();
}

void loop() {
  count ++;
  if (count == RECALC_RANDOM_INTERVAL){ // recalculate speed of all plates every so often to create some more randomness
    count = 0;
    randomSeed(analogRead(0));

    if (max_speed <= min_speed){ 
      speed_direction = 1;
      // Serial.println (" << Speed Direction Changed - UP >>");
    }

    if (max_speed >= MAX_SPEED_THRESHOLD){
      speed_direction = 0;
      // Serial.println (" << Speed Direction Changed - DOWN >>");
    }

    if (speed_direction == 0){ // start to bring speed randomness together
      max_speed--;
    }
    else { // start to spread speed randomness apart
      max_speed++;
    }

    for (int p = 0; p <= NUM_PLATES - 1; p++) { // set new speed
      speed[p] = random(min_speed,max_speed);
      freeze[p] = 0;
      Serial.print ("Plate: ");
      Serial.print (p);
      Serial.print (" | Direction: ");
      Serial.print (speed_direction);
      Serial.print (" | Max Speed: ");
      Serial.print (max_speed);
      Serial.print (" | Speed: ");
      Serial.println (speed[p]);
    }
  }

  for (int p = 0; p <= NUM_PLATES - 1; p++) { // sync plates with matching speeds
    for (int o = 0; o <= NUM_PLATES - 1; o++) {
      if (p != o){
        if (speed[p] == speed[o]){ // allow plates to "catch" one another by locking brightness momentarily
          if (v[o] == v[p]){ 
            if (freeze[p] == 1){
              freeze[p] = 0; // thaw
              Serial.print("Thawing Plate ");
              Serial.println(p);
            }
            if (freeze[o] == 1){
              freeze[o] = 0; // thaw
              Serial.print("Thawing Plate ");
              Serial.println(o);
            }
          }
          else{
            if (freeze[p] != 1 && freeze[o] != 1){
              freeze[p] = 1; // freeze
              Serial.print("Freezing Plate ");
              Serial.println(p);
            }
          }
        }
      }
    }
  }

  for (int p = 0; p <= NUM_PLATES - 1; p++) {
    if (direction[p] == 1) { // fade in
      if (freeze[p] != 1){
        v[p] = v[p] + speed[p];
      }
      if ( v[p] >= MAX_BRIGHTNESS) {
        direction[p] = 0;
      }
    }
    else { // fade out
      if (freeze[p] != 1){
        v[p] = v[p] - speed[p];
      }
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

