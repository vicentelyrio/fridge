#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class Wifi {
  const char* _ssid;
  const char* _pass;
  const char* _hostname;
  const char* _hostpass;

  public:
    Wifi(const char* ssid,
      const char* pass,
      const char* hostname,
      const char* hostpass
    )
    :
    _ssid(ssid),
    _pass(pass),
    _hostname(hostname),
    _hostpass(hostpass)
    { }

    void setup() {
      // onStart OTA
      ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      });

      // onEnd OTA
      ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
      });

      // onProgress OTA
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });

      // onError OTA
      ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

      Serial.println();
      Serial.println("Starting Wifi Setup");
      Serial.println("-----------------------");

      // Connecting to wifi
      WiFi.mode(WIFI_STA);
      WiFi.begin(_ssid, _pass);

      // Wait for Connection
      // Change for a counting tries
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
      }

      Serial.println("Wifi Ready");

      Serial.println("Starting OTA Setup");

      // Set HOST name and password
      ArduinoOTA.setHostname(_hostname);
      // ArduinoOTA.setPassword(_hostpass);

      // Begin OTA
      ArduinoOTA.begin();

      Serial.println("OTA Ready");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }

    void loop() {
      ArduinoOTA.handle();
    }
};
