// retrieved from: https://create.arduino.cc/editor/SuzanneT96/0595f989-e0e7-4c98-90e8-14a5fcb5a0aa/preview

/*
This is a work in progress but hopefully it will help someone else by providing
a base to start and work from.
Please check out my Youtube videos here and consider a thumbs up if this helped you!
Youtube : http://www.youtube.com/mkmeorg
Website, Forum and store are at http://mkme.org
0X3C is address for cheap ebay units
Used I2C Scanner to find address
SDA connected to pin Analog 4
SCL connected to pin Analog 5
*/

#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>     //Using u8g2 library

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino

int limit;
int value;

void setup() {
  u8x8.begin();
  u8x8.setPowerSave(0);

  u8x8.setFont(u8x8_font_pxplusibmcgathin_f);
}

void loop()
{
value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin

// text display tests
  u8x8.drawString(0,7,value);


 if(value<200)
  {

      u8x8.drawString(0,3,"sober");


  }
  if (value>=200 && value<280)
  {
       u8x8.drawString(0,3,"alcohol detected");


  }
  if (value>=280 && value<350)
  {

       u8x8.drawString(0,3,"many drinks");


  if (value>=350 && value <450)
  {

       u8x8.drawString(0,3,"huge booze");

  }
  if(value>450)
  {

      u8x8.drawString(0,3,"too much");

  }

  delay(5000);
  u8x8.drawString(0,3,0);
  delay(1000);


}}
