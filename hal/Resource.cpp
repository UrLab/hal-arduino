#include "hal.h"

Resource::Resource(const char *name, int pin) : 
    _name(name), _id(0), _pin(pin) 
{}

void Resource::declare(HALMsg & msg) const
{
    strcpy((char*) msg.data, name());
    msg.len = strlen(name());
    msg.rid = id();
    msg.write();
}

const char *Resource::name() const {return _name;}

void Resource::setID(int id){_id = id;}

int Resource::id() const {return _id;}

int Resource::pin() const {return _pin;}
