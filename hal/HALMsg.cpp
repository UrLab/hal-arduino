#include "HALMsg.h"
#include <Arduino.h>

static inline bool waitSerial(unsigned long int timeout)
{
    unsigned long int start = millis();
    while (! Serial.available())
        if (millis() - start > timeout){
            Serial.write(0xff);
            Serial.write(0xff);
            Serial.write(0xff);
            Serial.write(0xff);
            return false;
        }
    return true;
}

bool HALMsg::is(HALCommand type) const
{
    return (cmd & ~PARAM_CHANGE) == type;
}

bool HALMsg::change() const
{
    return (cmd & PARAM_CHANGE) == PARAM_CHANGE;
}

void HALMsg::read(unsigned long int timeout_ms)
{
    if (! waitSerial(timeout_ms)) return;
    cmd = Serial.read();

    if (! waitSerial(timeout_ms)) return;
    rid = Serial.read();

    if (! waitSerial(timeout_ms)) return;
    len = Serial.read();

    if (! waitSerial(timeout_ms)) return;
    chk = Serial.read();

    for (unsigned char i=0; i<len; i++){
        if (! waitSerial(timeout_ms)) return;
        data[i] = Serial.read();
    }
}

void HALMsg::write()
{
    chk = compute_checksum();
    Serial.write(cmd);
    Serial.write(rid);
    Serial.write(len);
    Serial.write(chk);
    Serial.write(data, len);
}

bool HALMsg::is_valid() const
{
    return compute_checksum() == chk;
}

unsigned char HALMsg::compute_checksum() const
{
    unsigned char res = 0;
    res += cmd;
    res += rid;
    res += len;
    for (unsigned char i=0; i<len; i++)
        res += data[i];
    return res;
}

void HALMsg::reset()
{
    cmd = 0;
    len = 0;
    chk = 0;
    rid = 0;
    memset(data, 0, sizeof(data));
}
