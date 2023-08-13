#include "sensor_compass.h"


// todo
// i2c maybe wrong (0x3C >> 1) ????????



    
    // constructor call
    Compass::Compass(int i2c_adress)
    {
        this->i2c_address = i2c_adress;
    }

    
    u_int16_t Compass::initSensor() 
    {
        //HMC5883L_3-Axis_Digital_Compass_IC.pdf
        Wire.begin();
        delay(10);

        // Initialize communication with the HMC5883L
        Wire.beginTransmission(i2c_address);
        Wire.write(IDENTIFICATION_REGISTER_A); // Set adresspointer on sensor to identification register (A)
        Wire.endTransmission();

        // Request 3 bytes of data
        Wire.requestFrom(i2c_address, 3);


        byte identification_reg_a = Wire.read(); 
        byte identification_reg_b = Wire.read();    
        byte identification_reg_c = Wire.read(); 

        //TODO   Check of register matches otherwise return rc

        //TODO    Set config register  may not nessesary 

        return 0;

    }
    
    int Compass::triggerMeasurement()
    {
        Wire.beginTransmission(i2c_address);              //Init Transmition with 0x2c device
        Wire.write(MODE_REGISTER);                       //Set Register to be written
        Wire.write(SINGLE_MEASUREMENT_MODE);             //Set Register Value
        Wire.endTransmission();

        return 0;
    }

    
    int Compass::fetchData()
    {
        Wire.beginTransmission(i2c_address);                 //Init Transmition with 0x2c device
        Wire.write(DATA_OUTPUT_X_MSB_REGISTER);             //Set Adresspointer to first data register
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, length);     

        dataBuffer[0] = Wire.read();
        dataBuffer[1] = Wire.read();
        dataBuffer[2] = Wire.read();
        dataBuffer[3] = Wire.read();
        dataBuffer[4] = Wire.read();
        dataBuffer[5] = Wire.read();          

        return 0;


    }

    
    byte* Compass::getData()
    {
        return dataBuffer;
    }

    
    int Compass::getLength()
    {
        return this->length;
    }

