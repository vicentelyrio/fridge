#include <config.h>
#include <Wifi.h>
#include <Thermometers.h>
#include <Blynker.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

BlynkTimer timer;
WidgetRTC rtc;

WidgetLED heatLED(V4);
WidgetLED fanLED(V5);
WidgetLED compressorLED(V6);

int currentMin;
int currentHour;
String currentTime;

int currentTimeInSeconds;

void syncRTC() {
  int hourInSeconds = hour() * 3600;
  int minuteInSeconds = minute() * 60;
  int seconds = second();

  currentTimeInSeconds = hourInSeconds + minuteInSeconds + seconds;
}

void sendTempsDataToServer() {
  float fridgeTemp = thermometers.getFridgeTemp();
  float beerTemp = thermometers.getBeerTemp();
  float freezerTemp = thermometers.getFreezerTemp();

  Blynk.virtualWrite(V1, freezerTemp);
  Blynk.virtualWrite(V2, fridgeTemp);
  Blynk.virtualWrite(V3, beerTemp);
}

BLYNK_WRITE(V4) {
  param.asInt() ? heatLED.on() : heatLED.off();
}

BLYNK_WRITE(V5) {
  param.asInt() ? fanLED.on() : fanLED.off();
}

BLYNK_WRITE(V6) {
  param.asInt() ? compressorLED.on() : compressorLED.off();
}

BLYNK_CONNECTED() {
  rtc.begin();
  Blynk.syncAll();
}

Blynker::Blynker() {}

void Blynker::setup(char* token) {
  Blynk.config(token);
  Blynk.connect();

  while (!Blynk.connected())

  if(!Blynk.connected()) {
    display.println("Wrong Blynk Auth");
    delay(1000);
    display.println("Resetting");
    wifi.resetConfig();
  }

  timer.setInterval(5000L, syncRTC);
  timer.setInterval(1000L, sendTempsDataToServer);
}

void Blynker::loop() {
  Blynk.run();
  timer.run();
}

const char* Blynker::getTime() {
  currentMin = minute();
  currentHour = hour();

  if(currentMin && currentHour) {
    String hou = String(currentHour);
    String min = String(currentMin);

    if(currentHour < 10) hou = String("0") + currentHour;
    if(currentMin < 10) min = String("0") + currentMin;

    currentTime = hou + ":" + min;
  }

  return currentTime.c_str();
}

Blynker blynk = Blynker();
