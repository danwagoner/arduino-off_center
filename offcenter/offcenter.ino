#include <FastLED.h>

#define LED_PIN                 10
#define NUM_LEDS_TOTAL          300
#define NUM_LEDS                130
#define NUM_PLATES              5
#define NUM_LEDS_PER_PLATE      26
#define MAX_BRIGHTNESS          200
#define MIN_BRIGHTNESS          50
#define MIN_DEPTH               100
#define MAX_SPEED_THRESHOLD     8
#define RECALC_RANDOM_INTERVAL  600

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
int pulse_together = 0;
int depth [NUM_PLATES];
int ready = 0;

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
    speed[p] = random(min_speed, max_speed);
    depth[p] = random(MIN_BRIGHTNESS, MIN_DEPTH);
    h[p] = 35;
    s[p] = 200;
    v[p] = 0;
  }
  FastLED.show();
}

void loop() {
  offcenterPulse();
}

void offcenterPulse(){
// this function causes the plates to pulse randomly, slowly moving toward a pattern of pulsing in unison, then slowly moving back to a pattern for randomness.

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

    if (max_speed == min_speed){
      pulse_together = 1;
    }
    else {
      pulse_together = 0;
      ready = 0;
    }

    if (speed_direction == 0){ // start to bring pulse speed into unison
      max_speed--;
    }
    else { // start to spread pulse speed apart
      max_speed++;
    }

    for (int p = 0; p <= NUM_PLATES - 1; p++) { // set new speed
      speed[p] = random(min_speed,max_speed);
      if (pulse_together == 1){
        depth[p] = MIN_BRIGHTNESS;
      }
      else{
        depth[p] = random(MIN_BRIGHTNESS, MIN_DEPTH);
      }
      freeze[p] = 0;
      Serial.print ("Plate: ");
      Serial.print (p);
      Serial.print (" | Direction: ");
      Serial.print (speed_direction);
      Serial.print (" | Max Speed: ");
      Serial.print (max_speed);
      Serial.print (" | Depth: ");
      Serial.print (depth[p]);
      Serial.print (" | Speed: ");
      Serial.println (speed[p]);
    }
    Serial.println ("======================================================== ");
  }

  if (pulse_together == 1) {
    if (ready == 0) {
      for (int p = 0; p <= NUM_PLATES - 1; p++) {
        if (v[p] == MIN_BRIGHTNESS){
          ready = 1;
          if (freeze[p] != 1){
            freeze[p] = 1; // freeze
            Serial.print("Freezing Plate ");
            Serial.print(p);
            Serial.print(" - ");
            Serial.println(v[p]);
          }
        }
        else{
          ready = 0;
        }
      }
    }
    else {
      for (int p = 0; p <= NUM_PLATES - 1; p++) {
        if (freeze[p] == 1){
          freeze[p] = 0; //thaw
          Serial.print("Thawing Plate ");
          Serial.print(p);
          Serial.print(" - ");
          Serial.println(v[p]);
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
      if ( v[p] <= depth[p]) {
        v[p] = depth[p];
        direction[p] = 1;
      }
    }

    CRGB rgb;
    hsv2rgb_rainbow( CHSV(h[p], s[p], v[p]), rgb );
    writeLEDs();
  }
}

void writeLEDs(){
  for (int i = 0; i < NUM_LEDS; i++) {
    int p = i/NUM_LEDS_PER_PLATE;
    leds[i] = (CHSV( h[p], s[p], v[p]));
  }
  FastLED.show();
}

