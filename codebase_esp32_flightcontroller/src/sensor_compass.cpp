#include "sensor_compass.h"


// todo
// i2c maybe wrong (0x3C >> 1) ????????

class compass : ISensor 

{
  public:
    
    // constructor call
    compass(int i2c_adress)
    {
        this->i2c_adress = i2c_adress;
    }

    
    u_int16_t initSensor() override 
    {
        //HMC5883L_3-Axis_Digital_Compass_IC.pdf
        Wire.begin();
        delay(10);

        // Initialize communication with the HMC5883L
        Wire.beginTransmission(i2c_adress);
        Wire.write(IDENTIFICATION_REGISTER_A); // Set adresspointer on sensor to identification register (A)
        Wire.endTransmission();

        // Request 3 bytes of data
        Wire.requestFrom(i2c_adress, 3);


        byte identification_reg_a = Wire.read(); 
        byte identification_reg_b = Wire.read();    
        byte identification_reg_c = Wire.read(); 

        //TODO   Check of register matches otherwise return rc

        //TODO    Set config register  may not nessesary 

    }
    
    int triggerMeasurement() override
    {
        Wire.beginTransmission(i2c_adress);              //Init Transmition with 0x2c device
        Wire.write(MODE_REGISTER);                       //Set Register to be written
        Wire.write(SINGLE_MEASUREMENT_MODE);             //Set Register Value
        Wire.endTransmission();
    }

    
    int fetchData() override
    {
        Wire.beginTransmission(i2c_adress);                 //Init Transmition with 0x2c device
        Wire.write(DATA_OUTPUT_X_MSB_REGISTER);             //Set Adresspointer to first data register
        Wire.endTransmission();
        Wire.requestFrom(i2c_adress, lenght);     

        databuffer[0] = Wire.read();
        databuffer[1] = Wire.read();
        databuffer[2] = Wire.read();
        databuffer[3] = Wire.read();
        databuffer[4] = Wire.read();
        databuffer[5] = Wire.read();          



    }

    
    byte* getData() override
    {
        return databuffer;
    }

    
    int getLength() override
    {
        return this->lenght;
    }

  private:

    int i2c_adress;
    int lenght = 6;
    byte databuffer[6];

    byte identification_reg_a;
    byte identification_reg_b;
    byte identification_reg_c;



};

