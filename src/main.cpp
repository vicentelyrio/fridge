#include <Config.h>
#include <Display.h>
#include <Wifi.h>
#include <OTA.h>
#include <Blynker.h>

unsigned int buttonState = 0;
unsigned int lastButtonState = 0;
unsigned long startPressed = 0;
unsigned long endPressed = 0;
unsigned long timeHold = 0;
unsigned long firstPressed = 0;

unsigned long dashboardStart;
unsigned int dashboardInterval = 3000;

void displayCountDown(String message, int seconds) {
  display.println(message, 1, 10, 16);
  delay(1000);

  while (seconds > 0) {
    display.println(String(seconds), 3, 24, 14);
    delay(seconds * 500);
    seconds--;
  }
  display.println("0", 3, 24, 14);
}

void displayOK() {
  display.println("OK", 3, 24, 14);
  delay(500);
}

void setupWiFi(int duration) {
  display.println("WIFI Setup", 1, 12, 20);
  wifi.setup();
  while(WiFi.status() != WL_CONNECTED)
  delay(duration);
  displayOK();
}

void setupOTA(int duration) {
  display.println("OTA Setup", 1, 14, 20);
  ota.setup();
  delay(duration);
  displayOK();
}

void setupBlynk(int duration) {
  display.println("Blynk Auth", 1, 10, 20);
  delay(duration);
  char* token = wifi.getBlynkToken();
  blynk.setup(token);
  display.println("OK", 3, 24, 14);
  delay(duration);
}

void displaySplash(int duration) {
  display.splash();
  delay(duration);
}

void displayWelcome(int duration) {
  display.println("Welcome", 2, 0, 16);
  delay(duration);
}

void displayDashboard() {
  unsigned long dashboardCurrent = millis();

  if(dashboardCurrent - dashboardStart > dashboardInterval) {
    // int temperature = dht.getTemperature();
    // int humidity = dht.getHumidity();
    const char* startTime = blynk.getStartTime();
    const char* stopTime = blynk.getStopTime();
    const char* dateNow = blynk.getTime();

    // display.printDashboard(String(dateNow), String(temperature), String(humidity), String(startTime), String(stopTime));

    dashboardStart = dashboardCurrent;
  }
}

void checkButtonAction() {
  buttonState = digitalRead(BUTTONPIN);

  firstPressed = millis();

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      startPressed = millis();
    }
    else {
      endPressed = millis();
      timeHold = endPressed - startPressed;

      if (timeHold >= 2000 && timeHold <= 5000) {
        displayCountDown("Resetting", 2);
        ESP.reset();
      }

      if (timeHold >= 10000) {
        displayCountDown("Deleting data", 3);
        wifi.resetConfig();
      }
    }
  }

  lastButtonState = buttonState;
}

void setup() {
  Serial.begin(BAUD);

  while(!Serial)

  Serial.setDebugOutput(true);

  display.setup();

  setupWiFi(0);
  setupOTA(500);
  setupBlynk(500);
  displaySplash(1000);
  displayWelcome(1000);

  dashboardStart = millis();
}

void loop() {
  ota.loop();
  blynk.loop();
  checkButtonAction();
  displayDashboard();
}
