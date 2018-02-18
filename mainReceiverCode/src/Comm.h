#include <Arduino.h>

class Comm
{
  public:
    Comm(int pin);
  private:
    int _pin;
};

Comm::Comm(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}
