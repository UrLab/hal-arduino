#include "hal.h"

Resource::Resource(const char *name, int pin) : 
    _name(name), _id(0), _pin(pin) 
{}

const char *Resource::name() const {return _name;}

void Resource::setID(int id){_id = id;}

int Resource::id() const {return _id;}

int Resource::pin() const {return _pin;}
