#include "hal.h"

Switch::Switch(const char *name, int pin, unsigned long int min_delay) :
    Resource(name, pin), _state(LOW), _min_delay(min_delay), _last_change(0)
{
    pinMode(pin, OUTPUT);
}

void Switch::activate()
{
    unsigned long int now = millis();
    if (now - _last_change > _min_delay){
        _state = HIGH;
        _last_change = now;
    }
}

void Switch::deactivate()
{
    unsigned long int now = millis();
    if (now - _last_change > _min_delay){
        _state = LOW;
        _last_change = now;
    }
}

bool Switch::isActive() const
{
    return _state == HIGH;
}

void Switch::writeVal() const
{
    digitalWrite(pin(), _state);
}