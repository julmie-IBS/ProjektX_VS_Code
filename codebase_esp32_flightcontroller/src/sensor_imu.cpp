#include "sensor_imu.h"

//https://wolles-elektronikkiste.de/mpu6050-beschleunigungssensor-und-gyroskop
//https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf
    
    // constructor call
    IMU::IMU(int i2c_adress)
    {
        this->i2c_address = i2c_adress;
    }

    
    u_int16_t IMU::initSensor() 
    {
        Wire.begin();
        Wire.beginTransmission(this->i2c_address);
        Wire.write(0x6B); // PWR_MGMT_1 register
        Wire.write(0); // wake up!
        Wire.endTransmission(true);

        delay(100);
        setAccRange();
        delay(100);
        setGyrRange();
        delay(100);
        setLowPassFilter();




        return 0;

    }
    
    int IMU::triggerMeasurement()
    {
        return 0;
    }



    void IMU::setAccRange()
    {
        Wire.beginTransmission(i2c_address);
        Wire.write(MPU6050_ACCEL_CONFIG); 
        Wire.write(m_accRange); 
        Wire.endTransmission(true);
    }

    void IMU::setGyrRange()
    {
        Wire.beginTransmission(i2c_address);
        Wire.write(MPU6050_GYRO_CONFIG); 
        Wire.write(m_gyroRange); 
        Wire.endTransmission(true);
    }


    void IMU::setLowPassFilter()
    {
        Wire.beginTransmission(i2c_address);
        Wire.write(MPU6050_LOWPASS_CONFIG); 
        Wire.write(m_lowpassfilter); 
        Wire.endTransmission(true);
    }





    
    int IMU::fetchData()
    {
        Wire.beginTransmission(i2c_address);                //Init Transmition with 0x2c device
        Wire.write(ACCEL_XOUT_H);                           //Set Adresspointer to first data register
        Wire.endTransmission();                             //TODO Wire.endTransmission(false);   ?????
        Wire.requestFrom(i2c_address, length);              //TODO Wire.requestFrom(MPU6050_ADDR, 14, true);

        dataBuffer[0] = Wire.read();                        //0x3B (ACCEL_XOUT_H)
        dataBuffer[1] = Wire.read();                        //0x3C (ACCEL_XOUT_L)
        dataBuffer[2] = Wire.read();                        //0x3D (ACCEL_YOUT_H)
        dataBuffer[3] = Wire.read();                        //0x3E (ACCEL_YOUT_L)
        dataBuffer[4] = Wire.read();                        //0x3F (ACCEL_ZOUT_H)
        dataBuffer[5] = Wire.read();                        //0x40 (ACCEL_ZOUT_L)
        dataBuffer[6] = Wire.read();                        //0x41 (TEMP_OUT_H)
        dataBuffer[7] = Wire.read();                        //0x42 (TEMP_OUT_L)
        dataBuffer[8] = Wire.read();                        //0x43 (GYRO_XOUT_H)
        dataBuffer[9] = Wire.read();                        //0x44 (GYRO_XOUT_L)
        dataBuffer[10] = Wire.read();                       //0x45 (GYRO_YOUT_H)
        dataBuffer[11] = Wire.read();                       //0x46 (GYRO_YOUT_L)
        dataBuffer[12] = Wire.read();                       //0x47 (GYRO_ZOUT_H)
        dataBuffer[13] = Wire.read();                       //0x48 (GYRO_ZOUT_L)
        
        return 0;


    }

    
    byte* IMU::getData()
    {
        return dataBuffer;
    }

    
    int IMU::getLength()
    {
        return this->length;
    }

