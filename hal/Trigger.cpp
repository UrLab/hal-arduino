#include "hal.h"

Trigger::Trigger(const char *name, int pin, int active_state) : 
    Resource(name, pin), _active_state(active_state), _count_active(0), 
    _count_inactive(0)
{
    pinMode(pin, INPUT);
}


bool Trigger::isActive() const
{
    return _count_active == TRIGGER_AFTER;
}


void Trigger::check()
{
    hit(digitalRead(pin()));
}


void Trigger::hit(int state)
{
    if (state == _active_state && _count_active < TRIGGER_AFTER){
        _count_active ++;
        if (_count_active == TRIGGER_AFTER){
            Serial.print("!");
            Serial.print(id(), DEC);
            Serial.println("1");
            _count_inactive = 0;
        }
    } else if (state != _active_state && _count_inactive < TRIGGER_AFTER){
        _count_inactive ++;
        if (_count_inactive == TRIGGER_AFTER){
            Serial.print("!");
            Serial.print(id(), DEC);
            Serial.println("0");
            _count_active = 0;
        }
    }
}