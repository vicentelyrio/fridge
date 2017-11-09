#ifndef OTA_h
#define OTA_h

#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Display.h>

class OTA {
  void onEnd();
  void onError();
  void onProgress();
  void onStart();

  public:
    OTA();
    void setup();
    void loop();
};

extern OTA ota;

#endif
