#ifndef Blynker_h
#define Blynker_h

#include <string>
using namespace std;

class Blynker {
  public:
    Blynker();
    const char* getStartTime();
    const char* getStopTime();
    const char* getTime();
    void setup(char* token);
    void loop();
};

extern Blynker blynk;

#endif
