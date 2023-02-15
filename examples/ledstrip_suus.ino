// retrieved from https://create.arduino.cc/editor/SuzanneT96/64c7d3c1-b765-46cd-b2ab-0df369c73552/preview

#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = aantal LEDs in de strip
// Parameter 2 = pin nummer
// Parameter 3 = RGB LED vlaggen, combineer indien nodig:
//   NEO_KHZ800  800 KHz bitstream (de meeste NeoPixel producten met WS2812 LEDs)
//   NEO_KHZ400  400 KHz (klassieke 'v1' (niet v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     RGB LED volgens GRB bitstream (de meeste NeoPixel produkten)
//   NEO_RGB     RGB LED volgens RGB bitstream (v1 FLORA pixels, niet v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_BRG+NEO_KHZ400);

void setup() {
  strip.begin();
  strip.show(); // initialisatie van allLEDs (resulteert in UIT zetten hier)
}

void loop() {
  // Een paar voorbeelden:
  colorWipe(strip.Color(255, 0, 0), 70); // Rood
  delay(1000);
  colorWipe(strip.Color(0, 255, 0), 70); // Groen
  delay(1000);
  colorWipe(strip.Color(0, 0, 255), 70); // Blauw
  delay(1000);

  // Zogenaamde thearter chase voorbeeld ...
  theaterChase(strip.Color(127, 127, 127), 50); // Wit
  theaterChase(strip.Color(127,   0,   0), 50); // Rood
  theaterChase(strip.Color(  0,   0, 127), 50); // Blauw

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Schakel LED na LED aan
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Iets andere variatie van de rainbow functie maar dan iets gelijkmatiger
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style kruipende LEDs.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //10 x doorlopen
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //zet elke 3de RGD LED aan
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //zet elke 3de RGB LED uit
      }
    }
  }
}

//Theatre-style kruipen met regenboog effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // gebruik alle 256 kleuren in het wiel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //zet elke 3de LED aan
        }
        strip.show();

        delay(wait);

        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //zet elke 3de LED uit
        }
    }
  }
}

// Voer een waarde van 0 tot 255 in voor berekening van kleur waarden.
// De kleuren zijn een r - g - b combinatie.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}