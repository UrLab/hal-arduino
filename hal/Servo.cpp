#include "hal.h"

ServoAnim::ServoAnim(const char *name, int pin) : Resource(name, pin){
    _servo.attach(pin);
    _servo.write(105);
}

void ServoAnim::write(unsigned char val){
    _servo.write(val);
}

unsigned char ServoAnim::read(){
    return _servo.read();
}
