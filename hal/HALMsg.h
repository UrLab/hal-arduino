#ifndef DEFINE_HALMSG_HEADER
#define DEFINE_HALMSG_HEADER

typedef enum {
    BOOT='#',
    HAL_PING='*',
    VERSION='?',
    TREE='$',

    TRIGGER='T',
    SWITCH='S',
    SENSOR='C',
    ANIMATION_FRAMES='F',
    ANIMATION_DELAY='D',
    ANIMATION_LOOP='L',
    ANIMATION_PLAY='P',

    PARAM_CHANGE=0x80,
    PARAM_ASK=0x00
} HALCommand;

class HALMsg {
    private:
        unsigned char chk; //!< Checksum
    public:
        unsigned char cmd; //!< Hal command
        unsigned char rid; //!< Resource id
        unsigned char len; //!< Data len
        unsigned char data[255]; //!< Command data, if any

        bool is(HALCommand type) const;
        bool change() const;
        void read(unsigned long int timeout_ms=500);
        void write();
        bool is_valid() const;

        unsigned char compute_checksum() const;

        void reset();
};

#endif
