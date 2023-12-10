#ifndef Sensor_Imu_h
#define Sensor_Imu_h



#include "sensor_interface.h"
#include <Wire.h>
#include "arduino.h"



#define ACCEL_XOUT_H                    0x3B



#define MPU6050_ACCEL_CONFIG            0x1C
#define MPU6050_ACC_RANGE_2G            0<<3               // +/- 2g (default)
#define MPU6050_ACC_RANGE_4G            1<<3               // +/- 4g
#define MPU6050_ACC_RANGE_8G            2<<3               // +/- 8g
#define MPU6050_ACC_RANGE_16G           3<<3               // +/- 16g

#define MPU6050_GYRO_CONFIG             0x1B
#define MPU6050_GYR_RANGE_250           0<<3               // +/- 250 deg/s (default)
#define MPU6050_GYR_RANGE_500           1<<3               // +/- 500 deg/s
#define MPU6050_GYR_RANGE_1000          2<<3               // +/- 1000 deg/s
#define MPU6050_GYR_RANGE_2000          3<<3               // +/- 2000 deg/s

#define MPU6050_LOWPASS_CONFIG          0x1A
#define MPU6050_LOWPASS_FILTER_260Hz    0
#define MPU6050_LOWPASS_FILTER_184Hz    1
#define MPU6050_LOWPASS_FILTER_94Hz     2
#define MPU6050_LOWPASS_FILTER_44Hz     3
#define MPU6050_LOWPASS_FILTER_21Hz     4 
#define MPU6050_LOWPASS_FILTER_10Hz     5
#define MPU6050_LOWPASS_FILTER_5Hz      6


#define ACC_ROLLING_BUFFER_LEN          10









class IMU : public ISensor {
public:
    IMU(int i2c_address, float calAccXBias, float calAccYBias, float calAccZBias, float calAccXScale, float calAccYScale, float calAccZScale);

    u_int16_t initSensor() override;
    int triggerMeasurement() override;
    int fetchData() override;
    byte* getData() override;
    int getLength() override;
    void DEBUG(int debugValue);
    
    float m_imuAccX=0;
    float m_imuAccY=0;
    float m_imuAccZ=0;


    float m_imuAccXRollingBuffer[ACC_ROLLING_BUFFER_LEN]={0};
    float m_imuAccYRollingBuffer[ACC_ROLLING_BUFFER_LEN]={0};
    float m_imuAccZRollingBuffer[ACC_ROLLING_BUFFER_LEN]={0};

    float m_imuGyroX;
    float m_imuGyroY;
    float m_imuGyroZ;
    float m_imuTemp;

    float m_calAccXBias;
    float m_calAccYBias;
    float m_calAccZBias;
    float m_calAccXScale;
    float m_calAccYScale;
    float m_calAccZScale;

    float m_calGyroXBias=0;
    float m_calGyroYBias=0;
    float m_calGyroZBias=0;





private:
    void setAccRange();
    void setGyrRange();
    void setLowPassFilter();
    void measureGyroBias();
    void calibrateValues();
    void applyAccRollBuffer();
    int m_accRollBufferPos=0;
    int m_accRange = MPU6050_ACC_RANGE_2G;
    int m_gyroRange = MPU6050_GYR_RANGE_250;
    int m_lowpassfilter = MPU6050_LOWPASS_FILTER_44Hz;
    int i2c_address;
    int length = 14;
    byte dataBuffer[14];





};


#endif

