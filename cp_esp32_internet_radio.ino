#include <Arduino.h>

#if defined(ESP32)
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "Audio.h"
#include "wifi_const.h"
#include "CPOledDisplay.h"
#include "CPIoTMqtt.h"
//#define WIFI_SSID "xxx"
//#define WIFI_PASSWD "xxx"

#define MAX98357A_I2S_DOUT  22
#define MAX98357A_I2S_BCLK 26
#define MAX98357A_I2S_LRC  25
Audio audio;
CPOledDisplay display;
CPIoTMqtt mqtt;


void beep() {
}

void mqtt_callback_display(String text) {
  display.setStatus(text);
}

void mqtt_callback_pager_message(String sender, String receiver, String message, String textPixelBase64, int textCount) {
  Serial.println("mqtt_callback_pager_message");
  beep();
  display.updatePagerMessage(sender, receiver, message, textPixelBase64, textCount);
}

void mqtt_callback_radio(int event, String url) {
  Serial.printf("mqtt_callback_radio event: %d\n", event);
  Serial.println(url);
  if (event == 1) {
    display.setStatus(url);
    radio_play(url);
  } else {
    radio_stop();
  }
}

void radio_play(String url) {
  mqtt.disconnect();
  delay(2000);
  Serial.println("init wifi for audio...");
  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  Serial.println("wifi connect");
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }
  audio.setPinout(MAX98357A_I2S_BCLK, MAX98357A_I2S_LRC, MAX98357A_I2S_DOUT);
  audio.setVolume(15);

  Serial.println("audio connecting...");
  if (audio.connecttohost(url.c_str())) { // "https://lhttp.qtfm.cn/live/1099/64k.mp3"
    Serial.println("audio playing...");
  } else {
    Serial.println("audio connect failed");
  }

}

void radio_stop() {
  if (audio.isRunning()) {
    audio.stopSong();
  }
}

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.setDebugOutput(true);


  Serial.println("init display...");
  display.init();

#if 1
  Serial.println("init wifi for audio...");
  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  Serial.println("wifi connect");
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }

#endif

  Serial.println("init mqtt...");
  delay(1000);
  display.setStatus("你好 CP IoT==========");
  delay(2000);

  mqtt.addDisplayCallback(&mqtt_callback_display);
  mqtt.addPagerCallback(&mqtt_callback_pager_message);
  mqtt.addRadioCallback(&mqtt_callback_radio);
  mqtt.connect(MQTT_BROKER, MQTT_PORT);

  Serial.println("");
  Serial.println("wifi connected");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
}


void loop() {
  mqtt.loop();
  if (audio.isRunning()) {
    audio.loop();
  }
}
