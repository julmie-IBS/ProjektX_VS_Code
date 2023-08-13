#ifndef Sensor_Compass_h
#define Sensor_Compass_h

#include "sensor_interface.h"
#include <Wire.h>
#include "arduino.h"



#define CONFIGURATION_REGISTER_A 0x00
#define CONFIGURATION_REGISTER_B 0x01
#define IDENTIFICATION_REGISTER_A 0x0A
#define MODE_REGISTER 0x02
#define SINGLE_MEASUREMENT_MODE  0x01
#define DATA_OUTPUT_X_MSB_REGISTER 0x03


class Compass : public ISensor {
public:
    Compass(int i2c_address);

    u_int16_t initSensor() override;
    int triggerMeasurement() override;
    int fetchData() override;
    byte* getData() override;
    int getLength() override;

private:
    int i2c_address;
    int length = 6;
    byte dataBuffer[6];

    byte identification_reg_a;
    byte identification_reg_b;
    byte identification_reg_c;


};





#endif