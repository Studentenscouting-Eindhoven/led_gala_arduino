#include <Adafruit_NeoPixel.h>

// Program constants
#define ANALOGUE_MAX 1023
#define TIMESTEP 100

// pin configurations
#define MQ3_PIN 14 // alcohol sensor on analogue in pin 0
#define POT_PIN 15 // Tuning potentiometer on analogue in pin 1
#define LEDSTRIP_PIN 6 // ledstrip on digital out pin 6

// tuning for the alcoholmeter
#define BASELINE_ALPHA 0.01f // digital filter coefficient for baseline measurement
#define MEAS_ALPHA 0.25f // digital filter coefficient for new measurement
#define MAX_GAIN_POTMETER 5.0f // maximum gain allowed by potentiometer tuning

// tuning for LED strip
#define GREEN_LED 10 // number of green LED's
#define YELLOW_LED 10 // number of yellow LED's
#define RED_LED 10 // number of red LED's
#define TOTAL_LED GREEN_LED+YELLOW_LED+RED_LED

struct alcohol_meter{
  float baseline_ppm;
  float measured_ppm;
  float tuning_factor;
};

alcohol_meter am = {0.0f, 0.0f, 1.0f};

// Parameter 1 = number of LEDs in the strip
// Parameter 2 = Pin number
// Parameter 3 = RGB LED flags, combine when needed:
//   NEO_KHZ800         800 KHz bitstream (mosr NeoPixel products with WS2812 LEDs)
//   NEO_KHZ400         400 KHz (Classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_{RGB/BGR/GRB}  RGB LED bitstream order
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LED, LEDSTRIP_PIN, NEO_BRG+NEO_KHZ400);

void update_alcoholmeter(alcohol_meter* am) {
  // obtain new measurement
  float new_meas = analogRead(MQ3_PIN);
  //update new measurement and baseline filtered values
  am->measured_ppm = (1.0f - MEAS_ALPHA) * am->measured_ppm + MEAS_ALPHA * new_meas;
  am->baseline_ppm = (1.0f - BASELINE_ALPHA) * am->baseline_ppm + BASELINE_ALPHA * new_meas;
  //update tuning factor based on pot meter
  am->tuning_factor = analogRead(POT_PIN) / (float) ANALOGUE_MAX * (float) MAX_GAIN_POTMETER;
}

void set_led_strip(Adafruit_NeoPixel* strip, alcohol_meter* am){
  // calculate number of pixels to turn on
  float fraction_meas = (am->measured_ppm - am->baseline_ppm)/((float) ANALOGUE_MAX - am->baseline_ppm);
  fraction_meas *= am->tuning_factor;
  uint8_t num_pixels = max(floor(fraction_meas * (TOTAL_LED)), TOTAL_LED);
  // turn on correct amount of pixels in correct color
  uint32_t pixelcolor;
  for(uint8_t idx=0; idx < num_pixels; ++idx){
    switch (idx)
    {
    case 0 ... RED_LED:
      pixelcolor = strip->Color(255, 0, 0);
      break;
    case RED_LED + 1 ... RED_LED + YELLOW_LED:
      pixelcolor = strip->Color(255, 255, 0);
      break;
    case RED_LED + YELLOW_LED + 1 ... TOTAL_LED:
      pixelcolor = strip->Color(0, 255, 0);
      break;
    default: // default to off/black pixel
      pixelcolor = strip->Color(0, 0, 0);
      break;
    }
    strip->setPixelColor(idx, pixelcolor);
  }
  strip->fill();
}

// Arduino does not seem to support floats in sprintf, store in char arrays first
void print_to_serial(alcohol_meter* am){
  char buffer[50];
  char base[8];
  char meas[8];
  char gain[8];
  dtostrf(am->baseline_ppm, 6, 1, base);
  dtostrf(am->measured_ppm, 6, 1, meas);
  dtostrf(am->tuning_factor, 6, 1, gain);
  sprintf(buffer, "Baseline: %s; Meas: %s; Gain: %s", base, meas, gain);
  Serial.println(buffer);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialize alcohol measurements without filter
  float alcohol_meas = analogRead(MQ3_PIN);
  am.measured_ppm = alcohol_meas;
  am.baseline_ppm = alcohol_meas;
}

void loop() {
  // put your main code here, to run repeatedly:
  update_alcoholmeter(&am);
  print_to_serial(&am);
  set_led_strip(&strip, &am);
  delay(TIMESTEP);
}
