#include <radio.h>
#include <RDA5807M.h>

// Initial radio settings

#define RDA_BAND     RADIO_BAND_FM
#define RDA_STATION  10530 
#define RDA_VOLUME   1

RDA5807M radio;

void setup() {
  radio.init();
  radio.setBandFrequency(RDA_BAND, RDA_STATION);
  radio.setVolume(RDA_VOLUME);
  radio.setMono(false);
  radio.setMute(false);
}

void loop() {
  // Currently not in use
} 
