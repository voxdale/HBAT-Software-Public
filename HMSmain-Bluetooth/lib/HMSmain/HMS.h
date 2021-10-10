#ifndef HMS_h
#define HMS_h
#include "Arduino.h"

class HMS
{
  public:
    HMS();
    float* readSensAndCondition();
    float* climate();
  /* private:
    int _pin;
    const int ADC1 = analogRead(_pin); */
};
#endif