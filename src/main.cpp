#include <ESP8266WiFi.h>
#include <CMMC_OTA.h>

#ifndef WIFI_SSID
  #define WIFI_SSID       "Vicente Lyrio's Wi-Fi Network"
  #define WIFI_PASSPHRASE "giuliaCarina"
#endif

CMMC_OTA ota;

void init_hardware()
{
  WiFi.disconnect(true);
  Serial.begin(115200);
  Serial.flush();
  Serial.println();
  Serial.println();
  Serial.println("being started...");
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);
  while (WiFi.status() != WL_CONNECTED) {
   Serial.printf("connecting %s:%s \r\n", WIFI_SSID, WIFI_PASSPHRASE);
   delay(100);
 }

  Serial.print("READY!! IP address: ");

  Serial.println(WiFi.localIP());
}

void setup()
{
  init_hardware();
  ota.init();
}

void loop()
{
  ota.loop();
}
