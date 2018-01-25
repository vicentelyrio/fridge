#ifndef Thermometers_h
#define Thermometers_h

class Thermometers {
  public:
    Thermometers();
    void setup();
    float getFridgeTemp();
    float getBeerTemp();
    float getFreezerTemp();
};

extern Thermometers thermometers;

#endif
