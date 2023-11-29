#include "sensor_imu.h"

//https://wolles-elektronikkiste.de/mpu6050-beschleunigungssensor-und-gyroskop
//https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf
    
    // constructor call
    IMU::IMU(int i2c_address, float calAccXBias, float calAccYBias, float calAccZBias, float calAccXScale, float calAccYScale, float calAccZScale)
    {
        this->i2c_address = i2c_address;

        this->m_calAccXBias=calAccXBias;
        this->m_calAccYBias=calAccYBias;
        this->m_calAccZBias=calAccZBias;
        this->m_calAccXScale=calAccXScale;
        this->m_calAccYScale=calAccYScale;
        this->m_calAccZScale=calAccZScale;


    }

    
    u_int16_t IMU::initSensor() 
    {
        Wire.begin();
        Wire.setClock(400000);              //set i2c speed to 400kHz
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
        delay(100);
        measureGyroBias();

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


    void IMU::calibrateValues()
    {
        //save in member variables for calculation on esp32
        m_imuAccX = (m_imuAccX-m_calAccXBias)/m_calAccXScale;
        m_imuAccY = (m_imuAccY-m_calAccYBias)/m_calAccYScale;
        m_imuAccZ = (m_imuAccZ-m_calAccZBias)/m_calAccZScale;
       
        m_imuGyroX =    m_imuGyroX - m_calGyroXBias;    
        m_imuGyroY =    m_imuGyroY - m_calGyroYBias;       
        m_imuGyroZ =    m_imuGyroZ - m_calGyroZBias;


    }

    void IMU::measureGyroBias()
    {

        float tempCalGyroXBias=0;
        float tempCalGyroYBias=0;
        float tempCalGyroZBias=0;
        for(int i = 0; i<1000;i++)
        {
            delay(1);
            fetchData();
            tempCalGyroXBias=tempCalGyroXBias + m_imuGyroX;
            tempCalGyroYBias=tempCalGyroYBias + m_imuGyroY;
            tempCalGyroZBias=tempCalGyroZBias + m_imuGyroZ;

        }

        m_calGyroXBias=tempCalGyroXBias/1000;
        m_calGyroYBias=tempCalGyroYBias/1000;
        m_calGyroZBias=tempCalGyroZBias/1000;




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


        //int16_t temp = (dataBuffer[4]<<8 | dataBuffer[5]);

        //save in member variables for calculation on esp32
        m_imuAccX = -((float)((int16_t)((dataBuffer[0]<<8 | dataBuffer[1]))))/16384;            // "-" flip x axis to correct direction
        m_imuAccY = ((float)((int16_t)((dataBuffer[2]<<8 | dataBuffer[3]))))/16384;
        m_imuAccZ = ((float)((int16_t)((dataBuffer[4]<<8 | dataBuffer[5]))))/16384;

        m_imuTemp = (dataBuffer[6]<<8 | dataBuffer[7])/32768.0;

        /*        OLD CALCULATION IN DEGREE
        m_imuGyroX = ((float)((int16_t)((dataBuffer[8]<<8 | dataBuffer[9])))) /131;           // X -> Roll Bodyframe       //(250° * rawvalue)/250*131
        m_imuGyroY = -((float)((int16_t)((dataBuffer[10]<<8 | dataBuffer[11]))))/131;          // Y -> Pitch Bodyframe      //(250° * rawvalue)/250*131
        m_imuGyroZ = -((float)((int16_t)((dataBuffer[12]<<8 | dataBuffer[13]))))/131;          // Z -> Yaw Bodyframe        //(250° * rawvalue)/250*131
        */

        m_imuGyroX =  ((float)((int16_t)((dataBuffer[8]<<8  | dataBuffer[9])))) /7505;           // X -> Roll Bodyframe       //(250° * rawvalue)/250*131
        m_imuGyroY = -((float)((int16_t)((dataBuffer[10]<<8 | dataBuffer[11]))))/7505;          // Y -> Pitch Bodyframe      //(250° * rawvalue)/250*131
        m_imuGyroZ = -((float)((int16_t)((dataBuffer[12]<<8 | dataBuffer[13]))))/7505;          // Z -> Yaw Bodyframe        //(250° * rawvalue)/250*131



        // - ist für transformation in richtiges Koordinatensystem 
        // (linke hand regel, x nach vorne, y nach rechts, z nach unten)
        // (mit rechter hand anschließend Vorzeichen Roll,Pitch,Yaw)




        this->calibrateValues();








        

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

