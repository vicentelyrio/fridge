#include <config.h>
#include <Wifi.cpp>
#include <OneWire.h>
#include <DallasTemperature.h>

Wifi wifi(SSID, PASS, HOSTNAME, HOSTPASS);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress beerThermometer = { 0x28, 0xFF, 0xF5, 0x6B, 0xA1, 0x16, 0x04, 0x42 };
DeviceAddress fridgeThermometer = { 0x28, 0xFF, 0xA7, 0x6B, 0xA1, 0x16, 0x04, 0x54 };

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress) {
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

void getDeviceAddress(void) {
  byte i;
  byte addr[8];

  Serial.println("Getting the address...\n\r");
  /* initiate a search for the OneWire object we created and read its value into
  addr array we declared above*/

  while(oneWire.search(addr)) {
    Serial.println("The address is:\t");
    //read each byte in the address array
    for( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      // print each byte in the address array in hex format
      Serial.print(addr[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    // a check to make sure that what we read is correct.
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!\n");
        return;
    }
  }
  oneWire.reset_search();
  return;
}


// main function to print information about a device
void printData(DeviceAddress deviceAddress) {
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void setup() {
  Serial.begin(BAUD);

  while(!Serial)

  Serial.println("Starting setup");

  wifi.setup();

  // getDeviceAddress();

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(beerThermometer);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(fridgeThermometer);
  Serial.println();

  // set the resolution to 9 bit per device
  sensors.setResolution(beerThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(fridgeThermometer, TEMPERATURE_PRECISION);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(beerThermometer), DEC);
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(fridgeThermometer), DEC);
  Serial.println();
}

void loop() {
  wifi.loop();

  delay(1000);

  // Serial.print("Temperature for DS18B20 is: ");
  // Serial.println(sensors.getTempCByIndex(0));

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  // print the device information
  printData(beerThermometer);
  printData(fridgeThermometer);
}
