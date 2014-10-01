#include "hal.h"

Sensor::Sensor(const char *name, int pin) : Resource(name, pin) {}

unsigned int Sensor::getValue() const
{
    return analogRead(pin());
}
