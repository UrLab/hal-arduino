#include "HALMsg.h"

static unsigned char current_seq = 0;

unsigned char next_seq()
{
    current_seq = ARDUINO_SEQ(ABSOLUTE_SEQ(current_seq) + 1);
    return current_seq;
}

unsigned char read_byte()
{
    waitSerial();
    char c = Serial.read();
    if (c == ESC){
        waitSerial();
        c = Serial.read();
    }
    return c;
}

void write_byte(unsigned char c)
{
    if (c == ESC || c == SYNC){
        Serial.write(ESC);
    }
    Serial.write(c);
}
