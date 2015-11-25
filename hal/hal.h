#ifndef DEFINE_HAL_HEADER
#define DEFINE_HAL_HEADER

#include <Arduino.h>
#include "HALMsg.h"

class Resource {
    private:
        const char *_name;
        int _id, _pin;
    public:
        explicit Resource(const char *name, int pin);
        const char *name() const;
        void setID(int id);
        int id() const;
        int pin() const;
        void declare(HALMsg & msg) const;
};

class Trigger : public Resource {
    private:
        const int _active_state; 
        bool _active;
        unsigned int _cursor, _state_count;
    public:
        explicit Trigger(
            const char *name,
            int pin,
            int active_state=HIGH,
            unsigned int state_count=250
        );
        bool isActive() const;
        void check();
        void hit(int state);
        void notification() const;
};

class Switch : public Resource {
    private:
        int _state;
        unsigned long int _min_delay, _last_change;
    public:
        explicit Switch(
            const char *name,
            int pin,
            unsigned long int min_delay=1000
        );
        void activate();
        void deactivate();
        bool isActive() const;
        void writeVal() const;
};

class Animation : public Resource {
    private:
        unsigned char _frames[255];
        unsigned char _len, _i;
        bool _loop, _play;
        unsigned int _delay;
        unsigned long int _tlast;
        bool _buzzer;

        void setOutput(unsigned char val);
    public:
        Animation(const char *name, int pin, bool is_buzzer=false);
        unsigned char getLen() const;
        void setLen(unsigned char len);

        unsigned char getDelay() const;
        void setDelay(unsigned int delay);

        bool isLoop() const;
        void setLoop(bool loop);

        bool isPlaying() const;
        void play(bool stop=false);

        unsigned char &operator[](unsigned char i);
        void run(unsigned long int t);
};

class Sensor : public Resource {
    public:
        Sensor(const char *name, int pin);
        unsigned int getValue() const;
};

class Rgb : public Resource {
    private:
        bool _has_pwm;
        unsigned char _color[3];
        int _pin_g, _pin_b;
    public:
        explicit Rgb(const char *name, bool pwm, int pin_r, int pin_g, int pin_b);
        void setColor(unsigned char r, unsigned char g, unsigned char b);
        void writeColor() const;
        void getColor(unsigned char *rgb) const;
};

#define HAL_CREATE(name, sensors, triggers, switchs, anims, rgbs)\
HAL name(sizeof(sensors)/sizeof(Sensor), sensors,\
    sizeof(triggers)/sizeof(Trigger), triggers,\
    sizeof(switchs)/sizeof(Switch), switchs,\
    sizeof(anims)/sizeof(Animation), anims,\
    sizeof(rgbs)/sizeof(Rgb), rgbs)

class HAL {
    private:
        size_t N_SENSORS, N_TRIGGERS, N_SWITCHS, N_ANIMATIONS, N_RGBS;
        Sensor *sensors;
        Trigger *triggers;
        Switch *switchs;
        Animation *animations;
        Rgb *rgbs;
        unsigned long int now, last_com, last_ping, lag;
        int j;
        unsigned char c, d, e;
        HALMsg msg;
        void com();
        void tree();
        void send_ping();
        void send_version();
    public:
        HAL(
            size_t n_sens, Sensor *sens,
            size_t n_trigs, Trigger *trigs,
            size_t n_switchs, Switch *sw,
            size_t n_anim, Animation *anims,
            size_t n_rgbs, Rgb *rgbs
        );
        void setup();
        void loop();
        bool ping_timeout() const;
        unsigned long int last_com_delay() const;
};

#endif
