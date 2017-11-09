#include <config.h>
#include <Blynker.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

BlynkTimer timer;
WidgetRTC rtc;
WidgetLED lightStatusLed(V1);
WidgetLED buttonLed(V5);

boolean lightStatus = false;

int currentMin;
int currentHour;
String currentTime;
String startTime;
String stopTime;

int startTimeInSeconds;
int stopTimeInSeconds;
int currentTimeInSeconds;

void syncLightStatus() {
  lightStatus ? lightStatusLed.on() : lightStatusLed.off();
}

void setLightState() {
  lightStatus = startTimeInSeconds < currentTimeInSeconds && stopTimeInSeconds > currentTimeInSeconds;
  digitalWrite(RELAYPIN, lightStatus ? LOW : HIGH);
  syncLightStatus();
}

void syncRTC() {
  int hourInSeconds = hour() * 3600;
  int minuteInSeconds = minute() * 60;
  int seconds = second();

  currentTimeInSeconds = hourInSeconds + minuteInSeconds + seconds;

  setLightState();
}

void setLightPeriod(TimeInputParam t) {
  if (t.hasStartTime()) {
    int h = t.getStartHour();
    int m = t.getStartMinute();
    int s = t.getStartSecond();

    String hou = String(h);
    String min = String(m);

    if(h < 10) hou = String("0") + h;
    if(m < 10) min = String("0") + m;

    startTime = hou + ":" + min;

    Serial.println(startTime);
    startTimeInSeconds = (h * 3600) +  (m * 60) + s;
  }
  if (t.hasStopTime()) {
    int h = t.getStopHour();
    int m = t.getStopMinute();
    int s = t.getStopSecond();

    String hou = String(h);
    String min = String(m);

    if(h < 10) hou = String("0") + h;
    if(m < 10) min = String("0") + m;

    stopTime = hou + ":" + min;
    stopTimeInSeconds = (h * 3600) +  (m * 60) + s;
  }
}
//
// void sendDHTDataToServer() {
//   float humidity = dht.getHumidity();
//   float temperature = dht.getTemperature();
//
//   Blynk.virtualWrite(V2, temperature);
//   Blynk.virtualWrite(V3, humidity);
// }

BLYNK_WRITE(V0) {
  TimeInputParam t(param);
  setLightPeriod(t);
}

BLYNK_CONNECTED() {
  rtc.begin();
  Blynk.syncAll();
}

Blynker::Blynker() {}

void Blynker::setup(char* token) {
  Blynk.config(token);
  Blynk.connect();

  while (!Blynk.connected());

  pinMode(RELAYPIN, OUTPUT);
  pinMode(BL, OUTPUT);
  pinMode(BUTTONPIN, INPUT);

  timer.setInterval(5000L, syncRTC);
  // timer.setInterval(5000L, sendDHTDataToServer);
}

void Blynker::loop() {
  Blynk.run();
  timer.run();
}

const char* Blynker::getStartTime() {
  return startTime.c_str();
}

const char* Blynker::getStopTime() {
  return stopTime.c_str();
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
