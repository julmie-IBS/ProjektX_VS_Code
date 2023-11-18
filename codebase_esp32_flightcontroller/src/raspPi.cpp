#include "raspPi.h"

// file where all comunication stuff between raspberry and esp is implemented
// NOTE 
// every packet consists of the following:
// startbyte + lenghtbyte +  n payloadbytes + crcbyte + endbyte
// where lenghtbyte is n + 4

// payloadbytes
// the first byte indecades what the payload is about
// for runmode it is just the runmode_byte followed by the sensordata




raspPi::raspPi()
{
    SERIAL_PI.begin(PI_BAUDRATE);
}

u_int8_t raspPi::init()
{
    

    //this is a test start sequeunz
    //TODO CRC
    byte byteArray[5] = {PI_START_BYTE, 0x05 ,PI_INIT_CONN_BYTE, 0xFF , PI_END_BYTE};

    // try 100 secounds to connect to pi
    for (int i = 0; i<100; i++ ){
        sleep(1);

        this->sendData2Pi(byteArray,sizeof(byteArray));
        SERIAL_PI.flush();
        sleep(1);

        if (PiDataAvailable())
        {   
            break;
        }
    }

    int rc =    getDatafromPi2inputBuffer();

    //TODO make this smoother

    if ((inputBuffer[2]==PI_INIT_CONN_BYTE) && inputBufferLenght==5)
    {
        return 0;
    }

    
   return 0x14;



}

u_int8_t raspPi::startCalibration()
{
    byte CalibrationSequenz[5] = {PI_START_BYTE, 0x05,PI_STARTCALIBRATION_BYTE, 0xFF, PI_END_BYTE};
    this->sendData2Pi(CalibrationSequenz,sizeof(CalibrationSequenz));
    SERIAL_PI.flush();
    
    //Wait for response
    sleep(0.1);

    int rc =  getDatafromPi2inputBuffer();


    //TODO make this smoother
    if ((inputBuffer[2]==PI_STARTCALIBRATION_BYTE) && inputBufferLenght==5)
    {
        return 0;
    }
    
    return 0x18;
}


u_int8_t raspPi::startRunMode()
{
    byte startRunModeSequenz[5] = {PI_START_BYTE, 0x05,PI_STARTRUNMODE_BYTE, 0xFF, PI_END_BYTE};
    this->sendData2Pi(startRunModeSequenz,sizeof(startRunModeSequenz));
    SERIAL_PI.flush();
    sleep(0.01);

    int rc = getDatafromPi2inputBuffer();


    //TODO make this smoother
    if ((inputBuffer[2]==PI_STARTRUNMODE_BYTE) && inputBufferLenght==5)
    {
        return 0;
    }
    
    

    return 0x19;
    
}

u_int8_t raspPi::sendData2Pi(byte* dataAdress, u_int8_t length)
{

    for (size_t i = 0; i < length; i++) 
    {
        SERIAL_PI.write(dataAdress[i]);
    }
    SERIAL_PI.flush(); // Wait for all data to be sent

    return 0;
}



u_int16_t raspPi::PiDataAvailable()
{
    return SERIAL_PI.available();
}



int raspPi::getDatafromPi2inputBuffer()

{

    if (PiDataAvailable()==0)
    {   
        //if no response return 
        return 0x15;
    }

    if (checkTransmissionFinished())
    {
        return 0x16;
    }

    inputBufferLenght = PiDataAvailable();
    copyRxData2inputBuffer();
    if(paketValidityCheck()==false)
    {
        return 0x17;
    }
    
    return 0;
}



u_int8_t raspPi::testConnection()
{
    return 0;
}


boolean raspPi::checkTransmissionFinished()
{
    for(int attempts2finished=0; attempts2finished>10; attempts2finished++)
    {
        int numb_byte_aval = PiDataAvailable();
        //TODO make delay maybe smaller
        delay(1);
        if (numb_byte_aval == PiDataAvailable())
        {
            return true;
        }

   }
   
    return false;

        
}


boolean raspPi::paketValidityCheck()
{
    //check start byte
    if (inputBuffer[0] != PI_START_BYTE)
    {
        return false;
    }

    //check end byte
    if (inputBuffer[inputBufferLenght-1] != PI_END_BYTE)
    {
        return false;
    }

    //check lenght byte
    if (inputBuffer[1] != inputBufferLenght)
    {
        return false;
    }

    //check crc

    
    u_int8_t crc = 0;

    for (int i = 0; i < (inputBufferLenght-2); i++) 
    {
        crc ^= inputBuffer[i];  // XOR with data byte
    }

    if (crc != inputBuffer[inputBufferLenght-2])
    {
        return false;
    }

    

    return true;

}


boolean raspPi::copyRxData2inputBuffer()
{
    SERIAL_PI.readBytes(inputBuffer, inputBufferLenght);
    return true;
}



int raspPi::sendSensorPaket2Pi(byte* sensorpaket, int lenghtSensorpaket)
{
    
    //copy sensorvalues to outputbuffer
    // startbyte, lenght ,SENSOR_PAKET,endbyte,crc



    // HEADER OF PAKET
    uint8_t crc = 0;
    outputBuffer[0] = PI_START_BYTE;
    outputBuffer[1] = lenghtSensorpaket + 5;
    outputBuffer[2] = PI_SENSOR_PAKET_BYTE;
    //crc ^=outputBuffer[0];
    //crc ^=outputBuffer[1];
    //crc ^=outputBuffer[2];



    // PAYLOAD OF PAKET
    for (size_t i = 0; i < lenghtSensorpaket; i++) 
    {
        outputBuffer[2+i+1]=sensorpaket[i];
    }

    // calculate crc byte    +2 to exclude crc and endbyte

    for (int i = 0; i < (lenghtSensorpaket + 2); i++) 
    {
    crc ^= outputBuffer[i];  // XOR with data byte
    }


    //END OF PAKET
    outputBuffer[lenghtSensorpaket+3] = crc;
    outputBuffer[lenghtSensorpaket+4] = PI_END_BYTE;

    sendData2Pi(outputBuffer,(lenghtSensorpaket + 5));

    return 0;


}


int raspPi::getThrustPaketfromPi()
{
    //lenght of a Thrustpaket is 
    // start + end + lenght + crc + PI_THRUST_PAKET_BYTE + 2*4 thrustvalues

    // expect the following order m1-highbyte, m1-lowbyte, m2-highbyte, m2-lowbyte, ........  


    int ThrustPaketLenght = 13;
    int rc = getDatafromPi2inputBuffer();

    if (rc!=0)
    {
        return rc;
    }


    if (!((inputBuffer[2]==PI_THRUST_PAKET_BYTE) && inputBufferLenght==ThrustPaketLenght))
    {
       return rc;
    }


    thrustValues[0]=inputBuffer[3];
    thrustValues[0]=thrustValues[0]<<8;
    thrustValues[0] = thrustValues[0] & inputBuffer[4];

    thrustValues[1]=inputBuffer[5];
    thrustValues[1]=thrustValues[0]<<8;
    thrustValues[1] = thrustValues[0] & inputBuffer[6];

    thrustValues[2]=inputBuffer[7];
    thrustValues[2]=thrustValues[0]<<8;
    thrustValues[2] = thrustValues[0] & inputBuffer[8];

    thrustValues[3]=inputBuffer[9];
    thrustValues[3]=thrustValues[0]<<8;
    thrustValues[3] = thrustValues[0] & inputBuffer[10];
    
    return 0;



}


u_int16_t* raspPi::getThrustValues()
{
    return this->thrustValues;
}