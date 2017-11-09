#include <Config.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Display.h>
#include <OTA.h>

OTA::OTA() {}

void OTA::onStart() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
    display.println("Updating", 1, 16, 20);
  });
}

void OTA::onEnd() {
  ArduinoOTA.onEnd([]() {
    Serial.println("\nFinished");
    display.println("OK", 3, 24, 14);
  });
}

void OTA::onError() {
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
      display.println("Auth Failed", 1, 11, 20);
    }
    else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
      display.println("Begin Failed", 1, 11, 20);
    }
    else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
      display.println("Connect Failed", 1, 10, 20);
    }
    else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
      display.println("Receive Failed", 1, 9, 20);
    }
    else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
      display.println("End Failed", 1, 10, 20);
    }
  });
}

void OTA::onProgress() {
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int percent = (progress / (total / 100));
    Serial.printf("Upload Progress: %u%%\r", percent);
    display.println((String)(percent), 3, 16, 14);
  });
}

void OTA::setup() {
  onStart();
  onEnd();
  onProgress();
  onError();

  ArduinoOTA.setHostname(HOSTNAME);
  // ArduinoOTA.setPassword(HOSTPASS);
  ArduinoOTA.begin();
}

void OTA::loop() {
  ArduinoOTA.handle();
}

OTA ota = OTA();
