#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometers {
  uint8_t temp_precision;
  unsigned long previousMillis = 0;
  const long delay = 700;

  DallasTemperature sensors;

  // TODO Figure how to put this info in the config
  DeviceAddress beerThermometer = { 0x28, 0xFF, 0xF5, 0x6B, 0xA1, 0x16, 0x04, 0x42 };
  DeviceAddress fridgeThermometer = { 0x28, 0xFF, 0xA7, 0x6B, 0xA1, 0x16, 0x04, 0x54 };

  String beerThermometerName = "Beer Thermometer";
  String fridgeThermometerName = "Fridge Thermometer";

  float getTemperature(DeviceAddress deviceAddress, DallasTemperature sensors) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= delay) {
      sensors.requestTemperatures();
      previousMillis = currentMillis;

      return sensors.getTempC(deviceAddress);
    }
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

  void printData(String deviceName, DeviceAddress deviceAddress, DallasTemperature sensors) {
    Serial.println();
    Serial.print(deviceName);
    Serial.print(" - ");
    printTemperature(deviceAddress, sensors);
  }

  void printDetails(String deviceName, DeviceAddress deviceAddress, DallasTemperature sensors) {
    Serial.println("");
    Serial.println(deviceName);
    Serial.println("-----------------------");
    printAddress(deviceAddress);
    printTemperature(deviceAddress, sensors);
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

    float getBeerTemp() {
      return getTemperature(beerThermometer, sensors);
    }

    float getFridgeTemp() {
      return getTemperature(fridgeThermometer, sensors);
    }

    void setup() {
      sensors.begin();

      Serial.print(sensors.getDeviceCount(), DEC);

      sensors.setResolution(beerThermometer, temp_precision);
      sensors.setResolution(fridgeThermometer, temp_precision);

      printDetails(beerThermometerName, beerThermometer, sensors);
      printDetails(fridgeThermometerName, fridgeThermometer, sensors);
    }

    void loop() {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= delay) {
        previousMillis = currentMillis;

        printData(beerThermometerName, beerThermometer, sensors);
        printData(fridgeThermometerName, fridgeThermometer, sensors);
      }
    }
};
