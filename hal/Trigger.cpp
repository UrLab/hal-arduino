#include "hal.h"

Trigger::Trigger(
    const char *name,
    int pin,
    int active_state,
    unsigned int state_count
) :
    Resource(name, pin), _active_state(active_state), _active(false),
    _cursor(0), _state_count(state_count)
{
    pinMode(pin, INPUT);
}


bool Trigger::isActive() const
{
    return _active;
}


void Trigger::check()
{
    hit(digitalRead(pin()));
}


void Trigger::notification(const char *state)
{
    Serial.print("!");
    Serial.print(id(), DEC);
    Serial.println(state);
}


void Trigger::hit(int state)
{
    if (state == _active_state && _cursor < _state_count){
        _cursor++;
        if (_cursor == _state_count && ! _active){
            _active = true;
            notification("1");
        }
    } else if (state != _active_state && _cursor > 0){
        _cursor--;
        if (_cursor == 0 && _active){
            _active = false;
            notification("0");
        }
    }
}
