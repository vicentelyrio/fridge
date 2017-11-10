#ifndef Display_h
#define Display_h

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

class Display {
  public:
    Display();
    void splash();
    void println(String text, int size = 1, int x = 0, int y = 0);
    void printDashboard(String dateNow, String temperature, String humidity, String startTime, String stopTime);
    void setup();
};

extern Display display;

#endif
