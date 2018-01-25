#include <Thermometers.h>
#include <config.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String beerThermometerName = "Beer Thermometer";
String fridgeThermometerName = "Fridge Thermometer";
String freezerThermometerName = "Freezer Thermometer";

DeviceAddress beerThermometer, fridgeThermometer, freezerThermometer;

unsigned char* devices[] = { beerThermometer, fridgeThermometer, freezerThermometer };
String devicesNames[] = { beerThermometerName, fridgeThermometerName, freezerThermometerName };

float getTemperature(DeviceAddress deviceAddress, DallasTemperature sensors) {
  sensors.requestTemperatures();
  return sensors.getTempC(deviceAddress);
}

void printAddress(DeviceAddress deviceAddress) {
  Serial.print("Address: ");
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.println();
}

void printTemperature(DeviceAddress deviceAddress, DallasTemperature sensors) {
  float tempC = getTemperature(deviceAddress, sensors);
  Serial.print(tempC);
  Serial.println("ºC");
}

void printResolution(DeviceAddress deviceAddress, DallasTemperature sensors) {
  Serial.print("Resolution: ");
  Serial.println(sensors.getResolution(deviceAddress));
}

void printDetails(String deviceName, DeviceAddress deviceAddress, DallasTemperature sensors) {
  Serial.println("");
  Serial.println(deviceName);
  Serial.println("-----------------------");
  printAddress(deviceAddress);
  printTemperature(deviceAddress, sensors);
  printResolution(deviceAddress, sensors);
}

void setSensorAddresses(DallasTemperature sensors) {
  unsigned int length = sizeof(devices)/sizeof(devices[0]);

  for(unsigned int i = 0; i < length; i++) {
    unsigned char* device = devices[i];
    String name = devicesNames[i];

    if(!sensors.getAddress(device, i)) {
      Serial.println("Unable to find " + name);
    }
    else {
      sensors.setResolution(device, TEMPERATURE_PRECISION);
      printDetails(name, device, sensors);
    }
  }
}

Thermometers::Thermometers() {

}

void Thermometers::setup() {
  sensors.begin();

  setSensorAddresses(sensors);
}

float Thermometers::getBeerTemp() {
  return getTemperature(beerThermometer, sensors);
}

float Thermometers::getFridgeTemp() {
  return getTemperature(fridgeThermometer, sensors);
}

float Thermometers::getFreezerTemp() {
  return getTemperature(fridgeThermometer, sensors);
}

Thermometers thermometers = Thermometers();
