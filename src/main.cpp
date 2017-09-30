#include <OneWire.h>
#include <config.h>
#include <Wifi.cpp>
#include <Thermometers.cpp>

Wifi wifi(SSID, PASS, HOSTNAME, HOSTPASS);

OneWire oneWire(ONE_WIRE_BUS);
Thermometers thermometers(oneWire, TEMPERATURE_PRECISION);

void setup() {
  Serial.begin(BAUD);

  while(!Serial)

  Serial.println();
  Serial.println("Starting setup");
  Serial.println("-----------------------");

  wifi.setup();
  thermometers.setup();
}

void loop() {
  wifi.loop();
  thermometers.loop();

  delay(1000);
}
