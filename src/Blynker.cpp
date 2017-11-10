#include <config.h>
#include <Wifi.h>
#include <Blynker.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

BlynkTimer timer;
WidgetRTC rtc;
WidgetLED lightStatusLed(V0);

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

void sendTempDataToServer() {
  // float fridgeTemp = thermometers.getHumidity();
  // float beerTemp = thermometers.getTemperature();
  //
  // Blynk.virtualWrite(V1, fridgeTemp);
  // Blynk.virtualWrite(V2, beerTemp);
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
  timer.setInterval(1000L, sendTempDataToServer);
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
