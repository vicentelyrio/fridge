#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometers {
  uint8_t temp_precision;
  DallasTemperature sensors;
  DeviceAddress beerThermometer = { 0x28, 0xFF, 0xF5, 0x6B, 0xA1, 0x16, 0x04, 0x42 };
  DeviceAddress fridgeThermometer = { 0x28, 0xFF, 0xA7, 0x6B, 0xA1, 0x16, 0x04, 0x54 };
  char* beerThermometerName = "Beer Thermometer";
  char* fridgeThermometerName = "Fridge Thermometer";

  void printAddress(DeviceAddress deviceAddress) {
    Serial.print("Address: ");
    for (uint8_t i = 0; i < 8; i++) {
      if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
    }
    Serial.println();
  }

  void printTemperature(DeviceAddress deviceAddress, DallasTemperature sensors) {
    float tempC = sensors.getTempC(deviceAddress);
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.println("ºC");
  }

  void printResolution(DeviceAddress deviceAddress, DallasTemperature sensors) {
    Serial.print("Resolution: ");
    Serial.println(sensors.getResolution(deviceAddress));
  }

  void printData(char* deviceName, DeviceAddress deviceAddress, DallasTemperature sensors) {
    Serial.println();
    Serial.println(deviceName);
    Serial.println("-----------------------");
    printTemperature(deviceAddress, sensors);
  }

  void printDetails(char* deviceName, DeviceAddress deviceAddress, DallasTemperature sensors) {
    Serial.println("");
    Serial.println(deviceName);
    Serial.println("-----------------------");
    printAddress(deviceAddress);
    printResolution(deviceAddress, sensors);
  }

  public:
    Thermometers(
      OneWire _oneWire,
      uint8_t _temp_precision
    )
    :
    sensors(&_oneWire),
    temp_precision(_temp_precision)
    {
    }

    void setup() {
      sensors.begin();

      sensors.setResolution(beerThermometer, temp_precision);
      sensors.setResolution(fridgeThermometer, temp_precision);

      printDetails(beerThermometerName, beerThermometer, sensors);
      printDetails(fridgeThermometerName, fridgeThermometer, sensors);
    }

    void loop() {
      printData(beerThermometerName, beerThermometer, sensors);
      printData(fridgeThermometerName, fridgeThermometer, sensors);
    }
};
