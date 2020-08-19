

#include <radio.h>
#include <RDA5807M.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>


// Initial radio settings
#define RDA_BAND     RADIO_BAND_FM
#define RDA_STATION  10530 
#define RDA_VOLUME   1

#define OLED_I2C_ADDRESS 0x3C
//#define RST_PIN -1

RDA5807M radio;
SSD1306AsciiWire oled;

void setup() {
  radio.init();
  radio.setBandFrequency(RDA_BAND, RDA_STATION);
  radio.setVolume(RDA_VOLUME);
  radio.setMono(false);
  radio.setMute(false);

  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&SH1106_128x64, OLED_I2C_ADDRESS);
  oled.setFont(System5x7);
  
  oled.clear();
  
  oled.println();
  oled.set2X();
  oled.println("105.3 FM");

}

void loop() {
  // Currently not in use
} 
