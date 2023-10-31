#include <Arduino.h>
#include <WiFi.h>
#include "Audio.h"

#define WIFI_SSID "xxx"
#define WIFI_PASSWD "xxx"

#define MAX98357A_I2S_DOUT  25
#define MAX98357A_I2S_BCLK 27
#define MAX98357A_I2S_LRC  26
Audio audio;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  Serial.println("init wifi...");
  WiFi.disconnect();

  WiFi.mode(WIFI_STA);

  Serial.println("wifi connect");
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }
  Serial.println("wifi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  audio.setPinout(MAX98357A_I2S_BCLK, MAX98357A_I2S_LRC, MAX98357A_I2S_DOUT);
  audio.setVolume(100);

  Serial.println("audio connecting...");
  audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3");

  Serial.println("audio playing...");
}


void loop() {
  audio.loop();
}
