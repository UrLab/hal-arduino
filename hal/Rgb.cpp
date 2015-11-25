#include "hal.h"

Rgb::Rgb(const char *name, bool pwm, int pin_r, int pin_g, int pin_b) :
    Resource(name, pin_r), _has_pwm(pwm), _pin_g(pin_g), _pin_b(pin_b)
{
    pinMode(pin_r, OUTPUT);
    pinMode(pin_g, OUTPUT);
    pinMode(pin_b, OUTPUT);
    setColor(0, 0, 0);
}

void Rgb::setColor(unsigned char r, unsigned char g, unsigned char b)
{
    if (_has_pwm){
        _color[0] = r;
        _color[1] = g;
        _color[2] = b;
    } else {
        _color[0] = (r > 127) ? 255 : 0;
        _color[1] = (g > 127) ? 255 : 0;
        _color[2] = (b > 127) ? 255 : 0;
    }
}

void Rgb::writeColor() const
{
    if (_has_pwm){
        analogWrite(pin(), _color[0]);
        analogWrite(_pin_g, _color[1]);
        analogWrite(_pin_b, _color[2]);
    } else {
        digitalWrite(pin(), (_color[0] > 127) ? HIGH : LOW);
        digitalWrite(_pin_g, (_color[1] > 127) ? HIGH : LOW);
        digitalWrite(_pin_b, (_color[2] > 127) ? HIGH : LOW);
    }
}

void Rgb::getColor(unsigned char *rgb) const
{
    rgb[0] = _color[0];
    rgb[1] = _color[1];
    rgb[2] = _color[2];
}
