#ifndef RASPI_h
#define RASPI_h


#include "arduino.h"



//KREINDL
#define GPIO_MOSFET ???
#define GPIO_ESC_1 ???
#define GPIO_ESC_2  ???
#define GPIO_ESC_3 ??????
#define GPIO_ESC_4 ??????


#define PI_START_BYTE 0x10101010



class raspPi  {
public:

    raspPi();

    u_int8_t init();
    //should establish connection by 3 way handshake
    //use class methods to communicate

    u_int8_t startCalibration();
    //send a message to the pi that it should go into a calibration state.
    //TODO find out how to do this smooth

    u_int8_t startRunMode();
    //send a message to the pi that it should go into a calibration state.

    u_int8_t sendData2Pi(byte* dataAdress, u_int8_t length);
    //send data to raspberryPI

    u_int16_t checkDatafromPi();
    //check data from raspberryPI
    // copy to internal buffer of the object
    // 1) length
    // 2) check crc
    // 3) check headerfile
    // 4) return useful rc

    byte* getDatafromPi();
    // if we have fixed packetsize
    // gives back the internal buffer of the object (adresspointer)

    u_int8_t testConnection();
    //this is a test methode to eval latency between esp32 <---> raspberryPI


 
};




#endif

