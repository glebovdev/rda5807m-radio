#include <Wire.h>

#include <radio.h>
#include <RDA5807M.h>

#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

// Encoder library -> https://github.com/AlexGyver/GyverLibs/tree/master/GyverEncoder
#include "GyverEncoder.h"

// Initial radio settings
#define RDA_BAND     RADIO_BAND_FM
#define RDA_STATION  10530

#define OLED_I2C_ADDRESS 0x3C
//#define RST_PIN -1

// Encoder pins setup
#define ENC_S1 6 // pin D6
#define ENC_S2 5 // pin D5
#define ENC_KEY 7 // pin D7

// Init Radio library
RDA5807M radio;
RADIO_INFO radioInfo;

// Init OLED library
SSD1306AsciiWire oled;

// Init Encoder library
Encoder enc1(ENC_S1, ENC_S2, ENC_KEY);

#define MIN_VOLUME 0
#define MAX_VOLUME 15
bool isMute = false;
byte volume = 1;
uint32_t dataUpdateTime; // don't need to check data on every ineration, use periods instead
uint16_t updateRSSITimeout = 500; // update RSSI every 500ms

void setup() {
  //Serial.begin(9600);

  radio.init();
  radio.setBandFrequency(RDA_BAND, RDA_STATION);
  radio.setMono(false);
  radio.setMute(false);

  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&SH1106_128x64, OLED_I2C_ADDRESS);
  oled.setFont(System5x7);

  oled.clear();

  oled.setCursor(15, 3);
  oled.set2X();
  oled.println("105.3 FM");

  enc1.setType(TYPE2);

  // TODO: get volume value from EEPROM or set default
  setVolume(volume);

  dataUpdateTime = millis();
}

void loop() {
  enc1.tick();

  // Turn Encoder left -> turn volume down
  if (enc1.isLeft()) {
    volumeDown();
  }

  // Turn Encoder right -> turn volume up
  if (enc1.isRight()) {
    volumeUp();
  }

  // Get Radio data (RSSI, noise level, etc.)
  radio.getRadioInfo(&radioInfo);

  // Check timer and display RSSI info
  if (millis() - dataUpdateTime > updateRSSITimeout) {
    displayRSSI();
    dataUpdateTime = millis();
  }
}

void displayRSSI() {
  byte rssi = getRSSI();
  oled.setCursor(0, 0);
  oled.set1X();

  oled.print('[');
  if (rssi > 0) oled.print('*'); else oled.print(' ');
  if (rssi > 25) oled.print('*'); else oled.print(' ');
  if (rssi > 35) oled.print('*'); else oled.print(' ');
  if (rssi > 50) oled.print('*'); else oled.print(' ');
  oled.print(']');
}

byte getRSSI() {
  return radioInfo.rssi;
}

void volumeDown()
{
  if (volume <= MIN_VOLUME) {
    return;
  }

  volume--;

  setVolume(volume);
}

void volumeUp()
{
  if (volume >= MAX_VOLUME) {
    return;
  }

  volume++;

  setVolume(volume);
}

void setVolume(byte volume) {
  radio.setVolume(volume);

  if (volume == MIN_VOLUME) {
    radio.setMute(true);
    isMute = true;
  }

  if (volume > MIN_VOLUME && isMute == true) {
    radio.setMute(false);
    isMute = false;
  }
}
