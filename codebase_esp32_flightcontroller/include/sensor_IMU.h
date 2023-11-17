#ifndef Sensor_Imu_h
#define Sensor_Imu_h



#include "sensor_interface.h"
#include <Wire.h>
#include "arduino.h"



#define ACCEL_XOUT_H 0x3B



class IMU : public ISensor {
public:
    IMU(int i2c_address);

    u_int16_t initSensor() override;
    int triggerMeasurement() override;
    int fetchData() override;
    byte* getData() override;
    int getLength() override;

private:
    int i2c_address;
    int length = 14;
    byte dataBuffer[14];


};


#endif