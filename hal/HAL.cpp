#include "hal.h"

#define VERSION_STR "0123456789abcdef0123456789abcdef01234567"

HAL::HAL(
    size_t n_sens, Sensor *sens,
    size_t n_trig, Trigger *trig,
    size_t n_switch, Switch *sw,
    size_t n_anim, Animation *anim
) :
    N_SENSORS(n_sens), N_TRIGGERS(n_trig), N_SWITCHS(n_switch), 
    N_ANIMATIONS(n_anim), sensors(sens), triggers(trig), switchs(sw),
    animations(anim), now(0), last_com(0), last_ping(0), lag(0), j(0), c(0), 
    d(0), e(0)
{}

void HAL::setup()
{
    for (int i=0; i<255; i++){
        if (i<N_ANIMATIONS) animations[i].setID(i);
        if (i<N_SENSORS) sensors[i].setID(i);
        if (i<N_TRIGGERS) triggers[i].setID(i);
        if (i<N_SWITCHS) switchs[i].setID(i);
    }

    Serial.begin(115200);

    msg.cmd = BOOT;
    msg.len = 0;
    msg.rid = 0;
    msg.write();

    last_ping = millis();
}

void HAL::loop()
{
    now = millis();

    if (Serial.available()){
        com();
    }

    /* Ping every second if no communication within last 750ms */
    if (now - last_com > 750 && now - last_ping > 1000){
        send_ping();
        last_ping = now;
    }

    for (int i=0; i<N_TRIGGERS; i++)
        triggers[i].check();
    for (int i=0; i<N_ANIMATIONS; i++)
        animations[i].run(now);
    for (int i=0; i<N_SWITCHS; i++)
        switchs[i].writeVal();
}

long unsigned int HAL::last_com_delay() const
{
    return millis() - last_com;
}

bool HAL::ping_timeout() const
{
    return (last_com_delay() > 2500);
}

void HAL::send_ping()
{
    msg.clear();
    msg.cmd = HAL_PING;
    msg.len = 0;
    msg.rid = 0;
    msg.write();
}

void HAL::tree()
{
    size_t i, l;
    msg.cmd = TREE;

    msg.data[0] = SENSOR;
    msg.len = 1;
    msg.rid = N_SENSORS;
    msg.write();
    for (size_t i=0; i<N_SENSORS; i++){
        sensors[i].declare(msg);
    }

    msg.data[0] = SWITCH;
    msg.len = 1;
    msg.rid = N_SWITCHS;
    msg.write();
    for (i=0; i<N_SWITCHS; i++){
        switchs[i].declare(msg);
    }

    msg.data[0] = TRIGGER;
    msg.len = 1;
    msg.rid = N_TRIGGERS;
    msg.write();
    for (i=0; i<N_TRIGGERS; i++){
        triggers[i].declare(msg);
    }

    msg.data[0] = ANIMATION_FRAMES;
    msg.len = 1;
    msg.rid = N_ANIMATIONS;
    msg.write();
    for (i=0; i<N_ANIMATIONS; i++){
        animations[i].declare(msg);
    }
}

void HAL::send_version()
{
    msg.clear();
    
}

void HAL::com()
{
    msg.clear();
    msg.read();
    if (msg.checksum() != msg.chk)
        return;

    last_com = millis();
    if (msg.type() == HAL_PING){
        lag = now - last_ping;
        return;
    }

    else if (msg.type() == TREE){
        tree();
        return;
    }

    else if (msg.type() == VERSION){
        msg.cmd = VERSION|PARAM_CHANGE;
        msg.rid = 0;
        msg.len = 40;
        strcpy((char*) msg.data, VERSION_STR);
        msg.reply();
    }

    else if (msg.type() == SENSOR){
        if (msg.rid >= N_SENSORS)
            return;
        msg.len = 2;
        unsigned int val = sensors[msg.rid].getValue();
        msg.data[0] = (val>>8)&0xff;
        msg.data[1] = val&0xff;
        msg.reply();
    }

    else if (msg.type() == TRIGGER){
        if (msg.rid >= N_TRIGGERS)
            return;
        msg.len = 1;
        msg.data[0] = triggers[msg.rid].isActive() ? 1 : 0;
        msg.reply();
    }

    else if (msg.type() == SWITCH){
        if (msg.rid >= N_SWITCHS)
            return;

        if (msg.is_change() && msg.len == 1){
            if (msg.data[0]) switchs[msg.rid].activate();
            else switchs[msg.rid].deactivate();
            msg.cmd = SWITCH;
        }

        msg.len = 1;
        msg.data[0] = switchs[msg.rid].isActive() ? 1 : 0;
        msg.reply();
    }

    else if (msg.rid < N_ANIMATIONS){
        Animation & anim = animations[msg.rid];

        if (msg.type() == ANIMATION_FRAMES){
            if (msg.is_change()){
                anim.setLen(msg.len);
                for (unsigned char i=0; i<msg.len; i++){
                    anim[i] = msg.data[i];
                }
                msg.len = 0;
            } else {
                msg.len = anim.getLen();
                for (unsigned char i=0; i<msg.len; i++){
                    msg.data[i] = anim[i];
                }
            }
        }

        else if (msg.type() == ANIMATION_DELAY){
            if (msg.is_change() && msg.len == 1){
                anim.setDelay(msg.data[0]);
                msg.cmd = ANIMATION_DELAY;
            }
            msg.len = 1;
            msg.data[0] = anim.getDelay();
        }

        else if (msg.type() == ANIMATION_PLAY){
            if (msg.is_change() && msg.len == 1){
                anim.play(msg.data[0] ? false : true);
                msg.cmd = ANIMATION_PLAY;
            }
            msg.len = 1;
            msg.data[0] = anim.isPlaying() ? 1 : 0;
        }

        else if (msg.type() == ANIMATION_LOOP){
            if (msg.is_change() && msg.len == 1){
                anim.setLoop(msg.data[0] ? true : false);
                msg.cmd = ANIMATION_LOOP;
            }
            msg.len = 1;
            msg.data[0] = anim.isLoop() ? 1 : 0;
        }
        msg.reply();
    }
}
