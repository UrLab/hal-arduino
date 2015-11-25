#include "hal.h"

void Animation::setOutput(unsigned char val)
{
    if (! _buzzer)
        analogWrite(pin(), val);
    else if (val == 0)
        noTone(pin());
    else {
        int note = 10*val;
        tone(pin(), note);
    }
}

Animation::Animation(const char *name, int pin, bool is_buzzer) : 
    Resource(name, pin), _len(0), _loop(true), _play(true), _delay(100),
    _buzzer(is_buzzer)
{
    memset(_frames, 0, sizeof(_frames));
    pinMode(pin, OUTPUT);
    setLen(1);
}

unsigned char Animation::getLen() const {return _len;}
void Animation::setLen(unsigned char len){_len = len;}

unsigned char Animation::getDelay() const {return _delay;}
void Animation::setDelay(unsigned int delay){_delay = delay;}

bool Animation::isLoop() const {return _loop;}
void Animation::setLoop(bool loop){_loop = loop;}

bool Animation::isPlaying() const {return _play;}
void Animation::play(bool stop){_play = ! stop;}

unsigned char & Animation::operator[](unsigned char i){return _frames[i];}

void Animation::run(unsigned long int t)
{
    if (_len == 0 || ! _play){
        setOutput(0);
        return;
    }
    if (t - _tlast >= _delay){
        _i = (_i+1)%_len;
        /* 1 shot: stop when end has been reached */
        if (! _loop && _i == 0)
            _play = false;
        _tlast = t;
    }
    setOutput(_frames[_i]);
}