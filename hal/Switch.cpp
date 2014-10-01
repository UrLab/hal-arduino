#include "hal.h"

Switch::Switch(const char *name, int pin) : 
    Resource(name, pin), _state(LOW)
{
    pinMode(pin, OUTPUT);
}

void Switch::activate()
{
    _state = HIGH;
}

void Switch::deactivate()
{
    _state = LOW;
}

bool Switch::isActive() const
{
    return _state == HIGH;
}

void Switch::writeVal() const
{
    digitalWrite(pin(), _state);
}