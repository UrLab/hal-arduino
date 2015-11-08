#ifndef DEFINE_HALMSG_HEADER
#define DEFINE_HALMSG_HEADER

#include <Arduino.h>

static const unsigned char SYNC = 0xff;
static const unsigned char ESC  = 0xaa;

unsigned char next_seq();

unsigned char read_byte();

void write_byte(unsigned char c);

#define HALMSG_SEQ_MAX 0x7f
#define ABSOLUTE_SEQ(x) ((x) & 0x7f)
#define DRIVER_SEQ(x) ABSOLUTE_SEQ(x)
#define ARDUINO_SEQ(x) (0x80 | ABSOLUTE_SEQ(x))
#define IS_DRIVER_SEQ(x) ((x & 0x80) == 0x00)
#define IS_ARDUINO_SEQ(x) ((x & 0x80) == 0x80)

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

#define waitSerial() for (int i=0; i<1000 && ! Serial.available(); i++)

template <unsigned char datalen>
struct HALMsg_t {
    unsigned char chk;        //!< Checksum
    unsigned char seq;        //!< Sequence id of the message
    unsigned char cmd;        //!< Hal command
    unsigned char rid;        //!< Resource id
    unsigned char len;        //!< Data len
    unsigned char data[datalen];  //!< Command data, if any

    void read(){
        unsigned char c;

        /* 1. Find 3x SYNC */
        int sync_count = 0;

        while (sync_count != 3){
            waitSerial();
            c = Serial.read();
            if (c == SYNC){
                sync_count ++;
            } else {
                sync_count = 0;
            }
        }

        /* 2. Read header */
        chk = read_byte();
        seq = read_byte();
        cmd = read_byte();
        rid = read_byte();
        len = read_byte();

        /* 3. Read payload */
        for (unsigned char i=0; i<len && i<datalen; i++){
            data[i] = read_byte();
        }
    }

    void write(){
        seq = next_seq();
        reply();
    }

    void reply(){
        chk = checksum();
        for (int i=0; i<3; i++){
            Serial.write(SYNC);
        }
        write_byte(chk);
        write_byte(seq);
        write_byte(cmd);
        write_byte(rid);
        write_byte(len);
        for (int i=0; i<len && i<datalen; i++){
            write_byte(data[i]);
        }
    }

    unsigned char checksum(){
        unsigned char res = 0;
        res += seq;
        res += cmd;
        res += rid;
        res += len;
        for (unsigned char i=0; i<len && i<datalen; i++){
            res += data[i];
        }
        return res;
    }

    void clear(){
        chk = seq = cmd = rid = len = 0;
    }

    unsigned char type(){return cmd & 0x7f;}
    bool is_change(){return (cmd & 0x80) == PARAM_CHANGE;}
};

typedef HALMsg_t<255> HALMsg;

#endif
