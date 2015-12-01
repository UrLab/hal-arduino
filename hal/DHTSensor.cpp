#include "hal.h"
#include "DHT.h"

DHTSensor::DHTSensor(const char *name, DHT dht, bool type, int pin) : Resource(name, pin), _dht(dht), _type(type) {}

unsigned int DHTSensor::getValue()
{
    float value = 0;
    if (_type){
        value = _dht.readHumidity();
    }
    else {
        value = _dht.readTemperature();
    }
    return value;
}
