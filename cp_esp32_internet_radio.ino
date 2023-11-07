#include <Arduino.h>
#include <WiFi.h>
#include "Audio.h"
#include "wifi_const.h"
#include "CPOledDisplay.h"
//#define WIFI_SSID "xxx"
//#define WIFI_PASSWD "xxx"

#define MAX98357A_I2S_DOUT  22
#define MAX98357A_I2S_BCLK 26
#define MAX98357A_I2S_LRC  25
Audio audio;
CPOledDisplay display;

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.setDebugOutput(true);


  Serial.println("init display...");
  display.init();
  
  Serial.println("init wifi...");
  WiFi.disconnect();

  WiFi.mode(WIFI_STA);

  Serial.println("wifi connect");
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("wifi connected");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");


  audio.setPinout(MAX98357A_I2S_BCLK, MAX98357A_I2S_LRC, MAX98357A_I2S_DOUT);
  audio.setVolume(15);

  Serial.println("audio connecting...");
  if (audio.connecttohost("https://lhttp.qtfm.cn/live/1099/64k.mp3")) {
    Serial.println("audio playing...");
  } else {
    Serial.println("audio connect failed");
  }

}


void loop() {
  audio.loop();
}
