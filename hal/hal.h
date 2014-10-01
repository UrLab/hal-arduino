#ifndef DEFINE_HAL_HEADER
#define DEFINE_HAL_HEADER

#include <Arduino.h>

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
};

#define TRIGGER_AFTER 1000

class Trigger : public Resource {
    private:
        const int _active_state; 
        int _count_active, _count_inactive;
    public:
        explicit Trigger(const char *name, int pin, int active_state=HIGH);
        bool isActive() const;
        void check();
        void hit(int state);
};

class Switch : public Resource {
    private:
        int _state;
    public:
        explicit Switch(const char *name, int pin);
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


#define HAL_CREATE(name, sensors, triggers, switchs, anims)\
HAL name(sizeof(sensors)/sizeof(Sensor), sensors,\
    sizeof(triggers)/sizeof(Trigger), triggers,\
    sizeof(switchs)/sizeof(Switch), switchs,\
    sizeof(anims)/sizeof(Animation), anims)

class HAL {
    private:
        size_t N_SENSORS, N_TRIGGERS, N_SWITCHS, N_ANIMATIONS;
        Sensor *sensors;
        Trigger *triggers;
        Switch *switchs;
        Animation *animations;
        unsigned long int now=0, last_com=0, last_ping=0, lag=0;
        int j;
        unsigned char c, d, e;
        void com();
    public:
        HAL(
            size_t n_sens, Sensor *sens,
            size_t n_trigs, Trigger *trigs,
            size_t n_switchs, Switch *sw,
            size_t n_anim, Animation *anims
        );
        void setup();
        void loop();
        bool ping_timeout() const;
};

#endif
