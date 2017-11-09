#ifndef Wifi_h
#define Wifi_h

#include <FS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Display.h>
#include <Wifi.h>

#include <WiFiManager.h>

class Wifi {

  public:
    Wifi();
    IPAddress getIP();
    void saveConfig(char* blynk_token);
    void resetConfig();
    void getConfig();
    char* getBlynkToken();
    void setup();
};

extern Wifi wifi;

#endif
