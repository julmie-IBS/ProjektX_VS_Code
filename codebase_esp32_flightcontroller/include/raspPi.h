#ifndef RASPI_h
#define RASPI_h


#include "arduino.h"






#define PI_BAUDRATE                 250000
#define SERIAL_PI                   Serial


#define PI_START_BYTE               0xaa
#define PI_END_BYTE                 0xbb

#define PI_INIT_CONN_BYTE           0x01
#define PI_STARTCALIBRATION_BYTE    0x02
#define PI_STARTRUNMODE_BYTE        0x03
#define PI_RUNMODE_BYTE             0x04





class raspPi  {
public:

    raspPi();

    u_int8_t init();
    //should establish connection by 2 way handshake
    //use class methods to communicate

    u_int8_t startCalibration();
    //send a message to the pi that it should go into a calibration state.
    //TODO find out how to do this smooth

    u_int8_t startRunMode();
    //send a message to the pi that it should go into a calibration state.
  

    int sendSensorPaket2Pi(byte* sensorpaket, int lenghtSensorpaket);
    // methode takes a pointer to a sensorpaket
    // add startbit, pakettype endbyte and crc


    u_int16_t* getThrustPaketfromPi();
    // get back a pointer from thrust value array




    u_int8_t testConnection();


    //this is a test methode to eval latency between esp32 <---> raspberryPI
    

private:

    u_int16_t thrustValues[4];
    byte inputBuffer[50];
    int inputBufferLenght;
    // buffer for incomming data

    
    boolean checkTransmissionFinished();
    // check number of bytes available  --- wait --- check number of bytes available

    boolean paketValidityCheck();
    // check if lenght smaller 50
    // check start and endbyte, check lenght, check crc,
    // check paket type

    boolean copyRxData2inputBuffer();
    // copy uart buffer to object buffer

    int getDatafromPi2inputBuffer();
    // if we have fixed packetsize
    // gives back the internal buffer of the object (adresspointer)

    u_int8_t sendData2Pi(byte* dataAdress, u_int8_t length);
    //send data to raspberryPI


    u_int16_t PiDataAvailable();


    



    
    



 
};




#endif

